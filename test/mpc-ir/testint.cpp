//#include "lm.helloworld.pb.h"
#include "platon_integer.h"
//#include "platon_base_message.pb.h"
#include <stdio.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl_lite.h>

using namespace platon::mpc;
using namespace google::protobuf::io;

__attribute__((noinline)) 
int ListMsg(int in1, int in2){

  printf("alice: %d, bob: %d\n", in1, in2);

  Integer a(32, in1, ALICE);
  Integer b(32, in2, BOB);

  Integer ret = a - b;
  int plain = ret.reveal_int32();
  printf("to reveal the value, ret: %d...\n", plain);
  return plain;
}

__attribute__((noinline)) 
extern "C" unsigned PlatON_entry_ListMsg(void** buf, char** argv, unsigned* lengths){
  printf("come into, ListMsg_Main...#####\n");
  
  int v1, v2;
  CodedInputStream arg1((const uint8_t*)argv[0], (size_t)lengths[0]);
  if(!arg1.ReadVarintSizeAsInt(&v1))
  {
  	printf("read the arg0 failed\n");
	return 0;
  }

  CodedInputStream arg2((const uint8_t*)argv[1], (size_t)lengths[1]);
  if(!arg2.ReadVarintSizeAsInt(&v2))
  {
	printf("read the arg0 failed\n");
	return 0;
  }
  /*MPCInteger32 in1;
  in1.ParseFromArray(argv[0], lengths[0]);
  //hello.str()->init(1)	
  //init(hello, 1);
  
  MPCInteger32 in2;
  in2.ParseFromArray(argv[1], lengths[1]);
  //byt.init(2)
*/
  printf("ListMsg is will call...\n");
  int ret = ListMsg(v1, v2);
  //int ret = ListMsg(in1.value(), in2.value());
  printf("ListMsg call done .\n");
  //ret.reveal<std::string>();
  //MPCInteger32 encode_ret;
  //encode_ret.set_value(ret);
  //size_t size = encode_ret.ByteSizeLong();
  //*buf = malloc(size);
  //encode_ret.SerializeToArray(*buf, (unsigned)size);

  char szbuf[16]={0};
  ArrayOutputStream zout(szbuf, 16);
  CodedOutputStream out(&zout);
  out.WriteVarint32SignExtended(ret);

  printf("the ret encode size: %u", out.ByteCount());
  *buf = malloc(out.ByteCount());
  memcpy(*buf, szbuf, out.ByteCount());
  printf("ListMsg_Main call done, ret: %d\n", ret);
  return out.ByteCount();
 // return (unsigned)size;
  //return 0;
}

/*int main() {
  MPCInteger32 alice;
  alice.set_value(1000);
  MPCInteger32 bob;
  bob.set_value(999);
  
  char* inputs[2];
  char in0[1000]={0};
  char in1[1000]={0};
  inputs[0] = in0;
  inputs[1] = in1;

  unsigned lengths[]={1000, 1000};


  alice.SerializeToArray(inputs[0], 1000);
  bob.SerializeToArray(inputs[1], 1000);


  void* ret;
  unsigned retlen = ListMsg_Main(&ret, inputs, lengths);

  MPCInteger32 result;
  result.ParseFromArray(ret, retlen);

  printf("the output is %d \n", result.value());
  free(ret);
  return 0;
}*/

/*int main() {

  lm::helloworld hello;
  hello.set_id(101);
  hello.set_str("hello");

  lm::byeworld bye;
  bye.set_str("bye");


  char* buf[10];
  char buf0[1000], buf1[1000];
  buf[0] = buf0;
  buf[1] = buf1;

  unsigned lengths[]={1000, 1000};


  hello.SerializeToArray(buf[0], 1000);
  bye.SerializeToArray(buf[1], 1000);


  void* ret[1];
  unsigned retlen = ListMsg_Main(ret, buf, lengths);

  lm::byeworld b2;
  b2.ParseFromArray(*ret, retlen);

  printf("%s\n", b2.str().data());
  return 0;




}*/

