#include "emp-tool/io/net_io_channel.h"
#include "platon_core_conf.h"
#include "platon_logger.h"

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

#include "emp-tool/utils/sys_utils.h"


using namespace std;


namespace platon
{
	namespace mpc
	{


		NetIO::NetIO(const char * address, int port)
		{
#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
			static bool bLoad = false;
			if (!bLoad)
			{
				WORD wVersionRequested;
				WSADATA wsaData;
				int err;

				/* Use the MAKEWORD(lowbyte, highbyte) macro declared in Windef.h */
				wVersionRequested = MAKEWORD(2, 2);

				err = WSAStartup(wVersionRequested, &wsaData);
				if (err != 0)
				{
					printf("Windows load socket failed with error: %d\n", err);
					return;
				}
				bLoad = true;
			}
#endif

			this->port = port;
			is_server = (address == nullptr);

			if (address == nullptr)
			{
				struct sockaddr_in dest;
				struct sockaddr_in serv;
				socklen_t socksize = sizeof(struct sockaddr_in);
				memset(&serv, 0, sizeof(serv));
				serv.sin_family = AF_INET;
				serv.sin_addr.s_addr = htonl(INADDR_ANY); /* set our address to any interface */
				serv.sin_port = htons(port);           /* set the server port number */
				mysocket = socket(AF_INET, SOCK_STREAM, 0);
				int reuse = 1;
				setsockopt(mysocket, SOL_SOCKET, SO_REUSEADDR, (const char*)&reuse, sizeof(reuse));
				if (::bind(mysocket, (struct sockaddr *)&serv, sizeof(struct sockaddr)) < 0) {
					perror("error: bind");
					exit(1);
				}

				//cout << "run as server to listen..." << endl;
				if (listen(mysocket, 1) < 0) {
					perror("error: listen");
					exit(1);
				}
				LOGD("waiting for connect...");
				consocket = accept(mysocket, (struct sockaddr *)&dest, &socksize);

				LOGD("a client has a client connected ...");
			}
			else
			{
				addr = string(address);
				struct sockaddr_in dest;
				consocket = socket(AF_INET, SOCK_STREAM, 0);
				memset(&dest, 0, sizeof(dest));
				dest.sin_family = AF_INET;
				dest.sin_addr.s_addr = inet_addr(address);
				dest.sin_port = htons(port);
				LOGD("11 wait to connect...");
				while (connect(consocket, (struct sockaddr *)&dest, sizeof(struct sockaddr)) == -1) {
					SysUtils::ms_sleep(1000);
					LOGD("22 waiting to connect...");
				}
				LOGD("33 client connected.");
				//cout << "the client has connected to server..." << endl;
			}
			setNoDelay(true);

			buffer = new char[NETWORK_BUFFER_SIZE];
			memset(buffer, 0, NETWORK_BUFFER_SIZE);
			//		setvbuf(stream, buffer, _IOFBF, NETWORK_BUFFER_SIZE);
					//cout << "connected"<<endl;
		}

		void NetIO::sync()
		{
			int tmp = 0;
			if (is_server)
			{
				send_data(&tmp, 1);
				recv_data(&tmp, 1);
			}
			else
			{
				recv_data(&tmp, 1);
				send_data(&tmp, 1);
				flush();
			}
		}

		NetIO::~NetIO()
		{
#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
			closesocket(consocket);
			closesocket(mysocket);
#else
			close(consocket);
			close(mysocket);
#endif//

			delete[] buffer;
		}

		void NetIO::setNoDelay(bool flag)
		{
#if defined(WIN32)  || defined(__WIN32__) || defined(_WIN32) || defined(_WIN64) || defined(__WINDOWS__)
			bool nodelay = flag;
			int ret = setsockopt(consocket, SOL_SOCKET, TCP_NODELAY, (char *)&nodelay, sizeof(nodelay));
			if (ret == SOCKET_ERROR) {
				printf("setsockopt for SO_KEEPALIVE failed with error: %d, %s\n", SysUtils::getErrno(), SysUtils::getErrStr());
			}
#else
			const int one = flag ? 1 : 0;
			setsockopt(consocket, IPPROTO_TCP, TCP_NODELAY, &one, sizeof(one));
#endif//
		}

		void NetIO::flush()
		{
		}

		int NetIO::send_data_impl(const void * data, int len)
		{
			counter += len;
			int sent = 0;
			while (sent < len)
			{
				int res = send(consocket, (char*)data + sent, len - sent, 0);
				if (res >= 0)
				{
					sent += res;
					//cout << "the expect len: " << len << " NetIO::send_data_impl has sent: " << sent << " bytes! this round send: " << res << endl;
				}
				else
				{
					fprintf(stderr, "error: send %d\n", res);
					break;
				}
			}
			has_sent = true;

			if (sent != len)
				return -1;

			return len;
		}

		int NetIO::recv_data_impl(void  * data, int len)
		{
			has_sent = false;
			int sent = 0;
			while (sent < len)
			{
				int res = recv(consocket, (char*)data + sent, len - sent, 0);
				if (res > 0)
				{
					sent += res;
					//cout << "the expect len: " << len << "NetIO::recv_data_impl has received: " << sent << " bytes! this round recv: " << res << endl;
				}
				else if (res == 0)
				{
					//cout << "the peer has closed !" << endl;
					//SocketUtils::CloseSocket(consocket);
					break;
				}
				else
				{
					fprintf(stderr, "to recv %d bytes failed!! error: recv %d, errno: %d \n", len, res, errno);
					if (errno == EINTR || errno == EAGAIN)
						continue;

					break;
				}
			}

			if (sent != len)
				return -1;

			return len;
		}

	}//emp
}//platon

