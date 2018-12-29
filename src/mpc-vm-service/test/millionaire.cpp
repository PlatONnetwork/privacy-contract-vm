#include "emp-sh2pc/emp-sh2pc.h"
#include "emp-tool/io/net_io_channel.h"
#include "emp-tool/utils/pb_utils.h"
#include <string>
#include <iostream>
#include <thread>


//#ifndef WIN32
#include "jit.h"
//#endif//

//#include "miracl.h"



using namespace platon::mpc;
using namespace std;
using namespace emp;


const static std::string ir_full_name = "millionaire.cpp.ll";
const static std::string ir_func_name = "Millionaire";

template<typename T>
void test_Millionaire(int party, T v, T v2) {
	unsigned char* inputs[2] = { 0 };

	unsigned char szbuf1[16] = { 0 };
	unsigned char szbuf2[16] = { 0 };
	unsigned int len_a = WriteVarInt32(szbuf1, 16, v);
	unsigned int len_b = WriteVarInt32(szbuf2, 16, v2);

	printf("mpc role: %d, input: %d \n", party, party == emp::ALICE ? v : v2);

	inputs[0] = szbuf1;
	inputs[1] = szbuf2;

	unsigned lengths[2] = { 0 };
	lengths[0] = len_a;
	lengths[1] = len_b;
	char* result = nullptr;

	int result_len = PlatON_RunFuncByLazyJIT(ir_full_name.c_str(), ir_func_name.c_str(), &result, (char**)inputs, lengths);
	if (result_len <= 0)
	{
		printf("jit run faied !!!!\n");
		return;
	}

	printf("role: %s, mpc run result_len : %d \n", party == emp::ALICE  ? "Alice" : "Bob", result_len);

	int value;
	int len = ReadVarInt32(result, result_len, value);

	if (value == 1)
		printf("alice is the winner result : %d \n", value);
	else
		printf("bob is the winner, result: %d\n", value);

	delete result;

	PlatON_ClearJITCache(ir_full_name.data());
}

static void test_threads(long long a, long long b, int port)
{
	std::thread* threads[2]{ nullptr, nullptr };
	IOChannel* channels[2]{ nullptr, nullptr };

	for (int i = 0; i < 2; ++i)
	{
		threads[i] = new std::thread([&](IOChannel** _io, int _role, long long _a, long long _b) {
			if (!*_io)
				*_io = new NetIO(_role == ALICE ? nullptr : "127.0.0.1", port);

			std::thread::id this_id = std::this_thread::get_id();
			std::cout << "the threadid: " << this_id << std::endl;

			setup_semi_honest(*_io, _role);
			test_Millionaire(i, a, b);
		}, &(channels[i]), i, a, b);
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
}

void test_normal(int32_t a, int32_t b, int role, int port)
{
	IOChannel* io = new NetIO(role == ALICE ? nullptr : "127.0.0.1", port);

	setup_semi_honest(io, role);
	test_Millionaire(role, a, b);

	delete io;
}

int main(int argc, char** argv)
{
	int port, party;
	parse_party_and_port(argv, &party, &port);
#ifndef _WIN32
	std::vector<std::string> libs{ "../lib/libemp-tool.so", "../lib/libprotobufd.so"};
#else
	std::vector<std::string> libs{ "./emp-tool.dll", "./libprotobufd.dll" };
#endif
	if (!PlatON_InitJIT(libs))
	{
		printf("PlatON_RunFuncByLazyJIT failed !\n");
		return 1;
	}

	if (argc >= 4)
	{
		test_threads(19, 10, port);
	}
	else
	{
		test_normal(19, 10, party, port);
	}
}
