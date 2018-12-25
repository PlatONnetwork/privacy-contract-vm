#pragma once

#include "block.h"
#include "platon_core_conf.h"
#include <thread>
#include <iostream>

namespace platon
{
	namespace mpc
	{
		enum RTCktOpt { on, off };

		class CircuitExecution {
		public:
			virtual block and_gate(const block& in1, const block& in2) = 0;
			virtual block xor_gate(const block&in1, const block&in2) = 0;
			virtual block not_gate(const block& in1) = 0;
			virtual block public_label(bool b) = 0;
			virtual block delta() = 0;
			virtual ~CircuitExecution() { }
		};


		class CircuitExecutionProxy
		{
		public:
			thread_local static CircuitExecutionProxy circ_exec;

			CircuitExecution* circ_exec_ = nullptr;//only reference, not manage the object

			void setup(CircuitExecution* cuicuit_exec)
			{
				if (circ_exec_)
					delete circ_exec_;

				circ_exec_ = cuicuit_exec;
			}

			void finalize()
			{
				if (circ_exec_)
				{
					delete circ_exec_;
					circ_exec_ = nullptr;
				}
			}

			block and_gate(const block& in1, const block& in2)
			{
				return circ_exec_->and_gate(in1, in2);
			}

			block xor_gate(const block&in1, const block&in2)
			{
				return circ_exec_->xor_gate(in1, in2);
			}

			block not_gate(const block& in1)
			{
				return circ_exec_->not_gate(in1);
			}

			block public_label(bool b)
			{
				return circ_exec_->public_label(b);
			}

			block delta()
			{
				return circ_exec_->delta();
			}
				
		};

	}
}