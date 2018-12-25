#pragma once


#include <thread>
#include <atomic>
#include <map>
#include <functional>
#include <cstddef>
#include <mutex>
#include "mpc_task.h"
#include "safequeue.h"


#include "mpc_vm_service.h"
NS_PLATON_SDK_MPC_BEG

//processor handle busi of the tasks:  switch invit to allReay
class PLATONMPCVM_SERVICE_DLL_API MpcP2pTaskReadyProcessor
{
public:
	MpcP2pTaskReadyProcessor();
	~MpcP2pTaskReadyProcessor();

	int init(int _queSize);

	void destroy();

	//the task processor will call this function to clean
	int removeRequest(const string& taskId);

	int processReadyTask();

	bool running() { return running_; };

	bool callbackCheckConsumeAvailable();


private:
	volatile bool								    running_ = false;
	std::thread*										thread_;
	int													max_queue_size_ = 8;//2 times of MpcTaskProccess count

	std::mutex											 request_mutex_;
	std::condition_variable                         request_full_cond_;
	std::condition_variable                         request_empty_cond_;
	std::map<std::string, MPCTask>			 request_queue_;//working queue

};
NS_PLATON_SDK_MPC_END
