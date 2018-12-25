#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <iostream>

#include "ssl.h"
using namespace platon::mpc;

TEST(TestUtil, TestSSLmd5)
{
	std::string src, ret;

	src = "";
	ret = SSL::md5(src);
	EXPECT_EQ(ret, "d41d8cd98f00b204e9800998ecf8427e");

	src = "abcdefg";
	ret = SSL::md5(src);
	EXPECT_EQ(ret, "7ac66c0f148de9519b8bd264312c4d64");

	src = "abcdefgabcdefgabcdefgabcdefgabcdefgabcdefgabcdefgabcdefgabcdefgabcdefg";
	ret = SSL::md5(src);
	EXPECT_EQ(ret, "01bec5f107817e3b66c25cf215baf72f");
	
}

TEST(TestUtil, TestSSLsha256)
{
	std::string src, ret;

	src = "";
	ret = SSL::sha256(src);
	EXPECT_EQ(ret, "e3b0c44298fc1c149afbf4c8996fb92427ae41e4649b934ca495991b7852b855");

	src = "abcdefg";
	ret = SSL::sha256(src);
	EXPECT_EQ(ret, "7d1a54127b222502f5b79b5fb0803061152a44f92b37e23c6527baf665d4da9a");

	src = "abcdefgabcdefgabcdefgabcdefgabcdefgabcdefgabcdefgabcdefgabcdefgabcdefg";
	ret = SSL::sha256(src);
	EXPECT_EQ(ret, "583aedd211a41da04411420371a3a4ff82a60ed61d683b881900ce21686b7269");

}
