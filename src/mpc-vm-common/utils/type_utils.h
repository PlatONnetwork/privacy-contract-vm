#pragma once


#include <stdint.h>
#include <vector>
#include <string>


#include "mpc_common.h"
NS_PLATON_MPC_BEG

void   uchar_to_bool(const std::vector<unsigned char>& in, const int outputlen, bool *out);

void   uchar_to_bool(const std::vector<unsigned char>& in, const int outputlen, std::vector<bool>& out);


void   bool_to_uchar(const bool *in, const int inputlen, std::vector<unsigned char>& out);
void   bool_to_uchar(const std::vector<bool>& in, std::vector<unsigned char>& out);

template<typename T>
void uints_to_char(const T& in, std::vector<unsigned char>& out)
{
	int i = 0;
	unsigned char c;
	const int size = sizeof(in);
	for (i = 0; i < size; i++)
	{
		c = (in >> (i * 8)) & 0xff;
		out.push_back(c);
	}
}

template <typename T>
void char_to_uints(const std::vector<unsigned char>& in, T &out)
{
	const int size = sizeof(in);
	int i = 0, t = in.size();
	if (t > size)
		t = size;
	out = 0;
	for (i = t - 1; i >= 0; i--)
		out = (out << 8) | in[i];
}

void   int32_to_char(const int32_t in, std::vector<unsigned char>& out);

void   char_to_int32(const std::vector<unsigned char>& in, int32_t &out);

void   int64_to_char(const int32_t in, std::vector<unsigned char>& out);

void   char_to_int64(const std::vector<unsigned char>& in, int64_t &out);

void   uint32_to_char(const uint32_t in, std::vector<unsigned char>& out);

void   char_to_uint32(const std::vector<unsigned char>& in, uint32_t &out);

void   uint64_to_char(const uint32_t in, std::vector<unsigned char>& out);

void   char_to_uint64(const std::vector<unsigned char>& in, uint64_t &out);

int   hex_to_bytes(const std::string& input, std::vector<unsigned char> &output);


static constexpr char hexmap[] = { '0', '1', '2', '3', '4', '5', '6', '7', \
	'8', '9', 'a', 'b', 'c', 'd', 'e', 'f' };


template<typename T>
std::string ToHexString(T* bin, int count) noexcept {
	std::string hex(count * sizeof(T) * 2, ' ');
	unsigned char* byte = static_cast<unsigned char*>(bin);
	for (int i = 0; i < count * sizeof(T); ++i)
	{
		hex[2 * i] = hexmap[(byte[i] & 0xF0) >> 4];
		hex[2 * i + 1] = hexmap[byte[i] & 0x0F];
	}

	return hex;
};

template<typename T>
std::string ToHexString(T* v) noexcept {
	return ToHexString(v, 1);
};
NS_PLATON_MPC_END

