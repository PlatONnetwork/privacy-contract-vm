#include "mpc_client.h"

#include "platon_mpc_service.h"
using namespace ::platon::service;
using namespace ::platon::callback;


#include "mpc_transaction_callback_impl.h"
#include "mpc_task_callback_impl.h"


NS_PLATON_SDK_MPC_BEG

int MpcClientInner::run(int argc, char* argv[]) {
	{
		TaskSessionPrx server = TaskSessionPrx::checkedCast(
			communicator()->propertyToProxy("TaskCallback.Proxy")
			->ice_twoway()->ice_timeout(-1)->ice_secure(false));
		if (!server) {
			std::cerr << appName() << ": invalid proxy" << std::endl;
			return EXIT_FAILURE;
		}

		///
		Ice::ObjectAdapterPtr adapter = communicator()->createObjectAdapter("Callback.Client");

		adapter->add(new MpcTransactionCallbackImpl, communicator()->stringToIdentity("TransactionCallbackReceiver"));
		adapter->add(new MpcTaskCallbackImpl, communicator()->stringToIdentity("TaskCallbackReceiver"));
		adapter->activate();

		TransactionCallbackPrx transactionReceiver = TransactionCallbackPrx::uncheckedCast(
			adapter->createProxy(communicator()->stringToIdentity("TransactionCallbackReceiver")));
		TaskCallbackPrx taskReceiver = TaskCallbackPrx::uncheckedCast(
			adapter->createProxy(communicator()->stringToIdentity("TaskCallbackReceiver")));
		///

		server->registTransactionCallback("", transactionReceiver);
		server->registerIR("", taskReceiver);
	}

	return EXIT_SUCCESS;
}

int mainTestMpcClient(int argc, char* argv[]) {
	std::string config_path = "E:/work/local/dev-mmpc-llvm/juzix-mpc-toolkit/juzix-mpc-sdk2/slice/cfg.client.config";
	MpcClientInner app;
	return app.main(argc, argv, config_path.c_str());
}

NS_PLATON_SDK_MPC_END

