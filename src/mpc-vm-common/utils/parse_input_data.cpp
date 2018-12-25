#include "parse_input_data.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>
#include <regex>
using namespace std;

//
//#include <boost/regex.hpp>
//using namespace boost;

/*
not supported regex well before GCC 4.9 on linux
so, you should install/update gcc >= 4.9 for supported
*/
#ifndef _WIN32
#  define GCC_VERSION (__GNUC__ * 10000 + __GNUC_MINOR__ * 100  + __GNUC_PATCHLEVEL__) 
#  if (GCC_VERSION > 40900)
#    define SUPPORT_REGEX 1
#  else
#    define SUPPORT_REGEX 0
#  endif
#else
#  define SUPPORT_REGEX 1
#endif


#ifdef _WIN32
#include <Windows.h>
#else
#include <sys/sysinfo.h>
#endif

int32_t get_cpu_cores()
{
#if defined(_WIN32)
	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;
#else
	return get_nprocs();
#endif
}

#include "sys_utils_tool.h"
#include "type_utils.h"
using namespace platon::mpc;


bool stringToUL(const std::string &str, uint64_t  &val)
{
	bool isOK = false;
	const char *nptr = str.c_str();
	char *endptr = NULL;
	errno = 0;
	val = strtoull(nptr, &endptr, 10);
	//error ocur
	if (false)
		//if ((errno == ERANGE && (val == ULONG_MAX))
		//	|| (errno != 0 && val == 0))
	{

	}
	//no digit find
	else if (endptr == nptr)
	{

	}
	else if (*endptr != '\0')
	{
		// printf("Further characters after number: %s\n", endptr);
	}
	else
	{
		isOK = true;
	}

	return isOK;
}

std::vector<std::string> split(const std::string &s, char delim) {
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) {
		elems.push_back(std::move(item)); // C++11
	}
	return elems;
}

enum INPUT_TYPE {
	BASIC = 1,
	ARRAY1,
	ARRAY2,
	UNKNOW
};

#if SUPPORT_REGEX
std::string basic_type = "bool|int8|uint8|int16|uint16|int32|uint32|int64|uint64";
std::string variable = "[a-zA-Z][a-zA-Z0-9_]*";

std::string pattern_basic = "^#input (" + variable + ") (" + basic_type + ")$";
regex regex_basic(pattern_basic); // "#input i int32"

std::string pattern_array1 = "^#input (" + variable + ") (" + basic_type + ")\\[(\\d+)\\]$";
regex regex_array1(pattern_array1); // "#input i34 int32[3]"

std::string pattern_array2 = "^#input (" + variable + ") (" + basic_type + ")\\[(\\d+)\\]\\[(\\d+)\\]$";
regex regex_array2(pattern_array2); // "#input i34 int32[3][4]"

std::string pattern_struct = "typedef struct (" + variable + ")";
regex regex_struct(pattern_struct); // "typedef struct Point"
#endif

bool parseInputFromConsole(std::vector<unsigned char>& input, const std::string& input_type, const std::string& input_data);
bool parseInputFromFile(const std::string& inputFile, std::vector<unsigned char>& input);

int parseInputFromConsoleNoRegex(std::vector<unsigned char>& input, const std::string& input_type, const std::string& input_data) {
	string s_type = input_type;
	vector<std::string> vecInputDatas;

	size_t pos = s_type.find("-array");
	if (pos != string::npos) { // type-array
		s_type = s_type.substr(0, pos);
		vecInputDatas = split(input_data, ',');
	}
	else {// basic type
		vecInputDatas.push_back(input_data);
	}
	if (vecInputDatas.size() == 0) {
		std::cerr << "no data" << std::endl;
		return false;
	}

	for (auto data : vecInputDatas) {
		// data valid check
		if (s_type == "bool") {
			// inputType "bool"
			bool b = (data == "true" || data == "1");
			bool_to_uchar(&b, 1, input);
		}
		else {
			// inputType "int8|uint8|int16|uint16|int32|uint32|int64|uint64"
			uint64_t in;
			if (!stringToUL(data, in)) {
				std::cerr << "input data error: " << data << std::endl;
				return false;
			}

			if (false) {}
			else if (s_type == "int8") { uints_to_char((int8_t)in, input); }
			else if (s_type == "uint8") { uints_to_char((uint8_t)in, input); }
			else if (s_type == "int16") { uints_to_char((int16_t)in, input); }
			else if (s_type == "uint16") { uints_to_char((uint16_t)in, input); }
			else if (s_type == "int32") { uints_to_char((int32_t)in, input); }
			else if (s_type == "uint32") { uints_to_char((uint32_t)in, input); }
			else if (s_type == "int64") { uints_to_char((int64_t)in, input); }
			else if (s_type == "uint64") { uints_to_char((uint64_t)in, input); }
			else { std::cerr << "unknow type" << std::endl; return false; }
		}
	}

	return true;
}

bool parseInputFromFileNoRegex(const std::string& inputFile, std::vector<unsigned char>& input)
{
	std::cout << "not implement on linux" << endl;
	return false;
}

bool parseInputFromFile(const std::string& inputFile, std::vector<unsigned char>& input)
{
#if SUPPORT_REGEX
	/*
	还没有优化
	*/
	ifstream ifile(inputFile);
	if (!ifile.good()) {

		return false;
	}
	bool gottype = false; // got the input type

	string input_var = "";
	string input_type = "";
	string input_type2 = "";
	int arr_1_n = 0;
	int arr_2_n = 0;
	INPUT_TYPE inputtype = INPUT_TYPE::UNKNOW;

	struct field {
		string type;
		string var;
	};

	std::map<std::string, std::string> map_struct;
	std::map<std::string, vector<field>> map_struct_fields;
	string struct_var;
	string s_struct = "";
	bool instruct = false;
	string valid_type;

	string data_string = "";
	while (!ifile.eof()) {
		string line;
		getline(ifile, line);
		if (line == "") {
			continue;
		}

		cout << line << endl;
		std::vector<std::string> vecline;
		vecline = split(line, ' ');
		cout << vecline.size() << endl;

		if (line.find("example") == 0) {
			gottype = false;
			inputtype = INPUT_TYPE::UNKNOW;
			input.clear();
			continue;
		}

		std::string text;
		std::string pattern;

		text = line;

		match_results<std::string::iterator> results;

#ifdef _WIN32
#pragma region 处理结构体 待完成
#endif

		valid_type = basic_type;
		for (auto kv : map_struct_fields) {
			valid_type = valid_type + "|" + kv.first;
		}

		if (!instruct) {
			/*
			**	typedef struct Point
			**	{
			**		int32 x;
			**		int32 y;
			**	}
			*/
			if (regex_search(text.begin(), text.end(), results, regex_struct)) {
				instruct = true;
				struct_var = results[1].str();
			}
		}

		if (instruct) {
			s_struct += text;
			if (count(s_struct.begin(), s_struct.end(), '{') == 1) {
				regex filters("[^a-zA-Z0-9_ \t;]");
				text = regex_replace(text, filters, std::string(""));
				std::string pattern_struct_intype = "(" + valid_type + ") (" + variable + ");";
				regex regex_struct_intype(pattern_struct_intype); // "int32 x;"
				if (regex_search(text.begin(), text.end(), results, regex_struct_intype)) {
					field f;
					f.type = results[1].str();
					f.var = results[2].str();
					map_struct_fields[struct_var].push_back(f);
				}

				if (count(s_struct.begin(), s_struct.end(), '}') == 1) {
					instruct = false;
					s_struct = "";
				}

			}
			continue;
		}
#ifdef _WIN32
#pragma endregion
#endif
		// only get the first #input var type[m][n]
		if (!gottype) {
			if (regex_search(text.begin(), text.end(), results, regex_basic)) {
				inputtype = INPUT_TYPE::BASIC;
			}
			else if (regex_search(text.begin(), text.end(), results, regex_array1)) {
				inputtype = INPUT_TYPE::ARRAY1;
				arr_1_n = atoi(results[3].str().c_str());
			}
			else if (regex_search(text.begin(), text.end(), results, regex_array2)) {
				inputtype = INPUT_TYPE::ARRAY2;
				arr_1_n = atoi(results[3].str().c_str());
				arr_2_n = atoi(results[4].str().c_str());
			}
			else {
				inputtype = INPUT_TYPE::UNKNOW;
				continue;
			}
			gottype = true;
			input_var = results[1].str();
			input_type = results[2].str();
			continue;
		}

		regex filters("[^a-zA-Z0-9_=,\\{\\}]");
		text = regex_replace(text, filters, std::string(""));

		std::string pattern_data;
		if (inputtype == INPUT_TYPE::BASIC) {
			input_type2 = input_type;
			pattern_data = input_var + "=(.*)";
		}
		else if (inputtype == INPUT_TYPE::ARRAY1) {
			input_type2 = input_type + "-array";
			pattern_data = input_var + "=\\{(.*)\\}";

			data_string += text;
			if ((count(data_string.begin(), data_string.end(), '{') == 1)
				&& (count(data_string.begin(), data_string.end(), '}') == 1)) {
				text = data_string; // var={xx,yy}
				data_string = "";
			}
			else {
				continue;
			}
		}
		else if (inputtype == INPUT_TYPE::ARRAY2) {
			input_type2 = input_type + "-array";
			pattern_data = input_var + "=\\{(.*)\\}";

			data_string += text;
			if ((count(data_string.begin(), data_string.end(), '{') == arr_1_n + 1)
				&& (count(data_string.begin(), data_string.end(), '}') == arr_1_n + 1)) {
				text = data_string; // var={{xx,yy},{aa,bb}}
				data_string = "";

				text.pop_back(); // var={{xx,yy},{aa,bb}
				regex express1("\\{\\{");
				text = regex_replace(text, express1, std::string("{"));// var={xx,yy},{aa,bb}
				regex express2("\\},\\{");
				text = regex_replace(text, express2, std::string(","));// var={xx,yy,aa,bb}
			}
			else {
				continue;
			}

		}
		text = regex_replace(text, filters, std::string(""));

		regex regx_data(pattern_data);
		if (regex_search(text.begin(), text.end(), results, regx_data)) {
			string input_data = results[1].str();
			input_data = results[1].str();
			if (parseInputFromConsole(input, input_type2, input_data)) {
				return true;
			}
		}
	}

	return false;
#else
	return parseInputFromFileNoRegex(inputFile, input);
#endif
}

bool parseInputFromConsole(std::vector<unsigned char>& input, const std::string& input_type, const std::string& input_data) {
#if SUPPORT_REGEX
	std::string s_type = "";
	std::string s_input_type(input_type);
	std::string s_input_data(input_data);

	// inputType "bool|int8|uint8|int16|uint16|int32|uint32|int64|uint64" type
	// inputType "bool-array|int8-array|...|int64-array|uint64-array" type-array
	// inputType "bool-array-len|int8-array-len|...|uint64-array-len" type-array-len
	std::string pattern_basic = "(" + basic_type + ")(-array)?";
	regex regex_basic(pattern_basic);
	match_results<std::string::iterator> results;
	if (regex_search(s_input_type.begin(), s_input_type.end(), results, regex_basic)) {
		s_type = results[1].str();// inputType
		vector<std::string> vecInputDatas;
		if (results[2].matched) { // type-array
			vecInputDatas = split(input_data, ',');
			// type-array-len
			std::string pattern_array_len = "(" + basic_type + ")-array-(\\d+)";
		}
		else {// basic type
			vecInputDatas.push_back(input_data);
		}
		if (vecInputDatas.size() == 0) {
			std::cerr << "no data" << std::endl;
			return false;
		}

		std::string pattern_bool = "^(true|false|0|1)$";
		std::string pattern_ints = "^([-]?\\d+)$";
		regex regex_bool(pattern_bool);
		regex regex_ints(pattern_ints);

		for (auto data : vecInputDatas) {
			// data valid check
			if (s_type == "bool") {
				// inputType "bool"
				if (!regex_search(data.begin(), data.end(), results, regex_bool)) {
					std::cerr << "input data error: " << data << std::endl;
					return false;
				}
				bool b = (data == "true" || data == "1");
				bool_to_uchar(&b, 1, input);
			}
			else {
				// inputType "int8|uint8|int16|uint16|int32|uint32|int64|uint64"
				if (!regex_search(data.begin(), data.end(), results, regex_ints)) {
					std::cerr << "input data error: " << data << std::endl;
					return false;
				}

				uint64_t in;
				if (!stringToUL(data, in)) {
					std::cerr << "input data error: " << data << std::endl;
					return false;
				}

				if (false) {/* do nothing */ }
				else if (s_type == "int8") { uints_to_char((int8_t)in, input); }
				else if (s_type == "uint8") { uints_to_char((uint8_t)in, input); }
				else if (s_type == "int16") { uints_to_char((int16_t)in, input); }
				else if (s_type == "uint16") { uints_to_char((uint16_t)in, input); }
				else if (s_type == "int32") { uints_to_char((int32_t)in, input); }
				else if (s_type == "uint32") { uints_to_char((uint32_t)in, input); }
				else if (s_type == "int64") { uints_to_char((int64_t)in, input); }
				else if (s_type == "uint64") { uints_to_char((uint64_t)in, input); }
				else { std::cerr << "unknow type" << std::endl; return false; }
			}
		}
	}
	else {
		std::cout << "not implement, input type error" << endl;
		return false;
	}

	return true;
#else
	return parseInputFromConsoleNoRegex(input, input_type, input_data);
#endif
}

bool parseInput(std::vector<unsigned char>& input, const std::string& input_type, const std::string& input_data) {
	// inputType "file"
	if (input_type == "file" || input_type == "9") {
		if (!platon::mpc::SysUtilsTool::checkFileExists(input_data)) {
			std::cerr << "input data file[" << input_data << "] is not exist." << endl;
			return false;
		}
		if (!parseInputFromFile(input_data, input)) {
			std::cout << "parse input-data file error" << endl;
			return false;
		}
	}
	else {
		return parseInputFromConsole(input, input_type, input_data);
	}
	return true;
}
