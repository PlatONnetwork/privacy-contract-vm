#include <string>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <vector>
#include <thread>
#include <ctime>
#include <cassert>

#include "parse_input_data.h"
#include "CommandParameters.h"
#include "SimpleTimer.h"
#include "SysUtilsTool.h"
#include "Semihonest2PC.h"
#include "NetIOChannel.h"
#include "TypeUtils.h"
#include "CircuitFile.h"
#include "MPCInteger.h"
#include "CircuitCache.h"
#include "cJSON.h"

using namespace std;
using namespace juzix::mpc;

static void testInputParser() {
	std::vector<unsigned char> input;
	std::string input_type;
	std::string input_data;
	bool ret = false;

	input_type = "bool";
	input_data = "";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(!ret && input.size() == 0);

	input_type = "bool-array";
	input_data = "";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(!ret && input.size() == 0);

	input_type = "bool";
	input_data = "false";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 1 && input[0] == 0);

	input_type = "bool";
	input_data = "true";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 1 && input[0] == 1);

	input_type = "bool-array";
	input_data = "true,true,false";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 3 && input[0] == 1 && input[1] == 1 && input[2] == 0);


	input_type = "int8";
	input_data = "12";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 1 && input[0] == 12);

	input_type = "int16";
	input_data = "12";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 2 && input[0] == 12 && input[1] == 0);

	input_type = "int32";
	input_data = "12";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 4 && input[0] == 12 && input[1] == 0 && input[2] == 0 && input[3] == 0);

	input_type = "int64";
	input_data = "12";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 8 && input[0] == 12 && input[1] == 0 && input[2] == 0 && input[7] == 0);


	int a = 0;

}

static void testParseInputFile() {
	std::string input_type = "file";
	std::string input_data = "E:/work/local/dev-mpc-crossplat-new/in/xxx.h";
	std::vector<unsigned char> input;
	bool ret = false;

	if (false) {
		ret = parseInput(input, input_type, input_data);
		return;
	}

	input_data = "E:/work/local/dev-mpc-crossplat-new/in/eg1.h";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 4 && input[0] == 12 && input[1] == 0 && input[2] == 0 && input[3] == 0);

	input_data = "E:/work/local/dev-mpc-crossplat-new/in/eg2.h";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 12 && input[0] == 12 && input[4] == 34 && input[8] == 56);

	input_data = "E:/work/local/dev-mpc-crossplat-new/in/eg3.h";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 48 && input[0] == 11 && input[16] == 21 && input[32] == 31);

	input_data = "E:/work/local/dev-mpc-crossplat-new/in/eg4.h";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 80 && input[0] == 101 && input[40] == 121 && input[79] == 0);

	input_data = "E:/work/local/dev-mpc-crossplat-new/in/eg5.h";
	input.clear();
	ret = parseInput(input, input_type, input_data);
	assert(ret && input.size() == 800 && input[0] == 101 && input[40] == 102 && input[80] == 103);


	int a = 0;

}

int main(int argc, char* argv[]) {

	testInputParser();

	testParseInputFile();

	return 0;
}

