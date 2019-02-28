
/*
digest:
 * IR NAME: MPCTestBasicType
 * IR HASH: 8c0bb892f768ec8514b80cc4fb53946b
 * <p>
 * IR FUNC HASH(MD5)                 IR FUNC NAME            IR FUNC PROT
 * 56b5f54f4f33826a0d4a6e9c4698c0e9  CompareAge              CompareAge(int,int)
 * 0588f14217b11e0f77e50d03a88ba866  YaoMillionairesProblem  YaoMillionairesProblem(int,int)
 * dd70473cf645ce1fe5e1d1430a8c28c9  TestInt32Add            TestInt32Add(int,int)
 * d2861ce4a7b7df18ec5dd46a04ecb990  TestInt32Sub            TestInt32Sub(int,int)
 * 0e8b39c906bb46b8d658a8ac565b2935  TestInt32Mul            TestInt32Mul(int,int)
 * bf6537f6b8f6f4f393b17fdbb3eb3d1e  TestInt32Div            TestInt32Div(int,int)
 * 1de0c714dfd5519da8ccef556ed06aec  TestInt32Mod            TestInt32Mod(int,int)
 * 01ec2a137d26d3a328818bb6dd247bc4  TestInt32And            TestInt32And(int,int)
 * c8e4cbd53934ed337a96aa2837696c12  TestInt32Orr            TestInt32Orr(int,int)
 * 537a2e587995585592b6b06f3f71a795  TestInt32Eor            TestInt32Eor(int,int)
 * 8c2932bdc8f36de040906e0be6a8d298  TestInt64Add            TestInt64Add(long,long)
 * e93423e29d52d131e7468661702fc1bd  TestInt64Sub            TestInt64Sub(long,long)
 * 9c30250fc9df9497151217010aba4528  TestInt64Mul            TestInt64Mul(long,long)
 * e2992b032c6eab5435604481d0e8ee7c  TestInt64Div            TestInt64Div(long,long)
 * bb946dffb2dedc0881ff776f70b1b054  TestInt64Mod            TestInt64Mod(long,long)
 * 0b0a645f1a3f21477acfc77d32a8044b  TestInt64And            TestInt64And(long,long)
 * 70ebc050ba8fad4ef67f426227987e9b  TestInt64Orr            TestInt64Orr(long,long)
 * 2767484080c4f9a63e9ee4a50b0b1ece  TestInt64Eor            TestInt64Eor(long,long)
 * d0dacfba19446238aecea00a00d47986  TestFloatAdd            TestFloatAdd(float,float)
 * 3814d6bf1734de01af0a9f64c51cb908  TestFloatSub            TestFloatSub(float,float)
 * 862e33d1d5fd866b8e74ed9ffcad2861  TestFloatMul            TestFloatMul(float,float)
 * cd5fff286c70274df5bd0581f97a8dff  TestFloatDiv            TestFloatDiv(float,float)
 * 8d09a4cae4ef5f3a065804d0fa333e48  TestDoubleAdd           TestDoubleAdd(double,double)
 * 4b7511697f876d15856f88dcbac56180  TestDoubleSub           TestDoubleSub(double,double)
 * 3353a41469ad2eea560822666e5cad50  TestDoubleMul           TestDoubleMul(double,double)
 * 09a66757fdb52f7a27af693e429f92e1  TestDoubleDiv           TestDoubleDiv(double,double)
 * fc2c6c8dc4c9b58b382340c8f03e18a3  TestBool                TestBool(boolean,boolean)
 * 5b56f40f8828701f97fa4511ddcd25fb  TestString              TestString(String,String)
 */

#include "Test.h"

template <typename T>
int test_function2(const string& irpath, const string& funcname) {

    return 0;
}
template <typename RT>
int get_result() {

    return 0;
}

int test_function_int32(const std::string& ir) {
    srand((unsigned int)time(NULL));
    int32_t a = rand() % 12347 + 100;
    int32_t b = rand() % 113 + 1000;

    test_function_int32(ir, a, b, "CompareAge");
    test_function_int32(ir, a, b, "YaoMillionairesProblem");

    test_function_int32(ir, a, b, "TestInt32Add");
    test_function_int32(ir, a, b, "TestInt32Sub");
    test_function_int32(ir, a, b, "TestInt32Mul");
    test_function_int32(ir, a, b, "TestInt32Div");
    test_function_int32(ir, a, b, "TestInt32Mod");
    test_function_int32(ir, a, b, "TestInt32And");
    test_function_int32(ir, a, b, "TestInt32Orr");
    test_function_int32(ir, a, b, "TestInt32Eor");

    return 0;
}


int test_function_int64(const std::string& ir) {
    srand((unsigned int)time(NULL));
    int64_t a = rand() % 12347 + 100;
    int64_t b = rand() % 113 + 1000;

    test_function_int64(ir, a, b, "TestInt64Add");
    test_function_int64(ir, a, b, "TestInt64Sub");
    test_function_int64(ir, a, b, "TestInt64Mul");
    test_function_int64(ir, a, b, "TestInt64Div");
    test_function_int64(ir, a, b, "TestInt64Mod");
    test_function_int64(ir, a, b, "TestInt64And");
    test_function_int64(ir, a, b, "TestInt64Orr");
    test_function_int64(ir, a, b, "TestInt64Eor");

    return 0;
}



int test_function_float(const std::string& ir) {
    srand((unsigned int)time(NULL));
    float a = 1.0*(rand() % 12347 + 100);
    float b = 1.0*(rand() % 113 + 1000);

    test_function_float(ir, a, b, "TestFloatAdd");
    test_function_float(ir, a, b, "TestFloatSub");
    test_function_float(ir, a, b, "TestFloatMul");
    test_function_float(ir, a, b, "TestFloatDiv");

    return 0;
}

int test_function_double(const std::string& ir) {
    srand((unsigned int)time(NULL));
    double a = 1.0*(rand() % 12347 + 100);
    double b = 1.0*(rand() % 113 + 1000);

    test_function_double(ir, a, b, "TestDoubleAdd");
    test_function_double(ir, a, b, "TestDoubleSub");
    test_function_double(ir, a, b, "TestDoubleMul");
    test_function_double(ir, a, b, "TestDoubleDiv");
    return 0;
}

int test_function_bool(const std::string& ir) {
    srand((unsigned int)time(NULL));
    bool a = (rand() % 12347 + 100) % 2 == 0;
    bool b = (rand() % 113 + 1000) % 2 == 0;

    test_function_bool(ir, a, b, "TestBool");
    return 0;
}

int test_function_string(const std::string& ir) {
    srand((unsigned int)time(NULL));
    string a = std::to_string(rand() % 12347 + 100);
    string b = std::to_string(rand() % 113 + 1000);

    test_function_string(ir, a, b, "TestString");

    return 0;
}

int test_function(const std::string& ir) {
    test_function_int32(ir);
    test_function_int64(ir);
    test_function_float(ir);
    test_function_double(ir);

    test_function_bool(ir);
    test_function_string(ir);
    return 0;
}
