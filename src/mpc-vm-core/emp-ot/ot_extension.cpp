#include "emp-ot/ot_extension.h"
#include <string.h>
#include "emp-tool/utils/utils.h"
#include "emp-ot/np_ot.h"

#include "platon_logger.h"

namespace platon
{
	namespace mpc
	{

		OTExtension::OTExtension(IOChannel * io)
		{
			this->io = io;
			this->ssp = ssp;

			//LOGI("** use relic ot OTNP !");
			base_ot = new NPOT(io);

			pad_r = new bool[blockSize];
			array_s = new bool[l];
			k0 = new block[l];
			k1 = new block[l];
			G0 = new PRG[l];
			G1 = new PRG[l];
			ssp = 0;
		}

		OTExtension::~OTExtension()
		{
			delete base_ot;
			delete[] pad_r;
			delete[] array_s;
			delete[] k0;
			delete[] k1;
			delete[] G0;
			delete[] G1;

			if (Qq != nullptr)
				delete[]Qq;

			if (Tt != nullptr)
				delete[]Tt;
		}

		int OTExtension::setup_send(block * in_k0 /*= nullptr */, bool * is /*= nullptr */)
		{
			init = true;
			if (is != nullptr) {
				memcpy(k0, in_k0, l * sizeof(block));
				memcpy(array_s, is, l);
				bs = bits_2_block(array_s);
			}
			else {
				prg.random_bool(array_s, l);
				if (base_ot->recv(k0, array_s, l) != 0)  // S's input as the reciver is s
				{
					LOGE("OTNP recv failed");
					return -1;
				}
				bs = bits_2_block(array_s);
			}
			for (int i = 0; i < l; ++i)
				G0[i].reseed(&k0[i]);

			return 0;
		}

		int OTExtension::setup_recv(block * in_k0 /*= nullptr */, block * in_k1 /*= nullptr */)
		{
			int ret = 0;
			init = true;

			if (in_k0 != nullptr) {
				memcpy(k0, in_k0, l * sizeof(block));
				memcpy(k1, in_k1, l * sizeof(block));
			}
			else {
				prg.random_block(k0, l);
				prg.random_block(k1, l);

				ret = base_ot->send(k0, k1, l);  // R's input as the sender is k0,k1
				if (ret != 0)
				{
					LOGE("NPOT send failed !");
					return -1;
				}
			}
			for (int i = 0; i < l; ++i) {
				G0[i].reseed(&k0[i]);
				G1[i].reseed(&k1[i]);
			}

			return 0;
		}


		int OTExtension::padded_length(int length)
		{
			return ((length + blockSize - 1) / blockSize)*blockSize;
		}

		int OTExtension::send_pre(int length)
		{
			int ret = 0;
			length = padded_length(length);
			block *q = new block[blockSize];

			if (Qq != nullptr)
				delete[]Qq;
			Qq = new block[length];

			if (!init) {
				ret = setup_send();
				if (ret != 0)
				{
					LOGE("setup ot faiiled !");
					delete[]q;
					return -1;
				}
			}

			int bsize = l * (blockSize >> 7);
			block *recvbuff = new block[bsize];
			for (int j = 0; j < length / blockSize; ++j)
			{
				//recv batch size of blocks
				if (io->recv_data(recvbuff, bsize * sizeof(block)))
				{
					LOGE("send_pre  ot-extension recv %d blocks failed !", bsize);
					delete[]recvbuff;
					delete[] q;
					return -1;
				}
				for (int i = 0; i < l; ++i)
				{
					G0[i].random_data(q + (i*blockSize >> 7), blockSize >> 3);

					if (array_s[i])
						xorBlocks_arr(q + (i*blockSize >> 7), q + (i*blockSize >> 7), recvbuff + 8 * i, blockSize >> 7);
				}
				sse_trans((uint8_t *)(Qq + j * blockSize), (uint8_t*)q, 128, blockSize);
			}
			delete[]recvbuff;
			delete[] q;

			LOGD("ot extension send_pre done !");
			return 0;
		}

		int OTExtension::recv_pre(const bool* in_r, int length)
		{
			int old_length = length;
			length = padded_length(length);
			//block t[blockSize];
			block *t = new block[blockSize];
			if (Tt != nullptr)
				delete[]Tt;
			Tt = new block[length];

			int ret = 0;
			if (!init)
			{
				ret = setup_recv();
				if (ret != 0)
				{
					LOGE("ot-extensiion setup_recv failed !");
					delete[]t;
					return -1;
				}
			}

			bool * r2 = new bool[length];
			prg.random_bool(pad_r, blockSize);
			memcpy(r2, in_r, old_length);
			memcpy(r2 + old_length, pad_r, length - old_length);

			block *block_r = new block[length >> 7];
			for (int i = 0; i < length / 128; ++i)
			{
				block_r[i] = bits_2_block(r2 + (i << 7));
			}

			//todo: batch tmp to send, send 1k blocks one time
			int bsize = l * blockSize >> 7;
			block *sendBuff = new block[bsize];
			for (int j = 0; j * blockSize < length; ++j)
			{
				for (int i = 0; i < l; ++i)
				{
					G0[i].random_data(t + (i*blockSize >> 7), blockSize >> 3);
					G1[i].random_data(sendBuff + 8 * i, blockSize >> 3);
					xorBlocks_arr(sendBuff + 8 * i, t + (i*blockSize >> 7), sendBuff + 8 * i, blockSize >> 7);
					xorBlocks_arr(sendBuff + 8 * i, block_r + (j*blockSize >> 7), sendBuff + 8 * i, blockSize >> 7);//tmp=ui=t^i+G1+r, tmp = sendBuff[i] 
				}
				sse_trans((uint8_t *)(Tt + j * blockSize), (uint8_t*)t, 128, blockSize);

				if (0 != io->send_data(sendBuff, sizeof(block) * bsize))//发送Ui, 一个128bytes=> 16 * 8 => 8 blocks
				{
					LOGE("send_pre send random blocks failed !");
					delete[]sendBuff;
					delete[] block_r;
					delete[] r2;
					delete[] t;
					return -1;
				}
			}

			delete[]sendBuff;
			delete[] block_r;
			delete[] r2;
			delete[] t;

			LOGD("ot extension send_pre done !");
			return 0;
		}


	}
}


