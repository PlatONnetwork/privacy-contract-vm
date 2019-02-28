// client, simulator
#include <cstdio>
#include <cstdlib>
#include <cassert>
#include <cstring>

#include <map>
#include <vector>
#include <random>
#include <iostream>
#include <thread>
#include <string>
using namespace std;

#include "platonvmsdk.h"
#include "command_parameters.h"

/*
===================================================
        taskId: 54396487fef9eb7e3a098755ed88e187866e6d2032aba55592a1ba2e0ab7b4e8
        pubkey: a363d1243646b6eabf1d4851f646b523f5707d053caab95022f1682605aca0537ee0c5c14b4dfa76dcbce264b7e68d59de79a42b7cda059e9d358336a9ab8d80
        irhash: 7ad875d3eb570f244bcaa71fd9bce37c
        method: TestAdd
        extras:
        txaddr: 0x60Ceca9c1290EE56b98d4E160EF0453F7C40d219
        iraddr: 0x43355C787c50b647C425f594b441D4BD751951C1
       invitor: yes
          role: ALICE
        irpath: data/ir//7ad875d3eb570f244bcaa71fd9bce37c.data
   method_hash: d26161114e7457c9be1414e51637c641
     transhash:
        status: POP_READY
        errmsg: OK
===================================================

===================================================
        taskId: 54396487fef9eb7e3a098755ed88e187866e6d2032aba55592a1ba2e0ab7b4e8
        pubkey: a363d1243646b6eabf1d4851f646b523f5707d053caab95022f1682605aca0537ee0c5c14b4dfa76dcbce264b7e68d59de79a42b7cda059e9d358336a9ab8d80
        irhash: 7ad875d3eb570f244bcaa71fd9bce37c
        method: TestAdd
        extras:
        txaddr: 0x60Ceca9c1290EE56b98d4E160EF0453F7C40d219
        iraddr: 0x43355C787c50b647C425f594b441D4BD751951C1
       invitor: no
          role: BOB
        irpath: data/ir//7ad875d3eb570f244bcaa71fd9bce37c.data
   method_hash: d26161114e7457c9be1414e51637c641
     transhash:
        status: POP_READY
        errmsg: OK
===================================================

*/


void calculation(Paras& paras, int i) {
    //independent_bits_engine<default_random_engine, 64, unsigned long long int> random_engine;

    char buf[16] = { 0 };
    sprintf(buf, "%08x", i & 0xFFFFFFFF);
    string staskid = "54396487fef9eb7e3a098755ed88e187866e6d2032aba55592a1ba2e" + string(buf);
    const char* taskid = staskid.c_str();
    const char* pubkey = "a363d1243646b6eabf1d4851f646b523f5707d053caab95022f1682605aca0537ee0c5c14b4dfa76dcbce264b7e68d59de79a42b7cda059e9d358336a9ab8d80";
    const char* txaddr = "0x60Ceca9c1290EE56b98d4E160EF0453F7C40d219";
    const char* iraddr = "0x43355C787c50b647C425f594b441D4BD751951C1";
    const char* method = paras.alg.c_str();
    const char* extras = "extra data";

    int ret = notify_security_calculation(taskid, pubkey, txaddr, iraddr, method, extras);
    std::cout << "notify_security_calculation, ret:" << ret << std::endl;
}

int init(const Paras& paras) {
    string icecfg = paras.iceConfigFile;
    string url = paras.platonUrl;
    //assert((icecfg != "") && "iceConfigFile is empty");
    //assert((url != "") && "platonUrl is empty");
    return notify_security_init(icecfg.c_str(), url.c_str());
}

#include <random>
using std::default_random_engine;


/*
iceconfig
irpath

Usage:
mpc_vm_platonsdk-platonvmsdk_main.exe -iceConfigFile=cfg.server1.config

svr.config.json    on main.exe directory.
ir path            config on "svr.config.json"

mpc_vm_platonsdk-platonvmsdk_main.exe -iceConfigFile=cfg.server1.config -alg=TestAdd -testCount=1

*/
int main(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "help") {
        help_mpc_vm_sdk_test();
        return 0;
    }

    Paras paras;
    if (!parseParas(argc, argv, paras, false)) {
        std::cout << "parse parameters failed!" << std::endl;
        return -1;
    }
    paras.toString();

    if (0 != init(paras)) {
        std::cerr << "init failed" << std::endl;
        return -1;
    }

    default_random_engine e;
    if (paras.testCount <= 0)paras.testCount = 1;
    vector<int> orderArr(paras.testCount);
    vector<int> randArr(paras.testCount);
    for (int i = 0; i < paras.testCount; i++) {
        orderArr[i] = i;
    }
    for (int i = 0; i < paras.testCount; i++) {
        int pos = e() % (paras.testCount - i);
        randArr[i] = orderArr[pos];
        orderArr[pos] = orderArr[paras.testCount - i - 1];
        //cout << randArr[i] << " ";
    }

    std::cout << "press any key to continue, when start data-provider java client" << std::endl;
    getchar();


    for (int i = 0; i < paras.testCount; i++) {
        std::this_thread::sleep_for(std::chrono::microseconds(100));
        calculation(paras, randArr[i]);
    }


    std::cout << "press any key (5) to exit" << std::endl;
    getchar();
    std::cout << "press any key (4) to exit" << std::endl;
    getchar();
    std::cout << "press any key (3) to exit" << std::endl;
    getchar();
    std::cout << "press any key (2) to exit" << std::endl;
    getchar();
    std::cout << "press any key (1) to exit" << std::endl;
    getchar();

    return 0;
}
