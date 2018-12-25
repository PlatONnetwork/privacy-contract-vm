#include "emp-tool/utils/block_utils.h"
#include <cassert>
#include "emp-tool/utils/sse.h"

//#ifdef _WIN32
//#include <smmintrin.h>
//#endif


namespace platon
{
	namespace mpc
	{

		const char fix_key[] = "\x61\x7e\x8d\xa2\xa0\x51\x1e\x96"
			"\x5e\x41\xc2\x9b\x15\x3f\xc7\x7a";


		bool getLSB(const block & x) {
			return (*((char*)&x) & 1) == 1;
		}

		block makeBlock(int64_t x, int64_t y) {
			return juzix_mm_set_epi64x(x, y);
		}

		block zero_block() {
			return juzix_mm_setzero_si128();
		}

		block one_block() {
			return makeBlock(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL);
		}


		block make_delta(const block & a) {
			return juzix_mm_or_si128(makeBlock(0L, 1L), a);
		}

		block xorBlocks(block x, block y) {
			return juzix_mm_xor_si128(x, y);
		}

		block andBlocks(block x, block y) {
			return juzix_mm_and_si128(x, y);
		}


		void xorBlocks_arr(block* res, const block* x, const block* y, int nblocks) {
			const block * dest = nblocks + x;
			for (; x != dest;) {
				*(res++) = xorBlocks(*(x++), *(y++));
			}
		}

		void xorBlocks_arr(block* res, const block* x, block y, int nblocks) {
			const block * dest = nblocks + x;
			for (; x != dest;) {
				*(res++) = xorBlocks(*(x++), y);
			}
		}


		bool cmpBlock(const block * x, const block * y, int nblocks) {
			const block * dest = nblocks + x;
			for (; x != dest;) {
				block vcmp = juzix_mm_xor_si128(*(x++), *(y++));
				if (!juzix_mm_testz_si128(vcmp, vcmp))
					return false;
			}
			return true;
		}

		//deprecate soon
		bool block_cmp(const block * x, const block * y, int nblocks) {
			return cmpBlock(x, y, nblocks);
		}

		bool isZero(const block * b) {
//#ifdef WIN32
			return juzix_mm_testz_si128(*b,*b) > 0;
//#else
//			return _mm_testz_si128(*b, *b) > 0;
//#endif
		}

		bool isOne(const block * b) {
			//block neq = juzix_mm_xor_si128(*b, one_block());
			block neq = juzix_mm_xor_si128(*b, _mm_set_epi64x(0xFFFFFFFFFFFFFFFFULL, 0xFFFFFFFFFFFFFFFFULL));
//#ifdef WIN32
			return juzix_mm_testz_si128(neq, neq) > 0;
//#else
//			return _mm_testz_si128(neq, neq) > 0;
//#endif
		}

		std::string toHex(const std::vector<unsigned char> input)
		{
			std::string hex;
			char szTemp[32];
			for (int i = 0; i < input.size(); ++i)
			{
				snprintf(szTemp, sizeof(szTemp), "%02X", input[i]);
				hex.append(szTemp);
			}

			return hex;
		}

		void static printBlock(const block* b)
		{
			for (int i = 0; i < sizeof(*b); ++i)
			{
				printf("%02X", *(((unsigned char*)b) + i) & 0xff);

			}
			printf("\n");
		}

		void printBlock(const block* b, int num)
		{
			for (int i = 0; i < num; ++i)
			{
				printBlock(b + i);
			}
		}

		void printBools(const bool* b, int num)
		{
			for (unsigned int i = 0; i < sizeof(*b) * num; ++i)
			{
				printf("%02X", *(((unsigned char*)b) + i) & 0xff);

			}
			printf("\n");
		}

		std::string getBytes(void* b, int num)
		{
			std::string bytes;
			char szBuff[32];
			for (int i = 0; i < num; ++i)
			{
				snprintf(szBuff, 32, "%02X", *(((unsigned char*)b) + i) & 0xff);
				bytes += szBuff;
			}

			return bytes;
		}


		static const uint8_t* input_column(const uint8_t *_input, int _x, int _y, int _ncols)
		{
			return (const uint8_t*)&_input[(_x)* _ncols / 8 + (_y) / 8];
		}

		static uint8_t* output_row(uint8_t *_output, int _x, int _y, int _nrows)
		{
			return (uint8_t*)&_output[(_y)* _nrows / 8 + (_x) / 8];
		}


		//Modified from
		//https://mischasan.wordpress.com/2011/10/03/the-full-sse2-bit-matrix-transpose-routine/
		// with inner most loops changed to _mm_set_epi8 and _mm_set_epi16
		void sse_trans(uint8_t *out, uint8_t const *inp, int nrows, int ncols) {
			//#   define INP(x,y) inp[(x)*ncols/8 + (y)/8]
			//#   define OUT(x,y) out[(y)*nrows/8 + (x)/8]
			int rr, cc, i, h;
			union { block x; uint8_t b[16]; } tmp;
			block vec;
			assert(nrows % 8 == 0 && ncols % 8 == 0);

			// Do the main body in 16x8 blocks:
			for (rr = 0; rr <= nrows - 16; rr += 16) {
				for (cc = 0; cc < ncols; cc += 8) {
					vec = juzix_mm_set_epi8(
						*(const int8_t*)input_column(inp, rr + 15, cc, ncols), *(const int8_t*)input_column(inp, rr + 14, cc, ncols), \
						*(const int8_t*)input_column(inp, rr + 13, cc, ncols), *(const int8_t*)input_column(inp, rr + 12, cc, ncols), \
						*(const int8_t*)input_column(inp, rr + 11, cc, ncols), *(const int8_t*)input_column(inp, rr + 10, cc, ncols), \
						*(const int8_t*)input_column(inp, rr + 9, cc, ncols), *(const int8_t*)input_column(inp, rr + 8, cc, ncols), \
						*(const int8_t*)input_column(inp, rr + 7, cc, ncols), *(const int8_t*)input_column(inp, rr + 6, cc, ncols), \
						*(const int8_t*)input_column(inp, rr + 5, cc, ncols), *(const int8_t*)input_column(inp, rr + 4, cc, ncols), \
						*(const int8_t*)input_column(inp, rr + 3, cc, ncols), *(const int8_t*)input_column(inp, rr + 2, cc, ncols), \
						*(const int8_t*)input_column(inp, rr + 1, cc, ncols), *(const int8_t*)input_column(inp, rr + 0, cc, ncols));
					for (i = 8; --i >= 0; vec = juzix_mm_slli_epi64(vec, 1))
						*(uint16_t*)output_row(out, rr, cc + i, nrows) = juzix_mm_movemask_epi8(vec);
				}
			}
			if (rr == nrows) return;

			// The remainder is a block of 8x(16n+8) bits (n may be 0).
			//  Do a PAIR of 8x8 blocks in each step:
			for (cc = 0; cc <= ncols - 16; cc += 16) {
				vec = juzix_mm_set_epi16(
					*(uint16_t const*)input_column(inp, rr + 7, cc, ncols), *(uint16_t const*)input_column(inp, rr + 6, cc, ncols),
					*(uint16_t const*)input_column(inp, rr + 5, cc, ncols), *(uint16_t const*)input_column(inp, rr + 4, cc, ncols),
					*(uint16_t const*)input_column(inp, rr + 3, cc, ncols), *(uint16_t const*)input_column(inp, rr + 2, cc, ncols),
					*(uint16_t const*)input_column(inp, rr + 1, cc, ncols), *(uint16_t const*)input_column(inp, rr + 0, cc, ncols));
				for (i = 8; --i >= 0; vec = juzix_mm_slli_epi64(vec, 1)) {
					*output_row(out, rr, cc + i, nrows) = h = juzix_mm_movemask_epi8(vec);
					*output_row(out, rr, cc + i + 8, nrows) = h >> 8;
				}
			}
			if (cc == ncols) return;

			//  Do the remaining 8x8 block:
			for (i = 0; i < 8; ++i)
				tmp.b[i] = *input_column(inp, rr + i, cc, ncols);///////////////////////////////////// kelvin here !!!!
			for (i = 8; --i >= 0; tmp.x = juzix_mm_slli_epi64(tmp.x, 1))
				*output_row(out, rr, cc + i, nrows) = juzix_mm_movemask_epi8(tmp.x);
			//#undef INP
			//#undef OUT
		}

		block double_block(block bl) {
			const block mask = juzix_mm_set_epi32(135, 1, 1, 1);
			block tmp = juzix_mm_srai_epi32(bl, 31);
			tmp = juzix_mm_and_si128(tmp, mask);
			//tmp = _mm_shuffle_epi32(tmp, _MM_SHUFFLE(2,1,0,3));
			bl = juzix_mm_slli_epi32(bl, 1);
			return juzix_mm_xor_si128(bl, tmp);
		}

	}//emp
}//platon



