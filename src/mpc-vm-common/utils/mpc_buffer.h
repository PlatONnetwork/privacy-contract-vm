#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <cstring>
#include <cassert>
#include <memory>
#include "platon_logger.h"

using std::string;

#include "mpc_common.h"
NS_PLATON_MPC_BEG

class MpcBuffer;
typedef MpcBuffer*      MpcBufferPtr;

static const int BUFFER_SIZE = 128;

class MpcBuffer
{

public:
	int32_t         seq = 0;//send seq id
	char            taskId[BUFFER_SIZE];//eg.  e93f937f-212e-42f8-a4ec-82a10ea3afd4
	char            from[BUFFER_SIZE];
	int32_t         length = 0;

	MpcBuffer* next = nullptr;
	uint8_t* data = nullptr;

	~MpcBuffer()
	{
		free();
	}

	void Init() {
		memset(taskId, 0, BUFFER_SIZE);
		memset(from, 0, BUFFER_SIZE);
	}

	MpcBuffer() : data(nullptr), length(0), seq(0), next(nullptr)
	{
		Init();
		//LOGE("construction default, buffer: %p size: %d", data, length);
	};

	MpcBuffer(int len)
	{
		Init();

		data = new uint8_t[len];
		length = len;
		memset(data, 0, len);

		//LOGE("construction, buffer: %p size: %d", data, length);
	}

	MpcBuffer(uint8_t* d, int len)
	{
		Init();

		data = new uint8_t[len];
		length = len;
		memcpy(data, d, len);

		//LOGE("construction, buffer: %p size: %d", data, length);
	}

	MpcBuffer(const MpcBuffer& bf)
	{
		Init();

		memcpy(taskId, bf.taskId, BUFFER_SIZE);
		memcpy(from, bf.from, BUFFER_SIZE);

		length = bf.length;
		data = new uint8_t[bf.length];
		memcpy(data, bf.data, bf.length);

		seq = bf.seq;
		next = bf.next;

		//LOGE("construction, buffer: %p size: %d, task: %s", data, length, taskId);
	}

	MpcBuffer& operator=(const MpcBuffer& bf)
	{
		if (&bf == this)
			return *this;

		Init();

		memcpy(taskId, bf.taskId, BUFFER_SIZE);
		memcpy(from, bf.from, BUFFER_SIZE);

		length = bf.length;
		if (data)
		{
			delete[]data;
		}
		data = new uint8_t[bf.length];
		memcpy(data, bf.data, bf.length);

		seq = bf.seq;
		next = bf.next;

		return *this;
	};

	void setTaskId(const std::string& id)
	{
		assert((id.length() < BUFFER_SIZE) && id.c_str());
		memset(taskId, 0, BUFFER_SIZE);
		memcpy(taskId, id.c_str(), id.length());
	};

	void setFrom(const string& fr)
	{
		assert((fr.length() < BUFFER_SIZE) && fr.c_str());
		memset(from, 0, BUFFER_SIZE);
		memcpy(from, fr.c_str(), fr.length());
	};

	static MpcBuffer copy(void* d, int len)
	{
		uint8_t* data = new uint8_t[len];
		memcpy(data, d, len);
		return MpcBuffer(data, len);
	};

	void free()
	{
		//LOGE("deconstruct free, buffer: %p size: %d, task: %s", data, length, taskId);
		if (data && length > 0)
		{
			delete[]data;
			data = nullptr;
		}
		length = 0;

		MpcBuffer* elem = next;
		MpcBuffer* cur = next;
		while (elem)
		{
			cur = elem;
			elem = elem->next;
			delete cur;
			cur = nullptr;
		}
	}
};
NS_PLATON_MPC_END
