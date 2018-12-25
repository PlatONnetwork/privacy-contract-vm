//#include "emp-tool/circuits/integer.h"
//#include "emp-tool/execution/plain_circ.h"
//#include "emp-tool/execution/plain_prot.h"
//#include "emp-tool/io/net_io_channel.h"
//#include "emp-tool/utils/utils.h"
//#include "emp-sh2pc/semihonest.h"
//
//
//using namespace platon::emp;
//using namespace std;
//
//const string circuit_file_location = macro_xstr(EMP_CIRCUIT_PATH);
//
//int port, party;
//string file = circuit_file_location+"/AES-non-expanded.txt";//adder_32bit.txt";
//CircuitFile cf(file.c_str());
//
//void test() {
//	auto start = clock_start();
//	Integer a(128, 2, ALICE);
//	Integer b(128, 3, BOB);
//	Integer c(128, 1, PUBLIC);
//	for(int i = 0; i < 10000; ++i) {
//			cf.compute((block*)c.bits, (block*)a.bits, (block*)b.bits);
//	}
//	cout << time_from(start)<<" "<<party<<" "<<c.reveal<string>(BOB)<<endl;
//
//}
//int main(int argc, char** argv) {
//	parse_party_and_port(argv, &party, &port);
//	NetIO* io = new NetIO(party==ALICE?nullptr:"127.0.0.1", port);
//
//	setup_semi_honest(io, party);
//	test();
//	
//	delete io;
//}
