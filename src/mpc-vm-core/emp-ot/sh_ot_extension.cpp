#include "emp-ot/sh_ot_extension.h"
#include <string.h>
#include "emp-tool/utils/utils.h"
#include <algorithm>
#include <cstdint>
#include "platon_core_conf.h"
#include "platon_logger.h"


using namespace std;


namespace platon
{
	namespace mpc
	{


		SHOTExtension::SHOTExtension(IOChannel * io) : OTExtension(io)
		{
		}

		SHOTExtension::~SHOTExtension()
		{
		}

		int SHOTExtension::send(block* data0, const block* data1, int length)
		{
			return send_cot(data0, *data1, length);
		}

		int SHOTExtension::recv(block* data, const bool* b, int length)
		{
			return recv_cot(data, b, length);
		}

		int SHOTExtension::send_cot(block * data0, block delta, int length)
		{
			int ret = 0;
			auto startTime = std::chrono::high_resolution_clock::now();
			int64_t otStartTime = io->get_io_time();
			ret = send_pre(length);
			if (ret != 0)
			{
				LOGE("send_cot pre send failed !");
				return -1;
			}

			ret = cot_send_post(data0, delta, length);
			if (ret != 0)
			{
				LOGE("cot_send_post failed !");
				return -1;
			}

			std::chrono::duration<int64_t, std::micro> elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime);
			int64_t waitTime = elapsed_microseconds.count();
			LOGI("OT cost time: %lld us, calculate time: %lld", waitTime, waitTime - (io->get_io_time() - otStartTime));
			LOGI("OT io cost time: %lld us, io send bytes: %d, recv bytes: %d, io count: %d, send count: %d, recv count: %d", \
				io->get_io_time() - otStartTime, io->get_io_sentbytes(), io->get_io_recvbytes(), \
				io->get_io_count(), io->get_io_sent_count(), io->get_io_recv_count());

			return 0;
		}

		int SHOTExtension::recv_cot(block* data, const bool* b, int length)
		{
			auto startTime = std::chrono::high_resolution_clock::now();
			int64_t otStartTime = io->get_io_time();
			int ret = recv_pre(b, length);
			if (ret != 0)
			{
				LOGE("recv_pre failed !");
				return -1;
			}

			ret = cot_recv_post(data, b, length);
			if (ret != 0)
			{
				LOGE("cot_recv_post failed !");
				return -1;
			}

			std::chrono::duration<int64_t, std::micro> elapsed_microseconds = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - startTime);
			int64_t waitTime = elapsed_microseconds.count();
			LOGI("OT cost time: %lld us, calculate time: %lld", waitTime, waitTime - (io->get_io_time() - otStartTime));
			LOGI("OT io cost time: %lld us, io send bytes: %d, recv bytes: %d, io count: %d, send count: %d, recv count: %d", \
				io->get_io_time() - otStartTime, io->get_io_sentbytes(), io->get_io_recvbytes(), \
				io->get_io_count(), io->get_io_sent_count(), io->get_io_recv_count());

			return 0;
		}

		int SHOTExtension::cot_send_post(block* data0, block delta, int length)
		{
			const int bsize = AES_BATCH_SIZE;//1k blocks
			block *pad = new block[2 * bsize];
			block *tmp = new block[2 * bsize];

			int ret = -1;
			for (int i = 0; i < length; i += bsize)
			{
				for (int j = i; j < i + bsize && j < length; ++j)
				{
					pad[2 * (j - i)] = Qq[j];
					pad[2 * (j - i) + 1] = xorBlocks(Qq[j], bs);
				}
				//prp.Hn(pad, pad, 2 * i, 2 * bsize, tmp);
				crh.Hn(pad, pad, 2 * bsize, tmp);
				for (int j = i; j < i + bsize && j < length; ++j)
				{
					data0[j] = pad[2 * (j - i)];
					pad[2 * (j - i)] = xorBlocks(pad[2 * (j - i)], delta);
					tmp[j - i] = xorBlocks(pad[2 * (j - i) + 1], pad[2 * (j - i)]);
				}

				int sendSize = sizeof(block)*min(bsize, length - i);
				ret = io->send_data(tmp, sendSize);
				if (ret != 0)
				{
					LOGE("cot_send_post send blocks failed !!");
					delete[]pad;
					delete[]tmp;
					return -1;
				}
			}
			delete[]pad;
			delete[]tmp;
			delete[] Qq;
			Qq = nullptr;

			return 0;
		}

		int SHOTExtension::cot_recv_post(block* data, const bool* r, int length)
		{
			const int bsize = AES_BATCH_SIZE;
			block res[bsize];
			for (int i = 0; i < length; i += bsize) {
				if (0 != io->recv_data(res, sizeof(block)*min(bsize, length - i)))
				{
					LOGE("shot recv data size: %d failed.", sizeof(block)*min(bsize, length - i));
					return -1;
				}

				if (bsize <= length - i) crh.H<bsize>(data + i, Tt + i);
				else crh.Hn(data + i, Tt + i, length - i);
				for (int j = 0; j < bsize && j < length - i; ++j) {
					if (r[i + j]) data[i + j] = xorBlocks(res[j], data[i + j]);
				}
			}
			if (Tt != nullptr)
			{
				delete []Tt; 
				Tt = nullptr;
			}	

			return 0;
		}


	}//emp
}//platon

