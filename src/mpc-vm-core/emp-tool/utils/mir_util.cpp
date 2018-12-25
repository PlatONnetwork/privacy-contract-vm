#ifdef OT_NP_USE_MIRACL
#include "mir_util.h"
#include "platon_core_conf.h"
#include "platon_logger.h"



namespace platon
{
	namespace mpc
	{


		const static int EP_SIZES = 64;
		const static int FB_BYTES = 32;

        int get_ep_sizes()
        {
            return EP_SIZES;
        }

        int get_fb_bytes()
        {
            return FB_BYTES;
        }
        
		int get_byte_size(big x) {
			int bit_x = numdig(x) << 2;
			int byte_x = (bit_x / 8) + (int)((bit_x % 8) > 0);
			return byte_x;
		}


		int get_bigs_size(const big* x, int num) 
		{
			int size = 0;
			for (int i = 0; i < num; ++i)
			{
				size += get_byte_size(x[i]);
			}

			return size;
		}

		int get_point_size(epoint *ep) {
			big x, y;
			x = mirvar(0);
			y = mirvar(0);
			int len_x, len_y;
			epoint_get(ep, x, y);
			len_x = get_byte_size(x);
			len_y = get_byte_size(y);

			//free mem
			mirkill(x);
			mirkill(y);
			//LOGD("the epoint size: %d ", len_x + len_y);
			return len_x + len_y;
		}

		int get_epoints_size(epoint** eps, size_t num)
		{
			int size = 0;
			for (int i = 0; i < num; ++i)
				size += get_point_size(*(eps + i));

			return size;
		}

		void point_to_bytes(unsigned char *buf, epoint *ep)
		{
			big x, y;
			//alloc mem
			x = mirvar(0);
			y = mirvar(0);
			epoint_get(ep, x, y);
			big_to_bytes(32, x, (char*)buf, 1);
			big_to_bytes(32, y, (char*)(buf + FB_BYTES), 1);

			//free mem
			mirkill(x);
			mirkill(y);
		}

		int epoint_to_bytes(unsigned char *buf, const epoint *ep)
		{
			int len = 0;
			big x, y;
			//alloc mem
			x = mirvar(0);
			y = mirvar(0);
			epoint_get((epoint*)ep, x, y);

			len += big_to_bytes(32, x, (char*)buf, 0);
			len += big_to_bytes(32, y, (char*)(buf + len), 0);

			//free mem
			mirkill(x);
			mirkill(y);
			//printf("epoint len: %d \n", len);
			return len;
		}

		void bytes_to_point(epoint* ep, unsigned char *buf, int len)
		{
			big x, y;
			x = mirvar(0);
			y = mirvar(0);
			bytes_to_big(FB_BYTES, (const char*)buf, x);
			bytes_to_big(FB_BYTES, (const char*)(buf + FB_BYTES), y);
			epoint_set(x, y, 1, ep);

			//free mem
			mirkill(x);
			mirkill(y);

		}

		int bytes_to_epoint(epoint* ep, const unsigned char *buf, int len)
		{
			big x, y;
			x = mirvar(0);
			y = mirvar(0);
			bytes_to_big(len/2, (const char*)buf, x);
			bytes_to_big(len/2, (const char*)(buf + len/2), y);
			epoint_set(x, y, 1, ep);

			//free mem
			mirkill(x);
			mirkill(y);

			return len;
		}

		//---------------------------------- function for test ---------------------------------------
		void printBig(big a)
		{
			int size = FB_BYTES;
			char* buf = new char[size];

			big_to_bytes(FB_BYTES, a, buf, 1);
			printHex((unsigned char*)buf, size);
			delete[]buf;
		}

		void printHex(const unsigned char* buffer, int size)
		{
			for (int i = 0; i < size; ++i)
			{
				printf("%02X", buffer[i] & 0xff);
			}
			printf("\n");
		}

		void printPoint(epoint *a)
		{
			int size = EP_SIZES;
			unsigned char *buf = new unsigned char[size];
			memset(buf, 0, size);

			point_to_bytes(buf, a);
			printHex(buf, size);

			delete[]buf;
		}

		void printBlock(block *a)
		{
			printHex((unsigned char*)a, sizeof(a));
		}

	}

}

#endif
