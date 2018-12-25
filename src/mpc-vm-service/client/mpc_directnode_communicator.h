#pragma once

#include <cstddef>
#include "platon_mpc_service.h"
#include "mpc_communicator.h"
#include <memory>

#include "mpc_node_callback_impl.h"

#define USE_NEW_SESSION 1


//using NodeSessionPrxPtr = ::std::shared_ptr<::service::NodeSessionPrx>;


#include "mpc_common.h"
NS_PLATON_SDK_MPC_BEG

class MpcDirectNodeCommunicator : public MpcCommunicator
{
public:
	MpcDirectNodeCommunicator(const platon::NodeCommunicateInfo& nodeCommunicateInfo);
	virtual ~MpcDirectNodeCommunicator();

	virtual int init();
	virtual void uninit();


	//should not implement
	virtual string invite(const string& roomId, const string& algorithmId, \
		const string& starter, const vector<string>& takerList, \
		const vector<string>& resultReceiverList, const vector<string>& args) {
		return std::string("");
	};

	//转发数据, toUser, 服务节点数据的去向, direct input
	virtual void forward(const string& taskId, const string& fromUser, const string& toUser, const vector<unsigned char>& data);

	//should not implement
	virtual void result(string taskId, int code, vector<unsigned char> data) {};

	virtual void notify(const ::platon::service::TaskParams& params, NodeCallbackFunc cb);
	virtual void invite(const ::platon::service::TaskParams& params, NodeCallbackFunc cb);
	virtual void ready(const ::platon::service::TaskParams& params, NodeCallbackFunc cb);

	virtual int notify(const ::platon::service::TaskParams& params);
	virtual int invite(const ::platon::service::TaskParams& params);


private:
#if USE_NEW_SESSION
	::platon::service::NodeChannelSessionPrx m_sessionPrx;
#else
	::service::NodeSessionPrx m_sessionPrx;
#endif

	Ice::CommunicatorPtr				m_communicator;

	MpcNodeCallbackImpl*				m_nodecallbackImpl;
	::platon::callback::NodeCallbackPrx m_nodecallbackReceiver;
};

NS_PLATON_SDK_MPC_END
