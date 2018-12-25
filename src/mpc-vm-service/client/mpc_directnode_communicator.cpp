#include "mpc_directnode_communicator.h"
#include "platon_logger.h"
#include <mutex>

#include "mpc_node_callback_impl.h"

NS_PLATON_SDK_MPC_BEG

//static std::mutex sMutexSend;

MpcDirectNodeCommunicator::MpcDirectNodeCommunicator(
	const platon::NodeCommunicateInfo& nodeCommunicateInfo) : \
	MpcCommunicator(nodeCommunicateInfo)
{
}

MpcDirectNodeCommunicator::~MpcDirectNodeCommunicator()
{
	uninit();
}


int MpcDirectNodeCommunicator::init()
{
	if (m_inited >= 2)
	{
		LOGE("already INITED MpcDirectNodeCommunicator.");
		return -1;
	}

	Ice::InitializationData initData;
	initData.properties = Ice::createProperties();

	//if (m_nodeCommunicateInfo.endpoint.find("DirectNodeServer") == string::npos)
	//	m_nodeCommunicateInfo.endpoint.insert(0, "DirectNodeServer:");

	initData.properties->setProperty("Opposite.Proxy", m_nodeCommunicateInfo.endpoint);
	initData.properties->setProperty("Ice.ThreadPool.Client.Size", "2");
	initData.properties->setProperty("Ice.ThreadPool.Client.SizeMax", "2");
	initData.properties->setProperty("Ice.ACM.Close", "0");
	initData.properties->setProperty("Ice.ACM.Heartbeat", "3");
	initData.properties->setProperty("Ice.ACM.Timeout", "10");

	LOGI("Ice::initialize .... the service endpoint: %s", m_nodeCommunicateInfo.endpoint.data());
	m_communicator = Ice::initialize(initData);
	Ice::ObjectPrx base = m_communicator->propertyToProxy("Opposite.Proxy");
	LOGI("to create session client prx ....");
#if USE_NEW_SESSION
	m_sessionPrx = ::platon::service::NodeChannelSessionPrx::uncheckedCast(base)->ice_twoway();
#else
	m_sessionPrx = service::NodeSessionPrx::uncheckedCast(base)->ice_oneway();
#endif

	////////////////////////////////////
	{
		std::string endpoint_cb = "default -h localhost";
		LOGI("to create object adapter for callback: %s ....", endpoint_cb.data());
		Ice::ObjectAdapterPtr adapter = m_communicator->createObjectAdapterWithEndpoints("x", endpoint_cb); //m_communicator->createObjectAdapterWith("NodeCallback.Client");
		m_nodecallbackImpl = new MpcNodeCallbackImpl;
		adapter->add(m_nodecallbackImpl, m_communicator->stringToIdentity("NodeCallbackReceiver"));
		adapter->activate();
		m_nodecallbackReceiver = ::platon::callback::NodeCallbackPrx::uncheckedCast(
			adapter->createProxy(m_communicator->stringToIdentity("NodeCallbackReceiver")));
	}
	////////////////////////////////////

	setState(2);
	LOGI("direct MpcDirectNodeCommunicator INIT done, endpoint to: %s .", m_nodeCommunicateInfo.endpoint.data());

	return 0;
}

void MpcDirectNodeCommunicator::uninit()
{
	LOGI("direct MpcDirectNodeCommunicator unINIT done, endpoint to: %s .", m_nodeCommunicateInfo.endpoint.data());
	m_communicator->destroy();
	setState(0);
}


//direct input
void MpcDirectNodeCommunicator::forward(const string& taskId, const string& fromUser, const string& toUser, const vector<unsigned char>& data)
{
	//LOGD("==> do forward input of task: %s, data size: %d, fromUser: %s ", taskId.data(), data.size(), fromUser.data());
	m_sessionPrx->begin_inputData(taskId, fromUser, data);
	//m_sessionPrx->inputData(taskId, fromUser, data);
}

void MpcDirectNodeCommunicator::notify(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {
	LOGD("#### client notify, task: %s ...", params.taskid.data());
	m_nodecallbackImpl->setNotifyCB(params.taskid, cb);
	m_sessionPrx->begin_notify(params, m_nodecallbackReceiver);
}

void MpcDirectNodeCommunicator::invite(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {
	LOGE("#### client invite, task: %s ...", params.taskid.data());
	m_nodecallbackImpl->setInviteCB(params.taskid, cb);
	m_sessionPrx->begin_invite(params, m_nodecallbackReceiver);
}
void MpcDirectNodeCommunicator::ready(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {
	m_nodecallbackImpl->setReadyCB(params.taskid, cb);
	m_sessionPrx->begin_ready(params, m_nodecallbackReceiver);
}


int MpcDirectNodeCommunicator::notify(const ::platon::service::TaskParams& params) {
	LOGD("#### client notify2, task: %s ...", params.taskid.data());
	return m_sessionPrx->notify2(params);
}

int MpcDirectNodeCommunicator::invite(const ::platon::service::TaskParams& params) {
	LOGD("#### client invite2, task: %s ...", params.taskid.data());
	return m_sessionPrx->invite2(params);
}
NS_PLATON_SDK_MPC_END
