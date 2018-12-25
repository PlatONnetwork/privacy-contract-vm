#include "mpc_communicator.h"
#include "platon_logger.h"

NS_PLATON_SDK_MPC_BEG
static string getCommunictorType(::platon::NodeCommunicateMode mode)
{
	if (mode == ::platon::MCALLBACK)
		return "callback";
	else if (mode == ::platon::MSERVICE)
		return "service";

	return "unknown";
}

MpcCommunicator::MpcCommunicator(const platon::NodeCommunicateInfo& nodeCommunicateInfo)
	: m_nodeCommunicateInfo(nodeCommunicateInfo)
{
}

MpcCommunicator::~MpcCommunicator()
{
}

void MpcCommunicator::setState(int state)
{
	m_inited = state;
}

NS_PLATON_SDK_MPC_END