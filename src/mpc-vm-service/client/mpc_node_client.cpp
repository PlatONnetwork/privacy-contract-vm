#include "mpc_node_client.h"

NS_PLATON_SDK_MPC_BEG


MpcNodeClient::MpcNodeClient(int type) : m_type(type)
{

}

MpcNodeClient::~MpcNodeClient()
{

}


int MpcNodeClient::init(const string & user, const string & password, int mode, const string & proxyNodeEndpoint, const vector < string > & args)
{
	m_user = user;
	m_password = password;
	m_mode = mode;
	m_remoteEndpoint = proxyNodeEndpoint;
	m_args = args;

	return init_impl(user, password, mode, proxyNodeEndpoint, args);
}
NS_PLATON_SDK_MPC_END
