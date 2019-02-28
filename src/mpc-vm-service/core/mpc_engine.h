#pragma once

#include <cstddef>

#include "platon_client.h"
#include "mpc_node_servant.h"
#include "mpc_task_dispatcher.h"
#include "mpc_node_client_factory.h"
#include "mpc_p2p_task_ready_processor.h"

#include <libdevcore/Function.h>
#include "mpc_put_ready_monitor.h"
#include "mpc_vm_service.h"

using std::set;
using std::map;
using std::string;
using std::vector;

NS_PLATON_SDK_MPC_BEG


class PLATONMPCVM_SERVICE_DLL_API MpcEngine
{
public:
	enum MpcModeType
	{
		SERVER_NODE_MODE_CALLBACK = 0,
		SERVER_NODE_MODE_SERVICE = 1
	};

private:
	MpcEngine();
	MpcEngine(const MpcEngine& obj) = delete;
	MpcEngine& operator=(const MpcEngine& obj) = delete;

public:
	virtual ~MpcEngine();

	static MpcEngine* getInstance();
	static void unload();

	int init(
		const string& _iceCfg,
		const string& _user,
		const string& _password,
		platon::NodeCommunicateMode _mode,
		const string& _proxyEndpoint,
		const vector<string>& _args,
		const string& _platonUrl
	);


	void destroy();

	int cacheIR(const std::string& ir_data, std::string& ir_hash, std::string& ir_path);
	int getAllInfo(MPCTask& task);
	int commit(MPCTask& task);

	MpcNodeClient*					getNodeClient(const platon::NodeCommunicateInfo& nodeinfo);
	MpcTaskDispatcher*				getTaskDispatcher() { return m_taskDispatcher; };
	MpcP2pTaskReadyProcessor*		getTaskReadyProcessor() { return m_readyTaskProcessor; };

	string getUser() const { return m_user; };
	string getPassword() const { return m_password; };
	int getMode() const { return m_mode; };
	string getProxyEndpoint() const { return m_proxyEndpoint; };
	string getMyEndpoint();
	vector<string> getArgs() const { return m_args; };

	bool isDataLogEnable() const { return m_dataLogFlag; };
	void enableDataLog(bool flag) { m_dataLogFlag = flag; }

	string getMpcDataLogDir() const { return m_mpcDatDirectory; };
	void setMpcDataLogDir(const string& dir) { m_mpcDatDirectory = dir; };

	int getWorkerNum();

    const string getPlatonUrl() const { return m_platonUrl; }
	PlatonClient* getPlatonClient() { return m_platonClient; }
	MpcNodeServant* getMpcNodeServant2() { return m_mpcNodeServer2; }

	SafeMap<std::string, long>& getUserNonce() { return m_userNonce; }

private:
	string							m_user;
	string							m_password;
	int								m_mode;
	string							m_proxyEndpoint;
	string							m_mpcDatDirectory;//to store mpc_taskidxxxx_userid.data
    string                          m_platonUrl;
	vector<string>					m_args;
	bool							m_dataLogFlag = false;
	volatile bool					m_inited = false;
	string							m_irDataDir;
	set<string>						m_irDataHashs;



	MpcNodeClientFactory*			m_nodeClientFactory = nullptr;
	MpcTaskDispatcher*				m_taskDispatcher = nullptr;
	MpcP2pTaskReadyProcessor*		m_readyTaskProcessor = nullptr;
	MpcNodeServant*					m_mpcNodeServer2 = nullptr;
	PlatonClient*					m_platonClient = nullptr;
	MPCPutReadyMonitor*				m_readyMonitor = nullptr;
	vector<dev::func_info_st>		m_FuncInfos;
	map<string, dev::func_info_st>	m_mapFuncInfos; // funcName -> funcInfo

	SafeMap<std::string, long>		m_userNonce; // user_address -> nonce

    std::condition_variable         m_jrpcErrorCV;
    std::mutex                      m_jrpcErrorMtx;
};

NS_PLATON_SDK_MPC_END

