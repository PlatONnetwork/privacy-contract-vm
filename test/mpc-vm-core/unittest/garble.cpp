#include "io_channel.h"
#include "emp-tool/io/net_io_channel.h"
//#include "emp-tool/io/mem_io_channel.h"
//#include "emp-tool/io/file_io_channel.h"
//#include "emp-tool/io/hash_io_channel.h"
#include "emp-tool/circuits/circuit_file.h"
#include "emp-tool/utils/prg.h"
#include "emp-tool/gc/halfgate_eva.h"
#include "emp-tool/gc/halfgate_gen.h"
#include <iostream>

using namespace std;
using namespace platon::mpc;

int port, party;

void test(IOChannel* netio) {
	block *a = new block[128];
	block *b = new block[128];
	block *c = new block[128];

	PRG prg;
	prg.random_block(a, 128);
	prg.random_block(b, 128);

	string file = "AES-non-expanded.txt";
	CircuitFile cf(file.c_str());

	const static int COUNT = 100;
	if(party == BOB) {
		CircuitExecution* circ_exec = new HalfGateEva(netio);
		CircuitExecutionProxy::circ_exec.setup(circ_exec);

		for(int i = 0; i < COUNT; ++i)
			cf.compute(c, a, b);
		
		CircuitExecutionProxy::circ_exec.finalize();
	} 
	else
	{
		CircuitExecution* circ_exec = new HalfGateGen(netio);
		CircuitExecutionProxy::circ_exec.setup(circ_exec);

		auto start = clock_start();
		for(int i = 0; i < COUNT; ++i) {
			cf.compute(c, a, b);
		}
		auto interval = time_from(start);
		cout << "AES garbling + Loopback Network : "<< 10000*6800/interval<<" million gate per second\n";

		CircuitExecutionProxy::circ_exec.finalize();
	}

	delete[] a;
	delete[] b;
	delete[] c;
}

int main(int argc, char** argv) {
	parse_party_and_port(argv, argc, &party, &port);
	//arg1: role,    arg2: port
	IOChannel* io = new NetIO(party == ALICE?nullptr:"127.0.0.1",  port);
	test(io);
	delete io;
}
