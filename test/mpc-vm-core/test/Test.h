#include <typeinfo>
#include <string>
#include <iostream>
#include <random>
#include <climits>
#include <thread>
#include <cassert>
#include <cinttypes>

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


static std::default_random_engine e;

static std::uniform_int_distribution<int32_t> rand_int32(INT_MIN, INT_MAX);
static std::uniform_int_distribution<uint32_t> rand_uint32(0, UINT_MAX);
static std::uniform_int_distribution<int64_t> rand_int64(LLONG_MIN, LLONG_MAX);
static std::uniform_int_distribution<uint64_t> rand_uint64(0, ULLONG_MAX);

int test_function(const std::string& ir);

int Init() {

#ifdef _WIN32
    std::vector<std::string> libs{
        "E:/work/local/private-contract-vm/build32/bin/Debug/mpc_vm_core.dll",
        "E:/work/local/private-contract-vm/build32/bin/Debug/libprotobufd.dll",
        "E:/work/local/private-contract-vm/build32/bin/Debug/libprotobuf-lited.dll"
    };
#else
    std::vector<std::string> libs{
        // "/root/work/y/private-contract-vm/build/lib/libmpc_vm_core.so",
        // "/root/work/y/private-contract-vm/build/lib/libprotobuf.so.3.6.1",
        // "/root/work/y/private-contract-vm/build/lib/libprotobuf-lite.so.3.6.1"
    };
#endif

    if (!PlatON_InitJIT(libs))
    {
        printf("PlatON_InitJIT failed !\n");
        return -1;
    }
    printf("PlatON_InitJIT success !\n");

    return 0;
}

void Start(char* argv[]) {

    int role = atoi(argv[1]);
    int port = atoi(argv[2]);
    string r = string(argv[3]);

    IOChannel* io = new NetIO(role == ALICE ? nullptr : "127.0.0.1", port);
    setup_semi_honest(io, role);
    test_function(r);
    delete io;

}

/*
usage:
    ./mpc_test_functest_TestBasicType 1 9999 /root/work/z/sample/TestBasicType.cpp.ll
    ./mpc_test_functest_TestBasicType 2 9999 /root/work/z/sample/TestBasicType.cpp.ll
*/

int main(int argc, char* argv[])
{
    if (argc < 4) {
        printf("./xxx role port irpath\n");
        printf("      role: 1,alice 2,bob\n");
        return 1;
    }
    /*

    //#ifdef _WIN32
    //        std::string ir = "E:/work/local/private-contract-compiler/build32/Debug/bin/testint.cpp.ll";
    //#else
    //        std::string ir = "/root/work/z/sample/testint.cpp.ll";
    //#endif

    */

    if (0 != Init()) {
        return 1;
    }

    Start(argv);
}


/*
**
**
**
**
**
**
*/
int test_function_int32(const std::string& ir, int32_t a, int32_t b, const char* f) {

    printf("call f: %s, a: %d, b: %d\n", f, a, b);

    char szbuf1[64] = { 0 };
    char szbuf2[64] = { 0 };
    char* inputs[2];
    unsigned lengths[2];

    int len_a = WriteVarInt32(szbuf1, 64, a);
    int len_b = WriteVarInt32(szbuf2, 64, b);

    inputs[0] = szbuf1;
    inputs[1] = szbuf2;
    lengths[0] = len_a;
    lengths[1] = len_b;

    char* result = nullptr;
    int result_len = PlatON_RunFuncByLazyJIT(ir.c_str(), f, &result, inputs, lengths);
    if (result_len <= 0) {
        printf("the result len: %d, jit run failed !\n", result_len);
        return 1;
    }

    int32_t out;
    int ret = ReadVarInt32(result, result_len, out);
    printf("call f: %s, a: %d, b: %d, result_len: %d, ret: %d, out: %d\n", f, a, b, result_len, ret, out);

    PlatON_RunFuncByLazyJIT(ir.c_str(), "free", (char **)result, nullptr, nullptr);
    return 0;
}


int test_function_int64(const std::string& ir, int64_t a, int64_t b, const char* f) {

    printf("call f: %s, a: %" PRId64 ", b: %" PRId64 "\n", f, a, b);

    char szbuf1[64] = { 0 };
    char szbuf2[64] = { 0 };
    char* inputs[2];
    unsigned lengths[2];

    int len_a = WriteVarInt64(szbuf1, 64, a);
    int len_b = WriteVarInt64(szbuf2, 64, b);

    inputs[0] = szbuf1;
    inputs[1] = szbuf2;
    lengths[0] = len_a;
    lengths[1] = len_b;

    char* result = nullptr;
    int result_len = PlatON_RunFuncByLazyJIT(ir.c_str(), f, &result, inputs, lengths);
    if (result_len <= 0) {
        printf("the result len: %d, jit run failed !\n", result_len);
        return 1;
    }

    int64_t out;
    int ret = ReadVarInt64(result, result_len, out);
    printf("call f: %s, a: %" PRId64 ", b: %" PRId64 ", result_len: %d, ret: %d, out: %" PRId64 "\n", f, a, b, result_len, ret, out);

    PlatON_RunFuncByLazyJIT(ir.c_str(), "free", (char **)result, nullptr, nullptr);
    return 0;
}

int test_function_float(const std::string& ir, float a, float b, const char* f) {

    printf("call f: %s, a: %f, b: %f\n", f, a, b);

    char szbuf1[64] = { 0 };
    char szbuf2[64] = { 0 };
    char* inputs[2];
    unsigned lengths[2];

    int len_a = WriteVarFloat(szbuf1, 64, a);
    int len_b = WriteVarFloat(szbuf2, 64, b);

    inputs[0] = szbuf1;
    inputs[1] = szbuf2;
    lengths[0] = len_a;
    lengths[1] = len_b;

    char* result = nullptr;
    int result_len = PlatON_RunFuncByLazyJIT(ir.c_str(), f, &result, inputs, lengths);
    if (result_len <= 0) {
        printf("the result len: %d, jit run failed !\n", result_len);
        return 1;
    }

    float out;
    int ret = ReadVarFloat(result, result_len, out);
    printf("call f: %s, a: %f, b: %f, result_len: %d, ret: %d, out: %f\n", f, a, b, result_len, ret, out);

    PlatON_RunFuncByLazyJIT(ir.c_str(), "free", (char **)result, nullptr, nullptr);
    return 0;
}

int test_function_double(const std::string& ir, double a, double b, const char* f) {

    printf("call f: %s, a: %f, b: %f\n", f, a, b);

    char szbuf1[64] = { 0 };
    char szbuf2[64] = { 0 };
    char* inputs[2];
    unsigned lengths[2];

    int len_a = WriteVarDouble(szbuf1, 64, a);
    int len_b = WriteVarDouble(szbuf2, 64, b);

    inputs[0] = szbuf1;
    inputs[1] = szbuf2;
    lengths[0] = len_a;
    lengths[1] = len_b;

    char* result = nullptr;
    int result_len = PlatON_RunFuncByLazyJIT(ir.c_str(), f, &result, inputs, lengths);
    if (result_len <= 0) {
        printf("the result len: %d, jit run failed !\n", result_len);
        return 1;
    }

    double out;
    int ret = ReadVarDouble(result, result_len, out);
    printf("call f: %s, a: %f, b: %f, result_len: %d, ret: %d, out: %f\n", f, a, b, result_len, ret, out);

    PlatON_RunFuncByLazyJIT(ir.c_str(), "free", (char **)result, nullptr, nullptr);
    return 0;
}


int test_function_bool(const std::string& ir, bool a, bool b, const char* f) {

    printf("call f: %s, a: %d, b: %d\n", f, a, b);

    char szbuf1[64] = { 0 };
    char szbuf2[64] = { 0 };
    char* inputs[2];
    unsigned lengths[2];

    int len_a = WriteVarBool(szbuf1, 64, a);
    int len_b = WriteVarBool(szbuf2, 64, b);

    inputs[0] = szbuf1;
    inputs[1] = szbuf2;
    lengths[0] = len_a;
    lengths[1] = len_b;

    char* result = nullptr;
    int result_len = PlatON_RunFuncByLazyJIT(ir.c_str(), f, &result, inputs, lengths);
    if (result_len <= 0) {
        printf("the result len: %d, jit run failed !\n", result_len);
        return 1;
    }

    bool out;
    int ret = ReadVarBool(result, result_len, out);
    printf("call f: %s, a: %d, b: %d, result_len: %d, ret: %d, out: %d\n", f, a, b, result_len, ret, out);

    PlatON_RunFuncByLazyJIT(ir.c_str(), "free", (char **)result, nullptr, nullptr);
    return 0;
}

int test_function_string(const std::string& ir, string a, string b, const char* f) {

    printf("call f: %s, a: %s, b: %s\n", f, a.c_str(), b.c_str());

    char szbuf1[64] = { 0 };
    char szbuf2[64] = { 0 };
    char* inputs[2];
    unsigned lengths[2];

    int len_a = WriteVarString(szbuf1, 64, a);
    int len_b = WriteVarString(szbuf2, 64, b);

    inputs[0] = szbuf1;
    inputs[1] = szbuf2;
    lengths[0] = len_a;
    lengths[1] = len_b;

    char* result = nullptr;
    int result_len = PlatON_RunFuncByLazyJIT(ir.c_str(), f, &result, inputs, lengths);
    if (result_len <= 0) {
        printf("the result len: %d, jit run failed !\n", result_len);
        return 1;
    }

    string out;
    int ret = ReadVarString(result, result_len, out);
    printf("call f: %s, a: %s, b: %s, result_len: %d, ret: %d, out: %s\n", f, a.c_str(), b.c_str(), result_len, ret, out.c_str());

    PlatON_RunFuncByLazyJIT(ir.c_str(), "free", (char **)result, nullptr, nullptr);
    return 0;
}