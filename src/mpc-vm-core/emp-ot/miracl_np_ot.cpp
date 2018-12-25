#ifdef OT_NP_USE_MIRACL//use miracl version npot

#include "emp-ot/np_ot.h"
#include "emp-tool/utils/hash.h"
#include "platon_logger.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <miracl.h>

#ifdef __cplusplus
}
#endif



namespace platon
{
	namespace mpc
	{

		NPOT::NPOT(IOChannel* io)
		{
			this->io = io;
		}

		NPOT::~NPOT()
		{
			if (C)
				epoint_free(C);
		}

		void NPOT::init()
		{
			if (C == nullptr)
			{
				big c;
				c = mirvar(0);
				bigrand(getP(), c);
				C = epoint_init();
				ecurve_mult(c, getG(), C);  // C = c*G
				//big value should be free
				mirkill(c);
			}
		}

		//bob's send promitive
		int NPOT::send(block* data0, const block* data1, int length)
		{
			if (C == nullptr)
				init();

			int ret = 0;
			big r = mirvar(0);
			epoint *h1;
			h1 = epoint_init();
			epoint **hList = new epoint*[length];
			epoint **grList = new epoint*[length];
			//epoint **grList = (epoint **)mr_alloc(length, sizeof(epoint*));
			for (int i = 0; i < length; ++i)
			{
				hList[i] = epoint_init();
				grList[i] = epoint_init();
			}
			block *m = new block[length * 2];


			do {

				//recv hlis
				ret = io->recv_ep((void**)hList, length);
				if (ret != 0)
				{
					LOGE("OTNP recv epoint list failed, eppoint count: %d", length);
					ret = -1;
					break;
				}

				for (int i = 0; i < length; i++)
				{
					bigrand(getP(), r);

					ecurve_mult(r, getG(), grList[i]);  // g^r  
					epoint_copy(C, h1);   //h1 = C
					ecurve_sub(hList[i], h1);    // h1 = C-h
					ecurve_mult(r, hList[i], hList[i]);   // h' = h^r
					ecurve_mult(r, h1, h1);   // h1 = (C-h)^r

					/*m[0] = xorBlocks(data0[i], m0[i]);
					m[1] = xorBlocks(data1[i], m1[i]);*/
					m[2 * i] = xorBlocks(data0[i], Hash::hash_for_epoint(hList[i]));   //H(h^r)
					m[2 * i + 1] = xorBlocks(data1[i], Hash::hash_for_epoint(h1));  // H((C/h)^r) 

				}

				//printf("*** OTNP send blocks: \n");
				//send gr list 
				ret = io->send_ep((void**)grList, length);
				if (ret != 0)
				{
					LOGE("OTNP send epoint list failed, eppoint count: %d", length);
					ret = -1;
					break;
				}

				//send block list
				ret = io->send_block(m, length * 2);
				if (ret != 0)
				{
					LOGE("OTNP send blocks list failed, block count: %d", length * 2);
					ret = -1;
					break;
				}

			} while (0);

			//OTNP_SEND_EXIT:
				//free values
			for (int i = 0; i < length; ++i)
			{
				epoint_free(grList[i]);
				epoint_free(hList[i]);
			}

			delete[]grList;
			delete[]m;
			delete[]hList;
			mirkill(r);
			epoint_free(h1);

			return ret;
		}

		//alice's recv promitive
		int NPOT::recv(block* data, const bool* b, int length)
		{
			if (C == nullptr)
				init();

			int ret = 0;
			big *k = new big[length];
			epoint *pk1;//epoint *h;

			block *m = new block[2 * length];
			epoint **hList = new epoint*[length];
			epoint **grList = new epoint*[length];

			//init k, h, gr list,   notice: big value should free
			for (int i = 0; i < length; ++i)
			{
				k[i] = mirvar(0);
				hList[i] = epoint_init();
				grList[i] = epoint_init();
			}
			pk1 = epoint_init();

			//k random
			for (int i = 0; i < length; i++)
			{
				bigrand(getP(), k[i]);
			}


			//send hList
			for (int i = 0; i < length; i++)
			{
				if (b[i])
				{
					ecurve_mult(k[i], getG(), pk1);   // pk1 = g^ki
					epoint_copy(C, hList[i]);  // h = C
					ecurve_sub(pk1, hList[i]);  // h = C - pk1 = C - g^ki
				}
				else
				{
					ecurve_mult(k[i], getG(), hList[i]);  // h = g^ki
				}
			}

			do {

				ret = io->send_ep((void**)hList, length);
				if (ret != 0)
				{
					LOGE("OTNP send epoint list failed, eppoint count: %d", length);
					ret = -1;
					break;
				}

				//printf("*** OTNP recv blocks: \n");
				//recv gr list
				ret = io->recv_ep((void**)grList, length);
				if (ret != 0)
				{
					LOGE("OTNP recv epoint list failed, eppoint count: %d", length);
					ret = -1;
					break;
				}

				//recv block list
				ret = io->recv_block(m, 2 * length);
				if (ret != 0)
				{
					LOGE("OTNP recv blocks list failed, block count: %d", 2 * length);
					ret = -1;
					break;
				}

				for (int i = 0; i < length; ++i)
				{
					int ind = b[i] ? 1 : 0;

					ecurve_mult(k[i], grList[i], grList[i]);  // gr = gr^ki
					// data_b =H(gr^ki) + m_b
					block tempB1 = Hash::hash_for_epoint(grList[i]);
					block tempB2 = Hash::hash_for_epoint(grList[i]);
					data[i] = xorBlocks(m[2 * i + ind], Hash::hash_for_epoint(grList[i]));
				}

			} while (0);

			//OTNP_RECV_EXIT:

				//free values
			for (int i = 0; i < length; ++i)
			{
				mirkill(k[i]);
				epoint_free(hList[i]);
				epoint_free(grList[i]);
			}
			delete[]k;
			delete[]hList;
			delete[]grList;
			delete[]m;
			epoint_free(pk1);

			return ret;
		}


	}//emp
}//platon

#endif
