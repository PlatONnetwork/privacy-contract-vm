#pragma once

#include "platon_mpc_service.h"

#include "mpc_common.h"

NS_PLATON_SDK_MPC_BEG

class MpcTransactionCallbackImpl : public ::platon::callback::TransactionCallback {

	virtual ::std::string signTransaction(const ::std::string& transaction,
		const ::Ice::Current& current = ::Ice::Current());

	virtual ::std::string signTransaction2(const ::platon::callback::TransactionParams& tps,
		const ::Ice::Current& current = ::Ice::Current());

};

NS_PLATON_SDK_MPC_END
