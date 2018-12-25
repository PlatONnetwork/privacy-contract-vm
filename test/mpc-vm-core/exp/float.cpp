#include "test.h"
#include <typeinfo>
#include "emp-tool/circuits/float_circuit.h"
#include <iostream>

using namespace std;
using namespace platon::mpc;



//bool accurate(double a, double b, double err) {
//	if (fabs(a - b) < err*a && fabs(a - b) < err*b)
//		return true;
//	else return false;
//}

bool accurate(double a, double b, double err) {
	if (fabs(a - b) < err)
		return true;
	else return false;
}

template<typename Op, typename Op2>
void test_float(double precision, int runs = 1000) {

	PRG prg;
	for (int i = 0; i < runs; ++i) {
		long long ia, ib;
		prg.random_data(&ia, 8);
		prg.random_data(&ib, 8);
		double da = ia / 1000.0;
		double db = ib / 1000.0;
		while (! accurate(Op()((float)da, (float)db), Op()(da, db), precision)) {
			prg.random_data(&ia, 8);
			prg.random_data(&ib, 8);
			da = ia / 1000.0;
			db = ib / 1000.0;
		}

		Float a(24, 9, da, PUBLIC);
		Float b(24, 9, db, PUBLIC);
		Float res = Op2()(a, b);

		double plainRet = Op()(da, db);
		double result = res.reveal<double>(PUBLIC);
		if (! accurate(result, plainRet, precision)) {
			cout << "Inaccuracy:\t" << typeid(Op2).name() << "\t" << da << "\t" << db << "\t" << Op()(da, db) << "\t" << res.reveal<double>(PUBLIC) << endl << flush;
		}
		assert(accurate(res.reveal<double>(PUBLIC), Op()(da, db), precision * 10));
	}
	cout << typeid(Op2).name() << "\t\t\tDONE" << endl;

	/*PRG prg;
	for(int i = 0; i < runs; ++i) {
		long long ia, ib;
		prg.random_data(&ia, 8);
		prg.random_data(&ib, 8);
		double da = ia / 1000.0;
		double db = ib / 1000.0;
		while(! accurate(Op()((float)da, (float)db),  Op()(da, db), precision )) {
			prg.random_data(&ia, 8);
			prg.random_data(&ib, 8);
			da = ia / 1000.0;
			db = ib / 1000.0;
		}
	
		Float a(24, 9, da, PUBLIC);
		Float b(24, 9, db, PUBLIC);
		Float res = Op2()(a,b);

		EXPECT_FALSE(!accurate(res.reveal<double>(PUBLIC), Op()(da, db), precision)) <<
			"Inaccuracy:\t" << typeid(Op2).name() << "\t" << da << "\t" << db << "\t" << Op()(da, db) << "\t" << res.reveal<double>(PUBLIC) << endl;

		EXPECT_TRUE(accurate(res.reveal<double>(PUBLIC), Op()(da, db), precision * 10)) << " the " << precision * 10 << " is not match !" << endl;
	}
	
	cout << "Test operator: " << typeid(Op2).name()<<"\t\t\tDONE"<<endl;*/
}

void scratch_pad() {
	Float a(24, 9, 0.21, PUBLIC);
	Float b(24, 9, 0.41, PUBLIC);
	cout << a.reveal<double>(PUBLIC)<<endl;
	cout << b.reveal<double>(PUBLIC)<<endl;
	cout << (a+b).reveal<double>(PUBLIC)<<endl;
	cout << (a-b).reveal<double>(PUBLIC)<<endl;
	cout << (a*b).reveal<double>(PUBLIC)<<endl;
	double res = (a/b).reveal<double>(BOB);
	cout << res <<endl;
}

TEST_F(PlatonPlainTestFixture, TestSampleValue)
{
	//Float a(24, 9, -3483999046495688.0, PUBLIC);
	//Float b(24, 9, 1770078793584692.8, PUBLIC);
	//Float ret = a - b;
	//double result = ret.reveal<double>(PUBLIC);
	//double plain = -3483999046495688.0 - 1770078793584692.8;

	//cout << "-3483999046495688.0-1770078793584692.8=" << result << ", plain: " << plain << endl;

	Float a(24, 9, -3483.0, PUBLIC);
	Float b(24, 9, 17700.8, PUBLIC);
	Float ret = a - b;
	double result = ret.reveal<double>(PUBLIC);
	double plain = -3483.0 - 17700.8;

	cout << "-3483.0-17700.8=" << result << ", plain: " << plain << endl;

	EXPECT_TRUE(accurate(result, plain, 0.01)) << ", NOT accurate ! in precision: 0.0001" << endl;
}

TEST_F(PlatonPlainTestFixture, TestPlainOperator)
{
	/*test_float<std::plus<float>, std::plus<Float>>(1e-3);
	test_float<std::minus<float>, std::minus<Float>>(1e-3);
	test_float<std::multiplies<float>, std::multiplies<Float>>(1e-3);*/
	//test_float<std::divides<float>, std::divides<Float>>(1e-3);

	
	

}

//
//int main(int argc, char** argv) {
//	setup_plain_prot(false, "");
////	scratch_pad();return 0;
//	test_float<std::plus<float>, std::plus<Float>>(1e-3);
//	test_float<std::minus<float>, std::minus<Float>>(1e-3);
//	test_float<std::multiplies<float>, std::multiplies<Float>>(1e-3);
//	test_float<std::divides<float>, std::divides<Float>>(1e-3);
//
//	finalize_plain_prot();
//	return 0;
//}
