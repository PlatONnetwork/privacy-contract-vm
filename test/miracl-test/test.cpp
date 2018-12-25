
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <chrono>
#include <iostream>
using namespace std::chrono;


#ifdef __cplusplus
extern "C"{
#endif

#include "miracl.h"

#ifdef __cplusplus
}
#endif


void test1(int argc, char* argv[]);
int main(int argc, char* argv[]){
    
    test1(argc,argv);
    return 0;
}


static int MBIT = 8192; // max bits
static int bits = 128;
static int lens = 64;
static int loop = 1024;
void test1(int argc, char* argv[]){
    // std::cout << "Usage: ./test [[[bits] lens] loop]" << std::endl;
    if (argc > 1){
        bits = atoi(argv[1]);
        if (bits > MBIT) bits = MBIT;
    }
    if (argc > 2){
        lens = atoi(argv[2]);
    }
    if (argc > 3){
        loop = atoi(argv[3]);
    }
    std::cout << "./test bits:" << bits 
    <<" lens:" << lens << " loop:" << loop;
    
    epoint *C = NULL;

    miracl *mip = mirsys(MBIT,10); 
    
    big p = mirvar(0);
    bigbits(bits, p);
    epoint* g = epoint_init();

    // big x = mirvar(0), y = mirvar(0);
    // bigbits(30, x);
    // bigbits(30, y);
    // epoint_set(x, y, 0, g);
    
    big c;
    c = mirvar(0);
    bigrand(p, c);
    C = epoint_init();
    ecurve_mult(c, g, C); 
    mirkill(c);
    
    
    int ret = 0;
    big r = mirvar(0);
    epoint *h1;
    h1 = epoint_init();
    epoint **hList = new epoint*[lens];
    epoint **grList = new epoint*[lens];
    
    auto start = system_clock::now();
    
    for (int i = 0; i < lens; ++i)
    {
        hList[i] = epoint_init();
        grList[i] = epoint_init();
    }

    for (int k = 0; k < loop; k++){

        for(int i = 0; i < lens; i++) 
        {
            bigrand(p, r);
            ecurve_mult(r, g, grList[i]);  // g^r  
            epoint_copy(C, h1);   //h1 = C
            ecurve_sub(hList[i], h1);    // h1 = C-h
            ecurve_mult(r, hList[i], hList[i]);   // h' = h^r
            ecurve_mult(r, h1, h1);   // h1 = (C-h)^r
        }
    }
    
    for(int i = 0; i < lens; ++i) 
    {
        epoint_free(grList[i]);
        epoint_free(hList[i]);
    }
    
    auto end = system_clock::now();
    auto duration = duration_cast<microseconds>(end - start);
    std::cout << " elapse: " << duration.count() << "(us)" << std::endl;
    
    delete []grList;
    delete []hList;
    mirkill(r);
    epoint_free(h1);
    
}
