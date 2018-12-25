#ifndef OT_NP_USE_MIRACL
#pragma once

#include <fstream>
#include <openssl/sha.h>
#include <stdio.h>
#include "block.h"
#include "platon_core_conf.h"
#include <string>


using std::string;

namespace platon
{
	namespace mpc
	{


		class Hash
		{
		public:
			SHA256_CTX hash;
			char buffer[HASH_BUFFER_SIZE];
			int size = 0;
			static const int DIGEST_SIZE = SHA256_DIGEST_LENGTH;//sha256: 32

			Hash();
			~Hash();

			void put(const void * data, int nbyte);
			void put_block(const block* block, int nblock = 1);

			void digest(char * a);
			void reset();

			void put_eb(const void * eb, int length);

			static void hash_once(void * digest, const void * data, int nbyte);
			static block hash_for_block(const void * data, int nbyte);

			static string hash_once_hex(const void * data, int nbytes);

			static unsigned char* hash_file(unsigned char* output, const char* file);
		};


	}//emp
}//platon

#endif//OT_NP_USE_MIRACL

