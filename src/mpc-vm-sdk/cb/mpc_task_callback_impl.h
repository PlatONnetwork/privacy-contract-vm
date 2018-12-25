#pragma once

#include "platon_mpc_service.h"

#include "mpc_common.h"

NS_PLATON_SDK_MPC_BEG

class MpcTaskCallbackImpl : public ::platon::callback::TaskCallback {

	virtual void error(const ::platon::callback::MpcRequest& request, ::platon::ErrorEnum error,
		const ::Ice::Current& current = ::Ice::Current());
	virtual ::platon::ErrorEnum inputData(const ::platon::callback::MpcRequest& request,
		::platon::ByteList& data, ::platon::ByteList& data_default, const ::Ice::Current& current = ::Ice::Current());
	virtual void result(const ::platon::callback::MpcRequest& request,
		const ::platon::ByteList& data, const ::Ice::Current& current = ::Ice::Current());
};

NS_PLATON_SDK_MPC_END
