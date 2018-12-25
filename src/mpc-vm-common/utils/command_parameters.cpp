
#include "command_parameters.h"
#include "sys_utils_tool.h"

#include "gflags/gflags.h"

using std::endl;

static bool IsVaildPort(const char* flagname, int32_t value) {
	bool b = (value > 0 && value < 32768);
	if (b) return true;
	std::cerr << "error: flag[" << flagname << "] --- port[" << value << "] is not valid." << endl;
	return false;
}
static bool IsVaildDir(const char* flagname, const string& value) {
	bool b = (value == "." || platon::mpc::SysUtilsTool::checkDirExists(value));
	if (b) return true;
	std::cerr << "error: flag[" << flagname << "] --- direcotry[" << value << "] is not exist." << endl;
	return false;
}
static bool IsVaildFile(const char* flagname, const string& value) {
	bool b = (value == "." || platon::mpc::SysUtilsTool::checkFileExists(value));
	if (b) return true;
	std::cerr << "error: flag[" << flagname << "] --- file[" << value << "] is not exist." << endl;
	return false;
}

DEFINE_string(circuitFile, "", "circuit file");
DEFINE_string(circuitDir, "", "circuit files directory");
DEFINE_string(configFile, "", "config file");
DEFINE_string(configDir, "", "config files directory");
DEFINE_string(iceConfigFile, "", "ice config file");
DEFINE_string(iceConfigDir, "", "ice config files directory");
//DEFINE_validator(circuitFile, &IsVaildFile);
//DEFINE_validator(circuitDir, &IsVaildDir);
//DEFINE_validator(configFile, &IsVaildFile);
//DEFINE_validator(configDir, &IsVaildDir);
//DEFINE_validator(iceConfigFile, &IsVaildFile);
//DEFINE_validator(iceConfigDir, &IsVaildDir);


DEFINE_string(role, "", "1:alice;2:bob");
DEFINE_string(alg, "add", "1:add;2:sub;3:mul;4:div;5:cmp;6:bitAdd;7:setdiff;8:setdifff2048;9:chiFFP");
DEFINE_uint32(testCount, 1, "test count");
DEFINE_string(mpcMode, "simihonest", "simihonest or milicous");
DEFINE_int32(gcVersion, 1, "for circuit file, 0 is non-zipped and 1 is zip");
DEFINE_int32(threadNum, 1, "thread number, default cpu-cores*2");
DEFINE_bool(parallel, false, "is parallel or not");

DEFINE_string(inputType, "uint32", "type:bool|int8|uint8|int16|uint16|int32|uint32|int64|uint64 or type-array or file");
DEFINE_string(inputData, "1", "according to inputType");
DEFINE_bool(inputRandom, false, "not supported now. if true, generate input-data random, ignore inputData and if type is an array, should \"-inputType=type-array-len\"");

DEFINE_string(node, "127.0.0.1", "node address");
DEFINE_int32(port, 12002, "port");
DEFINE_bool(testAll, false, "for developer to test all gc files");

DEFINE_string(user01, "", "user01");
DEFINE_string(user02, "", "user02");
DEFINE_string(username, "", "username");
DEFINE_string(password, "", "password");
DEFINE_string(communicateMode, "1", "0:callback;1:service");
DEFINE_bool(nodeDirect, true, "true,direct;false,proxy");
DEFINE_string(proxyEndpoint, "", "proxyEndpoint");
DEFINE_string(room, "", "room id");

DEFINE_string(platonUrl, "http://10.10.8.155:9988", "platon url for send transaction");

bool checkValid(Paras& paras);
bool parseParas(int argc, char* argv[], Paras& paras, bool check/* = true*/) {
	gflags::SetVersionString("1.0.0");
	std::stringstream sss;
	sss << "Usage:" << std::endl
		<< "  " << "--help   to show default parameters." << std::endl
		<< "  eg." << std::endl
		<< "     " << "-role=alice -alg=add -testCount=1 -proxyRegion=aliyun -mode=callback" << std::endl
		<< std::endl;
	gflags::SetUsageMessage(sss.str());
	//gflags::ShowUsageWithFlagsRestrict(argv[0], "gflags_test");

	gflags::ParseCommandLineFlags(&argc, &argv, true);
#define SET_PARAS(a) paras.a = FLAGS_##a
	SET_PARAS(circuitFile);
	SET_PARAS(circuitDir);
	SET_PARAS(configFile);
	SET_PARAS(configDir);
	SET_PARAS(iceConfigFile);
	SET_PARAS(iceConfigDir);
	SET_PARAS(alg);
	SET_PARAS(role);
	SET_PARAS(testCount);
	SET_PARAS(mpcMode);
	SET_PARAS(inputType);
	SET_PARAS(inputData);
	SET_PARAS(inputRandom);
	SET_PARAS(node);
	SET_PARAS(port);
	SET_PARAS(gcVersion);
	SET_PARAS(threadNum);
	SET_PARAS(parallel);
	SET_PARAS(username);
	SET_PARAS(password);
	SET_PARAS(communicateMode);
	SET_PARAS(proxyEndpoint);
	SET_PARAS(nodeDirect);
	SET_PARAS(room);
	SET_PARAS(user01);
	SET_PARAS(user02);
	SET_PARAS(platonUrl);
#undef SET_PARAS
	gflags::ShutDownCommandLineFlags();

	if(!check) return true;
	return checkValid(paras);
}

template<typename T>
static void invalid_msg(const std::string& flag, const T& value) {
	std::cout << "invalid " << flag << ":" << value << std::endl;
}
#define return_invalid(name) invalid_msg(#name, paras.name); return false

// check valid, and map name to value instead of id
#define setvalue(name, id, value) else if (paras.name == #id || paras.name == #value) { paras.name = #value; }

bool checkValid(Paras& paras) {
	// role
	if (false) {}
	setvalue(role, 1, alice)
		setvalue(role, 2, bob)
	else { return_invalid(role); }

	// alg
	if (false) {}
	setvalue(alg, 1, add)
		setvalue(alg, 2, sub)
		setvalue(alg, 3, mul)
		setvalue(alg, 4, div)
		setvalue(alg, 5, cmp)
		setvalue(alg, 6, bitAdd)
		setvalue(alg, 7, setdiff)
		setvalue(alg, 8, setdiff2048)
		setvalue(alg, 9, chiFFP)
	else { return_invalid(alg); }

	// gcVersion
	if (paras.gcVersion != 0 && paras.gcVersion != 1) { return_invalid(gcVersion); }

	// communicateMode
	if (false) {}
	setvalue(communicateMode, 0, callback)
		setvalue(communicateMode, 1, service)
	else { return_invalid(communicateMode); }


	return true;
}



// inputType "bool|int8|uint8|int16|uint16|int32|uint32|int64|uint64" type
// inputType "bool-array|int8-array|...|int64-array|uint64-array" type-array

// public parameters
void help_test(stringstream& sss) {
	sss << std::endl << "Usage:"
		<< std::endl << "  -role              " << "1:alice;2:bob"
		<< std::endl << "  -alg               " << "1:add;2:sub;3:mul;4:div;5:cmp;6:bitAdd;7:setdiff;8:setdifff2048;9:chiFFP"
		<< std::endl << "  -mpcMode           " << "simihonest or milicous(not use now, for only supported simihonest)"
		<< std::endl << "  -testCount         " << "test count"
		<< std::endl << "  -inputType         " << "can be basic-type or type-array as follow, or file"
		<< std::endl << "                     " << "    basic-type: bool|int8|uint8|int16|uint16|int32|uint32|int64|uint64"
		<< std::endl << "                     " << "    type-array: bool-array|int8-array|...|int64-array|uint64-array"
		<< std::endl << "  -inputData         " << "according to inputType, if type is \"file\", must be filepath."
		<< std::endl << "  -inputRandom       " << "not support now"
		<< std::endl << "  -node              " << "node address, default 127.0.01"
		<< std::endl << "  -port              " << "node port, default 12002"
		<< std::endl << "  -gcVersion         " << "the version of circuit file, 0 is non-zipped and 1 is zipped"
		<< std::endl << "  -threadNum         " << "thread number, default 1"
		<< std::endl << "  -parallel          " << "is parallel or not, default false"
		<< std::endl << "  -circuitFile       " << "circuit file"
		<< std::endl;
}
void help_mpc_test() {
	std::stringstream sss;
	help_test(sss);
	sss << std::endl;
	std::cout << sss.str();
}
void help_mpc_sdk_test() {
	std::stringstream sss;
	help_test(sss);
	sss << std::endl << "  -iceConfigFile     " << "ice config file"
		<< std::endl << "  -user01            " << "user 01"
		<< std::endl << "  -user02            " << "user 02"
		<< std::endl << "  -username          " << "username"
		<< std::endl << "  -password          " << "password"
		<< std::endl << "  -communicateMode   " << "0:callback;1:service"
		<< std::endl << "  -nodeDirect        " << "true,direct;false,proxy"
		<< std::endl << "  -proxyEndpoint     " << "proxyEndpoint, eg:"
		<< std::endl << "                     " << "    ProxyGlacier2/router:tcp -h 192.168.112.158  -p 4502 -t 11000"
		<< std::endl << "                     " << "    DirectNodeServer:tcp -h 192.168.112.158 -p 13001"
		<< std::endl << "  -room              " << "room id"
		<< std::endl;
	std::cout << sss.str();
}
