#pragma once

#include <string>
#include <vector>
#include <map>


// these functions will be remove to utils-tookit.
int32_t get_cpu_cores();
std::vector<std::string> split(const std::string& s, char delim);
bool stringToUL(const std::string &str, uint64_t  &val);


bool parseInput(std::vector<unsigned char>& input, const std::string& input_type, const std::string& input_data);
