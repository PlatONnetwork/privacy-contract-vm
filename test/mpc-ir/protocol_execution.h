#pragma once


#include <thread>  
#include "block.h"
#include "platon_core_conf.h"

namespace platon
{
	namespace mpc
	{

		class ProtocolExecution {
		public:
			int cur_party;
			static thread_local ProtocolExecution * prot_exec;


			ProtocolExecution(int party = PUBLIC) : cur_party(party) {}
			virtual ~ProtocolExecution() {}
			virtual void feed(block * lbls, int party, const bool* b, int nel) = 0;
			virtual void reveal(bool*out, int party, const block *lbls, int nel) = 0;
			virtual void finalize() {}
		};
	}
}

