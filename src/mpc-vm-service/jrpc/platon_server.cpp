#ifndef _WIN32
#include <stdlib.h>

#include "platon_server.h"
/*
This is a simulate server.

cd current folder and
> g++ PlatonServer.cpp ABI.cpp -ljsoncpp -ljsonrpccpp-common -ljsonrpccpp-client -ljsonrpccpp-server -osimulateserver -std=c++11
*/
int main(int argc, char* argv[]) {
	int port = 9988;
	if (argc >= 2) {
		port = atoi(argv[1]);
	}
	return SimulatePlatONNode(port);
}
#endif
