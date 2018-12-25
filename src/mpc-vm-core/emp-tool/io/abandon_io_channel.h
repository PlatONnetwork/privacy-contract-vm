#pragma once

#include "io_channel.h"

namespace platon
{
	namespace mpc
	{
		// Essentially drop all communication
		class AbandonIO : public IOChannel {
		public:
			int size = 0;
			
			int send_data_impl(const void * data, int len) {
				size += len;
				return 0;
			}

			int recv_data_impl(void  * data, int len) {
				return 0;
			}
		};

	}
}
