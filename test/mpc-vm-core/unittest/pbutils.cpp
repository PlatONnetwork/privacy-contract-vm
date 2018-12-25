#include <gtest/gtest.h>
#include "pb_utils.h"


using namespace platon::mpc;

TEST(PbUtilsTest, TestReadWrite)
{
	{
		char szBuffer[32] = { 0 };
		int32_t ia = -899999;
		int bytes = WriteVarInt32(szBuffer, 32, ia);
		//printf("the write32 value: %d, ret: %d \n", ia, bytes);

		int32_t value = 0;
		bytes = ReadVarInt32(szBuffer, 32, value);
		//printf("the read32 value: %d, ret: %d \n", value, bytes);

		EXPECT_EQ(ia, value) << "write and read varint32 failed !" << std::endl;

		uint32_t uia = 99999;
		bytes = WriteVarUint32(szBuffer, 32, uia);
		//printf("the writeU32 value: %u, ret: %d \n", uia, bytes);

		uint32_t uiv = 0;
		bytes = ReadVarUint32(szBuffer, 32, uiv);
		//printf("the readU32 value: %d, ret: %d \n", uiv, bytes);

		EXPECT_EQ(uia, uiv) << "write and read varUint32 failed !" << std::endl;

		int64_t iia = -899999;
		bytes = WriteVarInt64(szBuffer, 32, iia);
		//printf("the write64 value: %lld, ret: %d \n", iia, bytes);

		int64_t value64 = 0;
		bytes = ReadVarInt64(szBuffer, 32, value64);
		//printf("the read64 value: %lld, ret: %d \n", value64, bytes);
		EXPECT_EQ(iia, value64) << "write and read varint64 failed !" << std::endl;

		float fa = -0.889f;
		double da = -0.999998;
		bytes = WriteVarFloat(szBuffer, 32, fa);
		//printf("the WriteFloat value: %f, bytes: %d \n", fa, bytes);

		float fv;
		bytes = ReadVarFloat(szBuffer, 32, fv);
		//printf("the ReadVarFloat value: %f, bytes: %d \n", fv, bytes);
		EXPECT_EQ(fa, fv) << "write and read float failed !" << std::endl;

		bytes = WriteVarDouble(szBuffer, 32, da);
		//printf("the WriteVarDouble value: %lf, bytes: %d \n", da, bytes);

		double dv;
		bytes = ReadVarDouble(szBuffer, 32, dv);
		//printf("the ReadVarDouble value: %lf, bytes: %d \n", dv, bytes);
		EXPECT_EQ(da, dv) << "write and read double failed !" << std::endl;

		bool ba = true;
		bytes = WriteVarBool(szBuffer, 32, ba);
		//printf("the WriteVarBool value: %s, bytes: %d \n", ba ? "true" : "false", bytes);

		bool bv;
		bytes = ReadVarBool(szBuffer, 32, bv);
		//printf("the ReadVarBool value: %s, bytes: %d \n", bv ? "true" : "false", bytes);
		EXPECT_EQ(ba, bv) << "write and read bool failed !" << std::endl;

		std::string str = "hello, world";
		bytes = WriteVarString(szBuffer, 32, str);
		//printf("the WriteVarString value: %s, bytes: %d \n", str.data(), bytes);

		std::string hello;
		bytes = ReadVarString(szBuffer, 32, hello);
		//printf("the ReadString value: %s, bytes: %d \n", hello.data(), bytes);
		EXPECT_EQ(str, hello) << "write and read string failed !" << std::endl;

		//return 0;
	}
}