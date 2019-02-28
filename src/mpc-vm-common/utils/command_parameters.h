#pragma once

#include <iostream>
#include <string>
#include <sstream>
#include <inttypes.h>


using std::string;
using std::stringstream;

struct Paras {
	string role = "";
	string alg = "add";
	uint32_t testCount = 1;
	string mpcMode = "simihonest";
	int    gcVersion = 1;
	int32_t threadNum = 0;
	bool   parallel = false;

	string circuitFile = "";
	string circuitDir = "";
	string configFile = "";
	string configDir = "";
	string iceConfigFile = "";
	string iceConfigDir = "";

	string inputType = "uint32";
	string inputData = "1";
	bool inputRandom = false;

	string node = "127.0.0.1";
	int    port = 12002;
	bool   testAll = false;

	string user01 = "test0003";
	string user02 = "test0004";
	string username = "";
	string password = "";
	string communicateMode = "service";
	bool   nodeDirect = true;
	string proxyEndpoint = "";
	string room = "";

	string platonUrl = "http://10.10.8.155:9988";


	void toString() {
		std::stringstream sss;
		sss << "Parameters:"
			<< "\n   " << "role            : " << role
			<< "\n   " << "alg             : " << alg
			<< "\n   " << "testCount       : " << testCount
			<< "\n   " << "mpcMode         : " << mpcMode
			<< "\n   " << "gcVersion       : " << gcVersion
			<< "\n   " << "threadNum       : " << threadNum
			<< "\n   " << "parallel        : " << std::string(parallel ? "Yes" : "No")
			<< "\n"
			<< "\n   " << "circuitFile     : " << circuitFile
			<< "\n   " << "circuitDir      : " << circuitDir
			<< "\n   " << "configFile      : " << configFile
			<< "\n   " << "configDir       : " << configDir
			<< "\n   " << "iceConfigFile   : " << iceConfigFile
			<< "\n   " << "iceConfigDir    : " << iceConfigDir
			<< "\n"
			<< "\n   " << "inputType       : " << inputType
			<< "\n   " << "inputData       : " << inputData
			<< "\n   " << "inputRandom     : " << std::string(inputRandom ? "Yes" : "No")
			<< "\n"
			<< "\n   " << "node            : " << node
			<< "\n   " << "port            : " << port
			<< "\n   " << "testAll         : " << std::string(testAll ? "Yes" : "No")
			<< "\n"
			<< "\n   " << "user01          : " << user01
			<< "\n   " << "user02          : " << user02
			<< "\n   " << "username        : " << username
			<< "\n   " << "password        : " << password
			<< "\n   " << "communicateMode : " << communicateMode
			<< "\n   " << "nodeDirect      : " << std::string(nodeDirect ? "Yes" : "No")
			<< "\n   " << "proxyEndpoint   : " << proxyEndpoint
			<< "\n   " << "room            : " << room
			<< "\n";
		std::cout << sss.str() << std::endl;
	}
};

bool parseParas(int argc, char* argv[], Paras& paras, bool check = true);

void help_test(stringstream& sss);
void help_mpc_test();
void help_mpc_sdk_test();
void help_mpc_vm_sdk_test();

