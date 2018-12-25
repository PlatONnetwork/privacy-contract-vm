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
#include "CommandParameters.h"

void calculation(Paras& paras) {
	independent_bits_engine<default_random_engine, 64, unsigned long long int> random_engine;

	const char* taskid = to_string(random_engine()).c_str();
	const char* pubkey = "MIGHAoGBAMAfEWZZN+sjNErwbnfHtzk2+TVHuU8o+NRHZZG6p+uO9DSGwTfs/WB5pHmPlBww1Jlno0IG92KP6IphAfz7IEhWxgFYs7gNJd8oTJapuieBve7QpH0cRwaram3faoemA8q8Ioa91LO+/0b5TDepiawU7zUTemooKh88Mu97cqfFAgED";
	const char* txaddr = "0x123456789dc4567909aacdf567ed9dc4567c";
	const char* iraddr = "0xaac9dc4567dfe12309dc459d567c45676789";
	const char* method = "ListMsg";
	const char* extras = "extra data";

	int ret = notify_security_calculation(taskid, pubkey, txaddr, iraddr, method, extras);
	std::cout << "notify_security_calculation, ret:" << ret << std::endl;
}

int init(const Paras& paras) {
	string icecfg = paras.iceConfigFile;
	string url = paras.platonUrl;
	assert((icecfg != "") && "icecfg is empty");
	assert((url != "") && "url is empty");
	return notify_security_init(icecfg.c_str(), url.c_str());
}

int main(int argc, char* argv[]) {
	if (argc > 1 && std::string(argv[1]) == "help") {
		help_mpc_sdk_test();
		return 0;
	}

	Paras paras;
	if (!parseParas(argc, argv, paras)) {
		std::cout << "parse parameters failed!" << std::endl;
		return -1;
	}
	paras.toString();

	if (0 != init(paras)) {
		std::cerr << "init failed" << std::endl;
		return -1;
	}

	while (true) {
		std::this_thread::sleep_for(std::chrono::seconds(rand() % 3 + 1));
		calculation(paras);
	}

	return 0;
}
