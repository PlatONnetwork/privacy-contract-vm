#ifndef _MPC_PUT_READY_MONITOR_H_
#define _MPC_PUT_READY_MONITOR_H_

#include "mpc_task.h"
#include "mpc_vm_service_define.h"
#include <thread>

NS_PLATON_SDK_MPC_BEG


class PLATONMPCVM_SERVICE_DLL_API MPCPutReadyMonitor
{
public:
	MPCPutReadyMonitor();
	~MPCPutReadyMonitor();

	void destroy();
	bool running() { return running_; }

	int init();

private:
	std::thread* thread_;
	bool volatile running_;

};

NS_PLATON_SDK_MPC_END


#endif//_MPC_PUT_READY_MONITOR_H_