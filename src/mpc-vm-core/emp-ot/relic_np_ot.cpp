#ifndef OT_NP_USE_MIRACL

#include "emp-ot/np_ot.h"
#include "io_channel.h"

namespace platon
{
	namespace mpc
	{

	NPOT::NPOT(IOChannel* io)
{
	this->io = io;
	initialize_relic();
	eb_curve_get_gen(g);
	eb_curve_get_ord(q);
	gTbl = eb_curve_get_tab();
	PRG fix_prg(fix_key);
	fix_prg.random_eb(&C, 1);
}

int NPOT::send(block* data0, const block* data1, int length)
{
	bn_t r0, r1;
	eb_t pk0, pk1, *gr0 = new eb_t[length], *gr1 = new eb_t[length];
	eb_newl(pk0, pk1);
	bn_newl(r0, r1);

	block *m0 = new block[length];
	block *m1 = new block[length];
	for(int i = 0; i < length; ++i) 
	{
		eb_newl(gr0[i], gr1[i]);
	}

	for(int i = 0; i < length; ++i) 
	{
		prg.random_bn(r0, r1);
		io->recv_eb((void*)&pk0, 1);
		eb_sub_norm(pk1, C, pk0);
		eb_mul_fix_norm(gr0[i], gTbl, r0);
		eb_mul_fix_norm(gr1[i], gTbl, r1);
		eb_mul_norm(pk0, pk0, r0);
		eb_mul_norm(pk1, pk1, r1);
		m0[i] = KDF(pk0);
		m1[i] = KDF(pk1);
	}
	block m[2];
	for(int i = 0 ; i < length; ++i) 
	{
		io->send_eb((void*)&gr0[i], 1);
		io->send_eb((void*)&gr1[i], 1);

//		printf("the data0, m0, index=%d: ", i);
//        printBlock(&data0[i], 1);
//        printBlock(&m0[i], 1);
//
//        printf("the data1, m1, index=%d: ", i);
//        printBlock(&data1[i], 1);
//        printBlock(&m1[i], 1);
        
		m[0] = xorBlocks(data0[i], m0[i]);
		m[1] = xorBlocks(data1[i], m1[i]);
		io->send_data(m, 2*sizeof(block));

//		printf("OTNP xorBlocks and send block i=%d: ", i);
//        printBlock(m, 2);
	}

	bn_freel(r0, r1);
	eb_freel(pk0, pk1);
	for(int i =0; i < length; ++i)
		eb_freel(gr0[i], gr1[i]);
	delete[] gr0;
	delete[] gr1;
	delete[] m0;
	delete[] m1;

	return 0;
}

int NPOT::recv(block* data, const bool* b, int length)
{
	bn_t * k = new bn_t[length];
	eb_t pk[2], E[2];
	block m[2];
	eb_newl(E[0], E[1], pk[0], pk[1]);
	for(int i = 0; i < length; ++i) 
	{
		bn_newl(k[i]);
	}
	prg.random_bn(k, length);

	for(int i = 0; i< length; ++i) 
	{
		if(b[i]) 
		{
			eb_mul_fix_norm(pk[1], gTbl, k[i]);
			eb_sub_norm(pk[0], C, pk[1]);
		} 
		else
		{
			eb_mul_fix_norm(pk[0], gTbl, k[i]);
		}
		io->send_eb((void*)&pk[0], 1);
	}

    //printf("OTNP recv blocks: ");
	for(int i = 0; i < length; ++i) 
	{
		io->recv_eb((void*)E, 2);
		io->recv_data(m, 2*sizeof(block));

		//printBlock(m, 2);
		
		int ind = b[i] ? 1 : 0;
		eb_mul_norm(E[ind], E[ind], k[i]);
		data[i] = xorBlocks(m[ind], KDF(E[ind]));

		//printf("OTNP xorBlocks block i=%d: ", i);
        //printBlock(m, 2);
	}
	
	for(int i = 0; i < length; ++i) 
	{
		bn_freel(k[i]);
	}
	eb_freel(pk[0], pk[1], E[0], E[1]);
	delete[] k;

	return 0;
}


}//emp
}//juzix

#endif//OT_NP_USE_MIRACL


