#include "mpc_put_ready_monitor.h"
#include "mpc_engine.h"
#include "mpc_task_dispatcher.h"
#include "platon_logger.h"

NS_PLATON_SDK_MPC_BEG

MPCPutReadyMonitor::MPCPutReadyMonitor()
{
}

MPCPutReadyMonitor::~MPCPutReadyMonitor()
{
	running_ = false;
	destroy();
}

void MPCPutReadyMonitor::destroy()
{
	if (thread_)
		thread_->join();

	delete thread_;
}

int MPCPutReadyMonitor::init()
{
	running_ = true;
	
	thread_ = new std::thread([&](MPCPutReadyMonitor* monitor) {
		//routine body
		while (this->running())
		{
			MpcTaskDispatcher* disp = MpcEngine::getInstance()->getTaskDispatcher();
			if (!disp)
			{
				LOGW("dispatcher is null, may terminated by system !");
				return;
			}

			//try to fetch a state == 0x03 task
			LOGI("to pop a invite and notify ok task ...");
			MPCTask task;
			disp->popPutTask(task);

			//put task to read task queue
			LOGI("to put ready task, with task: %s, type: %s", task.taskId.data(), task.invitor ? "alice" : "bob");
			disp->putReadyTask(task);

			LOGI("put ready task, with task: %s, type: %s ok", task.taskId.data(), task.invitor ? "alice" : "bob");
		}
		LOGW("running of put ready monitor done... exit!");
		
	}, this);

	return 0;
}

NS_PLATON_SDK_MPC_END