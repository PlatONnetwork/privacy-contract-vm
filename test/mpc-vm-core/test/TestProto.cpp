
#include "code/TestProto.pb.h"
#include "Test.h"

using namespace testproto;

void setFoo(Foo& foo) {
    foo.set_i32(1);
    foo.set_f(2.0f);
    foo.set_s("abcd");
}

void setBar(Bar& bar) {
    Foo* foo = new Foo();
    setFoo(*foo);
    bar.set_allocated_foo(foo);
    bar.set_i32(1);
    bar.set_f(2.0f);
}

void setPoint(Point& point) {
    point.set_x(1);
    point.set_y(2);
}

void setPoints(Points& points) {
    for (int i = 0; i < 3; i++) {
        Point* point = points.add_points();
        point->set_x(1);
        point->set_y(2);
    }
}

void printFoo(const Foo& foo) {
    cout << "foo i: " << foo.i32() << endl;
    cout << "foo f: " << foo.f() << endl;
    cout << "foo s: " << foo.s() << endl;
}

void printBar(const Bar& bar) {
    cout << "bar i: " << bar.i32() << endl;
    cout << "bar f: " << bar.f() << endl;
    printFoo(bar.foo());
}

void printPoint(const Point& point) {
    printf("point (%d, %d)\n", point.x(), point.y());
}

void printPoints(const Points& points) {
    for (int i = 0; i < points.points_size(); i++) {
        printPoint(points.points(i));
    }
}

int test_function_EuclideanDistance(const std::string& ir) {

    printf("call f: EuclideanDistance\n");

    char* inputs[2];
    unsigned lengths[2];

    Point p1, p2;
    setPoint(p1);
    setPoint(p2);

    int len_a = p1.ByteSize();
    int len_b = p2.ByteSize();
    char* szbuf1 = new char[len_a];
    char* szbuf2 = new char[len_b];
    memset(szbuf1, 0, len_a);
    memset(szbuf2, 0, len_b);

    p1.SerializeToArray(szbuf1, len_a);
    p2.SerializeToArray(szbuf2, len_b);
    printPoint(p1);
    printPoint(p2);

    inputs[0] = szbuf1;
    inputs[1] = szbuf2;
    lengths[0] = len_a;
    lengths[1] = len_b;

    char* result = nullptr;
    int result_len = PlatON_RunFuncByLazyJIT(ir.c_str(), "EuclideanDistance", &result, inputs, lengths);

    delete[] szbuf1;
    delete[] szbuf2;

    if (result_len <= 0) {
        printf("the result len: %d, jit run failed !\n", result_len);
        return 1;
    }

    int32_t out;
    int ret = ReadVarInt32(result, result_len, out);
    printf("call f: EuclideanDistance, result_len: %d, ret: %d, out: %d\n", result_len, ret, out);

    PlatON_RunFuncByLazyJIT(ir.c_str(), "free", (char **)result, nullptr, nullptr);
    return 0;
}


int test_function_TestBarAdd02(const std::string& ir) {

    printf("call f: TestBarAdd02\n");

    char* inputs[2];
    unsigned lengths[2];

    Foo foo1, foo2;
    setFoo(foo1);
    setFoo(foo2);

    int len_a = foo1.ByteSize();
    int len_b = foo2.ByteSize();
    char* szbuf1 = new char[len_a];
    char* szbuf2 = new char[len_b];
    memset(szbuf1, 0, len_a);
    memset(szbuf2, 0, len_b);

    foo1.SerializeToArray(szbuf1, len_a);
    foo2.SerializeToArray(szbuf2, len_b);
    printFoo(foo1);
    printFoo(foo2);

    inputs[0] = szbuf1;
    inputs[1] = szbuf2;
    lengths[0] = len_a;
    lengths[1] = len_b;

    char* result = nullptr;
    int result_len = PlatON_RunFuncByLazyJIT(ir.c_str(), "TestBarAdd02", &result, inputs, lengths);

    delete[] szbuf1;
    delete[] szbuf2;

    if (result_len <= 0) {
        printf("the result len: %d, jit run failed !\n", result_len);
        return 1;
    }

    Bar bar;
    bool ret = bar.ParseFromArray(result, result_len);
    printf("call f: TestBarAdd02, result_len: %d, ret: %d, out: %d\n", result_len, ret, 0);
    printBar(bar);

    PlatON_RunFuncByLazyJIT(ir.c_str(), "free", (char **)result, nullptr, nullptr);
    return 0;
}
int test_function_ReturnPoints(const std::string& ir) {

    printf("call f: ReturnPoints\n");

    char szbuf1[64] = { 0 };
    char szbuf2[64] = { 0 };
    char* inputs[2];
    unsigned lengths[2];

    int len_a = WriteVarInt32(szbuf1, 64, 111);
    int len_b = WriteVarInt32(szbuf2, 64, 222);

    inputs[0] = szbuf1;
    inputs[1] = szbuf2;
    lengths[0] = len_a;
    lengths[1] = len_b;

    char* result = nullptr;
    int result_len = PlatON_RunFuncByLazyJIT(ir.c_str(), "ReturnPoints", &result, inputs, lengths);
    if (result_len <= 0) {
        printf("the result len: %d, jit run failed !\n", result_len);
        return 1;
    }

    Points points;
    bool ret = points.ParseFromArray(result, result_len);
    printf("call f: ReturnPoints, result_len: %d, ret: %d, out: %d\n", result_len, ret, points.points_size());
    printPoints(points);

    PlatON_RunFuncByLazyJIT(ir.c_str(), "free", (char **)result, nullptr, nullptr);
    return 0;
}

int test_function(const std::string& ir) {
    test_function_EuclideanDistance(ir);
    test_function_TestBarAdd02(ir);
    test_function_ReturnPoints(ir);

    return 0;
}
