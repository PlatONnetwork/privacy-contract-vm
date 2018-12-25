#include "mpc_node_channel_session_impl.h"

#include <vector>
#include <thread>
#include <chrono>

#include "mpc_task.h"
#include "mpc_engine.h"
#include "platon_logger.h"

NS_PLATON_SDK_MPC_BEG

void NodeChannelSessionImpl::invite(const ::platon::service::TaskParams& params,
	const ::platon::callback::NodeCallbackPrx& proxy, const ::Ice::Current& current/* = ::Ice::Current()*/) {

	MPCTask task(
		params.taskid,
		params.pubkey,
		params.txaddr,
		params.iraddr,
		params.method,
		params.extras
	);
	
}

void NodeChannelSessionImpl::notify(const ::platon::service::TaskParams& params,
	const ::platon::callback::NodeCallbackPrx& proxy, const ::Ice::Current& current/* = ::Ice::Current()*/) {

}

void NodeChannelSessionImpl::ready(const ::platon::service::TaskParams& params,
	const ::platon::callback::NodeCallbackPrx& proxy, const ::Ice::Current& current/* = ::Ice::Current()*/) {
	std::cout << __FUNCTION__ << std::endl;

	proxy->ready(params.taskid, 0);
}

int NodeChannelSessionImpl::notify2(const ::platon::service::TaskParams& task, const ::Ice::Current& c)
{
	LOGI("node peer notify2 coming ..., task: %s", task.taskid.data());
	MpcTaskDispatcher *disp = MpcEngine::getInstance()->getTaskDispatcher();

	int status = 0x02;//state code:   alice: 0x01 bob: 0x02
	int st = 0;
	if (0 == disp->findTaskState(task.taskid, st) && (st & 0x01))
	{
		st |= status;
	}
	else
	{
		st = status;
	}

	disp->updateTaskState(task.taskid, st);

	return st;
}

int NodeChannelSessionImpl::invite2(const ::platon::service::TaskParams& task, const ::Ice::Current& c)
{
	LOGI("node peer invite2 coming ..., task: %s", task.taskid.data());
	MpcTaskDispatcher *disp = MpcEngine::getInstance()->getTaskDispatcher();

	int status = 0x01;//state code:   alice: 0x01 bob: 0x02
	int st = 0;
	if (0 == disp->findTaskState(task.taskid, st) && (st & 0x02))
	{
		st |= status;
	}
	else
	{
		st = status;
	}

	disp->updateTaskState(task.taskid, st);
	
	return st;
}
void NodeChannelSessionImpl::inputData(const ::std::string& taskId, const ::std::string& fromUser,
	const ::platon::ByteList& data, const ::Ice::Current& current/* = ::Ice::Current()*/) {

	std::cout << __FUNCTION__ << " receive data from peer node task:" << taskId
		<< " user:" << fromUser << " data size:" << data.size() << std::endl;

	MpcBufferPtr buffer = new MpcBuffer((uint8_t*)data.data(), data.size() - sizeof(int));
	buffer->setFrom(fromUser);
	buffer->setTaskId(taskId);
	memcpy(&(buffer->seq), data.data() + data.size() - sizeof(int), sizeof(int));//msg seq

	MpcEngine::getInstance()->getTaskDispatcher()->putBuffer(taskId, buffer);
}

void NodeChannelSessionImpl::input(const ::platon::ByteList& data,
	const ::Ice::Current& current/* = ::Ice::Current()*/) {

}

void NodeChannelSessionImpl::error(const ::std::string&, ::platon::MPCRole,
	const ::std::string&, ::platon::ErrorEnum, const ::Ice::Current& current/* = ::Ice::Current()*/) {

}


NS_PLATON_SDK_MPC_END
