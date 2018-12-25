#ifndef OT_NP_USE_MIRACL
#pragma once


#ifdef __cplusplus
extern "C" {
#endif

#include "relic.h"
#include "relic_types.h"

#ifdef __cplusplus
}
#endif

#include <stdio.h>
#include "emp-tool/utils/block_utils.h"
#include "emp-tool/utils/relic_hash.h"


#define ECC_PACK false
#define BIT_LEN 128
#define EB_SIZE 65


//printf("##** call in XXXl function func##l(H p) function=%s\n", __FUNCTION__);

#define __batch(func)\
    template <typename H> void func##l(H p) {\
            func(p);\
        }\
	template <typename H, typename V, typename... T> void func##l(H p, V v, T... t) {\
		func(p);\
		func##l(p, t...);\
	}\
	
	

#define __batch2(func)\
template <typename H1, typename H2, typename H3> void func##_norm(H1 h1, H2 h2, H3 h3) {\
	func(h1, h2, h3);\
	eb_norm(h1, h1);\
}\

#define __batch3(func)\
template <typename H1, typename H2, typename H3, typename H4> void func##_mod(H1 h1, H2 h2, H3 h3, H4 h4) {\
	func(h1, h2, h3);\
	bn_mod(h1, h1, h4);\
}\



namespace platon
{
	namespace mpc
	{


typedef eb_t eb_tpl[2];
typedef bn_t bn_tpl[2];



__batch(eb_new);
__batch(eb_free);
__batch(bn_new);
__batch(bn_free);



__batch2(eb_mul);
__batch2(eb_mul_fix);
__batch2(eb_sub);
__batch2(eb_add);



__batch3(bn_mul);
__batch3(bn_sub);
__batch3(bn_add);



//static bool initialized = false;


//TODO: fix the following.
block KDF(eb_t in);

void bn_to_block(block * b, const bn_t bn);

void block_to_bn(bn_t bn, const block * b);


int initialize_relic();

}
}

#endif//OT_NP_USE_MIRACL
