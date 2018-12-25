#pragma once

#include "io_channel.h"
#include "block.h"
#include "emp-tool/utils/sse.h"
#include "emp-tool/utils/utils.h"
#include "emp-tool/utils/prp.h"
#include "emp-tool/execution/circuit_execution.h"
#include "emp-tool/garble/garble_gate_halfgates.h"
#include <iostream>

namespace platon
{
	namespace mpc
	{
		class HalfGateGen : public CircuitExecution {
		public:
			int64_t gid = 0;
			block delta_;
			PRP prp;
			block seed;
			IOChannel* io = nullptr;
			bool with_file_io = false;
			block fix_point;

			void setup(IOChannel* _io)
			{
				io = _io;
				PRG prg(fix_key); 
				prg.random_block(&fix_point, 1);
				PRG tmp;
				tmp.random_block(&seed, 1);
				block a;
				tmp.random_block(&a, 1);
				set_delta(a);
			}

			HalfGateGen(IOChannel * _io)
			{
				setup(_io);
			}

			~HalfGateGen()
			{
			}

			bool is_public(const block & b, int party) {
				return isZero(&b) || isOne(&b);
			}
			void set_delta(const block &_delta) {
				delta_ = make_delta(_delta);
			}
			block public_label(bool b) {
				return b ? one_block() : zero_block();
			}
#ifdef WIN32
			static 
			block platon_func_mm_xor_si128(block x, block y)//_mm_xor_si128
			{
				block blk;
				uint64_t* pt = (uint64_t*)&blk;
				uint64_t* ptx = (uint64_t*)&x;
				uint64_t* pty = (uint64_t*)&y;
				pt[0] = ptx[0] ^ pty[0];
				pt[1] = ptx[1] ^ pty[1];

				return blk;
		}
#endif//
			bool isDelta(const block & b) {
#ifndef WIN32
				__m128i neq = _mm_xor_si128(b, delta_);
				return _mm_testz_si128(neq, neq);
#else
				__m128i neq = platon_func_mm_xor_si128(b, delta_);
				return juzix_mm_testz_si128(neq, neq);
#endif
				//return _mm_testz_si128(neq, neq);
			}

			block and_gate(const block& a, const block& b) override {
				block out[2], table[2];
				if (isZero(&a) || isZero(&b)) {
					return zero_block();
				}
				else if (isOne(&a)) {
					return b;
				}
				else if (isOne(&b)) {
					return a;
				}
				else {
					garble_gate_garble_halfgates(a, xorBlocks(a, delta_), b, xorBlocks(b, delta_),
						&out[0], &out[1], delta_, table, gid++, &prp.aes);
					io->send_block(table, 2);
					return out[0];
				}
			}
			block xor_gate(const block&a, const block& b) override {
				if (isOne(&a))
					return not_gate(b);
				else if (isOne(&b))
					return not_gate(a);
				else if (isZero(&a))
					return b;
				else if (isZero(&b))
					return a;
				else {
					block res = xorBlocks(a, b);
					if (isZero(&res))
						return fix_point;
					if (isDelta(res))
						return xorBlocks(fix_point, delta_);
					else
						return res;//xorBlocks(a, b);
				}
			}
			block not_gate(const block&a) override {
				if (isZero(&a))
					return one_block();
				else if (isOne(&a))
					return zero_block();
				else
					return xorBlocks(a, delta_);
			}
			void generic_to_xor(const block* new_b0, const block * b0, const block * b1, int length) {
				block h[4];
				for (int i = 0; i < length; ++i) {
					h[0] = prp.H(b0[i], 2 * i);
					h[1] = prp.H(b0[i], 2 * i + 1);
					h[2] = prp.H(b1[i], 2 * i);
					h[3] = prp.H(b1[i], 2 * i + 1);

					h[1] = xorBlocks(new_b0[i], h[1]);
					h[3] = xorBlocks(new_b0[i], h[3]);
					h[3] = xorBlocks(delta_, h[3]);
					io->send_block(h, 4);
				}
			}

			block delta() {
				return delta_;
			}
		};
	}
}
