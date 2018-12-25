#include "communicator_io_channel.h"

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include "sys_utils_tool.h"
#include "mpc_engine.h"

///#include "MpcDataWriter.h"
///#include "BlockUtils.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds

using platon::mpc::MpcBuffer;
using platon::mpc::MpcBufferPtr;
using namespace std;


NS_PLATON_SDK_MPC_BEG

//
//static void setTcpNoDelay(int fd)
//{
//	int flag = 1;
//	if (setsockopt(fd, IPPROTO_TCP, TCP_NODELAY, reinterpret_cast<char*>(&flag), int(sizeof(int))) != 0)
//	{
//		LOGW("set tcp no delay failed !!!!");
//	}
//	LOGI("set tcp no delay success  !!!!");
//}


CommunicatorIOChannel::CommunicatorIOChannel(MpcTaskProcessor* processor) :
	m_processor(processor), m_seq(0)
{
	//setTcpNoDelay();
}


CommunicatorIOChannel::~CommunicatorIOChannel()
{

}

int CommunicatorIOChannel::send_data(const void * data, int len)
{
	m_tempOutput.clear();

	++m_seq;
	m_tempOutput.assign((unsigned char*)data, (unsigned char*)data + len);
	m_tempOutput.resize(len + sizeof(m_seq));
	//the send message seq will attach to the tailer
	memcpy(m_tempOutput.data() + len, &m_seq, sizeof(m_seq));

	
	MPCTask& task = m_processor->getTask();
	int size = task.nodeInfoList.size();
	string user = MpcEngine::getInstance()->getMpcNodeServant2()->m_taskSession->getAddress();
	int mode = MpcEngine::getInstance()->getMode();
	bool dataLog = MpcEngine::getInstance()->isDataLogEnable();

	for (int i = 0; i < size; ++i)
	{
		if (user != task.nodeInfoList[i].user)//not self
		{
			try {
				if (mode == static_cast<int>(platon::NodeCommunicateMode::MSERVICE))
				{
					MpcNodeClient* mpcClient = MpcEngine::getInstance()->getNodeClient(task.nodeInfoList[i]);
					if (!mpcClient)
					{
						LOGE("cannot find the node client, nullptr, endpoint: %s", task.nodeInfoList[i].endpoint.data());
						return -1;
					}

					LOGE("==> send data task: %s,   %s => %s,     data size: %d, SEQ: %d ... ", task.taskId.data(), user.data(), task.nodeInfoList[i].user.data(), len, m_seq);
					mpcClient->forward(task.taskId, user, task.nodeInfoList[i].user, m_tempOutput);
					LOGE("==> send data task: %s,   %s => %s,     datasize: %d    OK", task.taskId.data(), user.data(), task.participants[i].data(), len);
				}
				else
				{
					LOGE("bad mode: %d, check the init arguments !", mode);
					return -1;
				}
			}
			catch (...)
			{
				int err = errno;
				LOGE("got exception in send data, task: %s, size: %d, errno: %d", task.taskId.data(), len, err);
				
				return -1;
			}
		}
	}

	m_sendLen += len;
	return len;
}

int CommunicatorIOChannel::recv_data(void  * data, int len)
{
	//if has data remain, and it is enough
	MPCTask& task = m_processor->getTask();

	int timeout = 10;//default timeout 10s
	LOGE("<==  task: %s, to recv  len: %d, remain buffer size: %d ", task.taskId.data(), len, m_recvBuffer.size());
	int ret = 0;
	while (m_recvBuffer.size() < len)
	{
		//comsume buffer
		MpcBufferPtr buffer;

		ret = m_processor->consumeBuffer(buffer, timeout);
		if (0 != ret || !buffer)
		{
			LOGW("consume buffer failed, this run of the task: %s exec failed!", task.taskId.data());
			if (ret == 1)
				return -2;//timeout

			return -1;
		}

		if (buffer->taskId != task.taskId)
		{
			LOGE("bad buffer (size %d) consume, with taskid: %s, (expect taskid: %s, processor: %d ), put a queue again !", \
				buffer->length, buffer->taskId, task.taskId.data(), m_processor->getID());
			buffer->free();
			delete buffer;
			continue;
		}

		m_from = buffer->from;
		LOGE("<== task: %s, recv datasize: %d , seq: %d", buffer->taskId, buffer->length, buffer->seq);
		m_recvBuffer.append((char*)buffer->data, buffer->length);

		buffer->free();
		delete buffer;
	}

	//set recv buffer
	memcpy(data, m_recvBuffer.data(), len);
	m_recvBuffer.erase(m_recvBuffer.begin(), m_recvBuffer.begin() + len);

	m_recvLen += len;
	return len;
}
NS_PLATON_SDK_MPC_END
