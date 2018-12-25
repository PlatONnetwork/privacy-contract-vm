#pragma once


#include "emp-ot/ot_extension.h"
#include "io_channel.h"
#include "emp-tool/utils/crh.h"

namespace platon
{
	namespace mpc
	{

		class SHOTExtension : public OTExtension
		{
		public:
			SHOTExtension(IOChannel * io);

			~SHOTExtension();

			virtual int send(block* data0, const block* data1, int length);
			virtual int recv(block* data, const bool* b, int length);


			//	void got_send_post(block* data0, const block* data1, int length);
			//	void got_recv_post(block* data, const bool* r, int length);

			int send_cot(block * data0, block delta, int length);
			int recv_cot(block* data, const bool* b, int length);
			//	
			//	void send_rot(block * data0, block * data1, int length);
			//	void recv_rot(block* data, const bool* b, int length);
			//
			//    void send_got(block * data0, block * data1, int length);
			//	void recv_got(block* data, const bool* b, int length);

			int cot_send_post(block* data0, block delta, int length);
			int cot_recv_post(block* data, const bool* r, int length);

			//	void rot_send_post(block* data0, block* data1, int length);
			//	void rot_recv_post(block* data, const bool* r, int length);

			//  void got_send_post(block* data0, block* data1, int length);
			//  void got_recv_post(block* data, const bool* r, int length);



		};

	}
}