#pragma once

#include "block.h"
#include "platon_core_conf.h"
#include <thread>

namespace platon
{
	namespace mpc
	{
		enum RTCktOpt { on, off };

		class CircuitExecution {
		public:
			thread_local static CircuitExecution * circ_exec;
			virtual block and_gate(const block& in1, const block& in2) = 0;
			virtual block xor_gate(const block&in1, const block&in2) = 0;
			virtual block not_gate(const block& in1) = 0;
			virtual block public_label(bool b) = 0;
			virtual ~CircuitExecution() { }
		};

	}
}