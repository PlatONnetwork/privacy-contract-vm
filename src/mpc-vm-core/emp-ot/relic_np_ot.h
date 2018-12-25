#ifndef OT_NP_USE_MIRACL
#pragma once

#include <cstddef>

#include "emp-ot/ot_base.h"
#include "io_channel.h"
#include "emp-tool/utils/relic_ec_utils.h"
#include "emp-tool/utils/prg.h"



namespace platon
{
	namespace mpc
	{

		class NPOT : public OTBase
		{
		public:
			NPOT(IOChannel* io);
			~NPOT() {};
			virtual int send(block* data0, const block* data1, int length);

			virtual int recv(block* data, const bool* b, int length);

		private:
			int cnt;
			eb_t g, C;
			const eb_t *gTbl;
			bn_t q;
			PRG prg;
			IOChannel* io;

		};



	}//emp
}//juzix




#endif//OT_NP_USE_MIRACL

