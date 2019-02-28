#include "mpc_task_dispatcher.h"
#include <mutex>
#include <chrono>
#include <thread>
#include <climits>

#include "mpc_engine.h"
#include "platon_logger.h"
#include "mpc_node_callback_impl.h"

#include "ssl.h"
#include "libdevcore/RLP.h"


const static int MAX_WORKER_SIZE = 4;

NS_PLATON_SDK_MPC_BEG


MpcTaskDispatcher::MpcTaskDispatcher(int _workersNum, int _aWorkerLoad) : m_atomicCommitNum(0)//, m_readyNum(0)
{
	int coresNum = std::thread::hardware_concurrency();
	std::cout << "coresNum: " << coresNum << std::endl;
	if (coresNum < 2)
	{
		LOGW("hardware_concurrency is ineffient in the machine, core num: %d ! use double processors !", coresNum);
		coresNum = 2;
	}

	coresNum = 1;
	int realWorkerSize = coresNum * 2;
	int type = MpcTaskProcessor::InvitorProcessor;
	for (int i = 0; i < realWorkerSize; ++i)
	{
		type = ((i < realWorkerSize / 2) ? MpcTaskProcessor::InvitorProcessor : MpcTaskProcessor::InviteeProcessor);
		m_mpcProcessors.push_back(new MpcTaskProcessor(i, type));
	}

	m_loadLimit = _aWorkerLoad * realWorkerSize;
	LOGI("construct task dispatcher, with %d processor, load balence is: %d", m_mpcProcessors.size(), m_loadLimit);
}

MpcTaskDispatcher::~MpcTaskDispatcher()
{
	destroy();
}

void MpcTaskDispatcher::destroy()
{
	if (!m_inited)
		return;

	LOGW("task dispatcher destroy ...");
	m_inited = false;
	m_commitQueue.notify_exit();

	m_inviteeReadyEmptyCv.notify_all();
	m_readyEmptyCv.notify_all();

	m_inviteeReadyFullCv.notify_all();
	m_readyFullCv.notify_all();

	LOGW("to destruct the mpc processors ...");
	int size = m_mpcProcessors.size();
	for (int i = 0; i < size; ++i)
	{
		m_inviteeReadyEmptyCv.notify_all();
		m_readyEmptyCv.notify_all();
		delete m_mpcProcessors[i];
		m_mpcProcessors[i] = nullptr;
	}
	m_mpcProcessors.clear();

	LOGW("destruct the mpc processors ok.");
}

int MpcTaskDispatcher::init()
{
	if (m_inited)
		return 0;

	//start the processors
	m_inited = true;
	for (int i = 0; i < m_mpcProcessors.size(); ++i)
	{
		if (0 != m_mpcProcessors[i]->init())
		{
			LOGE("INIT processor-%d failed !", i);
			return -1;
		}
	}

	m_atomicCommitNum.store(0, std::memory_order_relaxed);
	return 0;
}

int MpcTaskDispatcher::putBuffer(const string& taskId, MpcBufferPtr buffer)
{
	if (!m_inited)
	{
		LOGW("not INIT, cannot put buffer.");
		return -1;
	}

	//find the working processor
	for (int i = 0; i < m_mpcProcessors.size(); ++i)
	{
		string proccessTask = m_mpcProcessors[i]->getTaskId();
		if (taskId == proccessTask)
		{
			//LOGI("to put a buffer(size: %d, seq: %d) with task: %s, to processor: %d Ok!", buffer->length, buffer->seq, taskId.data(), i);
			m_mpcProcessors[i]->inputBuffer(buffer);
			return 0;
		}
	}
	
	//cannot find the working process, maybe the invitee is quite slow or not gets the processor
	//put the buffer to first buffer queue
	{
		putFirstInputBuffer(taskId, buffer);
		//LOGW("cannot find the working process, put a buffer(%d size) with task: %s to first input buffer", buffer->length, taskId.data());
	}
	
	return 0;
}

//only for the bob
bool MpcTaskDispatcher::getFirstInputBuffer(const string& taskId, map<int, MpcBufferPtr>& buffers)
{
	if (0 != m_firstInputBufferMap.getElem(taskId, buffers))
	{
		return false;
	}

	return true;
}

bool MpcTaskDispatcher::removeFirstInputBuffer(const string& taskId, map<int, MpcBufferPtr>& buffers)
{
	if (0 != m_firstInputBufferMap.erase(taskId, buffers))
	{
		return false;
	}

	return true;
}

void MpcTaskDispatcher::putFirstInputBuffer(const string& taskId, MpcBufferPtr buffer)
{
	map<int, MpcBufferPtr> buffers;
	m_firstInputBufferMap.getElem(taskId, buffers);

	//buffers.clear();
	buffers[buffer->seq] = buffer;

	m_firstInputBufferMap.insert(taskId, buffers);
}

int MpcTaskDispatcher::findTaskState(const std::string& taskid, int& state)
{
	std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
	auto item = m_taskState.find(taskid);
	if (item == m_taskState.end())
		return -1;

	state = item->second;
	return 0;
}

int MpcTaskDispatcher::removeTaskState(const std::string& taskid)
{
	std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
	m_taskState.erase(taskid);
	return 0;
}

void MpcTaskDispatcher::updateTaskState(const std::string& taskid, const int& state)
{
	std::lock_guard<std::recursive_mutex> lock(m_stateMutex);
	auto item = m_taskState.find(taskid);
	if (item == m_taskState.end())
		m_taskState[taskid] = state;
	else
		item->second |= state;
}

int MpcTaskDispatcher::PutTimeoutTasks(const MPCTask& task)
{
	if (!m_inited)
	{
		LOGW("not INIT, cannot put commit task.");
		return -1;
	}

	{
		std::unique_lock<std::mutex> lock(m_timeoutMutex);
		m_timeoutQueue.push(task);
	}

	return 0;
}

int MpcTaskDispatcher::GetTimeoutTasks(std::vector<MPCTask>& tasks)
{
	if (!m_inited)
	{
		LOGW("not INIT, cannot put commit task.");
		return -1;
	}

	{
		double timeout = m_taskTimeout;
		std::unique_lock<std::mutex> lock(m_timeoutMutex);
		while (!m_timeoutQueue.empty() && m_timeoutQueue.front().timer.elapse() > timeout)
		{
			LOGI("get a timeout task: %s,  abandon it !", m_timeoutQueue.front().taskId.data());

			tasks.push_back(m_timeoutQueue.front());
			m_timeoutQueue.pop();
		}
		return tasks.size();
	}

	return 0;
}

int MpcTaskDispatcher::putCommitTask(MPCTask& task)
{
	if (!m_inited)
	{
		LOGW("not INIT, cannot put commit task.");
		return -1;
	}

	LOGI("to put a commit task: %s, task size: %d ...", task.taskId.data(), m_atomicCommitNum.load(std::memory_order_relaxed));
	if (m_atomicCommitNum.load(std::memory_order_relaxed) >= m_loadLimit)
	{
		LOGW("cannot put commit task now, the waiting queue is full to: %d", m_loadLimit);
		return 1;
	}

    task.status = MPCTask::Status::PUSH_COMMIT;
	m_commitQueue.push(task);
	m_atomicCommitNum.fetch_add(1);

	LOGI("put a commit task: %s done, task size: %d", task.taskId.data(), m_atomicCommitNum.load(std::memory_order_relaxed));
	return 0;
}

int MpcTaskDispatcher::popCommitTask(MPCTask& task)
{
	if (!m_inited)
	{
		LOGW("not INIT, cannot pop commit task.");
		return -1;
	}

	LOGI("to POP a commit task, task size: %d  ...", m_atomicCommitNum.load(std::memory_order_relaxed));
	int ret = m_commitQueue.pop(task, 1000);
	if (0 != ret){
        if(ret == -2){/*timeout*/ }
        else {
            LOGE("pop a commit failed, break by others !");
        }
		return ret;
	}
    task.status = MPCTask::Status::POP_COMMIT;
    m_atomicCommitNum.fetch_sub(1);

	LOGI("POP a commit task: %s done, task size: %d", task.taskId.data(), m_atomicCommitNum.load(std::memory_order_relaxed));
	return 0;
}

int MpcTaskDispatcher::putReadyTask(const MPCTask& task)
{
	if (!task.invitor)
	{
		return putInviteeReadyTask(task);
	}
	else//else starter
	{
		return putInvitorReadyTask(task);
	}
}

int MpcTaskDispatcher::popReadyTask(MPCTask& task, int type)
{

	if (type == 1)
	{
		return popInviteeReadyTask(task);
	}
	else//else starter
	{
		return popInvitorReadyTask(task);
	}
}

int MpcTaskDispatcher::putInviteeReadyTask(const MPCTask& task)
{
	if (!m_inited)
	{
		LOGW("not INIT, cannot put invitee ready task.");
		return -1;
	}

	//to consume a ready slot
	int ready_num = 0;
	{
		LOGI("to PUT a task: %s to invitee ready queue, now ready_num: %d...", task.taskId.data(), m_inviteeReadyQueue.size());
		std::unique_lock<std::mutex> lock(m_inviteeReadyMutex);
		m_inviteeReadyFullCv.wait(lock, [&]() {return m_inviteeReadyQueue.size() < m_mpcProcessors.size() / 2 || !m_inited; });
		if (!m_inited)
		{
			LOGW("notify exit by outside when put ready task !");
			return -1;
		}

		//put a invitee ready task
		m_inviteeReadyQueue.push(task);
		ready_num = m_inviteeReadyQueue.size();
	}
	//notify the processor there is new task
	m_inviteeReadyEmptyCv.notify_one();

	LOGI("ok PUT a task: %s to invitee ready queue, mpc will do immediately, now ready_num: %d", task.taskId.data(), ready_num);
	return 0;
}

int MpcTaskDispatcher::popInviteeReadyTask(MPCTask& task)
{
	if (!m_inited)
	{
		LOGW("not INIT, cannot pop ready task. from invitee ready queue");
		return -1;
	}


	//to release a ready slot
	int ready_num = 0;
	{
		LOGI("to POP a ready task from ready invitee queue to processor, now read_num: %d ...", m_inviteeReadyQueue.size());
		std::unique_lock<std::mutex> lock(m_inviteeReadyMutex);
		m_inviteeReadyEmptyCv.wait(lock, [&]() {return m_inviteeReadyQueue.size() > 0 || !m_inited; });
		if (!m_inited)
		{
			LOGD("notify exit by outside when pop ready task !");
			return -1;
		}

		task = m_inviteeReadyQueue.front();
		m_inviteeReadyQueue.pop();
		ready_num = m_inviteeReadyQueue.size();
	}
	m_inviteeReadyFullCv.notify_one();

	LOGI("ok POP a ready task: %s from invitee ready queue to processor, now read_num: %d", task.taskId.data(), ready_num);
	return 0;
}

int MpcTaskDispatcher::putInvitorReadyTask(const MPCTask& task)
{
	if (!m_inited)
	{
		LOGW("not INIT, cannot put invitor ready task.");
		return -1;
	}

	//to consume a ready slot
	int ready_num = 0;
	{
		LOGI("to PUT a task: %s to invitor ready queue, now ready_num: %d...", task.taskId.data(), m_readyQueue.size());
		std::unique_lock<std::mutex> lock(m_readyMutex);
		m_readyFullCv.wait(lock, [&]() {return m_readyQueue.size() < m_mpcProcessors.size() / 2 || !m_inited; });
		if (!m_inited)
		{
			LOGW("notify exit by outside when put ready task !");
			return -1;
		}

		//put a ready task
		m_readyQueue.push(task);
		ready_num = m_readyQueue.size();
	}
	//notify the processor there is new task
	m_readyEmptyCv.notify_one();

	LOGI("ok PUT a task: %s to invitor ready queue, mpc will do immediately, now ready_num: %d", task.taskId.data(), ready_num);
	return 0;
}

int MpcTaskDispatcher::popInvitorReadyTask(MPCTask& task)
{
	if (!m_inited)
	{
		LOGW("not INIT, cannot pop ready task. from invitor ready queue");
		return -1;
	}


	//to release a ready slot
	int ready_num = m_readyQueue.size();
	{
		LOGI("to POP a ready task from ready invitor queue to processor, now read_num: %d ...", ready_num);
		std::unique_lock<std::mutex> lock(m_readyMutex);
		m_readyEmptyCv.wait(lock, [&]() {return m_readyQueue.size() > 0 || !m_inited; });
		if (!m_inited)
		{
			LOGD("notify exit by outside when pop ready task !");
			return -1;
		}

		task = m_readyQueue.front();
		m_readyQueue.pop();
		ready_num = m_readyQueue.size();
	}
	m_readyFullCv.notify_one();

	LOGI("ok POP a ready task: %s from invitor ready queue to processor, now read_num: %d", task.taskId.data(), ready_num);
	return 0;
}


int MpcTaskDispatcher::pushPutTask(const MPCTask& task)
{
	LOGI("to push a invited and notified, task: %s ...", task.taskId.data());
	m_putQueue.push(task);
	return 0;
}

int MpcTaskDispatcher::popPutTask(MPCTask& task)
{
	return m_putQueue.pop(task);
}

int MpcTaskDispatcher::removeRequest(const string& taskId)
{
	{
		LOGD("to remove the request task: %s ...", taskId.data());
		std::unique_lock<std::mutex> lock(m_request_mutex);
		m_request_empty_cond.wait(lock, [&]() {
			return !m_request_queue.empty();
		});

		m_request_queue.erase(taskId);
		m_request_full_cond.notify_one();//may notify_all
	}

	LOGI("remove a to-ready task: %s, tasksize: %d ", taskId.data(), m_request_queue.size());
	return 0;
}

int MpcTaskDispatcher::addRequest(const string& taskId, const MPCTask& task)
{
	//put a in-processing ready task
	LOGI("to add a to-ready task: %s, tasksize: %d ...", taskId.data(), m_request_queue.size());
	{
		std::unique_lock<std::mutex> lock(m_request_mutex);
		m_request_full_cond.wait(lock, [&]() {return (m_request_queue.size() < getProcessorNum()); });
		m_request_queue.insert(std::make_pair(taskId, task));
	}
	m_request_empty_cond.notify_one();//may be ontify_all  

	LOGI("add a working task, task: %s", taskId.data());
	return 0;
}

void MpcTaskDispatcher::putWorkingTask(const MPCTask& task)
{
	m_workingTasks.insert(task.taskId, task);
}

int MpcTaskDispatcher::getWorkingTask(const string& id, MPCTask& task)
{
	return m_workingTasks.getElem(id, task);
}

void MpcTaskDispatcher::removeWorkingTask(const string& id)
{
	m_workingTasks.erase(id);
}

int MpcTaskDispatcher::setFailedResult(const MPCTask& task) {
    string address = MpcEngine::getInstance()->getMpcNodeServant2()->getTaskSession()->getAddress();

    string errmsg = task.getStatusErrDesc();
    vector<byte> txtype = { 0,0,0,0,0,0,0,1 };
    vector<byte> errcode = { 0,0,0,0,0,0,0,2 };

    TransParams transparams;
    transparams.from = address;
    transparams.to = task.iraddr;
    transparams.data = dev::toHexPrefixed(dev::rlpList(txtype, string("set_result"), task.taskId, errcode, errmsg));
    LOGI("set_result transparams %s", transparams.toString().c_str());
    MpcEngine::getInstance()->getPlatonClient()->set_result(transparams);

    return 0;
}

NS_PLATON_SDK_MPC_END
