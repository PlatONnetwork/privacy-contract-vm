#include <typeinfo>
#include "platon_integer.h"
#include "emp-tool/circuits/batcher.h"
#include "emp-tool/execution/plain_circ.h"
#include "emp-tool/execution/plain_prot.h"
#include "emp-tool/io/net_io_channel.h"
#include "emp-tool/utils/utils.h"
#include "emp-sh2pc/semihonest.h"
#include "emp-tool/circuits/float_circuit.h"
#include <cassert>

using namespace platon::mpc;

using namespace std;

bool accurate(double a, double b, double err) {
	if (fabs(a - b) < err*a && fabs(a - b) < err*b)
		return true;
	else return false;
}
template<typename Op, typename Op2>
void test_float(double precision, int runs = 100) {
	PRG prg(fix_key);
	for(int i = 0; i < runs; ++i) {
		int64_t ia, ib;
		prg.random_data(&ia, 8);
		prg.random_data(&ib, 8);
		double da = ia / 1000.0;
		double db = ib / 1000.0;
		while( ! accurate(Op()((float)da, (float)db),  Op()(da, db), precision )) {
			prg.random_data(&ia, 8);
			prg.random_data(&ib, 8);
			da = ia / 1000.0;
			db = ib / 1000.0;
		}
	
		Float a(24, 9, da, ALICE);
		Float b(24, 9, db, BOB);
		Float res = Op2()(a, b);

		if (! accurate(res.reveal<double>(PUBLIC), Op()(da,db), precision)) {
			cout << "Inaccuracy:\t"<<typeid(Op2).name()<<"\t"<< da <<"\t"<<db<<"\t"<<Op()(da,db)<<"\t"<<res.reveal<double>(PUBLIC)<<endl<<flush;
		}
		assert(accurate(res.reveal<double>(PUBLIC),  Op()(da,db), precision*10));
	}
	cout << typeid(Op2).name()<<"\t\t\tDONE"<<endl;
}

void scratch_pad() {
	Float a(24, 9, 709.466, ALICE);
	Float b(24, 9, 540.486, BOB);
	cout << a.reveal<double>(PUBLIC)<<endl;
	cout << b.reveal<double>(PUBLIC)<<endl;
	cout << (a+b).reveal<double>(PUBLIC)<<endl;
	cout << (a-b).reveal<double>(PUBLIC)<<endl;
	cout << (a*b).reveal<double>(PUBLIC)<<endl;
	double res = (a/b).reveal<double>(BOB);
	cout << res <<endl;
}

int main(int argc, char** argv) {
	PRG prg;
	int port, party;
	parse_party_and_port(argv, &party, &port);
	NetIO * io = new NetIO(party==ALICE ? nullptr : "127.0.0.1", port);
	setup_semi_honest(io, party);
//	scratch_pad();return 0;
	test_float<std::plus<float>, std::plus<Float>>(1e-4);
	test_float<std::minus<float>, std::minus<Float>>(1e-4);
	test_float<std::multiplies<float>, std::multiplies<Float>>(1e-4);
	test_float<std::divides<float>, std::divides<Float>>(1e-4);

	delete io;
	return 0;
}

