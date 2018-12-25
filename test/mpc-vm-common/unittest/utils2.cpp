#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <iostream>

#include "utils2.h"
using namespace platon::mpc;

TEST(TestUtil, TestUtils2_hexstr2buf)
{
	std::vector<std::string> vecs = { "13d4f68a","13D4f68a","13D4F68A" };
	for (const auto& s : vecs) {
		std::cout << "s:" << s << std::endl;
		int len = s.length() / 2;
		unsigned char* bufpubkey = new unsigned char[len];
		memset(bufpubkey, 0, len);
		int ret = hexstr2buf(s, bufpubkey);
		ASSERT_EQ(len, ret);

		unsigned char res[] = { 0x13,0xd4,0xf6,0x8a };
		for (int i = 0; i < len; i++) {
			EXPECT_EQ(bufpubkey[i], res[i]);
		}

		delete[] bufpubkey;
	}
}

TEST(TestUtil, TestUtils2_split)
{
	std::string src = "a,b,c,d";
	std::vector<std::string> vecr = { "a","b","c","d" };
	std::vector<std::string> vecs = split(src, ',');
	ASSERT_EQ(vecs.size(), vecr.size());

	for (int i = 0; i < vecr.size(); i++) {
		EXPECT_EQ(vecs[i], vecr[i]);
	}
}

TEST(TestUtil, TestUtils2_makedirs)
{
	EXPECT_EQ(0, 0);
}

TEST(TestUtil, TestUtils2_vectobuf)
{
	std::vector<unsigned char> in = { 0x01,0x02,0x80,0x00,0x81,0xff };
	unsigned char* buf = new unsigned char[in.size()];
	int ret = vectobuf(in, buf);
	EXPECT_EQ(in.size(), ret);
	for (int i = 0; i < in.size(); i++) {
		EXPECT_EQ(in[i], buf[i]);
	}
	delete[] buf;
}

TEST(TestUtil, TestUtils2_buftovec)
{
	std::vector<unsigned char> in;
	unsigned char buf[] = { 0x01,0x02,0x80,0x00,0x81,0xff };
	int ret = buftovec(in, buf, 6);
	EXPECT_EQ(6, ret);
	for (int i = 0; i < in.size(); i++) {
		EXPECT_EQ(in[i], buf[i]);
	}
}

TEST(TestUtil, TestUtils2xxxxxxxxxxxxxxxxx)
{
	EXPECT_EQ(0, 0);
}
