#include "test.h"
#include "io_channel.h"
#include "emp-tool/io/net_io_channel.h"
#include "emp-tool/utils/utils.h"
#include "emp-ot/sh_ot_extension.h"
#include "emp-ot/np_ot.h"
#include <iostream>

using namespace platon::mpc;
using namespace std;

int main(int argc, char** argv) {
	int length = 1<<24, port, party;
	parse_party_and_port(argv, 2, &party, &port);
	NetIO * io = new NetIO(party==ALICE ? nullptr:"127.0.0.1", port);
	//cout <<"NPOT\t"<<10000.0/test_ot<NPOT>(io, party, 10000)*1e6<<" OTps"<<endl;
	cout <<"Semi Honest OT Extension\t"<<double(length)/test_ot<SHOTExtension>(io, party, length)*1e6<<" OTps"<<endl;
	delete io;
}

