#pragma once

#include "platon_mpc_service.h"

#include <string>
#include <map>

#include "mpc_common.h"
NS_PLATON_SDK_MPC_BEG

using ::platon::callback::TaskCallbackPrx;
using ::platon::callback::TransactionCallbackPrx;

class NodeChannelSessionImpl : public ::platon::service::NodeChannelSession {
public:
	virtual void notify(const ::platon::service::TaskParams&, const ::platon::callback::NodeCallbackPrx&, const ::Ice::Current& = ::Ice::Current());
	virtual void invite(const ::platon::service::TaskParams&, const ::platon::callback::NodeCallbackPrx&, const ::Ice::Current& = ::Ice::Current());
	virtual void ready(const ::platon::service::TaskParams&, const ::platon::callback::NodeCallbackPrx&, const ::Ice::Current& = ::Ice::Current());

	virtual int notify2(const ::platon::service::TaskParams&, const ::Ice::Current& = ::Ice::Current());
	virtual int invite2(const ::platon::service::TaskParams&, const ::Ice::Current& = ::Ice::Current());

	virtual void inputData(const ::std::string&, const ::std::string&, const ::platon::ByteList&, const ::Ice::Current& = ::Ice::Current());
	virtual void input(const ::platon::ByteList& data, const ::Ice::Current& current = ::Ice::Current());
	virtual void error(const ::std::string&, ::platon::MPCRole, const ::std::string&, ::platon::ErrorEnum, const ::Ice::Current& = ::Ice::Current());
};

NS_PLATON_SDK_MPC_END

