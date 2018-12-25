#include "utils2.h"

#include <map>
#include <regex>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <string>
using namespace std;

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#define access _access
#define mkdir _mkdir
#else
#include <fcntl.h>
#include <sys/io.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#endif

NS_PLATON_MPC_BEG

int hexstr2buf(const std::string& s, unsigned char* buf) {
	int i=0, n = 0;
	if ((s.length() >= 2) && (s[0] == '0') && (toupper(s[1]) == 'X')) {
		i = 2;
	}
	for (; i < s.length(); i += 2) {
		unsigned char c = toupper(s[i]);
		if (c >= 'A' && c <= 'F')
			buf[n] = c - 'A' + 10;
		else buf[n] = c - '0';
		c = toupper(s[i + 1]);
		if (c >= 'A' && c <= 'F')
			buf[n] = (buf[n] << 4) | (c - 'A' + 10);
		else buf[n] = (buf[n] << 4) | (c - '0');
		++n;
	}
	return n;
}

std::vector<std::string> split(const std::string &src, char delim) {
	std::stringstream ss(src);
	std::string item;
	std::vector<std::string> elems;
	while (std::getline(ss, item, delim)) {
		elems.push_back(std::move(item));
	}
	return elems;
}

bool makedirs(const std::string& dirpath) {
	string path(dirpath);
	path = regex_replace(path, regex("\\\\"), string("/"));

	std::string folder_builder;
	std::string sub;
	sub.reserve(path.size());
	for (auto it = path.begin(); it != path.end(); ++it) {
		const char c = *it;
		sub.push_back(c);
		if (c == '/' || it == path.end() - 1) {
			folder_builder.append(sub);
			if (0 != access(folder_builder.c_str(), 0)) {
#ifdef  _WIN32
				if (0 != mkdir(folder_builder.c_str())) {
#else
				if (0 != mkdir(folder_builder.c_str(), 0755)) {
#endif
					return false;
				}
			}
			sub.clear();
		}
	}
	return true;
}

int vectobuf(const std::vector<unsigned char>& in, unsigned char* buf) {
	for (int i = 0; i < in.size(); i++) {
		buf[i] = in[i];
	}
	return (int)in.size();
}

int buftovec(std::vector<unsigned char>& out, const unsigned char* buf, int len) {
	for (int i = 0; i < len; i++) {
		out.push_back(buf[i]);
	}
	return len;
}

NS_PLATON_MPC_END
