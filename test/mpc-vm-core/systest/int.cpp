#include <typeinfo>
#include "emp-sh2pc/emp-sh2pc.h"
#include "emp-tool/io/net_io_channel.h"
#include "emp-tool/io/mem_io_channel.h"
#include "emp-tool/utils/utils.h"
#include "platon_integer.h"
#include <string>
#include <iostream>
#include <thread>
#include <cassert>

#ifndef WIN32
#include "jit.h"
#include "pb_utils.h"
#endif//

#include "miracl.h"

using namespace platon::mpc;
using namespace platon::mpc;
using namespace std;


template<typename Op, typename Op2>
void test_int(int party, int range1 = 1<<25, int range2 = 1<<25, int runs = 100) {
	PRG prg(fix_key);
	for(int i = 0; i < runs; ++i) {
		int64_t ia, ib;
		prg.random_data(&ia, 8);
		prg.random_data(&ib, 8);
		ia %= range1;
		ib %= range2;
		while( Op()(int(ia), int(ib)) != Op()(ia, ib) ) {
			prg.random_data(&ia, 8);
			prg.random_data(&ib, 8);
			ia %= range1;
			ib %= range2;
		}
	
		Integer a((int)ia, ALICE);
		Integer b((int)ib, BOB);

		Integer res = Op2()(a,b);

		if (res.reveal_int(PUBLIC) != Op()(ia,ib)) {
			cout << ia <<"\t"<<ib<<"\t"<<Op()(ia,ib)<<"\t"<<res.reveal_int(PUBLIC)<<endl<<flush;
		}
		assert(res.reveal_int(PUBLIC) == Op()(ia,ib));
	}
	cout << typeid(Op2).name()<<"\t\t\tDONE"<<endl;
}

void scratch_pad() {
	Integer a(9, ALICE);
	cout << "HW "<<a.hamming_weight().reveal_string(PUBLIC)<<endl;
	cout << "LZ "<<a.leading_zeros().reveal_string(PUBLIC)<<endl;
}

void test_full(int party)
{
	////	scratch_pad();return 0;
	test_int<std::plus<int>, std::plus<Integer>>(party);
	test_int<std::minus<int>, std::minus<Integer>>(party);
	test_int<std::multiplies<int>, std::multiplies<Integer>>(party);
	test_int<std::divides<int>, std::divides<Integer>>(party);
	test_int<std::modulus<int>, std::modulus<Integer>>(party);

	test_int<std::bit_and<int>, std::bit_and<Integer>>(party);
	test_int<std::bit_or<int>, std::bit_or<Integer>>(party);
	test_int<std::bit_xor<int>, std::bit_xor<Integer>>(party);
	
}

#ifndef WIN32
int test_pb_add(const std::string& ir, int32_t va, int32_t vb)
{
	char szbuf1[16] = { 0 };
	char szbuf2[16] = { 0 };
	int len_a = WriteVarInt32(szbuf1, 16, va);
	int len_b = WriteVarInt32(szbuf2, 16, vb);

	std::vector<std::string> libs{ "../lib/libmpc_vm_core.so", "/usr/local/lib/libprotobuf.so.17", "/usr/local/lib/libprotobuf-lite.so.17" };
	if (!PlatON_InitJIT(libs))
	{
		printf("PlatON_RunFuncByLazyJIT failed !\n");
		return 1;
	}
	
	char* inputs[2];
	unsigned lengths[2];

	inputs[0] = szbuf1;
	inputs[1] = szbuf2;
	lengths[0] = len_a;
	lengths[1] = len_b;

	//call the jit module
	char* result = nullptr;
	int result_len = PlatON_RunFuncByLazyJIT(ir.data(), "ListMsg", &result, inputs, lengths);
	if (result_len <= 0)
	{
		printf("the result len: %d, jit run failed !\n", result_len);
		return 1;
	}
	printf("the result len: %d \n", result_len);

	int32_t out;
	int ret = ReadVarInt32(result, result_len, out);
	printf("the output of ListMsg (a-b) is: %d , ret: %d\n", out, ret);
	free(result);

	return 0;
}
#endif

int test_add(int32_t va, int32_t vb)
{
	Integer a(va, ALICE);
	Integer b(vb, BOB);
	int ret = (a + b).reveal_int(PUBLIC);
	printf("the mpc a+b=%d\n", ret);
	printf("the plain a+b=%d\n", va + vb);

	return ret;
}


static void test_threads(long long a, long long b, int port)
{
	std::thread* threads[2]{ nullptr, nullptr };
	IOChannel* channels[2]{ nullptr, nullptr };

	int results[2]{ 0, 0 };
	for (int i = 0; i < 2; ++i)
	{
		threads[i] = new std::thread([&](IOChannel** _io, int _role, long long _a, long long _b, int* _results) {
			if (!*_io)
				*_io = new NetIO(_role == ALICE ? nullptr : "127.0.0.1", port);

			std::thread::id this_id = std::this_thread::get_id();
			std::cout << "the threadid: " << this_id << std::endl;

			setup_semi_honest(*_io, _role);
			*_results = test_add(a, b);
#ifndef WIN32
			std::string ir = "./testint.ll";
			test_pb_add(ir, a, b);
#endif//
			//test_full(_role);
		}, &(channels[i]), i, a, b, results + i);
	}

	for (int i = 0; i < 2; ++i)
	{
		threads[i]->join();
		delete threads[i];

		if (channels[i])
		{
			delete channels[i];
			channels[i] = nullptr;
		}
	}

	//check
	if ((results[0]) == (results[1]) && (results[0]) == (a + b))
	{
		cout << "+ the mpc test a+b is [ right ] !" << endl;
	}
	else
	{
		cout << "* the mpc test a+b is [ right ] !" << endl;
	}
}

void test_normal(int32_t a, int32_t b, int role, int port)
{
	IOChannel* io = new NetIO(role == ALICE ? nullptr : "127.0.0.1", port);

	setup_semi_honest(io, role);
	for (int i =0 ; i < 2; ++i)
		int result = test_add(a, b);
	//test_full(role);

	delete io;
}

int main(int argc, char** argv) 
{
	
	int port, party;
	parse_party_and_port(argv, &party, &port);
	
	if (argc >= 4)
	{
		test_threads(19, 10, port);
	}
	else
	{
		test_normal(19, 10, party, port);
	}
}
