#ifndef OT_NP_USE_MIRACL

#include "emp-tool/utils/hash.h"
#include <fstream>
#include <openssl/sha.h>
#include <stdio.h>
#include "emp-tool/utils/block_utils.h"

#include <string>



#ifdef __cplusplus
extern "C" {
#endif//

#include "relic.h"

#ifdef __cplusplus
}
#endif//


using std::string;

namespace platon
{
	namespace mpc
	{

	Hash::Hash() {
		SHA256_Init(&hash);
	}
	
	Hash::~Hash() {
	}
	void Hash::put(const void * data, int nbyte) {
		if (nbyte > HASH_BUFFER_SIZE)
			SHA256_Update(&hash, data, nbyte);
		else if(size + nbyte < HASH_BUFFER_SIZE) {
			memcpy(buffer+size, data, nbyte);
			size+=nbyte;
		} else {
			SHA256_Update(&hash, (char*)buffer, size);
			memcpy(buffer, data, nbyte);
			size = nbyte;
		}
	}
	void Hash::put_block(const block* block, int nblock/*=1*/){
		put(block, sizeof(block)*nblock);
	}
	void Hash::digest(char * a) {
		if(size > 0) {
			SHA256_Update(&hash, (char*)buffer, size);
			size=0;
		}
		SHA256_Final((unsigned char *)a, &hash);
	}
	void Hash::reset() {
		SHA256_Init(&hash);
		size=0;
	}

	void Hash::put_eb(const void * eb, int length) {
		const eb_t* peb = static_cast<const eb_t*>(eb);
		uint8_t buffer[100];//large enough to hold one.
		for(int i = 0; i < length; ++i) {
			int eb_size = eb_size_bin(peb[i], false);
			eb_write_bin(buffer, eb_size, peb[i], false);
			put(buffer, eb_size);
		}
	}
	
	void Hash::hash_once(void * digest, const void * data, int nbyte) {
		(void )SHA256((const unsigned char *)data, nbyte, (unsigned char *)digest);
	}
	
	block Hash::hash_for_block(const void * data, int nbyte) {
		char digest[DIGEST_SIZE];
		hash_once(digest, data, nbyte);
		return _mm_load_si128((__m128i*)&digest[0]);
	}

	string Hash::hash_once_hex(const void * data, int nbytes)
    {
    	string inputHashHex;
    	unsigned char szHash[2 * DIGEST_SIZE];
    	hash_once(szHash, data, nbytes);
    	char szHex[4];
    	for (int i = 0; i < DIGEST_SIZE; ++i)
    	{
    		snprintf(szHex, 4, "%02X", szHash[i]);
    		inputHashHex.append(szHex, 2);
    	}

    	return inputHashHex;
    }

	unsigned char*Hash::hash_file(unsigned char* output, const char* file)
    {
    	std::ifstream  fin(file, std::fstream::binary);
    	if (!fin.good())
    	{
    		return NULL;
    	}

        SHA256_CTX ctx;
        SHA256_Init(&ctx);
    	
    	char sz32Bytes[DIGEST_SIZE] = {0};
		std::streamsize count = 0;
		std::streamsize read = 0;

		while (!fin.eof()) {
			fin.read(sz32Bytes, 32);
			read = fin.gcount();
			count += read;
			if (!SHA256_Update(&ctx, sz32Bytes, (size_t)read))
			{
				fin.close();
				return NULL;
			}
		}
    	
    	if (!SHA256_Final(output, &ctx))
    	{
    		fin.close();
    		return NULL;
    	}

    	fin.close();
    	return output;
    }

}//emp
}//platon

#endif//OT_NP_USE_MIRACL

