#pragma once

#include <stddef.h>
#include <thread>
#include "block.h"
#include "platon_core_conf.h"
//#include "emp-tool/execution/circuit_execution.h"
#include "io_channel.h"

namespace platon
{
	namespace mpc
	{

		class ProtocolExecution {
		public:
			int cur_party;
			//CircuitExecution* gc_ = nullptr;


			ProtocolExecution(int party = PUBLIC) : cur_party(party) {}
			virtual ~ProtocolExecution() {}
			virtual void feed(block * lbls, int party, const bool* b, int nel) = 0;
			virtual void reveal(bool*out, int party, const block *lbls, int nel) = 0;
			virtual void finalize() {}

			virtual void setup(IOChannel* io) = 0;

		};

		class ProtocolExecutionProxy
		{
		public:
			static thread_local ProtocolExecutionProxy prot_exec;

			ProtocolExecution* prot_exec_ = nullptr;

			void setup(ProtocolExecution* prot)
			{
				if (prot_exec_)
					delete prot_exec_;

				prot_exec_ = prot;
			}

			void finalize()
			{
				if (prot_exec_)
				{
					prot_exec_->finalize();
					delete prot_exec_;
					prot_exec_ = nullptr;
				}
			}

			void feed(block * lbls, int party, const bool* b, int nel)
			{
				return prot_exec_->feed(lbls, party, b, nel);
			}

			void reveal(bool*out, int party, const block *lbls, int nel)
			{
				return prot_exec_->reveal(out, party, lbls, nel);
			}

		};
	}
}

