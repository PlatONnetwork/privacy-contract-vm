#pragma once


#include "mpc_task.h"
#include "mpc_communicator.h"

#include "mpc_common.h"
NS_PLATON_SDK_MPC_BEG

class MpcNodeClient
{
public:
	enum MpcNodeType
	{
		NODE_DIRECT = 0,
		NODE_PROXY = 1
	};


	MpcNodeClient(int type);
	virtual ~MpcNodeClient();

	int init(const string& user, const string& password, int mode, const string& proxyNodeEndpoint, const vector<string>& args);
	virtual int init_impl(const string& user, const string& password, int mode, const string& proxyEndpoint, const vector<string>& args) = 0;
	//void inputData(string taskId, string fromUser, ::common::ByteList data);//use -> nodeService
	virtual void forward(const string& taskId, const string& fromUser, const string& toUser, const vector<unsigned char>& data) = 0;

	virtual void notify(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {}
	virtual void invite(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {}
	virtual void ready(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {}

	virtual int notify(const ::platon::service::TaskParams& params) {return -1;}
	virtual int invite(const ::platon::service::TaskParams& params) {return -1;}

protected:
	string m_user;
	string m_password;
	int m_mode;
	string m_selfEndpoint;
	string m_remoteEndpoint;
	vector<string> m_args;

	int             m_inited = 0;
	int			    m_type;
	MpcCommunicator* m_com = nullptr;

};
NS_PLATON_SDK_MPC_END