#pragma once

#include <Glacier2/SessionHelper.h>
#include <string>
#include <vector>
#include "platon_mpc_service.h"

using std::string;
using std::vector;

#include "mpc_common.h"
NS_PLATON_SDK_MPC_BEG

typedef std::function<void(const string& taskid, int status)> NodeCallbackFunc;

class MpcCommunicator
{
	//friend class callback::NodeCallback;

public:
	enum /*class*/ CommunicatorType
	{
		JUGO_PROXY,
		NODE_DIRECT,
		NODE_PROXY
	};

	MpcCommunicator(const platon::NodeCommunicateInfo& nodeCommunicateInfo);
	virtual ~MpcCommunicator();

	virtual int init() = 0;
	virtual void uninit() = 0;

	void gotError(int code, const string& err) { m_errno = code; m_error = err; };

	/**
	 * 发送邀请，发起方发出邀请后，是发送到JUGO的，有JUGO回调邀请各个受邀方
	 * PROXY收到消息后，马上返回结果，然后继续处理（回调所有用户）
	 *
	 * @param roomId
	 * @param algorithmId
	 * @param starter
	 * @param takerList
	 * @param resultReceiverList
	 * @param args
	 * @return
	 */
	virtual string invite(const string& roomId, const string& algorithmId, \
		const string& starter, const vector<string>& takerList, \
		const vector<string>& resultReceiverList, const vector<string>& args) = 0;


	//转发数据, toUser, 服务节点数据的去向	
	virtual void forward(const string& taskId, const string& fromUser, const string& toUser, const vector<unsigned char>& data) = 0;

	//转发数据, toUser, 服务节点数据的去向
	//virtual void forward(string taskId, vector<unsigned char> data);

	//计算节点向proxy报告任务结果，code=0表示执行完成；其它表示异常
	virtual void result(string taskId, int code, vector<unsigned char> data) = 0;

	::Glacier2::SessionHelperPtr getSessionHelper() { return m_sessionHelper; };

	::Glacier2::SessionFactoryHelperPtr getSessionFactoryHelper() { return m_sessionFactoryHelper; };

	void setState(int state);

	bool prepared() { return m_inited == 2 ? true : false; };

	platon::NodeCommunicateInfo getNodeInfo() { return m_nodeCommunicateInfo; };

	virtual void notify(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {}
	virtual void invite(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {}
	virtual void ready(const ::platon::service::TaskParams& params, NodeCallbackFunc cb) {}

	virtual int notify(const ::platon::service::TaskParams& params) { return -1; }
	virtual int invite(const ::platon::service::TaskParams& params) { return -1; }

protected:
	platon::NodeCommunicateInfo                 m_nodeCommunicateInfo;
	//string                                      m_proxyEndpoint;

	//service::ProxySessionPrxPtr                 m_SessionPrx;

	::Glacier2::SessionFactoryHelperPtr         m_sessionFactoryHelper = nullptr;
	::Glacier2::SessionHelperPtr                m_sessionHelper = nullptr;
	::Glacier2::SessionCallbackPtr              m_sessionCallback = nullptr;

	int                                         m_inited = 0;
	int                                         m_errno;
	string                                      m_error;

};

NS_PLATON_SDK_MPC_END