#pragma once

#include "emp-tool/utils/block_utils.h"
#include "block.h"

namespace platon
{
	namespace mpc
	{

		class OTBase
		{
		public:
			OTBase() {};
			virtual ~OTBase() {};
			virtual int send(block* data0, const block* data1, int length) = 0;

			virtual int recv(block* data, const bool* b, int length) = 0;
		};


	}//emp
}//platon
