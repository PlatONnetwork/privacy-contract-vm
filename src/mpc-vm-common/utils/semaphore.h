#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>

#include "mpc_common.h"

NS_PLATON_SDK_MPC_BEG

class Semaphore
{
public:
	Semaphore(long count = 0) : m_count(count)
	{
	}

	void signal()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		++m_count;
		cv_.notify_one();
	}

	void wait()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		cv_.wait(lock, [=] { return m_count > 0; });
		--m_count;
	}

private:
	std::mutex m_mutex;
	std::condition_variable cv_;
	long m_count;
};

NS_PLATON_SDK_MPC_END

