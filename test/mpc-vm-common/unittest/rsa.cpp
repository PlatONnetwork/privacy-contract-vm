#include <gtest/gtest.h>

#include <vector>
//#ifndef WIN32
#include "rsa.h"

TEST(TestUtil, TestRSA)
{
	std::vector<int> bits = { 512,1024,2048 };
	std::vector<int> paddings = { RSA_PKCS1_PADDING };

	for (auto bit : bits) {
		for (auto padding : paddings) {
			const unsigned char* plantext = (const unsigned char*)"I love you!";
			int ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);

			plantext = (const unsigned char*)"length is 116 data data data data data data data data data data data data data data data data data data data data da";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);

			plantext = (const unsigned char*)"length is 117 data data data data data data data data data data data data data data data data data data data data dat";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);

			EXPECT_EQ(0, ret);
			plantext = (const unsigned char*)"length is 118 data data data data data data data data data data data data data data data data data data data data data";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);

			plantext = (const unsigned char*)"length is 127 data data data data data data data data data data data data data data data data data data data data data data dat";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);

			plantext = (const unsigned char*)"length is 128 data data data data data data data data data data data data data data data data data data data data data data data";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);

			plantext = (const unsigned char*)"length is 129 data data data data data data data data data data data data data data data data data data data data data data data ";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);


			plantext = (const unsigned char*)"length is 244 data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data ";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);

			plantext = (const unsigned char*)"length is 245 data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data d";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);

			plantext = (const unsigned char*)"length is 246 data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data da";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);

			plantext = (const unsigned char*)"length is 254 data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data ";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);

			plantext = (const unsigned char*)"length is 255 data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data d";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);

			plantext = (const unsigned char*)"length is 256 data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data data da";
			ret = platon::mpc::RSA::Test(plantext, bit, padding);
			EXPECT_EQ(0, ret);
		}
	}
}
TEST(TestUtil, TestRSA02)
{
	std::string in = "I love you!";
	int ilen = in.length();
	unsigned char* out = new unsigned char[1024];
	memset(out, 0, 1024);
	int olen = 0;
	std::string hex_n = "C01F11665937EB23344AF06E77C7B73936F93547B94F28F8D4476591BAA7EB8EF43486C137ECFD6079A4798F941C30D49967A34206F7628FE88A6101FCFB204856C60158B3B80D25DF284C96A9BA2781BDEED0A47D1C4706AB6A6DDF6A87A603CABC2286BDD4B3BEFF46F94C37A989AC14EF35137A6A282A1F3C32EF7B72A7C5";
	int ret;
	ret = platon::mpc::RSA::PublicEncrypt(hex_n.c_str(), "03", (const unsigned char*)in.c_str(), ilen, out, &olen, RSA_PKCS1_PADDING);

	EXPECT_EQ(0, ret);
}
//#endif//
TEST(TestUtil, TestRSAxxx)
{
	EXPECT_EQ(0, 0);
}
