#pragma once


#include <cstddef>
#include "mpc_node_client.h"


#include "mpc_common.h"
NS_PLATON_SDK_MPC_BEG

class MpcDirectNodeClient : public MpcNodeClient
{
public:
	MpcDirectNodeClient();
	virtual ~MpcDirectNodeClient();

	//void inputData(string taskId, string fromUser, ::common::ByteList data);//use -> nodeService
	virtual void forward(const string& taskId, const string& fromUser, const string& toUser, const vector<unsigned char>& data);
	
	virtual void notify(const ::platon::service::TaskParams& params, NodeCallbackFunc cb);
	virtual void invite(const ::platon::service::TaskParams& params, NodeCallbackFunc cb);
	virtual void ready(const ::platon::service::TaskParams& params, NodeCallbackFunc cb);

	virtual int notify(const ::platon::service::TaskParams& params);
	virtual int invite(const ::platon::service::TaskParams& params);
private:
	virtual int init_impl(const string& user, const string& password, int mode, const string& proxyEndpoint, const vector<string>& args);
};
NS_PLATON_SDK_MPC_END