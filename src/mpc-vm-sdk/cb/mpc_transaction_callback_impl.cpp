#include "mpc_transaction_callback_impl.h"

NS_PLATON_SDK_MPC_BEG


::std::string MpcTransactionCallbackImpl::signTransaction(const ::std::string& transaction,
	const ::Ice::Current& current/* = ::Ice::Current()*/) {
	return std::string("");
}

::std::string MpcTransactionCallbackImpl::signTransaction2(const ::platon::callback::TransactionParams& tps,
	const ::Ice::Current& current/* = ::Ice::Current()*/) {
	return std::string("");
}

NS_PLATON_SDK_MPC_END
