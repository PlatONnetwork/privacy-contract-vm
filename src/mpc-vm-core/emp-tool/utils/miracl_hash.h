#ifdef OT_NP_USE_MIRACL
#pragma once

#include <stdio.h>
#include <string>

#include "block.h"
#include "platon_core_conf.h"

#include "emp-tool/utils/kdf.h"
#include "emp-tool/utils/mir_util.h"

using std::string;

namespace platon
{
	namespace mpc
	{


		class Hash
		{
		public:
			SM3_STATE hash;
			char buffer[HASH_BUFFER_SIZE];
			int size = 0;
			static const int DIGEST_SIZE = SM3_DIGEST_SIZE;//20;


			Hash();

			~Hash();

			void put(const void * data, int nbyte);

			void put_block(const block* block, int nblock = 1);

			void digest(char * a);

			void reset();

			static void hash_once(void * digest, const void * data, int nbyte);
			static string hash_once_hex(const void* data, int nbytes);

			static unsigned char* hash_file(unsigned char* output, const char* file);

			static block hash_for_block(const void * data, int nbyte);

			static block hash_for_epoint(epoint *in);

			void put_epoint(epoint **ep, int length);
		};


	}//emp
}//platon

#endif
