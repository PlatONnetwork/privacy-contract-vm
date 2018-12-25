#pragma once

#ifndef OT_NP_USE_MIRACL
#include "emp-tool/utils/relic_hash.h"
#else
#include "emp-tool/utils/hash.h"
#endif//

#include "emp-tool/io/hash_io_channel.h"
#include "emp-tool/io/net_io_channel.h"

namespace platon
{
	namespace mpc
	{
		class HashIO : public IOChannel 
		{
		public:
			Hash h;
			NetIO * netio;
			HashIO(NetIO * _netio) {
				this->netio = _netio;
			}

			int send_data_impl(const void * data, int len) {
				h.put(data, len);
				return len;
			}
			int recv_data_impl(void  * data, int len) {
				netio->recv_data(data, len);
				h.put(data, len);
				return len;
			}
			void get_digest(char * dgst) {
				h.digest(dgst);
			}
		};

	}
}


