
#include "mpc_client.h"

using namespace platon::sdk::mpc;

int main(int argc, char* argv[]) {
	std::string config_path = "E:/work/local/dev-mmpc-llvm/juzix-mpc-toolkit/mpc-vm-service/slice/cfg.client.config";
	MpcClient app;
	return app.main(argc, argv, config_path.c_str());
}