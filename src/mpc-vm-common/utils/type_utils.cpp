
#include "type_utils.h"


NS_PLATON_MPC_BEG


void uchar_to_bool(const std::vector<unsigned char>& in, const int outputlen, bool *out)
{
	int i = 0, j = 0, k = 0;
	int t = outputlen >> 3;
	if (outputlen < 8)
		t = 1;

	unsigned char filter[] = { 0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80 };
	if (t > in.size())
		t = in.size();

	for (i = 0; i < t; i++)
	{
		j = i * 8;
		for (k = 0; k < 8; k++)
			if (in[i] & filter[k])
				out[j + k] = true;
			else
				out[j + k] = false;
	}
	j = i * 8;
	if (j < outputlen)
	{
		if (t < in.size())
		{
			for (k = 0; k < outputlen - j; k++)
				if (in[i] & filter[k])
					out[j + k] = true;
		}
		else
		{
			for (; j < outputlen; j++)
				out[j] = false;
		}
	}
}


void uchar_to_bool(const std::vector<unsigned char>& in, const int outputlen, std::vector<bool>& out)
{
	bool* bout = new bool[outputlen];
	uchar_to_bool(in, outputlen, bout);

	out.assign(bout, bout + outputlen);
	delete[]bout;
}


void bool_to_uchar(const bool *in, const int inputlen, std::vector<unsigned char>& out)
{
	int i = 0, j = 0, k = 0, t = inputlen >> 3;
	unsigned char filter[] = { 0x1,0x2,0x4,0x8,0x10,0x20,0x40,0x80 };
	unsigned char temp;

	for (i = 0; i < t; i++)
	{
		j = i * 8;
		temp = 0;
		for (k = 0; k < 8; k++)
			if (in[j + k] == true)
				temp |= filter[k];
		out.push_back(temp);
	}
	j = i * 8;
	if (j < inputlen)
	{
		temp = 0;
		for (k = 0; k < inputlen - j; k++)
			if (in[j + k] == true)
				temp |= filter[k];
		out.push_back(temp);
	}
}


void bool_to_uchar(const std::vector<bool>& in, std::vector<unsigned char>& out)
{
	bool *pt = new bool[in.size()];
	for (int i = 0; i < in.size(); ++i)
		pt[i] = in[i];

	bool_to_uchar(pt, in.size(), out);
	delete[]pt;
}


void int32_to_char(const int32_t in, std::vector<unsigned char>& out)
{
	int i = 0;
	unsigned char c;
	for (i = 0; i < 4; i++)
	{
		c = (in >> (i * 8)) & 0xff;
		out.push_back(c);
	}

}
void char_to_int32(const std::vector<unsigned char>& in, int32_t &out)
{
	int i = 0, t = in.size();
	if (t > 4)
		t = 4;
	out = 0;
	for (i = t - 1; i >= 0; i--)
		out = (out << 8) | in[i];
}
void int64_to_char(const int32_t in, std::vector<unsigned char>& out)
{
	int i = 0;
	unsigned char c;
	for (i = 0; i < 8; i++)
	{
		c = (in >> (i * 8)) & 0xff;
		out.push_back(c);
	}

}
void char_to_int64(const std::vector<unsigned char>& in, int64_t &out)
{
	int i = 0, t = in.size();
	if (t > 8)
		t = 8;
	out = 0;
	for (i = t - 1; i >= 0; i--)
		out = (out << 8) | in[i];
}

void uint32_to_char(const uint32_t in, std::vector<unsigned char>& out)
{
	int i = 0;
	unsigned char c;
	for (i = 0; i < 4; i++)
	{
		c = (in >> (i * 8)) & 0xff;
		out.push_back(c);
	}
}

void char_to_uint32(const std::vector<unsigned char>& in, uint32_t &out)
{
	int i = 0, t = in.size();
	if (t > 4)
		t = 4;
	out = 0;
	for (i = t - 1; i >= 0; i--)
		out = (out << 8) | in[i];
}
void uint64_to_char(const uint32_t in, std::vector<unsigned char>& out)
{
	int i = 0;
	unsigned char c;
	for (i = 0; i < 8; i++)
	{
		c = (in >> (i * 8)) & 0xff;
		out.push_back(c);
	}

}
void char_to_uint64(const std::vector<unsigned char>& in, uint64_t &out)
{
	int i = 0, t = in.size();
	if (t > 8)
		t = 8;
	out = 0;
	for (i = t - 1; i >= 0; i--)
		out = (out << 8) | in[i];
}

int hex_to_bytes(const std::string& input, std::vector<unsigned char> &output)
{
	//"0xef2323ef"
	std::vector<unsigned char> temp;
	std::string hex = input;
	if (hex.size() > 2 && hex[0] == '0' && (hex[1] == 'x' || hex[1] == 'X'))
		hex = hex.substr(2);

	for (int i = 0; i < hex.size(); i += 2)
	{
		unsigned char low = 0;
		unsigned char high = 0;
		if (hex[i] >= '0' && hex[i] <= '9')
			high = 16 * (hex[i] - '0');
		else if (hex[i] >= 'a' && hex[i] <= 'f')
			high = 16 * (hex[i] - 'a' + 10);
		else if (hex[i] >= 'A' && hex[i] <= 'F')
			high = 16 * (hex[i] - 'A' + 10);
		else
			return -1;

		if (hex[i + 1] >= '0' && hex[i + 1] <= '9')
			low = (hex[i + 1] - '0');
		else if (hex[i + 1] >= 'a' && hex[i + 1] <= 'f')
			low = (hex[i + 1] - 'a' + 10);
		else if (hex[i + 1] >= 'A' && hex[i + 1] <= 'F')
			low = (hex[i + 1] - 'A' + 10);
		else
			return -1;

		temp.push_back(high + low);
	}

	output.swap(temp);
	return 0;
}
NS_PLATON_MPC_END
