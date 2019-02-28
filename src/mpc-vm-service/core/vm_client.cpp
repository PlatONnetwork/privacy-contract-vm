#include <cassert>
using namespace std;

#include "vm_client.h"

#include "mpc_engine.h"
#include "type_utils.h"
#include "platon_logger.h"


NS_PLATON_SDK_MPC_BEG

VMClient* VMClient::getInstance() {
	static VMClient vmclient;
	return &vmclient;
}

bool VMClient::init(const InitPara& para) {
	std::unique_lock<std::mutex> lock{ mtx };
	if (inited) return true;

	string icecfg(para.icecfg);
	string user(para.user);
	string pass(para.pass);
	string url(para.url); // platon url for send transaction

	platon::NodeCommunicateMode mode = platon::NodeCommunicateMode::MSERVICE;
	string proxyEndpoint = "127.0.0.1:10001";
	vector<string> args;

	MpcEngine* engine = MpcEngine::getInstance();
	if (0 != engine->init(
		icecfg,
		user,
		pass,
		mode,
		proxyEndpoint,
		args,
		url
	)) {
		std::cout << "init engine failed !!!!! exit !!!" << std::endl;
		engine->destroy();
		return false;
	}

	inited = true;

	return true;
}
bool VMClient::commit(const VMPara para) {
	LOGI("PlatON call vm cnt: %llu taskid: %s\n", call_cnt.fetch_add(1), para.taskid.c_str());

	{
		MPCTask task(
		para.taskid,
		para.pubkey,
		para.txaddr,
		para.iraddr,
		para.method,
		para.extras);

		MpcEngine* engine = MpcEngine::getInstance();
		if (0 != engine->commit(task)) {
            LOGW("commit task to engine failed !!!");
			return false;
		}
	}
	return true;
}

NS_PLATON_SDK_MPC_END
