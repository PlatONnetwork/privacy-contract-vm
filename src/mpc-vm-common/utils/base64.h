#pragma once

#include <cstring>

#include <string>  
#include <openssl/evp.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>

#include "mpc_common.h"
NS_PLATON_MPC_BEG

class Base64
{
public:
	static unsigned char* Encode(const unsigned char* in, int ilen, int* olen, bool with_new_line) {
		BIO * bmem = NULL;
		BIO * b64 = NULL;
		BUF_MEM * bptr = NULL;

		b64 = BIO_new(BIO_f_base64());
		if (!with_new_line) {
			BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
		}
		bmem = BIO_new(BIO_s_mem());
		b64 = BIO_push(b64, bmem);
		BIO_write(b64, in, ilen);
		BIO_flush(b64);
		BIO_get_mem_ptr(b64, &bptr);

		*olen = bptr->length;
		unsigned char * buff = (unsigned char *)malloc(bptr->length + 1);
		memcpy(buff, bptr->data, bptr->length);
		buff[bptr->length] = 0;

		BIO_free_all(b64);

		return buff;
	}

	static unsigned char* Decode(const unsigned char* in, int ilen, int* olen, bool with_new_line) {
		BIO * b64 = NULL;
		BIO * bmem = NULL;
		unsigned char * buffer = (unsigned char *)malloc(ilen);
		memset(buffer, 0, ilen);

		b64 = BIO_new(BIO_f_base64());
		if (!with_new_line) {
			BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
		}
		bmem = BIO_new_mem_buf(in, ilen);
		bmem = BIO_push(b64, bmem);
		*olen = BIO_read(bmem, buffer, ilen);

		BIO_free_all(bmem);

		return buffer;
	}

public:
	// for char*
	static int Test(const unsigned char* in) {

		return 0;
	}
};

NS_PLATON_MPC_END
