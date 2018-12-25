#pragma once

#include <map>
#include <vector>
#include <string>
#include <iostream>

namespace dev
{

struct func_info_st {
	std::string func_name;
	std::string func_full;
	std::string func_prot;
	std::string func_sha3;
	std::vector<unsigned char> func_byte;

	std::string toString() {
		std::stringstream sss;
		sss << "=================================="
			<< std::endl << "func_name: " << func_name
			<< std::endl << "func_full: " << func_full
			<< std::endl << "func_prot: " << func_prot
			<< std::endl << "func_sha3: " << func_sha3
			<< std::endl << "func_byte: ";
			for (auto b : func_byte) {
				sss  << b;
			}
			sss << std::endl;
		return sss.str();
	}
	void print() {
		std::cout << toString();
	}
};

int GetFunctionInfo(const char* filename, std::vector<func_info_st>& vecFuncInfos);
int GetFunctionInfo(const char* filename, std::map<std::string, func_info_st>& mapFuncInfos);

}