#include <cstdio>
using namespace std;

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

		printf("========= PlatON CGO INIT PARAMS ===========\n");
		printf("    icecfg: %s\n", icecfg);
		printf("       url: %s\n", url);
		printf("============================================\n");

		return ERR_NO_ERROR;
	}

	int notify_security_calculation(const char* taskid, const char* pubkey, const char* txaddr,
		const char* iraddr, const char* method, const char* extras) {

		printf("========= PlatON CGO CALC PARAMS ===========\n");
		printf("    taskid: %s\n", taskid);
		printf("    pubkey: %s\n", pubkey);
		printf("   address: %s\n", txaddr);
		printf("ir_address: %s\n", iraddr);
		printf("    method: %s\n", method);
		printf("     extra: %s\n", extras);
		printf("============================================\n");

		return ERR_NO_ERROR;
	}

	int main() {
		notify_security_init("icecfg", "url");
		notify_security_calculation("taskid", "pubkey", "address", "ir_address", "method", "extra");

		return 0;
	}
#if __cplusplus
}
#endif