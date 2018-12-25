#pragma once

#include <chrono>



#include "mpc_common.h"
NS_PLATON_MPC_BEG

class   SimpleTimer
{
public:
	SimpleTimer()
	{
		begin = std::chrono::system_clock::now();
		end = begin;
	}

	virtual ~SimpleTimer() {}

	void start()
	{
		begin = std::chrono::system_clock::now();
	}

	double stop()
	{
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - begin;
		double costTime = elapsed_seconds.count();
		return costTime;
	}

	double elapse() const
	{
		auto ending = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = ending - begin;
		return elapsed_seconds.count();
	}

protected:
	std::chrono::time_point<std::chrono::system_clock> begin;
	std::chrono::time_point<std::chrono::system_clock> end;
};
NS_PLATON_MPC_END
