


#include "Test.h"


int test_function(const std::string& ir) {
    srand((unsigned int)time(NULL));
    int32_t a = rand() % 12347 + 100;
    int32_t b = rand() % 113 + 1000;

    test_function_int32(ir, a, b, "TestPerfAll");
    
    return 0;
}

