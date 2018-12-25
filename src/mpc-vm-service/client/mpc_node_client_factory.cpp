#include "mpc_node_client_factory.h"
#include "mpc_engine.h"
#include "platon_logger.h"
#include "mpc_directnode_client.h"
///#include "MpcProxyNodeClient.h"
#include "platon_mpc_service.h"

using std::map;

NS_PLATON_SDK_MPC_BEG

MpcNodeClientFactory::MpcNodeClientFactory()
{
}

MpcNodeClientFactory::~MpcNodeClientFactory()
{
	clear();
}

MpcNodeClient*
MpcNodeClientFactory::get(const platon::NodeCommunicateInfo& peerNodeInfo)
{
	if (peerNodeInfo.user.empty() || peerNodeInfo.endpoint.empty())
	{
		LOGE("bad nodeinfo, please check, user: %s, endpoint: %s", peerNodeInfo.endpoint.data());
		return nullptr;
	}

	if (peerNodeInfo.mode == ::platon::NodeCommunicateMode::MSERVICE)//direct
	{
		return getDirectClient(peerNodeInfo);
	}
	///else if (peerNodeInfo.mode == ::platon::NodeCommunicateMode::MCALLBACK)//proxy
	///{
	///	return getProxyClient(peerNodeInfo);
	///}
	else
	{
		LOGE("nodeinfo of bad mode: %d !", static_cast<int>(peerNodeInfo.mode));
		return nullptr;
	}
}
///
///MpcNodeClient*
///MpcNodeClientFactory::getProxyClient(const platon::NodeCommunicateInfo& peerNodeInfo)
///{
///	std::lock_guard<std::mutex> lock(m_mutex);
///	MpcNodeClient* client = nullptr;
///	map<string, MpcNodeClient*>::iterator iter = m_proxyClients.find(peerNodeInfo.user);
///	if (iter == m_proxyClients.end())
///	{
///		MpcEngine* engine = MpcEngine::getInstance();
///		string user = peerNodeInfo.user;
///		string password = peerNodeInfo.password;
///		int mode = static_cast<int>(peerNodeInfo.mode);
///		string endpoint = engine->getProxyEndpoint();
///		vector<string> args = engine->getArgs();
///
///		client = new MpcProxyNodeClient();
///		if (0 != client->init(user, password, mode, endpoint, args))
///		{
///			LOGE("construct a proxy client for user: %s, endpoint: %s failed !", user.data(), endpoint.data());
///			delete client;
///			return nullptr;
///		}
///
///		m_proxyClients[user] = client;
///		LOGI("insert a new proxy client, user: %s, endpoint: %s", user.data(), endpoint.data());
///
///	}
///	else
///		client = iter->second;
///
///	return client;
///}
///

MpcNodeClient*
MpcNodeClientFactory::getDirectClient(const platon::NodeCommunicateInfo& peerNodeInfo)
{
	std::lock_guard<std::mutex> lock(m_mutex);

	MpcNodeClient* client = nullptr;
	map<string, MpcNodeClient*>::iterator iter = m_directClients.find(peerNodeInfo.user);
	if (iter == m_directClients.end())
	{
		MpcEngine* engine = MpcEngine::getInstance();
		string user = peerNodeInfo.user;
		string password = peerNodeInfo.password;
		int mode = static_cast<int>(peerNodeInfo.mode);
		string endpoint = peerNodeInfo.endpoint;//
		vector<string> args = engine->getArgs();

		client = new MpcDirectNodeClient();
		if (0 != client->init(user, password, mode, endpoint, args))
		{
			LOGE("construct a proxy client for user: %s, endpoint: %s failed !", user.data(), endpoint.data());
			delete client;
			return nullptr;
		}

		m_directClients[user] = client;
		LOGI("insert a new direct client, user: %s, endpoint: %s", user.data(), endpoint.data());
	}
	else
		client = iter->second;

	return client;
}


int
MpcNodeClientFactory::remove(const platon::NodeCommunicateInfo& peerNodeInfo)
{
	std::lock_guard<std::mutex> lock(m_mutex);
	if (peerNodeInfo.mode == ::platon::NodeCommunicateMode::MSERVICE)//direct
	{
		MpcNodeClient* client = nullptr;
		m_proxyClients.erase(peerNodeInfo.user);
		delete client;
		LOGW("remove nodeinfo of user: %s failed !", peerNodeInfo.user.data());
	}
	else if (peerNodeInfo.mode == ::platon::NodeCommunicateMode::MCALLBACK)//proxy
	{
		MpcNodeClient* client = nullptr;
		m_directClients.erase(peerNodeInfo.user);
		delete client;
		LOGW("remove nodeinfo of user: %s failed !", peerNodeInfo.user.data());
	}
	else
	{
		LOGW("bad nodeinfo to remove, mode: %d, user: %s", static_cast<int>(peerNodeInfo.mode), peerNodeInfo.user.data());
		return -1;
	}

	return 0;
}

void
MpcNodeClientFactory::clear()
{
	std::lock_guard<std::mutex> lock(m_mutex);
	std::map<string, MpcNodeClient*>::iterator iter = m_directClients.begin();
	std::map<string, MpcNodeClient*>::iterator iterProxy = m_proxyClients.begin();
	for (; iter != m_directClients.end(); ++iter)
	{
		delete iter->second;
		iter->second = nullptr;
	}
	m_directClients.clear();

	for (; iterProxy != m_proxyClients.end(); ++iterProxy)
	{
		delete iterProxy->second;
		iterProxy->second = nullptr;
	}
	m_proxyClients.clear();
}

NS_PLATON_SDK_MPC_END
