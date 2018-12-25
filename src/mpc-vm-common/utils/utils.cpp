#include "utils.h"
#include <stdlib.h>
#include <iostream>

using std::cout;
using std::endl;

NS_PLATON_MPC_BEG

uint64_t bool_to64(const bool * data)
{
	uint64_t res = 0;
	for (int i = 0; i < 64; ++i) {
		if (data[i])
			res |= (1ULL << i);
	}
	return res;
}



void int64_to_bool(bool * data, uint64_t input, int length)
{
	for (int i = 0; i < length; ++i) {
		data[i] = (input & 1) == 1;
		input >>= 1;
	}
}

static const char* hex_char_to_bin(char c)
{
	switch (toupper(c))
	{
	case '0': return "0000";
	case '1': return "0001";
	case '2': return "0010";
	case '3': return "0011";
	case '4': return "0100";
	case '5': return "0101";
	case '6': return "0110";
	case '7': return "0111";
	case '8': return "1000";
	case '9': return "1001";
	case 'A': return "1010";
	case 'B': return "1011";
	case 'C': return "1100";
	case 'D': return "1101";
	case 'E': return "1110";
	case 'F': return "1111";
	default: return "0";
	}
}


std::string hex_to_binary(std::string hex)
{
	std::string bin;
	for (unsigned i = 0; i != hex.length(); ++i)
		bin += hex_char_to_bin(hex[i]);
	return bin;
}

static string intToStr(int a, int base) {

	if (base == 16 || base == 10 || base == 8) {
		stringstream ss;
		ss << std::setbase(base) << a;
		return ss.str();
	}
	else if (base == 2)
	{
		// get legal bits number
		string result;
		int count = 0;
		int flag = 0;
		if (a > 0)
			flag = 1;

		int k = 0;
		int n = a;
		for (int c = 30; c >= 0; c--)
		{
			k = n >> c;
			if (flag)//positive
			{
				if (!(k & 1) && result.empty())
				{
					continue;
				}
				else
					result += (k & 1) ? '1' : '0';
			}
			else//negative
			{
				if ((k & 1) && result.empty())
				{
					continue;
				}
				else
					result += (k & 1) ? '1' : '0';
			}
		}
		result.insert(0, 1, flag == 1 ? '0' : '1');

		//cout << "*****  the thin bits: " << result << endl;
		return result;
	}
	else {
		return "";
	}
}

static int strToInt(string s, int base) {
	unsigned int a;
	if (base == 16 || base == 10 || base == 8) {
		stringstream ss;
		ss << s;
		ss >> std::setbase(base) >> a;
		return a;
	}
	else if (base == 2) {
		unsigned int *ptr = &a;
		a = bitset<sizeof(int) * 8>(s).to_ulong();
		return *((int *)ptr);
	}
	else {
		return 0;
	}
}

string trans_base(string str, int old_base, int new_base) {
	long long tmp = strtol(str.data(), NULL, old_base);
	//int tmp = strToInt(str, old_base);
//    char szInt[32];
//    memset(szInt, 0, 32);

	string res = intToStr(tmp, new_base);
	//    cout << "the trans_base str:" << str << ", old_base: " << old_base << ", new_base: " << new_base << endl;
	//    cout << "the trans_base res:  " << res << endl;
	return res;
}


string dec_to_bin(const string& dec)
{
	string bin = trans_base(dec, 10, 2);
	return bin;
	//	if(dec[0] != '-')
	//		return '0' + bin;
	//	bin[0] = '1';
	//	bool flip = false;
	//	for(int i = bin.size()-1; i>=1; --i) {
	//		if(flip)
	//			bin[i] = (bin[i] == '1' ? '0': '1');
	//		if(bin[i] == '1')
	//			flip = true;
	//	}
	//	return bin;
}

string bin_to_dec(const string& bin2)
{
	if (bin2[0] == '0')
		return trans_base(bin2, 2, 10);
	string bin = bin2;
	bin[0] = '0';
	bool flip = false;
	for (int i = bin.size() - 1; i >= 1; --i) {
		if (flip)
			bin[i] = (bin[i] == '1' ? '0' : '1');
		if (bin[i] == '1')
			flip = true;
	}
	return "-" + trans_base(bin, 2, 10);
}

// convert number of decimal format to binary format
string decimal_2_binary(const string& dec) {
	string binary = trans_base(dec, 10, 2);
	if (dec[0] != '-')
		return '0' + binary;
	binary[0] = '1';
	bool flip = false;
	for (int i = binary.size() - 1; i >= 1; --i) {
		if (flip)
			binary[i] = (binary[i] == '1' ? '0' : '1');
		if (binary[i] == '1')
			flip = true;
	}
	return binary;
}

void ulong_2_bits(bool *res, uint64_t in, int length) {
	for (int i = 0; i < length; ++i) {
		res[i] = ((in & 1) == 1);
		in = in >> 1;
	}
}

uint64_t bits_2_ulong(const bool *data) {
	uint64_t res = 0;
	for (int i = 0; i < 64; ++i) {
		if (data[i])
			res |= (1ULL << i);
	}
	return res;
}
NS_PLATON_MPC_END
