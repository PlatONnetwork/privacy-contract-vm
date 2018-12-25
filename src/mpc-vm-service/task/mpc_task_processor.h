#pragma once

#include <cstddef>

#include <map>
#include <queue>
#include <mutex>
#include <thread>
#include <atomic>
#include <functional>

#include "mpc_task.h"
#include "mpc_buffer.h"
#include "safequeue.h"
#include "safemap.h"


using platon::mpc::MpcBuffer;
using platon::mpc::MpcBufferPtr;

#define         JUZIX_MAX_QUEUE_SIZE            1000

#include "mpc_vm_service.h"
NS_PLATON_SDK_MPC_BEG

//processor will only process a task, no task queue
class PLATONMPCVM_SERVICE_DLL_API MpcTaskProcessor
{
public:
	enum ProcessorType
	{
		InvitorProcessor = 0,
		InviteeProcessor = 1
	};

public:
	MpcTaskProcessor(int id, int type = InvitorProcessor);
	~MpcTaskProcessor();

	int init(int max_queue = JUZIX_MAX_QUEUE_SIZE);

	void destory();

	int processTask();

	void inputBuffer(MpcBufferPtr buffer);
	int consumeBuffer(MpcBufferPtr &buffer, int timeout = 20);

	bool running() { return m_running; };

	int getBufferCount();

	int getID() { return m_id; };

	const string getTaskId();

	MPCTask& getTask() { return m_task; };

private:
	int processMpcBusi(MPCTask& task);
	int fetchTask(MPCTask& task);

private:
	int									m_id;
	int									m_type;
	std::atomic<int32_t> 				m_atomicBufferSize;

	volatile bool						m_running = false;
	std::thread*						m_thread;

	//buffer queue
	MPCTask								m_task;

	std::mutex							m_bufferMutex;
	std::condition_variable				m_cv;
	std::map<int, MpcBufferPtr>			m_taskBufferQue;
	std::atomic<int32_t>				m_bufferSeq;

	//std::map<int, MpcBuffer>			m_futureTaskBuffer;
	std::atomic<int32_t>				m_requestSeq;
};

NS_PLATON_SDK_MPC_END
