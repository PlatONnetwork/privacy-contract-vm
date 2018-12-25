
#include <chrono>
#include <thread>

#include "mpc_node_servant.h"
#include "platon_logger.h"
#include "mpc_task_session_impl.h"
#include "mpc_node_channel_session_impl.h"
#include "sys_utils_tool.h"

NS_PLATON_SDK_MPC_BEG

MpcNodeServant::MpcNodeServant()
{
}

MpcNodeServant::~MpcNodeServant()
{
	stop();
}

void MpcNodeServant::stop()
{
	LOGI("to stop the node server...");
	communicator()->shutdown();
	if (m_thread && m_thread->joinable())
	{
		m_thread->join();
	}

	if (m_thread)
	{
		delete m_thread;
		m_thread = nullptr;
	}

	m_running = false;
	LOGI("node server has stopped 1");
}

int MpcNodeServant::init(const string& iceConf, const string &user, bool direct, const vector<string>& args)
{
	int try_times = 10;

	if (running() || m_thread)
	{
		LOGW("node server is running, no need to INIT again !");
		return -1;
	}

	m_user = user;
	m_iceConf = iceConf;
	m_args = args;
	m_direct = direct;

	if (!platon::mpc::SysUtilsTool::checkFileExists(iceConf)) {
		LOGW("ice config file is not exist! %s", iceConf.c_str());

		if (m_args.size() == 0) {
			m_args.push_back("--MpcNode.Server.Endpoints=default -h 0.0.0.0 -p 8201");
			m_args.push_back("--Ice.Warn.Connections=1");
		}
		LOGI("use default config:");
		for (auto& arg : m_args) {
			LOGI("    %s", arg.c_str());
		}
		{
			Ice::InitializationData initData;
			initData.properties = Ice::createProperties();
			initData.properties->setProperty("MpcNode.Server.Endpoints", "default -h 0.0.0.0 -p 8201");
			initData.properties->setProperty("Ice.Warn.Connections", "1");

			//Ice::initialize(initData);
		}
		m_iceConf = "";
	}

	{
		LOGI("node server to init...");
		m_thread = new std::thread([&](MpcNodeServant* servant, const string& conf, const vector<string>& args) {
			LOGI("run the mpc vm server, config: %s, args size: %d", conf.data(), args.size());
			int ret = -1;
			if (args.empty())
				ret = servant->main(args, conf.data());
			else
				ret = servant->main(args);

			LOGI("the mpc vm server die! ret: %d", ret);
		}, this, iceConf, m_args);
	}

	{
		std::unique_lock<decltype(m_mutex)> lock{ m_mutex };
		while (!m_cv.wait_for(lock, std::chrono::seconds(1), [this] { return m_running; }))
		{
			LOGI("waiting for node server inited ... timeout 1s.. try %d ", try_times);
			try_times--;
			if (try_times < 1)
			{
				LOGI("timeout for node server inited ! exit");
				return -1;
			}
		}
	}

	LOGI("node server INIT ok. with user: %s, proxy endpoint: %s", user.data(), m_endpoint.data());
	return 0;
}

int MpcNodeServant::run(int argc, char* argv[])
{
	{
		{
			std::lock_guard<decltype(m_mutex)> guard{ m_mutex };

			Ice::ObjectAdapterPtr adapter;
			if (m_iceConf == "") {
				adapter = communicator()->createObjectAdapterWithEndpoints("MpcNode.Server", "default -h 0.0.0.0 -p 8201");
			}
			else {
				adapter = communicator()->createObjectAdapter("MpcNode.Server");
			}

			// servant, between node and data-provider
			m_taskSession = new TaskSessionImpl();
			adapter->add(m_taskSession, communicator()->stringToIdentity("tasksession"));

			// servant, between node and node, for mpc calculation
			adapter->add(new NodeChannelSessionImpl(), Ice::stringToIdentity("DirectNodeServer"));

			// add more servants here

			adapter->activate();

			m_running = true;
			m_cv.notify_one();
		}

		LOGI("node server is running ...");
		communicator()->waitForShutdown();

		LOGI("node server is exited.");
		m_running = false;

		return 0;
	}

	return 0;
}

NS_PLATON_SDK_MPC_END

