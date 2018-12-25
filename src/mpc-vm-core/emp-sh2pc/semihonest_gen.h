#pragma once

#include "emp-tool/execution/protocol_execution.h"
#include "emp-ot/sh_ot_extension.h"
#include "emp-tool/gc/halfgate_gen.h"
#include "io_channel.h"

namespace platon
{
	namespace mpc
	{
		//template<typename IO>
		class SemiHonestGen : public ProtocolExecution {
		public:
			IOChannel* io_ = nullptr;
			SHOTExtension* ot_ = nullptr;
			PRG prg_;
			PRG shared_prg_;
			
			void setup(IOChannel* io)
			{
				io_ = io;
				
				if (ot_)
				{
					delete ot_;
				}
				ot_ = new SHOTExtension(io);

				block seed; prg_.random_block(&seed, 1);
				io->send_block(&seed, 1);
				shared_prg_.reseed(&seed);
			}

			SemiHonestGen(IOChannel* io) : ProtocolExecution(ALICE)
			{
				setup(io);
			}

			~SemiHonestGen() {
				if (ot_)
					delete ot_;
			}

			void feed(block * label, int party, const bool* b, int length) {
				if (party == ALICE) {
					shared_prg_.random_block(label, length);
					for (int i = 0; i < length; ++i) {
						if (b[i])
							label[i] = xorBlocks(label[i], CircuitExecutionProxy::circ_exec.delta());
					}
				}
				else {
					ot_->send_cot(label, CircuitExecutionProxy::circ_exec.delta(), length);
				}
			}

			void reveal(bool* b, int party, const block * label, int length) {
				if (party == XOR) {
					for (int i = 0; i < length; ++i) {
						if (isOne(&label[i]) || isZero(&label[i]))
							b[i] = false;
						else
							b[i] = getLSB(label[i]);
					}
					return;
				}
				for (int i = 0; i < length; ++i) {
					if (isOne(&label[i]))
						b[i] = true;
					else if (isZero(&label[i]))
						b[i] = false;
					else {
						if (party == BOB || party == PUBLIC) {
							io_->send_block(&label[i], 1);
							b[i] = false;
						}
						else if (party == ALICE) {
							block tmp;
							io_->recv_block(&tmp, 1);
							b[i] = !(block_cmp(&tmp, &label[i], 1));
						}
					}
				}
				if (party == PUBLIC)
					io_->recv_data(b, length);
			}
		};
	}
}