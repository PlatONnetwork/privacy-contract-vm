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
        LOGI("the ready processor begin process.");

        while (processor->running())
        {
            if (0 != processor->processReadyTask())
            {
                //LOGW("ready process a commit busi task failed !");
            }
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

    {
        LOGD("to remove the request task: %s ...", taskId.data());
        std::unique_lock<std::mutex> lock(request_mutex_);
        request_empty_cond_.wait(lock, [&]() {
            return !request_queue_.empty() || !running_;
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

int MpcP2pTaskReadyProcessor::updateTaskState(MPCTask& task) {
    MpcEngine* engine = MpcEngine::getInstance();
    MpcTaskDispatcher* dispacher = engine->getTaskDispatcher();
    std::string address = engine->getMpcNodeServant2()->m_taskSession->getAddress();

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

    LOGI("invoke %s action ok 1, task: %s, status: %d wait ready back", action.c_str(), task.taskId.data(), st);
    dispacher->updateTaskState(task.taskId, st);
	int state = 0;
	int ret = dispacher->findTaskState(task.taskId, state);
	LOGI("invoke %s action ok 2, task: %s, status: %d wait ready back", action.c_str(), task.taskId.data(), state);

    return 0;
}

//int MpcP2pTaskReadyProcessor::processReadyTask()
//{
//    //LOGW("xxxxxxxxxxxxxxxxxxxx---------------!");
//    if (!running_)
//    {
//        LOGW("cannot process task, ready processor not INIT!");
//        return -1;
//    }
//
//    //error task should remove and notify to Java busi by setResult !!!!
//    MpcEngine* engine = MpcEngine::getInstance();
//    MpcTaskDispatcher* dispacher = engine->getTaskDispatcher();
//    std::string address = engine->getMpcNodeServant2()->m_taskSession->getAddress();
//    if (address.empty())
//    {
//        LOGW("no data provider register to the MPC VM , waiting !!!");
//        std::this_thread::sleep_for(std::chrono::milliseconds(2000));
//        return -1;
//    }
//
//    LOGI("request_queue_ size1: %d", request_queue_.size());
//    {
//        std::unique_lock<std::mutex> lock(request_mutex_);
//        for (auto iter = request_queue_.begin(); iter != request_queue_.end();) {
//
//            auto& task = iter->second;
//            int task_type = (task.invitor) ? 0 : 1;
//            int state = 0;
//            int ret = dispacher->findTaskState(task.taskId, state);
//            //LOGI("###===> the task:%s task_type: %d findTaskState ret: %d state: %d already: %s",
//            //    task.taskId.data(), task_type, ret, state, (task.already ? "true" : "false"));
//            
//            LOGI("task: %s state: %d", task.taskId.c_str(), state);
//
//            if (ret != 0) {
//                LOGW("cannot find the task state, peers not communication, task:%s task_type: %d", task.taskId.data(), task_type);
//                ++iter;
//                continue;
//            }
//
//            // check if timeout (should not enter here if in normal sutiation)
//            // if (task.timeout() && !task.already) {
//            //     LOGW("###===> the task:%s timeout!", task.taskId.data());
//            //     request_queue_.erase(iter++);
//            //     continue;
//            // }
//
//            if (ret == 0 && ((state & 0x03) == 0x03)) {
//                LOGI("put a task: %s to ready queue !", task.taskId.data());
//                task.status = MPCTask::Status::PUSH_READY;
//                task.already = true;
//                dispacher->putReadyTask(task, task_type);
//
//                dispacher->removeTaskState(task.taskId);
//                request_queue_.erase(iter++);
//                request_full_cond_.notify_one();
//                continue;
//            }
//            ++iter;
//        }
//    }
//    LOGI("request_queue_ size2: %d", request_queue_.size());
//
//    ///{
//    ///    std::unique_lock<std::mutex> lock(request_mutex_);
//    ///    for (auto iter = request_queue_.begin(); iter != request_queue_.end(); ++iter) {
//    ///        int state = 0;
//    ///        int ret = dispacher->findTaskState(iter->second.taskId, state);
//    ///        LOGI("EEEEEEEEEEEEEEEEEEEEEE: %s, %d", iter->second.taskId.c_str(), state);
//    ///        if (ret == 0 && state == 0x03) continue;
//    ///        //updateTaskState(iter->second);
//    ///    }
//    ///}
//
//    if (request_queue_.size() > 2) {
//        std::this_thread::sleep_for(std::chrono::microseconds(500));
//        return 0;
//    }
//
//
//    MPCTask task;
//    if (0 != dispacher->popCommitTask(task))
//    {
//        if (!dispacher->isInit())
//        {
//            LOGW("the dispatcher is exit, set exit for the read processor");
//            running_ = false;
//        }
//        return -1;
//    }
//    updateTaskState(task);
//    {
//        std::unique_lock<std::mutex> lock(request_mutex_);
//        task.timer.start(); // start timer
//        request_queue_.insert(std::make_pair(task.taskId, task));
//    }
//    request_empty_cond_.notify_one();//may be ontify_all  
//
//    return 0;
//}

int MpcP2pTaskReadyProcessor::processReadyTask()
{
	if (!running_)
	{
		LOGW("cannot process task, ready processor not INIT!");
		return -1;
	}

	MpcEngine* engine = MpcEngine::getInstance();
	MpcTaskDispatcher* dispacher = engine->getTaskDispatcher();

	std::string address = engine->getMpcNodeServant2()->m_taskSession->getAddress();
	if (address.empty())
	{
		LOGW("no data provider register to the MPC VM , waiting !!!");
		std::this_thread::sleep_for(std::chrono::milliseconds(2000));
		return -1;
	}

	// pop a commit task to trigger the => invite or notify busi
	MPCTask task;
	if (0 != dispacher->popCommitTask(task))
	{
		if (!dispacher->isInit())
		{
			LOGW("the dispatcher is exit, set exit for the read processor");
			running_ = false;
		}

		//LOGW("dispacher->popCommitTask(task) .........");

		return -1;
	}
	task.timer.start(); // total timeout (from commit to ... end)

	//put a in-processing ready task
	dispacher->putWorkingTask(task);
	dispacher->addRequest(task.taskId, task);

	//peer nodeinfo shoud avaiable
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
			//invite or notify
			if (task_type == 0)
			{ 
				status = client->invite(params);
			}// alice invite, status: 1: invite ok, 2 : notify and invite ok
			else
			{ 
				status = client->notify(params); 
			}// bob, notify, status: 1: notify ok, 2: notify and invite ok

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

		LOGW("got exection, should clear the cache task: %s", task.taskId.data());
		//get exception
		dispacher->removeTaskState(task.taskId);
		//dispacher->removeRequest(task.taskId);
		dispacher->removeWorkingTask(task.taskId);
		return -1;

	} while (false);

	//update task ready status
	int state = 0;
	if (task_type == 0)//alice
		state = 0x01 | status;
	else//bob
		state = 0x02 | status;

	dispacher->updateTaskState(task.taskId, state);
	dispacher->findTaskState(task.taskId, state);

	LOGI("invoke %s action ok, task: %s, status: %d", action.c_str(), task.taskId.data(), state);
	if (state == 0x03) {
		//dispacher->removeTaskState(task.taskId);
		dispacher->pushPutTask(task);
		LOGI("both alice and bob are invited and notified, task: %s", task.taskId.data());
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
