#pragma once



#include "block.h"
#include "emp-tool/garble/aes.h"


namespace platon
{
	namespace mpc
	{

class PRP
{
public:
    AES_KEY aes;

public:
	PRP(const char * seed = fix_key);

	PRP(const block& seed);

	void aes_set_key(const char * key);
	void aes_set_key(const block& v);

	void permute_block(block *data, int nblocks);

	void permute_data(void*data, int nbytes);

	block H(block in, uint64_t id);
	//void H(block *out, block *in, int n, uint64_t id);

	void Hn(block*out, block* in, uint64_t id, int length, block * scratch = nullptr);

	template<int n>
	void H(block out[n], block in[n], uint64_t id) {
		block scratch[n];
		for (int i = 0; i < n; ++i) {
			out[i] = scratch[i] = xorBlocks(double_block(in[i]), _mm_loadl_epi64((__m128i const *) (&id)));
			++id;
		}
		permute_block(scratch, n);
		xorBlocks_arr(out, scratch, out, n);
	}

	/*void Hn(block*out, block* in, uint64_t id, int length, block * scratch = nullptr) {
		bool del = false;
		if (scratch == nullptr) {
			del = true;
			scratch = new block[length];
		}
		for (int i = 0; i < length; ++i) {
			out[i] = scratch[i] = xorBlocks(double_block(in[i]), _mm_loadl_epi64((__m128i const *) (&id)));
			++id;
		}
		permute_block(scratch, length);
		xorBlocks_arr(out, scratch, out, length);
		if (del) {
			delete[] scratch;
			scratch = nullptr;
		}
	}*/
};


}//emp
}//juzix

