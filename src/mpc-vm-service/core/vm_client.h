#pragma once

#include <string>
#include <vector>
#include <random>
#include <atomic>
#include <mutex>
#include "mpc_vm_service.h"


using std::string;
using std::atomic;
using std::vector;

NS_PLATON_SDK_MPC_BEG

struct InitPara {
	string icecfg;
	string user;
	string pass;
	string url;
};

struct PeerInfo {
	string user;
	string pass;
	string host;
	int    port;
};

struct VMPara {
	string taskid;
	string pubkey;
	string txaddr;
	string iraddr;
	string method;
	string extras;

	string irhash;
	vector<PeerInfo> pinfos;
};

class PLATONMPCVM_SERVICE_DLL_API VMClient {

public:
	VMClient() :call_cnt(0) {}
	static VMClient* getInstance();

public:
	bool init(const InitPara& para);
	bool commit(const VMPara para);

public:
	bool Inited() const { return inited; }

private:
	bool inited = false;
	atomic<uint64_t> call_cnt; // platon call vm counts
	mutex mtx;
};

NS_PLATON_SDK_MPC_END
