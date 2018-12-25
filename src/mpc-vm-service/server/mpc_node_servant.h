#pragma once

#include <Ice/Ice.h>
#include "mpc_task_session_impl.h"

#include <mutex>
#include <thread>
#include <condition_variable>
#include <vector>
#include <string>
#include <cstddef>

using std::string;
using std::vector;


#include "mpc_vm_service.h"
NS_PLATON_SDK_MPC_BEG

class PLATONMPCVM_SERVICE_DLL_API MpcNodeServant : public Ice::Application
{
public:
	MpcNodeServant();
	~MpcNodeServant();

public:
	int init(const string& iceConf, const string& user, bool direct, const vector<string>& args);
	virtual int run(int, char*[]);

public:
	void stop();
	bool running() const { return m_running; }
	string getStartEndpoint() const { return m_endpoint; }
    string getPublicEndpoint() const { return m_publicEndpoint; }
    TaskSessionImpl* getTaskSession() const { return m_taskSession; }

private:
	volatile bool				m_running = false;
	std::thread*                m_thread = nullptr;

	string                      m_user;
	vector<string>              m_args;

	bool                        m_direct = true;

	std::mutex					m_mutex;
	std::condition_variable		m_cv;

public:
	string                      m_iceConf;
	string                      m_endpoint;
	string                      m_publicEndpoint;

	TaskSessionImpl*			m_taskSession;
};

NS_PLATON_SDK_MPC_END
