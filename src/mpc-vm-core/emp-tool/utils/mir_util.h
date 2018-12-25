#ifdef OT_NP_USE_MIRACL
#pragma once


#include "emp-tool/utils/block_utils.h"


#ifdef __cplusplus
extern "C" {
#endif//

#include <miracl.h>

#ifdef __cplusplus
}
#endif



namespace platon
{
	namespace mpc
	{

		int get_ep_sizes();

        int get_fb_bytes();
        
		int get_byte_size(big x);
		int get_bigs_size(const big* x, int num);

		int get_point_size(epoint *ep);

		void point_to_bytes(unsigned char *buf, epoint *ep);
		int epoint_to_bytes(unsigned char *buf, const epoint *ep);
		int get_epoints_size(epoint** eps, size_t num);

		void bytes_to_point(epoint* ep, unsigned char *buf, int len);
		int bytes_to_epoint(epoint* ep, const unsigned char *buf, int len);

		//---------------------------------- function for test ---------------------------------------
		void printBig(big a);

		void printPoint(epoint *a);

		void printBlock(block *a);

		void printHex(const unsigned char* buffer, int size);
	}
}

#endif
