#ifndef NETWORK_IO_CHANNEL
#define NETWORK_IO_CHANNEL

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <thread>
#include <chrono>
#include "io_channel.h"

using std::string;


#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>

#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
#include <WS2tcpip.h>
#include <Windows.h>
#else
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#endif//

namespace platon
{
	namespace mpc
	{
		class NetIO : public IOChannel {
		public:
			NetIO(const char * address, int port);
			~NetIO();

			void sync();
			void setNoDelay(bool flag);

			void flush();

			int send_data_impl(const void * data, int len);

			int recv_data_impl(void  * data, int len);


		private:
			bool is_server;
			int mysocket = -1;
			int consocket = -1;
			char * buffer = nullptr;
			bool has_sent = false;
			std::string addr;
			int port;
			uint64_t counter = 0;
		};

	}
}



#endif  //NETWORK_IO_CHANNEL
