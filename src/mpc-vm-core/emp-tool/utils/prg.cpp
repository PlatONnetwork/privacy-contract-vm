#include "emp-tool/utils/prg.h"
#include <cassert>
#include <ctime>
#include <cstddef>
#include "sse.h"
#include <iostream>
#include <string.h>
#include <fstream>
#include <mutex>

namespace platon
{
	namespace mpc
	{

		//initialization
		static PRG *rnd = nullptr;
		static std::recursive_mutex s_prgLocker;

		PRG::PRG(const void * seed /* = nullptr */, int id/* = 0*/)
		{
			if (rnd == nullptr)
			{
				int *data = new int[sizeof(block) / sizeof(int)];
				srand((int)time(nullptr));
				for (size_t i = 0; i < sizeof(block) / sizeof(int); ++i)
					data[i] = rand();

				if (rnd == nullptr)
				{
					std::lock_guard<std::recursive_mutex> autoLocker(s_prgLocker);
					//copy the prg object as static object
					rnd = new PRG(*this);
					uint64_t tempId = (uint64_t)rand();
					//rnd->reseed(&data, tempId);
					rnd->reseed(data, tempId);
				}

				delete []data;
			}

			if (seed != nullptr)
			{
				reseed(seed, id);
			}
			else
			{
				block data;
				rnd->random_block(&data, 1);
				reseed(&data, id);
			}
		}

		PRG::PRG(const PRG& obj)
		{
			this->counter = obj.counter;
			this->aes = obj.aes;
		}

		void PRG::reseed(const void * key, uint64_t id /*= 0 */)
		{
			const char * k = (const char *)key;
			//fix for centos run crash
			block v;
			memcpy(&v, key, sizeof(block));
			v = xorBlocks(v, juzix_mm_set_epi64x(0LL, id));
			/*block v = juzix_mm_load_si128((block*)&k[0]);
			v = xorBlocks(v, juzix_mm_set_epi64x(0LL, id));*/
			AES_set_encrypt_key(v, &aes);

			counter = 0;
			//take seed
			int* rseed = (int*)key;
			srand(*rseed);
			//srand((int)time(nullptr));
		}

		void PRG::random_data(void *data, int nbytes)
		{
			random_block((block *)data, nbytes / 16);
			if (nbytes % 16 != 0)
			{
				block extra;
				random_block(&extra, 1);
				memcpy((nbytes / 16 * 16) + (char *)data, &extra, nbytes % 16);
			}
		}

		void PRG::random_bool(bool * data, int length)
		{
			uint8_t * uint_data = (uint8_t*)data;
			random_data(uint_data, length);
			for (int i = 0; i < length; ++i)
				data[i] = uint_data[i] & 1;
		}

		void PRG::random_data_unaligned(void *data, int nbytes)
		{
			block tmp[AES_BATCH_SIZE];
			for (int i = 0; i < nbytes / (AES_BATCH_SIZE * 16); i++)
			{
				random_block(tmp, AES_BATCH_SIZE);
				memcpy((16 * i*AES_BATCH_SIZE) + (uint8_t*)data, tmp, 16 * AES_BATCH_SIZE);
			}
			if (nbytes % (16 * AES_BATCH_SIZE) != 0)
			{
				random_block(tmp, AES_BATCH_SIZE);
				memcpy((nbytes / (16 * AES_BATCH_SIZE)*(16 * AES_BATCH_SIZE)) + (uint8_t*)data, tmp, nbytes % (16 * AES_BATCH_SIZE));
			}
		}

		void PRG::random_block(block * data, int nblocks /* =1 */)
		{
			for (int i = 0; i < nblocks; ++i) {
				data[i] = juzix_mm_set_epi64x(0LL, counter++);
			}
			int i = 0;
			for (; i < nblocks - AES_BATCH_SIZE; i += AES_BATCH_SIZE) {
				AES_ecb_encrypt_blks(data + i, AES_BATCH_SIZE, &aes);
			}
			AES_ecb_encrypt_blks(data + i, (AES_BATCH_SIZE > nblocks - i) ? nblocks - i : AES_BATCH_SIZE, &aes);
		}

#ifndef OT_NP_USE_MIRACL
        void PRG::random_bn(bn_t a, int sign /*= BN_POS */, int bits /*= BIT_LEN */) {
            int digits;
            //SPLIT(bits, digits, bits, BN_DIG_LOG);
			SPLIT(bits, digits, bits, DIG_LOG);
            digits += (bits > 0 ? 1 : 0);
            bn_grow(a, digits);
            random_data((uint8_t*)a->dp, digits * sizeof(dig_t));
            a->used = digits;
            a->sign = sign;
            if (bits > 0) {
                dig_t mask = ((dig_t)1 << (dig_t)bits) - 1;
                a->dp[a->used - 1] &= mask;
            }
            bn_trim(a);
        }
        
        void PRG::random_bn(bn_t *a, int length /*=1*/, int sign /*= BN_POS */, int bits /* = BIT_LEN */) {
            for(int i = 0; i < length; ++i)
                random_bn(a[i]);
        }
        
        void PRG::random_eb(eb_t p) {
            bn_t n, k;
            bn_new(k);
            bn_new(n);
            eb_curve_get_ord(n);
            random_bn(k, BN_POS, bn_bits(n));
            bn_mod(k, k, n);
            eb_mul_gen(p, k);
        }
        
        void PRG::random_eb(eb_t *p, int length /*=1*/) {
            bn_t n, k;
            bn_new(k);
            bn_new(n);
            eb_curve_get_ord(n);
            for(int i = 0; i < length; ++i) {
                random_bn(k, BN_POS, bn_bits(n));
                bn_mod(k, k, n);
                eb_mul_gen(p[i], k);
            }
        }
#endif

	}//emp
}//juzix



