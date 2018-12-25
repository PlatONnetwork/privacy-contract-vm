#include "emp-tool/utils/prp.h"
#include <stdio.h>
#include "platon_core_conf.h"
#include "sse.h"

namespace platon
{
	namespace mpc
	{

		PRP::PRP(const char * seed/* = fix_key*/) 
		{
			aes_set_key(seed);
		}

		PRP::PRP(const block& seed)
		{
			aes_set_key((const char*)&seed);
		}

		void PRP::aes_set_key(const char * key)
		{
			aes_set_key(juzix_mm_loadu_si128((block*)key));
		}

		void PRP::aes_set_key(const block& v)
		{
			AES_set_encrypt_key(v, &aes);
		}

		void PRP::permute_block(block *data, int nblocks)
		{
			int i = 0;
			for (; i < nblocks - AES_BATCH_SIZE; i += AES_BATCH_SIZE)
			{
				AES_ecb_encrypt_blks(data + i, AES_BATCH_SIZE, &aes);
			}
			AES_ecb_encrypt_blks(data + i, (AES_BATCH_SIZE > nblocks - i) ? nblocks - i : AES_BATCH_SIZE, &aes);
		}

		void PRP::permute_data(void*data, int nbytes)
		{
			PRP::permute_block((block *)data, nbytes / 16);
			if (nbytes % 16 != 0)
			{
				uint8_t extra[16];
				memset(extra, 0, 16);
				memcpy(extra, (nbytes / 16 * 16) + (char *)data, nbytes % 16);
				permute_block((block*)extra, 1);
				memcpy((nbytes / 16 * 16) + (char *)data, &extra, nbytes % 16);
			}
		}

		block PRP::H(block in, uint64_t id)
		{
			in = double_block(in);
			block k_128 = juzix_mm_loadl_epi64(&id);
			in = xorBlocks(in, k_128);
			block t = in;
			permute_block(&t, 1);
			in = xorBlocks(in, t);
			return in;
		}

		void PRP::Hn(block*out, block* in, uint64_t id, int length, block * scratch /*= nullptr */)
		{
			bool del = false;
			if (scratch == nullptr) {
				del = true;
				scratch = new block[length];
			}
			for (int i = 0; i < length; ++i) {
				out[i] = scratch[i] = xorBlocks(double_block(in[i]), juzix_mm_loadl_epi64(&id));
				++id;
			}
			permute_block(scratch, length);
			xorBlocks_arr(out, scratch, out, length);
			if (del) {
				delete[] scratch;
				scratch = nullptr;
			}
		}


	}//emp
}//juzix
