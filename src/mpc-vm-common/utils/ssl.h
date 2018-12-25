#pragma once

#include <string>  
#include <openssl/md5.h>
#include <openssl/sha.h>

#include "mpc_common.h"
NS_PLATON_MPC_BEG

class SSL
{
public:
	static std::string md5(const unsigned char* in, int ilen, unsigned char* out) {
		MD5(in, ilen, out);
		char buf[33] = { 0 };
		for (int i = 0; i < 16; i++) {
			char tmp[3] = { 0 };
			sprintf(tmp, "%02x", out[i]);
			strcat(buf, tmp);
		}
		buf[32] = '\0';
		return std::string(buf);
	}
	static std::string md5(const unsigned char* in, int ilen) {
		unsigned char out[17] = { 0 };
		return md5(in, ilen, out);
	}
	static std::string md5(const std::string& in) {
		return md5((const unsigned char*)in.c_str(), in.length());
	}

	static std::string sha256(const unsigned char* in, int ilen, unsigned char* out) {
		SHA256(in, ilen, out);
		char buf[65] = { 0 };
		for (int i = 0; i < 32; i++) {
			char tmp[3] = { 0 };
			sprintf(tmp, "%02x", out[i]);
			strcat(buf, tmp);
		}
		buf[64] = '\0';
		return std::string(buf);
	}
	static std::string sha256(const std::string& in) {
		unsigned char out[33] = { 0 };
		return sha256((const unsigned char*)in.c_str(), in.length(), out);
	}

public:
	// for char*
	static int Test(const unsigned char* in) {

		return 0;
	}
};

NS_PLATON_MPC_END
