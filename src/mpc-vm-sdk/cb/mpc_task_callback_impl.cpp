#include "mpc_task_callback_impl.h"

NS_PLATON_SDK_MPC_BEG

void MpcTaskCallbackImpl::error(const ::platon::callback::MpcRequest& request, ::platon::ErrorEnum error,
	const ::Ice::Current& current/* = ::Ice::Current()*/) {

}
::platon::ErrorEnum MpcTaskCallbackImpl::inputData(const ::platon::callback::MpcRequest& request,
	::platon::ByteList& data, ::platon::ByteList& data_default, const ::Ice::Current& current/* = ::Ice::Current()*/) {

	return ::platon::ErrorEnum::NoError;
}
void MpcTaskCallbackImpl::result(const ::platon::callback::MpcRequest& request,
	const ::platon::ByteList& data, const ::Ice::Current& current/* = ::Ice::Current()*/) {

}

NS_PLATON_SDK_MPC_END
