#include "test.h"
#include "platon_integer.h"
#include <typeinfo>
#include <iostream>


using namespace std;
using namespace platon::mpc;

template<typename Op, typename Op2>
void test_int(int party, int range1 = 1<<25, int range2 = 1<<25, int runs = 2 << 10) 
{
	PRG prg;
	for(int i = 0; i < runs; ++i) 
	{
		int64_t ia, ib;
		prg.random_data(&ia, 8);
		prg.random_data(&ib, 8);
		ia %= range1;
		ib %= range2;
		
		int count = 0;
		bool state = false;
		while((state = (Op()(int(ia), int(ib)) != Op()(ia, ib))) ) 
		{
		    EXPECT_TRUE(state) << " ia, ib " << ia  <<","  << ib << "should equal, count = " << ++count << endl;
			prg.random_data(&ia, 8);
			prg.random_data(&ib, 8);
			ia %= range1;
			ib %= range2;
		}
	
		Integer a((int)ia, ALICE); 
		Integer b((int)ib, BOB);

		Integer res = Op2()(a,b);
		int plainRet = res.reveal_int(PUBLIC);
		int ret = Op()(ia, ib);

		EXPECT_TRUE(plainRet == ret) << ia << "\t" << ib << "\t" << Op()(ia, ib) << "\t" << res.reveal_int(PUBLIC) << endl;
	}

	cout << "+++ Test of \t " << typeid(Op2).name()<<"\t\t\tDONE"<<endl;
}

//void scratch_pad() 
//{
//	Integer a(99, 9, ALICE);
//	cout << "HW "<<a.hamming_weight().reveal_string(PUBLIC)<<endl;
//	cout << "LZ "<<a.leading_zeros().reveal_string(PUBLIC)<<endl;
//
//	Integer b(32, 9, BOB);
//	cout << "HW " << b.hamming_weight().reveal_string(PUBLIC) << endl;
//	cout << "LZ " << b.leading_zeros().reveal_string(PUBLIC) << endl;
//
//	Integer p(32, 9, PUBLIC);
//	cout << "HW " << p.hamming_weight().reveal_string(PUBLIC) << endl;
//	cout << "LZ " << p.leading_zeros().reveal_string(PUBLIC) << endl;
//}

TEST_F(PlatonPlainTestFixture, TestIntSimpleAdd)
{
	cout << "the threadid of TestIntSimpleAdd" << this_thread::get_id() << endl;
	int party = PUBLIC;

	Integer a((int)3, ALICE);
	Integer b((int)-3, BOB);
	
	int result = (a + b).reveal_int(PUBLIC);
	EXPECT_TRUE(result == 0) << "3-3=0, but here not right !" << endl;
}

TEST_F(PlatonPlainTestFixture, TestRandIntOp)
{
	cout << "the threadid of TestIntAllOp" << this_thread::get_id() << endl;
	int party = PUBLIC;

	test_int<std::plus<int>, std::plus<Integer>>(party);
	test_int<std::minus<int>, std::minus<Integer>>(party);
	test_int<std::multiplies<int>, std::multiplies<Integer>>(party);
	test_int<std::divides<int>, std::divides<Integer>>(party);
	test_int<std::modulus<int>, std::modulus<Integer>>(party);

	test_int<std::bit_and<int>, std::bit_and<Integer>>(party);
	test_int<std::bit_or<int>, std::bit_or<Integer>>(party);
	test_int<std::bit_xor<int>, std::bit_xor<Integer>>(party);
}
