#include "mpc_node_callback_impl.h"

#include "platon_logger.h"

NS_PLATON_SDK_MPC_BEG


void MpcNodeCallbackImpl::invite(const ::std::string& taskid, ::Ice::Int status, const ::Ice::Current&/* = ::Ice::Current()*/) {
	// if all ready, cb()
	NodeCallbackFunc cb = nullptr;
	LOGI("### MpcNodeCallbackImpl::invite  ####");
	{
		std::lock_guard<std::mutex> lock(mtx);
		auto iter = mapNodeInviteCB.find(taskid);
		if (iter == mapNodeInviteCB.end()) {
			LOGE("in fact, never enter into here!");
			return;
		}
		cb = std::move(mapNodeInviteCB[taskid]);
		mapNodeInviteCB.erase(iter);
	}
	cb(taskid, status);
}
void MpcNodeCallbackImpl::notify(const ::std::string& taskid, ::Ice::Int status, const ::Ice::Current&/* = ::Ice::Current()*/) {
	// if all ready, cb()
	NodeCallbackFunc cb = nullptr;
	LOGI("### MpcNodeCallbackImpl::notify  ####");
	{
		std::lock_guard<std::mutex> lock(mtx);
		auto iter = mapNodeNotifyCB.find(taskid);
		if (iter == mapNodeNotifyCB.end()) {
			LOGE("in fact, never enter into here!");
			return;
		}
		cb = std::move(mapNodeNotifyCB[taskid]);
		mapNodeNotifyCB.erase(iter);
	}
	cb(taskid, status);
}
void MpcNodeCallbackImpl::ready(const ::std::string& taskid, ::Ice::Int status, const ::Ice::Current&/* = ::Ice::Current()*/) {
	// if all ready, cb()
	NodeCallbackFunc cb = nullptr;
	{
		std::lock_guard<std::mutex> lock(mtx);
		auto iter = mapNodeReadyCB.find(taskid);
		if (iter == mapNodeReadyCB.end()) {
			LOGE("in fact, never enter into here!");
			return;
		}
		cb = std::move(mapNodeReadyCB[taskid]);
		mapNodeReadyCB.erase(iter);
	}
	cb(taskid, status);
}

NS_PLATON_SDK_MPC_END
