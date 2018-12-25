#pragma once

#include <map>
#include <mutex>
#include <string>

#include "platon_mpc_service.h"
#include "mpc_common.h"

NS_PLATON_SDK_MPC_BEG

typedef std::function<void(const std::string& taskid, int status)> NodeCallbackFunc;

class MpcNodeCallbackImpl : public ::platon::callback::NodeCallback {
public:
	virtual void invite(const ::std::string&, ::Ice::Int, const ::Ice::Current& = ::Ice::Current());
	virtual void notify(const ::std::string&, ::Ice::Int, const ::Ice::Current& = ::Ice::Current());
	virtual void ready(const ::std::string&, ::Ice::Int, const ::Ice::Current& = ::Ice::Current());

public:
	void setInviteCB(const std::string& taskid, NodeCallbackFunc cb) {
		mapNodeInviteCB[taskid] = cb;
	}
	void setNotifyCB(const std::string& taskid, NodeCallbackFunc cb) {
		mapNodeNotifyCB[taskid] = cb;
	}
	void setReadyCB(const std::string& taskid, NodeCallbackFunc cb) {
		mapNodeReadyCB[taskid] = cb;
	}

private:
	std::map<std::string, NodeCallbackFunc> mapNodeInviteCB;
	std::map<std::string, NodeCallbackFunc> mapNodeNotifyCB;
	std::map<std::string, NodeCallbackFunc> mapNodeReadyCB;
	std::mutex mtx;
};

NS_PLATON_SDK_MPC_END
