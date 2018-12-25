#include <gtest/gtest.h>

#include <string>
#include <vector>
#include <iostream>

#include "base64.h"
using namespace platon::mpc;


TEST(TestUtil, TestBase64_no_nl_01)
{
	int olen = -1;
	bool with_new_line = false;
	std::string enc_input = "Base64 is a group of similar binary-to-text encoding schemes that represent binary data in an ASCII string format by translating it into a radix-64 representation.";
	int ilen = (int)enc_input.length();
	unsigned char* enc_output = Base64::Encode((const unsigned char*)enc_input.c_str(), ilen, &olen, with_new_line);
	ASSERT_EQ(220, olen);
	EXPECT_STREQ((char*)enc_output, "QmFzZTY0IGlzIGEgZ3JvdXAgb2Ygc2ltaWxhciBiaW5hcnktdG8tdGV4dCBlbmNvZGluZyBzY2hlbWVzIHRoYXQgcmVwcmVzZW50IGJpbmFyeSBkYXRhIGluIGFuIEFTQ0lJIHN0cmluZyBmb3JtYXQgYnkgdHJhbnNsYXRpbmcgaXQgaW50byBhIHJhZGl4LTY0IHJlcHJlc2VudGF0aW9uLg==");

	std::string dec_input((char*)enc_output);
	unsigned char* dec_output = Base64::Decode((const unsigned char *)dec_input.c_str(), dec_input.length(), &olen, with_new_line);
	ASSERT_EQ(ilen, olen);
	EXPECT_STREQ((char*)dec_output, enc_input.c_str());

	free(enc_output);
	free(dec_output);
}

TEST(TestUtil, TestBase64_no_nl_02)
{
	int olen = -1;
	bool with_new_line = false;
	unsigned char enc_input[] = { 0x01,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x04,0x05,0x00,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x80,0x05,0x00,0x80,0x05,0x00,0x80, 0x01,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x04,0x05,0x00,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x80,0x05,0x00,0x80,0x05,0x00,0x80, 0x01,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x04,0x05,0x00,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x80,0x05,0x00,0x80,0x05,0x00,0x80,0xff };
	int ilen = sizeof(enc_input) / sizeof(char);
	unsigned char* enc_output = Base64::Encode(enc_input, ilen, &olen, with_new_line);
	ASSERT_EQ(164, olen);
	EXPECT_STREQ((char*)enc_output, "AQIDBAEFAIACAwIDBAEFAIACAwQEBQACAwQBBQCAAgMEgAUAgAUAgAECAwQBBQCAAgMCAwQBBQCAAgMEBAUAAgMEAQUAgAIDBIAFAIAFAIABAgMEAQUAgAIDAgMEAQUAgAIDBAQFAAIDBAEFAIACAwSABQCABQCA/w==");

	std::string dec_input((char*)enc_output);
	unsigned char* dec_output = Base64::Decode((const unsigned char *)dec_input.c_str(), dec_input.length(), &olen, with_new_line);
	ASSERT_EQ(ilen, olen);
	for (int i = 0; i < olen; i++) {
		EXPECT_EQ(enc_input[i], dec_output[i]);
	}

	free(enc_output);
	free(dec_output);
}

TEST(TestUtil, TestBase64_nl_01)
{
	int olen = -1;
	bool with_new_line = true;
	std::string enc_input = "Base64 is a group of similar binary-to-text encoding schemes that represent binary data in an ASCII string format by translating it into a radix-64 representation.";
	int ilen = (int)enc_input.length();
	unsigned char* enc_output = Base64::Encode((const unsigned char *)enc_input.c_str(), ilen, &olen, with_new_line);
	ASSERT_EQ(224, olen);
	EXPECT_STREQ((char*)enc_output, "QmFzZTY0IGlzIGEgZ3JvdXAgb2Ygc2ltaWxhciBiaW5hcnktdG8tdGV4dCBlbmNv\nZGluZyBzY2hlbWVzIHRoYXQgcmVwcmVzZW50IGJpbmFyeSBkYXRhIGluIGFuIEFT\nQ0lJIHN0cmluZyBmb3JtYXQgYnkgdHJhbnNsYXRpbmcgaXQgaW50byBhIHJhZGl4\nLTY0IHJlcHJlc2VudGF0aW9uLg==\n");

	std::string dec_input((char*)enc_output);
	unsigned char* dec_output = Base64::Decode((const unsigned char *)dec_input.c_str(), dec_input.length(), &olen, with_new_line);
	ASSERT_EQ(ilen, olen);
	EXPECT_STREQ((char*)dec_output, enc_input.c_str());

	free(enc_output);
	free(dec_output);
}

TEST(TestUtil, TestBase64_nl_02)
{
	int olen = -1;
	bool with_new_line = true;
	unsigned char enc_input[] = { 0x01,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x04,0x05,0x00,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x80,0x05,0x00,0x80,0x05,0x00,0x80, 0x01,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x04,0x05,0x00,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x80,0x05,0x00,0x80,0x05,0x00,0x80, 0x01,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x04,0x05,0x00,0x02,0x03,0x04,0x01,0x05,0x00,0x80,0x02,0x03,0x04,0x80,0x05,0x00,0x80,0x05,0x00,0x80,0xff };
	int ilen = sizeof(enc_input) / sizeof(char);
	unsigned char* enc_output = Base64::Encode(enc_input, ilen, &olen, with_new_line);
	ASSERT_EQ(167, olen);
	EXPECT_STREQ((char*)enc_output, "AQIDBAEFAIACAwIDBAEFAIACAwQEBQACAwQBBQCAAgMEgAUAgAUAgAECAwQBBQCA\nAgMCAwQBBQCAAgMEBAUAAgMEAQUAgAIDBIAFAIAFAIABAgMEAQUAgAIDAgMEAQUA\ngAIDBAQFAAIDBAEFAIACAwSABQCABQCA/w==\n");

	std::string dec_input((char*)enc_output);
	unsigned char* dec_output = Base64::Decode((const unsigned char *)dec_input.c_str(), dec_input.length(), &olen, with_new_line);
	ASSERT_EQ(ilen, olen);
	for (int i = 0; i < 8; i++) {
		EXPECT_EQ(enc_input[i], dec_output[i]);
	}

	free(enc_output);
	free(dec_output);
}

TEST(TestUtil, TestBase64xxxxxxxxxxx)
{
	EXPECT_STREQ("", "");
}

