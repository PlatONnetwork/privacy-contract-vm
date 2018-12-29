#pragma once

#include <stdio.h>
#include <string>
#include <cstddef>
///#include "IOChannel.h"
#include "mpc_task_processor.h"

#include "emp-tool/io/io_channel.h"

using std::string;

#include "mpc_common.h"
NS_PLATON_SDK_MPC_BEG

class CommunicatorIOChannel : public emp::IOChannel
{
public:
	CommunicatorIOChannel(MpcTaskProcessor* processor);
	virtual ~CommunicatorIOChannel();

	int send_data(const void * data, int len) override;
	int recv_data(void  * data, int len) override;

public:
	int64_t								m_sendLen = 0;
	int64_t								m_recvLen = 0;
	int									m_seq = 0;

private:
	MpcTaskProcessor*					m_processor;
	string										m_recvBuffer;
	vector<unsigned char>           m_tempOutput;
	string										m_from;
};

NS_PLATON_SDK_MPC_END
