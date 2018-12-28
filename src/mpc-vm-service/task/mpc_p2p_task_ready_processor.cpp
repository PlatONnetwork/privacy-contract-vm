#include "mpc_p2p_task_ready_processor.h"
#include "mpc_engine.h"
#include "platon_logger.h"

NS_PLATON_SDK_MPC_BEG


MpcP2pTaskReadyProcessor::MpcP2pTaskReadyProcessor()
{

}


MpcP2pTaskReadyProcessor::~MpcP2pTaskReadyProcessor()
{
    destroy();
}

int MpcP2pTaskReadyProcessor::init(int _queSize)
{
    if (running())
    {
        LOGW("ready processor is already running !");
        return 0;
    }
    max_queue_size_ = _queSize;

    //start the work thread
    running_ = true;
    thread_ = new std::thread([&](MpcP2pTaskReadyProcessor* processor) {
        while (processor->running())
        {
			processor->processReadyTask();
        }

        LOGW("the ready processor exited !");
    }, this);

    LOGI("ready process init ok. max request queue size: %d", _queSize);
    return 0;
}

void MpcP2pTaskReadyProcessor::destroy()
{
    //dispacher should destory first
    LOGW("to destroy the ready processor ...");
    if (running_ && thread_)
    {
        running_ = false;
        LOGW("to destroy the ready processor notify...");
        request_full_cond_.notify_all();
        request_empty_cond_.notify_all();
        LOGW("to destroy the ready processor wait thread join...");
        thread_->join();
        delete thread_;
        thread_ = nullptr;
    }
    LOGW("the ready processor destroyed !");
}

int MpcP2pTaskReadyProcessor::removeRequest(const string& taskId)
{
    if (!running_)
    {
        LOGW("put all ready failed, ready processor not INIT!");
        return -1;
    }

    //remove the request task
    {
        LOGD("to remove the request task: %s ...", taskId.data());
        std::unique_lock<std::mutex> lock(request_mutex_);
        request_empty_cond_.wait(lock, [&]() {
            return !request_queue_.empty() || !running_;//wait (!pred()) wait()
        });
        if (!running_)
        {
            LOGW("thread not running, exit ");
            return -1;
        }

        request_queue_.erase(taskId);
        request_full_cond_.notify_one();//may notify_all
    }

    LOGI("remove a working request, task: %s", taskId.data());
    return 0;
}


int MpcP2pTaskReadyProcessor::processReadyTask()
{
    if (!running_)
    {
        LOGW("cannot process task, ready processor not INIT!");
        return -1;
    }

    //error task should remove and notify to Java busi by setResult !!!!
    MpcEngine* engine = MpcEngine::getInstance();
    MpcTaskDispatcher* dispacher = engine->getTaskDispatcher();

    std::string address = engine->getMpcNodeServant2()->m_taskSession->getAddress();
    if (address.empty())
    {
        LOGW("no data provider register to the MPC VM , waiting !!!");
        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
        return -1;
    }
    
    //// 1. update the commited and not ready tasks, check if timeout
    std::vector<MPCTask> tasks;
    //dispacher->GetTimeoutTasks(tasks);

    //2. firstly, if the task state is not less than 2, put to ready queue
    {
        std::unique_lock<std::mutex> lock(request_mutex_);
        tasks.clear();
        for (auto iter = request_queue_.begin(); iter != request_queue_.end();) {
            auto& task = iter->second;
            int task_type = (task.invitor) ? 0 : 1;
            int state = 0;
            int ret = dispacher->findTaskState(task.taskId, state);
            if(ret != 0)
            {
                LOGW("cannot find the task state, peers not communication, task:%s task_type: %d", task.taskId.data(), task_type);
                continue;
            }

            // check if timeout (should not enter here if in normal sutiation)
            // if (task.timeout() && !task.already) {
            //     LOGW("###===> the task:%s timeout!", task.taskId.data());
            //     request_queue_.erase(iter++);
            //     continue;
            // }

            if (ret == 0 && ((state & 0x03) == 0x03)) {
                LOGI("put a task: %s to ready queue !", task.taskId.data());
                task.status = MPCTask::Status::PUSH_READY;
                task.already = true;
                dispacher->putReadyTask(task, task_type);
                dispacher->removeTaskState(task.taskId);
            }
            ++iter;
        }
    }

    //3. pop a commit task to trigger the => invite or notify busi
    MPCTask task;
    if (0 != dispacher->popCommitTask(task))
    {
        if (!dispacher->isInit())
        {
            LOGW("the dispatcher is exit, set exit for the read processor");
            running_ = false;
        }

        return -1;
    }
    task.timer.start(); // total timeout (from commit to ... end)
    tasks.push_back(task);

    LOGI("got commit tasks count: %d, to request busi, now request num: %d", tasks.size(), request_queue_.size());

    for (int i = 0; i < tasks.size(); ++i)
    {
        //4. put a request to queue [full then wait]
        int request_num = 0;
        //wait until the working queue is not full
        {
            std::unique_lock<std::mutex> lock(request_mutex_);
            request_full_cond_.wait(lock, [&]() {return (request_queue_.size() < max_queue_size_) || !running_; });
            if (!running_)
            {
                LOGW("read thread not running, exit ");
                return -2;
            }
            request_num = request_queue_.size();
        }

        //5. starter, do invite
        //5.1. get the peer nodeinfo
        platon::NodeCommunicateInfo *info = nullptr;
        for (auto &i : task.nodeInfoList)
        {
            if (i.user != address)
            {
                info = &i;
                break;
            }
        }
        if (!info)
        {
            LOGE("## Cannot find the peer node !!! ###");
            return -1;
        }
        LOGI("to handle commit task: %s, current user: %s,  is invitor: %s:   the peer:(user: %s) ...", task.taskId.data(), address.data(), task.invitor ? "true" : "false", info->user.data());
        MpcNodeClient* client = MpcEngine::getInstance()->getNodeClient(*info);

        platon::service::TaskParams params;
        params.iraddr = task.iraddr;
        params.extras = task.extras;
        params.irhash = task.irhash;
        params.taskid = task.taskId;
        params.pubkey = task.pubkey;
        params.method = task.method;
        params.txaddr = task.txaddr;
        params.role = task.invitor ? platon::MPCRole::PART_ALICE : platon::MPCRole::PART_BOB;

        task.status = MPCTask::Status::INVITE_OR_NOTIFY;
        int task_type = task.invitor ? 0 : 1;
        string action(task.invitor ? "invite" : "notify");
        int status = 0;
        do {
            try {
                if (task_type == 0) { status = client->invite(params); }// alice invite, status: 1: invite ok, 2 : notify and invite ok
                else { status = client->notify(params); }// bob, notify, status: 1: notify ok, 2: notify and invite ok
                break;
            }
            catch (const Ice::ConnectionRefusedException& e) {
                LOGE("%s Ice ConnectionRefusedException ice_name:%s, error:%d %s\n", action.c_str(), e.ice_name().c_str(), e.error, e.what());
            }
            catch (const Ice::Exception& e) {
                LOGE("%s Ice Exception ice_name:%s, error:%s\n", action.c_str(), e.ice_name().c_str(), e.what());
            }

            if (!task.invitor) {
                int ret = dispacher->setFailedResult(task);
                LOGI("Bob set the failed result, task:%s, ret:%d", task.taskId.c_str(), ret);
            }
            dispacher->removeTaskState(task.taskId);
            removeRequest(task.taskId);
            return -1;

        } while (false);

        //with the return status, handle 
        int st = 0;
        if ((status & 0x03) == 0x03)//all ready
        {
            st = status;
            LOGI("###**** the task:%s all ready, will put to ready queue next round !", task.taskId.data());
        }
        else
        {
            //set the state to 1, waiting for the peer => notify or invite
            LOGI("### the task:%s waiting for the peer ready !   status: %d", task.taskId.data(), status);
            if (task_type == 0)
                st = 0x01 | status;
            else
                st = 0x02 | status;
        }
        dispacher->updateTaskState(task.taskId, st);

        {
            std::unique_lock<std::mutex> lock(request_mutex_);
            task.timer.start(); // start timer
            request_queue_.insert(std::make_pair(task.taskId, task));
            request_num = request_queue_.size();
        }
        request_empty_cond_.notify_one();//may be ontify_all  

        LOGI("invoke %s action ok, task: %s, wait ready back... total request queue: %d", action.c_str(), task.taskId.data(), request_num);
    }

    return 0;
}


bool MpcP2pTaskReadyProcessor::callbackCheckConsumeAvailable()
{
    if (running_ && request_queue_.size() > 0)
    {
        return true;
    }

    return false;
}

NS_PLATON_SDK_MPC_END
