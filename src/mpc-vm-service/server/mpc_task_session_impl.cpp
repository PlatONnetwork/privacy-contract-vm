#include "mpc_task_session_impl.h"

#include <vector>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "platon_logger.h"

#include "mpc_engine.h"

NS_PLATON_SDK_MPC_BEG

::platon::ErrorEnum TaskSessionImpl::registTransactionCallback(const ::std::string& user_address,
	const ::platon::callback::TransactionCallbackPrx& proxy, const ::Ice::Current& current/* = ::Ice::Current()*/) {
	LOGW("##  user_address:%s, registTransactionCallback", user_address.c_str());
	mapTransactionCB.insert(user_address, proxy);
	this->user_address = user_address;

	//MpcEngine::getInstance()->getPlatonClient()->setActor(user_address);

	return ::platon::ErrorEnum::NoError;
}

::platon::ErrorEnum TaskSessionImpl::registerIR(const ::std::string& ir_hash,
	const ::platon::callback::TaskCallbackPrx& proxy, const ::Ice::Current& current/* = ::Ice::Current()*/) {
	LOGW("## ir_hash:%s, registerIR", ir_hash.c_str()); // also user_address
	mapTaskCB.insert(ir_hash, proxy);

	return ::platon::ErrorEnum::NoError;
}

void TaskSessionImpl::setTransactionCB(const ::std::string& user_address, TransactionCallbackPrx& prx) {
	if (0 != mapTransactionCB.getElem(user_address, prx)) {
		std::logic_error ex("TransactionCB not ok");
		throw std::exception(ex);
	}
}

void TaskSessionImpl::setTaskCB(const ::std::string& ir_hash, const ::std::string& method, TaskCallbackPrx& prx) {
	if (0 != mapTaskCB.getElem(ir_hash, prx)) {
		std::logic_error ex("TaskCB not ok");
		throw std::exception(ex);
	}
}
bool TaskSessionImpl::isRegistered(const std::string& user_address) {
	TransactionCallbackPrx x;
	return (0 == mapTransactionCB.getElem(user_address, x));
}
std::string TaskSessionImpl::getAddress() {
	return user_address;
}

NS_PLATON_SDK_MPC_END
