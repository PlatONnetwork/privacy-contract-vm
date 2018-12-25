#pragma once

#include <Ice/Ice.h>

#include "mpc_common.h"

#include "mpc_sdk_client.h"

NS_PLATON_SDK_MPC_BEG


class MpcClientInner :public Ice::Application {
	virtual int run(int argc, char* argv[]);
};


class PLATONMPCVM_SDK_DLL_API MpcClient {
public:
	int main(int argc, char* argv[], const char* cfg) {
		return client.main(argc, argv, cfg);
	}
private:
	MpcClientInner client;
};

NS_PLATON_SDK_MPC_END

