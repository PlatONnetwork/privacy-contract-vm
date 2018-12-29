#include <cstdio>
#include <sstream>
#include <iostream>
using namespace std;

#include "platon_logger.h"

#include "platonvmsdk.h"
#include "vm_client.h"
using namespace platon::sdk::mpc;

// error code for return to platon
static const int ERR_NO_ERROR = 0;
static const int ERR_INIT_ENGINE = 1;
static const int ERR_COMMIT_TASK = 2;
static const int ERR_NOT_INIT = 3;


#if __cplusplus
extern "C" {
#endif

	int notify_security_init(const char* icecfg, const char* url) {
		stringstream sss;
		sss << ""
			<< "========= PlatON CGO INIT PARAMS ===========\n"
			<< "    icecfg: " << icecfg << std::endl
			<< "       url: " << url << std::endl
			<< "============================================\n";
		LOGI("%s", sss.str().c_str());

		VMClient* client = VMClient::getInstance();
		if (!client->Inited()) {
            LOGI("init vm client, cfg: %s...", icecfg);
			InitPara para;
			{
				para.icecfg = std::string(icecfg);
				para.url = std::string(url);
			}
			if (!client->init(para)) {
                LOGW("init the vm client failed.");
				return ERR_INIT_ENGINE;
			}
		}

        LOGW("init the vm client ok.");
		return ERR_NO_ERROR;
	}

	int notify_security_calculation(const char* taskid, const char* pubkey, const char* txaddr,
		const char* iraddr, const char* method, const char* extras) {
		stringstream sss;
		sss << ""
			<< "========= PlatON CGO CALC PARAMS ===========\n"
			<< "    taskid: " << taskid << std::endl
			<< "    pubkey: " << pubkey << std::endl
			<< "   address: " << txaddr << std::endl
			<< "ir_address: " << iraddr << std::endl
			<< "    method: " << method << std::endl
			<< "     extra: " << extras << std::endl
			<< "============================================\n";
		LOGI("%s", sss.str().c_str());


		VMClient* client = VMClient::getInstance();
		if (!client->Inited()) {
            LOGW("vm client is not init");
			return ERR_NOT_INIT;
		}

		VMPara para;
		{
			para.taskid = std::string(taskid);
			para.pubkey = std::string(pubkey);
			para.txaddr = std::string(txaddr);
			para.iraddr = std::string(iraddr);
			para.method = std::string(method);
			para.extras = std::string(extras);
		}
		if (!client->commit(para)) {
            LOGW("vm client commit failed");
			return ERR_COMMIT_TASK;
		}

        LOGI("commit a task: %s to mpc vm", taskid);
		return ERR_NO_ERROR;
	}


#if __cplusplus
}
#endif