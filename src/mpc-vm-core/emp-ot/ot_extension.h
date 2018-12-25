#pragma once

#include "emp-tool/utils/block_utils.h"
#include "emp-tool/utils/prg.h"
//#include "emp-tool/utils/prp.h"
#include "emp-tool/utils/crh.h"

#include "emp-ot/ot_base.h"
#include "io_channel.h"


namespace platon
{
	namespace mpc
	{

		class OTExtension : public OTBase
		{
		public:
			OTExtension(IOChannel * io);
			virtual ~OTExtension();

			virtual int setup_send(block * in_k0 = nullptr, bool * in_s = nullptr);

			virtual int setup_recv(block * in_k0 = nullptr, block * in_k1 = nullptr);

			virtual int send_pre(int length);

			virtual int recv_pre(const bool* r, int length);

			virtual int padded_length(int length);


			//interface to implement
			virtual int send(block* data0, const block* data1, int length) = 0;

			virtual int recv(block* data, const bool* b, int length) = 0;



		protected:
			OTBase * base_ot = nullptr;
			IOChannel * io = nullptr;

			PRG prg;
			//PRP prp;
			CRH crh;
			const static int blockSize = 1024;//1<<14 ;  // 16384

			block *k0 = nullptr;
			block *k1 = nullptr;
			block *Tt = nullptr;
			block *Qq = nullptr;
			//block *tmp = nullptr;
			block bs;
			bool *pad_r = nullptr;
			bool *array_s = nullptr;
			bool init = false;
			PRG *G0 = nullptr;
			PRG *G1 = nullptr;

			int ssp;

		public:
			int l = 128;//kos: 128, alsz: 192, shot; 128

		};

	}//emp
}//platon