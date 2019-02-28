#pragma once


#include <map>
#include <mutex>
#include <vector>
#include <atomic>
#include <cstdint>

#include "mpc_task.h"
#include "safemap.h"
#include "safequeue.h"
#include "mpc_buffer.h"
#include "mpc_task_processor.h"



using std::map;
using std::mutex;
using std::recursive_mutex;

using platon::mpc::MpcBuffer;
using platon::mpc::MpcBufferPtr;

#include "mpc_vm_service.h"
NS_PLATON_SDK_MPC_BEG

class PLATONMPCVM_SERVICE_DLL_API MpcTaskDispatcher
{
public:
	MpcTaskDispatcher(int _workersNum = 0, int _aWorkerLoad = 2048);
	~MpcTaskDispatcher();
	/*
	* node_type CALLBACK or Service
	* router_type when the node is Service type, the @router_type can be Proxy or Direct
	*/

public:
	int init();

	void destroy();

	//bool removeTask(const string taskId);
	int putBuffer(const string& taskId, MpcBufferPtr buffer);

	bool removeFirstInputBuffer(const string& taskId, map<int, MpcBufferPtr>& buffers);

	bool getFirstInputBuffer(const string& taskId, map<int, MpcBufferPtr>& buffers);

	void putFirstInputBuffer(const string& taskId, MpcBufferPtr buffer);

	int getProcessorNum() const { return (int)m_mpcProcessors.size(); };


public:
	// task type can be: 0: invitor(alice), 1: invitee (bob)
	/*
	** enum ProcessorType
	** {
	**     InvitorProcessor = 0,
	**     InviteeProcessor = 1
	** };
	*/
	int putReadyTask(const MPCTask& task);
	int popReadyTask(MPCTask& task, int type = 0);

	int putInviteeReadyTask(const MPCTask& task);
	int popInviteeReadyTask(MPCTask& task);

	int putInvitorReadyTask(const MPCTask& task);
	int popInvitorReadyTask(MPCTask& task);

	int findTaskState(const std::string& taskid,  int& state);
	int removeTaskState(const std::string& taskid);
	void updateTaskState(const std::string& taskid, const int& state);
	
	int putCommitTask(MPCTask& task);
	int popCommitTask(MPCTask& task);

	int GetTaskTimeout() const { return m_taskTimeout; };

	int GetTimeoutTasks(std::vector<MPCTask>& tasks);
	int PutTimeoutTasks(const MPCTask& task);

	int pushPutTask(const MPCTask& task);
	int popPutTask(MPCTask& task);

	int removeRequest(const string& taskId);
	int addRequest(const string& taskId, const MPCTask& task);

	void putWorkingTask(const MPCTask& task);
	int getWorkingTask(const string& id, MPCTask& task);
	void removeWorkingTask(const string& id);

    int setFailedResult(const MPCTask& task);

public:
	bool isInit() const { return m_inited; };

private:
	bool volatile								m_inited = false;
	int												m_loadLimit;//per core load limit
	vector<MpcTaskProcessor*>		m_mpcProcessors;


	//外部提交任务队列 （主动计算）
	std::atomic<int32_t> 						m_atomicCommitNum;//跟每个工作处理器负载一致， 超过则返回1（try again)
	SafeQueue<MPCTask>					m_commitQueue;

	// invitor allReady队列 
	std::mutex										m_readyMutex;
	std::condition_variable					m_readyEmptyCv;//初始值与half处理器个数一致
	std::condition_variable					m_readyFullCv;//初始值与处理器个数一致
	std::queue<MPCTask>					m_readyQueue;

	// invitee allReady队列 
	std::mutex										 m_inviteeReadyMutex;
	std::condition_variable                     m_inviteeReadyEmptyCv;//初始值与half处理器个数一致
	std::condition_variable                     m_inviteeReadyFullCv;//初始值与处理器个数一致
	std::queue<MPCTask>                     m_inviteeReadyQueue;

	// timeout task queue
	std::mutex										 m_timeoutMutex;
	int														m_taskTimeout = 80;//about 4 times of the cost block ensure time
	std::queue<MPCTask>						m_timeoutQueue;

	//// invite, notify put队列 
	SafeQueue<MPCTask>					m_putQueue;

	//// working tasks 
	//std::mutex											 m_workingMutex;
	SafeMap<std::string, MPCTask>			 m_workingTasks;//working queue

	////// processing reading tasks
	std::mutex											 m_request_mutex;
	std::condition_variable                         m_request_full_cond;
	std::condition_variable                         m_request_empty_cond;
	std::map<std::string, MPCTask>			 m_request_queue;//request queue

	// peer waiting task map
	//SafeMap<string, MPCTask>					m_PeerWaitTasks; // taskid -> MPCTask
	std::recursive_mutex							m_stateMutex;
	std::map<string, int>							m_taskState;

	//Bob 第一次输入缓存队列
	SafeMap<string, map<int, MpcBufferPtr> >	m_firstInputBufferMap;


};

NS_PLATON_SDK_MPC_END
