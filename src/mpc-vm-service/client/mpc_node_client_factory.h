#pragma once

#include "mpc_node_client.h"
#include <mutex>
#include <map>

#include "mpc_vm_service.h"
NS_PLATON_SDK_MPC_BEG

class PLATONMPCVM_SERVICE_DLL_API MpcNodeClientFactory
{
public:
	MpcNodeClientFactory();
	~MpcNodeClientFactory();


	MpcNodeClient*    get(const platon::NodeCommunicateInfo& peerNodeInfo);
	int               remove(const platon::NodeCommunicateInfo& peerNodeInfo);

private:
	void clear();
	///MpcNodeClient*      getProxyClient(const platon::NodeCommunicateInfo& peerNodeInfo);
	MpcNodeClient*      getDirectClient(const platon::NodeCommunicateInfo& peerNodeInfo);

private:

	std::map<string, MpcNodeClient*>    m_directClients;
	std::map<string, MpcNodeClient*>    m_proxyClients;
	std::mutex                     m_mutex;
};

NS_PLATON_SDK_MPC_END
