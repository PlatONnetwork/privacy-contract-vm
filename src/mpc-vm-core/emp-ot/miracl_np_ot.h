#ifdef OT_NP_USE_MIRACL

#pragma once


#include "emp-ot/ot_base.h"
#include "io_channel.h"
#include "emp-tool/utils/sm2_params.h"
#include "emp-tool/utils/prg.h"
#include <cstddef>


namespace platon
{
	namespace mpc
	{

		class NPOT : public OTBase
		{
		public:
			NPOT(IOChannel* io);
			~NPOT();

			virtual int send(block* data0, const block* data1, int length);

			virtual int recv(block* data, const bool* b, int length);

		private:
			void init();

		private:
			int cnt = 0;
			epoint *C = nullptr;
			IOChannel* io;
			PRG prg;


		};

	}//mpc
}//platon

#endif
