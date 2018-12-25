#include "platon_integer.h"

int ListMsg(int in1, int in2){

  printf("alice: %d, bob: %d\n", in1, in2);

  Integer a(32, in1, ALICE);
  Integer b(32, in2, BOB);

  Integer ret = a - b;
  int plain = ret.reveal_int32();
  printf("to reveal the value, ret: %d...\n", plain);
  return plain;
}