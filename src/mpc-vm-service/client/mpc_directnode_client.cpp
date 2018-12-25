#include "mpc_directnode_communicator.h"
#include "mpc_directnode_client.h"
#include "platon_logger.h"



NS_PLATON_SDK_MPC_BEG

MpcDirectNodeClient::MpcDirectNodeClient() : MpcNodeClient(MpcNodeClient::NODE_DIRECT)
{

}

MpcDirectNodeClient::~MpcDirectNodeClient()
{
	if (m_com)
	{
		delete m_com;
		m_com = nullptr;
	}
}

int MpcDirectNodeClient::init_impl(const string& user, const string& password, int mode, const string& proxyEndpoint, const vector<string>& args)
{
	platon::NodeCommunicateInfo nodeCommunicateInfo;
	nodeCommunicateInfo.endpoint = proxyEndpoint;
	nodeCommunicateInfo.user = user;
	nodeCommunicateInfo.password = password;
	nodeCommunicateInfo.mode = (0 == mode ? ::platon::NodeCommunicateMode::MCALLBACK : ::platon::NodeCommunicateMode::MSERVICE);

	m_com = new MpcDirectNodeCommunicator(nodeCommunicateInfo);
	if (0 != m_com->init())
	{
		LOGE("INIT direct MpcDirectNodeCommunicator failed ");
		return -1;
	}
	m_inited = 2;

	LOGI("INIT core direct node client success.");
	return 0;
}

void MpcDirectNodeClient::forward(const string& taskId, const string& fromUser, const string& toUser, const vector<unsigned char>& data)
{
	if (!m_com)
	{
		LOGE("not INIT core mpc client !");
		return;
	}

	//LOGD("%s send message to peer: %s, msgSize: %d", fromUser.data(), toUser.data(), data.size());
	m_com->forward(taskId, fromUser, toUser, data);
}

void MpcDirectNodeClient::notify(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {
	if (!m_com) {
		LOGE("not INIT core mpc client !");
		return;
	}
	m_com->notify(params, cb);
}
void MpcDirectNodeClient::invite(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {
	if (!m_com) {
		LOGE("not INIT core mpc client !");
		return;
	}
	m_com->invite(params, cb);
}

void MpcDirectNodeClient::ready(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {
	if (!m_com) {
		LOGE("not INIT core mpc client !");
		return;
	}
	m_com->ready(params, cb);
}

int MpcDirectNodeClient::notify(const ::platon::service::TaskParams& params) {
	if (!m_com) {
		LOGE("not INIT core mpc client !");
		return -1;
	}

	return m_com->notify(params);
}

int MpcDirectNodeClient::invite(const ::platon::service::TaskParams& params) {
	if (!m_com) {
		LOGE("not INIT core mpc client !");
		return -1;
	}

	return m_com->invite(params);
}

NS_PLATON_SDK_MPC_END