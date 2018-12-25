#pragma once


#include <cstddef>
#include "emp-tool/garble/aes.h"


#ifndef OT_NP_USE_MIRACL
#include "relic_ec_utils.h"
#endif



namespace platon
{
	namespace mpc
	{


class PRG
{ 
public:
	uint64_t counter = 0;
	AES_KEY aes;
	//static void* rnd;
	
public:
	PRG(const void * seed = nullptr, int id = 0);
	PRG(const PRG& obj);
	
	void reseed(const void * key, uint64_t id = 0);

	void random_data(void *data, int nbytes);

	void random_bool(bool * data, int length);

	void random_data_unaligned(void *data, int nbytes);

	void random_block(block * data, int nblocks=1);

#ifndef OT_NP_USE_MIRACL
//    template<typename T>
//	void random_bn(T t) {
//		random_bn(t);
//	}
	
	template<typename T, typename ... L>
	void random_bn(T t, L... l) {
		random_bn(l...);
		random_bn(t);
	}

	void random_bn(bn_t a, int sign = BN_POS, int bits = BIT_LEN);

	void random_bn(bn_t *a, int length=1, int sign = BN_POS, int bits = BIT_LEN);

//    template<typename T>
//	void random_eb(T t) {
//		random_eb(t);
//	}
	
	template<typename T, typename ... L>
	void random_eb(T t, L... l) {
		random_eb(l...);
		random_eb(t);
	}


    void random_eb(eb_t p);

    void random_eb(eb_t *p, int length=1);
#endif
	
};


}//emp
}//juzix

