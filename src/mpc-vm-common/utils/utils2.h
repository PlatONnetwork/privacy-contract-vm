#pragma once

#include <vector>
#include <string>

#include "mpc_common.h"
NS_PLATON_MPC_BEG

int hexstr2buf(const std::string& s, unsigned char* buf);
std::vector<std::string> split(const std::string &src, char delim);
bool makedirs(const std::string& dirpath);

int vectobuf(const std::vector<unsigned char>& in, unsigned char* buf);
int buftovec(std::vector<unsigned char>& out, const unsigned char* buf, int len);

NS_PLATON_MPC_END
