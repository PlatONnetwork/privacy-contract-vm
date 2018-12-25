#pragma once

#include "platon_mpc_service.h"

#include <string>
#include <map>

#include "safemap.h"

#include "mpc_common.h"
NS_PLATON_SDK_MPC_BEG

using ::platon::callback::TaskCallbackPrx;
using ::platon::callback::TransactionCallbackPrx;

class TaskSessionImpl : public ::platon::service::TaskSession {
public:
	virtual ::platon::ErrorEnum registTransactionCallback(const ::std::string& user_address,
		const ::platon::callback::TransactionCallbackPrx& proxy, const ::Ice::Current& current = ::Ice::Current());
	virtual ::platon::ErrorEnum registerIR(const ::std::string& ir_hash,// only need user_address
		const ::platon::callback::TaskCallbackPrx& proxy, const ::Ice::Current& current = ::Ice::Current());

public:
	void setTransactionCB(const ::std::string& user_address, TransactionCallbackPrx& prx);
	void setTaskCB(const ::std::string& ir_hash, const ::std::string& method, TaskCallbackPrx& prx);
	bool isRegistered(const std::string& user_address);
	std::string getAddress();

private:
	std::string user_address = "";
	SafeMap<std::string, TransactionCallbackPrx> mapTransactionCB;
	SafeMap<std::string, TaskCallbackPrx> mapTaskCB;

};

NS_PLATON_SDK_MPC_END

