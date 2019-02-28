#include <typeinfo>
#include <string>
#include <iostream>
#include <thread>
#include <cassert>

#include "emp-sh2pc/emp-sh2pc.h"
#include "emp-tool/io/net_io_channel.h"
#include "emp-tool/io/mem_io_channel.h"
#include "emp-tool/utils/pb_utils.h"
#include "emp-tool/utils/utils.h"

#include "jit.h"

#ifdef OT_NP_USE_MIRACL
#include "miracl.h"
#endif

using namespace platon::mpc;
using namespace std;
using namespace emp;

void testFooAdd(){
}



int test_pb_add(const std::string& ir, int32_t va, int32_t vb)
{
	char szbuf1[16] = { 0 };
	char szbuf2[16] = { 0 };
	int len_a = WriteVarInt32(szbuf1, 16, va);
	int len_b = WriteVarInt32(szbuf2, 16, vb);

	
	char* inputs[2];
	unsigned lengths[2];

	inputs[0] = szbuf1;
	inputs[1] = szbuf2;
	lengths[0] = len_a;
	lengths[1] = len_b;

	//call the jit module
	char* result = nullptr;
	int result_len = PlatON_RunFuncByLazyJIT(ir.data(), "EuclideanDistance", &result, inputs, lengths);
	if (result_len <= 0)
	{
		printf("the result len: %d, jit run failed !\n", result_len);
		return 1;
	}
    
	printf("the result len: %d \n", result_len);

	int32_t out;
	int ret = ReadVarInt32(result, result_len, out);
	printf("the output of ListMsg (a-b) is: %d , ret: %d\n", out, ret);
    
    ret = PlatON_RunFuncByLazyJIT(ir.data(), "free", (char **)result, 0, 0);
    printf("ret %d\n", ret);

	return 0;
}

void test_normal(int32_t a, int32_t b, int role, int port)
{
#ifdef _WIN32
    std::string ir = "E:/work/local/private-contract-compiler/build32/Debug/bin/Samples.cpp.ll";
#else
    std::string ir = "/root/work/z/sample/Samples.cpp.ll";
#endif

	IOChannel* io = new NetIO(role == ALICE ? nullptr : "127.0.0.1", port);
	setup_semi_honest(io, role);
    test_pb_add(ir, a, b);

	delete io;
}
#include<thread>
using namespace std;

#ifndef _WIN32
#include <dlfcn.h>
#endif

int main(int argc, char** argv) 
{
    if (argc < 3)  {
        printf("./xxx party port\n");
        return 1;
    }

#ifdef _WIN32
    std::vector<std::string> libs{
  //      "E:/work/local/private-contract-vm/build/bin/Debug/mpc_vm_core.dll",
  //      "E:/work/local/private-contract-vm/build/bin/Debug/libprotobufd.dll",
  //      "E:/work/local/private-contract-vm/build/bin/Debug/libprotobuf-lited.dll"
    };
#else

    std::vector<std::string> libs{
        "/root/work/y/private-contract-vm/build/lib/libmpc_vm_core.so",
        "/root/work/y/private-contract-vm/build/lib/libprotobuf.so.3.6.1",
        "/root/work/y/private-contract-vm/build/lib/libprotobuf-lite.so.3.6.1",
        "/root/work/z/sample/InternalProto.so"
    };
#endif
    for (auto lib : libs) {
        cout << "lib: " << lib << endl;
    }
    if (!PlatON_InitJIT(libs))
    {
       printf("PlatON_RunFuncByLazyJIT failed !\n");
        return 1;
    }

    printf("PlatON_RunFuncByLazyJIT success !\n");

	int port, party;
	parse_party_and_port(argv, &party, &port);
	test_normal(19, 10, party, port);

}
