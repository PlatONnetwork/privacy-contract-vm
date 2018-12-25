#pragma once

#include <gtest/gtest.h>
#include "emp-tool/execution/plain_prot.h"
#include "emp-tool/io/net_io_channel.h"
#include <iostream>
#include <cmath>

using namespace platon::mpc;
using std::cout;
using std::cerr;
using std::endl;


// To use a test fixture, derive a class from testing::Test.
class PlatonPlainTestFixture : public testing::Test {
protected:  // You should make the members protected s.t. they can be
// accessed from sub-classes.

 // virtual void SetUp() will be called before each test is run.  You
 // should define it if you need to initialize the variables.
 // Otherwise, this can be skipped.
	virtual void SetUp() {
		cout << "setup_plain_prot is called !" << endl;
		setup_plain_prot(false, "");
	}

	// virtual void TearDown() will be called after each test is run.
	// You should define it if there is cleanup work to do.  Otherwise,
	// you don't have to provide it.
	//
	virtual void TearDown() {
		cout << "finalize_plain_prot is called !" << endl;
		finalize_plain_prot();
	}
};


template <typename T>
double test_ot(IOChannel *io, int party, int length) {
  block *b0 = new block[length], *b1 = new block[length],
        *r = new block[length];
  PRG prg(fix_key);
  prg.random_block(b0, length);
  prg.random_block(b1, length);
  bool *b = new bool[length];
  prg.random_bool(b, length);

  io->sync();
  auto start = clock_start();
  T *ot = new T(io);
  if (party == ALICE) {
    ot->send(b0, b1, length);
  } else {
    ot->recv(r, b, length);
  }
  io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    for (int i = 0; i < length; ++i) {
      if (b[i])
        assert(block_cmp(&r[i], &b1[i], 1));
      else
        assert(block_cmp(&r[i], &b0[i], 1));
    }
  }
  std::cout << "Tests passed.\n";
  delete ot;
  delete[] b0;
  delete[] b1;
  delete[] r;
  delete[] b;
  return t;
}

// test BIT ot - only expecting to receive back same *bit* sent
// length is the number of OT's, *not* the length of each message
// (each message is usually one block--but, in this case, each
// is a single bit: the LSB of each block)
template <typename IO, template <typename, int> class T, int N_BITS>
double test_bit_ot(IO *io, int party, int length) {
  if (party == ALICE)
    std::cout << "Testing: " << length << " many " << N_BITS << "-bit OT's\n";
  block *b0 = new block[length], *b1 = new block[length],
        *r = new block[length];
  PRG prg(fix_key);
  prg.random_block(b0, length);
  prg.random_block(b1, length);
  bool *b = new bool[length];
  prg.random_bool(b, length);

  io->sync();
  auto start = clock_start();
  T<IO, N_BITS> *ot = new T<IO, N_BITS>(io);
  if (party == ALICE) {
    ot->send(b0, b1, length);
  } else {
    ot->recv(r, b, length);
  }
  io->flush();
  long long t = time_from(start);
  if (party == BOB) {
    int failures = 0;
    int i = 0;
  nextcmp:
    for (; i < length; ++i) {
      int received[4];
      received[0] = _mm_extract_epi32(r[i], 0);
      received[1] = _mm_extract_epi32(r[i], 1);
      received[2] = _mm_extract_epi32(r[i], 2);
      received[3] = _mm_extract_epi32(r[i], 3);
      int expected[4];
      if (b[i] == 0) { // requested item 0
        expected[0] = _mm_extract_epi32(b0[i], 0);
        expected[1] = _mm_extract_epi32(b0[i], 1);
        expected[2] = _mm_extract_epi32(b0[i], 2);
        expected[3] = _mm_extract_epi32(b0[i], 3);
      } else { // b[i] == 1
        expected[0] = _mm_extract_epi32(b1[i], 0);
        expected[1] = _mm_extract_epi32(b1[i], 1);
        expected[2] = _mm_extract_epi32(b1[i], 2);
        expected[3] = _mm_extract_epi32(b1[i], 3);
      }

      std::cout << std::hex;

      for (int subblk = 0; subblk < std::ceil(N_BITS / 32.0); ++subblk) {
        int n_remaining_bits = N_BITS - (subblk * 32);
        if (n_remaining_bits >= 32) {
          if (received[subblk] != expected[subblk]) {
            std::cout << "Failure: Got quarter-block " << received[subblk]
                      << " on branch " << b[i] << " but expected "
                      << expected[subblk] << std::endl;
            ++failures;
            ++i;
            goto nextcmp;
          }
        } else {
          unsigned mask;
          if (n_remaining_bits == 31)
            mask = -1;
          else
            mask = (1 << n_remaining_bits) - 1;
          if ((received[subblk] & mask) != (expected[subblk] & mask)) {
            std::cout << "Failure: Got quarter-block "
                      << (received[subblk] & mask) << " on branch " << b[i]
                      << " but expected " << (expected[subblk] & mask)
                      << std::endl;
            ++failures;
          }
        }
      }
    }
    if (!failures) {
      std::cout << "Passed " << std::dec << length << " tests.\n";
    } else {
      std::cout << std::dec << "Failed " << failures << " of " << length
                << " tests.\n";
    }
  }
  delete ot;
  delete[] b0;
  delete[] b1;
  delete[] r;
  delete[] b;
  return t;
}

template <typename T>
double test_cot(NetIO *io, int party, int length) {
  block *b0 = new block[length], *r = new block[length];
  bool *b = new bool[length];
  block delta;
  PRG prg(fix_key);
  prg.random_block(&delta, 1);
  prg.random_bool(b, length);

  io->sync();
  auto start = clock_start();
  T *ot = new T(io);
  if (party == ALICE) {
    ot->send_cot(b0, delta, length);
  } else {
    ot->recv_cot(r, b, length);
  }
  io->flush();
  long long t = time_from(start);
  if (party == ALICE)
    io->send_block(b0, length);
  else if (party == BOB) {
    io->recv_block(b0, length);
    for (int i = 0; i < length; ++i) {
      block b1 = xorBlocks(b0[i], delta);
      if (b[i]) {
        if (!block_cmp(&r[i], &b1, 1))
          error("COT failed!");
      } else {
        if (!block_cmp(&r[i], &b0[i], 1))
          error("COT failed!");
      }
    }
  }
  io->flush();
  delete ot;
  delete[] b0;
  delete[] r;
  delete[] b;
  return t;
}

template <typename T>
double test_rot(IOChannel *io, int party, int length) {
  block *b0 = new block[length], *r = new block[length];
  block *b1 = new block[length];
  bool *b = new bool[length];
  PRG prg;
  prg.random_bool(b, length);

  io->sync();
  auto start = clock_start();
  T *ot = new T(io);
  if (party == ALICE) {
    ot->send_rot(b0, b1, length);
  } else {
    ot->recv_rot(r, b, length);
  }
  io->flush();
  long long t = time_from(start);
  if (party == ALICE) {
    io->send_block(b0, length);
    io->send_block(b1, length);
  } else if (party == BOB) {
    io->recv_block(b0, length);
    io->recv_block(b1, length);
    for (int i = 0; i < length; ++i) {
      if (b[i])
        assert(block_cmp(&r[i], &b1[i], 1));
      else
        assert(block_cmp(&r[i], &b0[i], 1));
    }
  }
  io->flush();
  delete ot;
  delete[] b0;
  delete[] b1;
  delete[] r;
  delete[] b;
  return t;
}
