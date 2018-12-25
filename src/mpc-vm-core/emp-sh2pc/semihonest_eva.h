#pragma once

#include "emp-tool/execution/protocol_execution.h"
#include "emp-tool/gc/halfgate_eva.h"


namespace platon
{
	namespace mpc
	{
		//template<typename IO>
		class SemiHonestEva : public ProtocolExecution {
		public:
			IOChannel* io_ = nullptr;
			SHOTExtension* ot_ = nullptr;
			PRG shared_prg_;

			void setup(IOChannel* io)
			{
				io_ = io;

				//if (gc_)
				//{
				//	delete gc_;
				//	gc_ = new HalfGateEva(io);
				//}

				if (ot_)
				{
					delete ot_;
				}
				ot_ = new SHOTExtension(io);

				block seed; io->recv_block(&seed, 1);
				shared_prg_.reseed(&seed);
			}

			SemiHonestEva(IOChannel *io) : ProtocolExecution(BOB) {
				setup(io);
			}
			~SemiHonestEva() {
				if (ot_)
					delete ot_;
			}

			void feed(block * label, int party, const bool* b, int length) {
				if (party == ALICE) {
					shared_prg_.random_block(label, length);
				}
				else {
					ot_->recv_cot(label, b, length);
				}
			}

			void reveal(bool * b, int party, const block * label, int length) {
				if (party == XOR) {
					for (int i = 0; i < length; ++i) {
						if (isOne(&label[i]))
							b[i] = true;
						else if (isZero(&label[i]))
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
						block tmp;
						if (party == BOB || party == PUBLIC) {
							io_->recv_block(&tmp, 1);
							b[i] = !(block_cmp(&tmp, &label[i], 1));
						}
						else if (party == ALICE) {
							io_->send_block(&label[i], 1);
							b[i] = false;
						}
					}
				}
				if (party == PUBLIC)
					io_->send_data(b, length);
			}
		};
	}
}