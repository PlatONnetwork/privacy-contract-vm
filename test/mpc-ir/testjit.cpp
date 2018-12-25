#include "jit.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include "platon_base_message.pb.h"
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>
#include "emp-sh2pc/emp-sh2pc.h"
#include "emp-tool/io/net_io_channel.h"
#include "platon_core_conf.h"

#define kIrFile "./testint.ll"


using namespace platon::mpc;
using namespace google::protobuf::io;


//input=>   port, input_value
static int parse_args(int argc, char** argv, int& role, int& port, int& value)
{
	if (argc < 4)
	{
		printf("bad input arguments, usage:  ./prog port input_value \n");
		return -1;
	}

	role = atoi(argv[1]);
	port = atoi(argv[2]);
	value = atoi(argv[3]);
	return 0;
}

int main(int argc, char** argv)
{
	int role, port, value;
	if (0 != parse_args(argc, argv, role, port, value))
		return 1;

	//setupu mpc running environment
	IOChannel* io = new NetIO(role == ALICE ? nullptr : "127.0.0.1", port);
	setup_semi_honest(io, role);

	/*if (!PlatON_ClearJITCache(kIrFile))
	{
		printf("clear failed, %s\n", kIrFile);
		return 1;
	}*/

	//IR: ./testint_all.ll
	//so: ./libjuzixmpcvm_core.so
	std::vector<std::string> libs{"./libjuzixmpcvm_core.so", "/usr/local/lib/libprotobuf.so.17", "/usr/local/lib/libprotobuf-lite.so.17"};
	if(!PlatON_InitJIT(libs))
	{
		printf("PlatON_RunFuncByLazyJIT failed !\n");
		return 1;
	}

	/*MPCInteger32 alice;
	alice.set_value(1000);
	MPCInteger32 bob;
	bob.set_value(999);
*/
	char* inputs[2];
	char in0[1000]={0};
	char in1[1000]={0};
	inputs[0] = in0;
	inputs[1] = in1;

	unsigned lengths[]={1000, 1000};


	//	alice.SerializeToArray(inputs[0], 1000);
	char szbuf1[16]={0};
	ArrayOutputStream zout1(szbuf1, 16);
	CodedOutputStream out1(&zout1);
	out1.WriteVarint32SignExtended(value);
	//	bob.SerializeToArray(inputs[1], 1000);
	char szbuf2[16]={0};
	ArrayOutputStream zout2(szbuf2, 16);
	CodedOutputStream out2(&zout2);
	out2.WriteVarint32SignExtended(value);

	inputs[0] = szbuf1;
	inputs[1] = szbuf2;
	lengths[0] = out1.ByteCount();
	lengths[1] = out2.ByteCount();
	
	char* result = nullptr;
	int result_len = PlatON_RunFuncByLazyJIT(kIrFile, "ListMsg", &result, inputs, lengths);
	if (result_len <= 0)
	{
		printf("the result len: %d, jit run failed !\n", result_len);
		return 1;
	}
	printf("the result len: %d \n", result_len);
	
	//MPCInteger32 ret;
	//ret.ParseFromArray(result, result_len);
	int ret;
	CodedInputStream retstream((const uint8_t*)result, (size_t)result_len);
	if(!retstream.ReadVarintSizeAsInt(&ret))
	{
		printf("read the ret failed\n");
		return 0;
	}

	printf("the output of ListMsg is: %d \n", ret);
	free(result);

	return 0;
}

