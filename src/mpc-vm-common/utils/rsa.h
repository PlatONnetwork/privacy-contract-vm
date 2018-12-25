#pragma once

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <iostream>  
#include <cassert>
#include <string>  
#include <vector>  
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/sha.h>


static const unsigned char* test_prikey_512 = (const unsigned char*)"-----BEGIN RSA PRIVATE KEY-----\n"
"MIIBOwIBAAJBAM8Ti4lL9YgXZdOgZIi3isvDCVIt7v7YR7PrijgjdeJ3W49dZfkh\n"
"foKNGB/oq7O2dLTZQc5M1035atx7UW10JlECAQMCQQCKDQew3U5aukPiau2welyH\n"
"11uMHp9UkC/NR7F6wk6W+QoEpIONlJb22Ze3X1F7sXyHGach4qK6jp6r9/gqE71L\n"
"AiEA6ZgXaw+tsUy+hMag71+44eiuKWEqU02XgDZxiXWoxA0CIQDi8E81lRTqw4gv\n"
"xjjCGnNYAYSduk6P6Iv8pBXTuK3GVQIhAJu6ukdfySDd1FiEa0o/0JabHsZAxuIz\n"
"ulV5oQZOcIKzAiEAl0rfeQ4N8dewH9l7LBGiOqutvnw0X/BdUxgOjSXJLuMCIQCu\n"
"4DkHkFdiG8MNYEwHyFLetBu6Y1VQCY3Jd80Keaud+w==\n"
"-----END RSA PRIVATE KEY-----\n";
static const unsigned char* test_pubkey_512 = (const unsigned char*)"-----BEGIN RSA PUBLIC KEY-----\n"
"MEYCQQDPE4uJS/WIF2XToGSIt4rLwwlSLe7+2Eez64o4I3Xid1uPXWX5IX6CjRgf\n"
"6KuztnS02UHOTNdN+Wrce1FtdCZRAgED\n"
"-----END RSA PUBLIC KEY-----\n";

static const unsigned char* test_prikey_1024 = (const unsigned char*)"-----BEGIN RSA PRIVATE KEY-----\n"
"MIICXAIBAAKBgQDAHxFmWTfrIzRK8G53x7c5Nvk1R7lPKPjUR2WRuqfrjvQ0hsE3\n"
"7P1geaR5j5QcMNSZZ6NCBvdij+iKYQH8+yBIVsYBWLO4DSXfKEyWqbongb3u0KR9\n"
"HEcGq2pt32qHpgPKvCKGvdSzvv9G+Uw3qYmsFO81E3pqKCofPDLve3KnxQIBAwKB\n"
"gQCAFLZEO3qcwiLcoEmlL897eft42nuKG1CNhO5hJxqdCfgjBIDP81OVpm2mX7gS\n"
"yzhmRReBWfpBtUWxlgFTUhWEZzDqcDSAyT2rc6SB2aUAH9dytP6Dfe9qT9BgNF+8\n"
"okel7zBP7O75jWpHL6wwo/HYeK5foPHX6glFSFEgjOZQawJBAOfkuq8D+XIBdO3d\n"
"FDveyU+FpaVffcoLRgh2BVcPb1EBE/KICoZMC/+CMgS3sImrt5rN/8a2B0O2qDsc\n"
"267yMK0CQQDUF+cBYP1tR+kM+L+nY94CdR0bxzoVVKErO9g5y31hlz3i0gRUIjFr\n"
"XaotEj4p8y/FG6XbWZ8FZY8UnGL5Jv55AkEAmph8dK1Q9qujST4NfT8w365ubj+p\n"
"MVzZWvlY5LT04KtiobAHBDKyqlbMAyUgW8fPvIlVLyQE188a0hM9H0wgcwJBAI1l\n"
"RKuV/kjam137Km+X6Vb4vhKE0WONwMd9OtEyU5Zk0+yMAuLBdkeTxsi21Bv3dS4S\n"
"bpI7v1juX2MS7KYZ/vsCQDagDp9R8DsPxPGEzF4H+VrLfSWYlZI7eoGzZkVCxIio\n"
"NZnNiu2FuM5yaDkyugVO0g0q2lehyBuZBQNmkHCJ5c4=\n"
"-----END RSA PRIVATE KEY-----\n";
static const unsigned char* test_pubkey_1024 = (const unsigned char*)"-----BEGIN RSA PUBLIC KEY-----\n"
"MIGHAoGBAMAfEWZZN+sjNErwbnfHtzk2+TVHuU8o+NRHZZG6p+uO9DSGwTfs/WB5\n"
"pHmPlBww1Jlno0IG92KP6IphAfz7IEhWxgFYs7gNJd8oTJapuieBve7QpH0cRwar\n"
"am3faoemA8q8Ioa91LO+/0b5TDepiawU7zUTemooKh88Mu97cqfFAgED\n"
"-----END RSA PUBLIC KEY-----\n";

static const unsigned char* test_prikey_2048 = (const unsigned char*)"-----BEGIN RSA PRIVATE KEY-----\n"
"MIIEogIBAAKCAQEAuSzHDM7IXenUQ98TrNNyKKVulxwh/FWHpFMTiOVVVjzZwZEV\n"
"42MZH8DQg5CmIbEioSR7Ajx7kzpaefKCJCuqbxAb7gKYpD9MK9B444UDqlWxuRPY\n"
"kitBmRDKMD4RN5IdK1dMttwW7joQnl/DswiQlfvHYrmAEDEXh0k6klxO6SUDjFBC\n"
"/e2IQFTzPSTg6P9IiqegrIikHVQuwSaBoyWNpPcvF7DlSzrO+AtgqIpq30DyOrgz\n"
"MGCDqEAXx51GF3mJ+T2XYTc3tJFzwSCKr/cu2JrM/pRuaqLvEzGDanrVmtCnofGz\n"
"qDa6wL2Nk26NPoLNdl6p1QYaoxUQOaNA2loCnQIBAwKCAQB7cy9d3zA+m+LX6g0d\n"
"4kwbGPRkvWv9jlptjLewmOOO0zvWYLlCQhC/1eBXtcQWdhcWGFIBfae3fDxRTFbC\n"
"x8b0tWf0AbsYKjLH4FCXrgJxjnZ7YpBhciu7YIbK1At6YWjHj4h56A9JfAsUP9fM\n"
"sGBj/S+XJlVgILpaMNG26DSbbN9b7QWF3mkwFRXwAA466qteHgMUdnL2EtyowwER\n"
"xqf85qgyaQvhZnBUEyVR1OtCvfcRl/Lhzl8uaaaC/bDg9vu7+emC0k1Xj/WtO6q0\n"
"tzayM42VA9h0U0CLlpzWHq9aaW5J2pSzrqXaTboUbsj20ZcGfstKA61zoINd9AjQ\n"
"vs9rAoGBAOlunZxO6zBHEb66ZKpadHht9GBWSNO02iCw3wbJ/nQ0RztiF6KYmwZt\n"
"CY/uIm4Rvy6IAM4kIEbbiiJEgQeJxnI3jZ4mj1ISCOiHKlH+v9x9xd8P6S0tdXOq\n"
"wzg4w9MI624UWNukMWib4GtNg3Rw6eE8tWjaPK3qfNpy+K3DJFXVAoGBAMsTzx5m\n"
"NLoxI5OawCE2Ks8Phju3jiP3XcMTIvk+fR11Vfdp+7DghiAM5Ri/qJvpZNEYU3i5\n"
"iq4HF3u+n7U8QJ4nuRr2bHGfMJq2HLiiJIBPh5mPJXyOsJqXDF/wiPuKRxQe0Pj2\n"
"f1ldaIsnROtbXomv6xALw+iLJ3C5vIXeF3WpAoGBAJufE72J8iAvYSnRmHGRovrz\n"
"+EA5heJ4kWsglK8xVE14L3zsD8G7EgRIsQqewZ62f3RaqzQYFYSSXBbYVgUGhEwl\n"
"CRQZtOFhW0WvcYv/KpL+g+oKm3Nzo6Jx13rQgoywnPQNkJJtdkW9QEeJAk2gm+t9\n"
"zkXm0x6cUzxMpckswuPjAoGBAIdiihREIybLbQ0R1Wt5cd9frtJ6XsKk6Sy3bKYp\n"
"qL5OOU+b/SCVrsAImLsqcGfw7eC64lB7sclaD6fUanjS1b7FJhH5naEUyxHOvdBs\n"
"GFWKWmZfblMJyxG6CD/1sKexhLgUi1CkVOY+RbIaLfI86bEf8gqygpsHb6B70wPp\n"
"ZPkbAoGAWup2AcZvL5+hvkm8rd/bk1OL6ioS5AMau7gKpsGBnN1Rgp3NbUc17+k8\n"
"S/N2jz1g8OW9KQ/fXQez20FILeDpV5k5QKYO1bCI4DZ4J0UjdzxfR5+0MbFghDt0\n"
"nRa6Q6HAYSdL7gVGIdFUZNp/IhcFEL/QATjhFkWmzhGrxCYl7+M=\n"
"-----END RSA PRIVATE KEY-----\n";
static const unsigned char* test_pubkey_2048 = (const unsigned char*)"-----BEGIN RSA PUBLIC KEY-----\n"
"MIIBCAKCAQEAuSzHDM7IXenUQ98TrNNyKKVulxwh/FWHpFMTiOVVVjzZwZEV42MZ\n"
"H8DQg5CmIbEioSR7Ajx7kzpaefKCJCuqbxAb7gKYpD9MK9B444UDqlWxuRPYkitB\n"
"mRDKMD4RN5IdK1dMttwW7joQnl/DswiQlfvHYrmAEDEXh0k6klxO6SUDjFBC/e2I\n"
"QFTzPSTg6P9IiqegrIikHVQuwSaBoyWNpPcvF7DlSzrO+AtgqIpq30DyOrgzMGCD\n"
"qEAXx51GF3mJ+T2XYTc3tJFzwSCKr/cu2JrM/pRuaqLvEzGDanrVmtCnofGzqDa6\n"
"wL2Nk26NPoLNdl6p1QYaoxUQOaNA2loCnQIBAw==\n"
"-----END RSA PUBLIC KEY-----\n";

#include "mpc_common.h"
NS_PLATON_MPC_BEG

/*
openssl genrsa -out prikey.pem 1024
openssl rsa -in privkey.pem -pubout -out pubkey.pem

ÃüÁî PUBLIC KEY      PEM_read_RSA_PUBKEY
º¯Êý RSA PUBLIC KEY  PEM_read_RSAPublicKEY

padding
RSA_PKCS1_PADDING	RSA_size-11
RSA_NO_PADDING		RSA_size-0
RSA_X931_PADDING	RSA_size-2
*/

class RSA
{
public:
	static int GenRSAKey(std::string strKey[2], int keylen = 1024)
	{
		size_t pri_len;
		size_t pub_len;
		char *pri_key = NULL;
		char *pub_key = NULL;

		::RSA *keypair = RSA_generate_key(keylen, RSA_3, NULL, NULL);

		BIO *pri = BIO_new(BIO_s_mem());
		BIO *pub = BIO_new(BIO_s_mem());

		PEM_write_bio_RSAPrivateKey(pri, keypair, NULL, NULL, 0, NULL, NULL);
		PEM_write_bio_RSAPublicKey(pub, keypair);

		pri_len = BIO_pending(pri);
		pub_len = BIO_pending(pub);

		pri_key = (char *)malloc(pri_len + 1);
		pub_key = (char *)malloc(pub_len + 1);

		BIO_read(pri, pri_key, pri_len);
		BIO_read(pub, pub_key, pub_len);

		pri_key[pri_len] = '\0';
		pub_key[pub_len] = '\0';

		strKey[0] = pub_key;
		strKey[1] = pri_key;

		std::string pub_pem("pubkey_" + std::to_string(keylen) + ".pem");
		FILE *pubFile = fopen(pub_pem.c_str(), "w");
		if (pubFile == NULL)
		{
			assert(false);
			return -1;
		}
		fputs(pub_key, pubFile);
		fclose(pubFile);

		std::string pri_pem("prikey_" + std::to_string(keylen) + ".pem");
		FILE *priFile = fopen(pri_pem.c_str(), "w");
		if (priFile == NULL)
		{
			assert(false);
			return -1;
		}
		fputs(pri_key, priFile);
		fclose(priFile);

		RSA_free(keypair);
		BIO_free_all(pub);
		BIO_free_all(pri);

		free(pri_key);
		free(pub_key);

		return 0;
	}

	static int PublicEncrypt(const char* hex_n, const char* hex_e, const unsigned char* in, int ilen, unsigned char* out, int* olen, int padding = RSA_PKCS1_PADDING) {
		::RSA* rsa = getPubKEY(hex_n, hex_e);

		int ret = PublicEncrypt(rsa, in, ilen, out, olen, padding);

		RSA_free(rsa);
		return ret;
	}

	static int PublicEncrypt(const unsigned char* pubkey, const unsigned char* in, int ilen, unsigned char* out, int* olen, int padding = RSA_PKCS1_PADDING) {
		::RSA* rsa = NULL;
		BIO* bio = BIO_new_mem_buf(pubkey, -1);
		rsa = PEM_read_bio_RSAPublicKey(bio, &rsa, NULL, NULL);

		int ret = PublicEncrypt(rsa, in, ilen, out, olen, padding);

		BIO_free_all(bio);
		RSA_free(rsa);
		return ret;
	}

	static int PublicEncrypt(::RSA* rsa, const unsigned char* in, int ilen, unsigned char* out, int* olen, int padding = RSA_PKCS1_PADDING) {
		int rsa_len = RSA_size(rsa);
		int block_size = -1;
		if (padding == RSA_PKCS1_PADDING) { block_size = rsa_len - 11; }
		else if (padding == RSA_NO_PADDING) { block_size = rsa_len - 0; }
		else if (padding == RSA_X931_PADDING) { block_size = rsa_len - 2; }
		if (block_size == -1) { return -1; }
		// 1024bit RSA_PKCS1_PADDING rsa_len:128 block_size:117

		*olen = 0;
		int offset = 0;
		int j = 0;
		for (int i = 0; offset < ilen; i++, j++, offset = i * block_size) {
			if (ilen - offset > block_size) {
				if (RSA_public_encrypt(block_size, in + offset, out + j * rsa_len, rsa, padding) < 0) {
					return -1;
				}
			}
			else {
				if (RSA_public_encrypt(ilen - offset, in + offset, out + j * rsa_len, rsa, padding) < 0) {
					return -1;
				}

			}
			*olen += rsa_len;
		}

		return 0;
	}

	static int PrivateDecrypt(const unsigned char* prikey, const unsigned char* in, int ilen, unsigned char* out, int* olen, int padding = RSA_PKCS1_PADDING) {
		::RSA* rsa = NULL;
		BIO* bio = BIO_new_mem_buf(prikey, -1);
		rsa = PEM_read_bio_RSAPrivateKey(bio, &rsa, NULL, NULL);

		int ret = PrivateDecrypt(rsa, in, ilen, out, olen, padding);

		BIO_free_all(bio);
		RSA_free(rsa);
		return ret;
	}

	static int PrivateDecrypt(::RSA* rsa, const unsigned char* in, int ilen, unsigned char* out, int* olen, int padding = RSA_PKCS1_PADDING) {
		int rsa_len = RSA_size(rsa);
		int block_size = -1;
		if (padding == RSA_PKCS1_PADDING) { block_size = rsa_len - 11; }
		else if (padding == RSA_NO_PADDING) { block_size = rsa_len - 0; }
		else if (padding == RSA_X931_PADDING) { block_size = rsa_len - 2; }
		if (block_size == -1) { return -1; }
		// 1024bit RSA_PKCS1_PADDING rsa_len:128 block_size:117

		*olen = 0;
		int retlen = 0;
		int j = 0;
		for (int i = 0; i < ilen; j++, i += rsa_len) {
			if ((retlen = RSA_private_decrypt(rsa_len, in + i, out + j * block_size, rsa, padding)) < 0) {
				return -1;
			}
			*olen += retlen;
		}

		return 0;
	}
	static int PrivateEncrypt(const unsigned char* in, int ilen, unsigned char* out, int* olen);
	static int PublicDecrypt(const unsigned char* in, int ilen, unsigned char* out, int* olen);


	static int Sign();
	static int Verify();

	static int GetPublicHexFromPEM(const unsigned char* pubkey, std::string& hex_n, std::string& hex_e) {
		::RSA* rsa = NULL;
		BIO* bio = BIO_new_mem_buf(pubkey, -1);
		rsa = PEM_read_bio_RSAPublicKey(bio, &rsa, NULL, NULL);

#if OPENSSL_VERSION_NUMBER < 0x1010009fL
		hex_e = BN_bn2hex(rsa->e);
		hex_n = BN_bn2hex(rsa->n);
#else
		//fix to this:   reference: https://github.com/openssl/openssl/issues/1604
		const BIGNUM *rsa_e, *rsa_n, *rsa_d;
		RSA_get0_key(rsa, &rsa_n, &rsa_e, &rsa_d);
		hex_e = BN_bn2hex(rsa_e);
		hex_n = BN_bn2hex(rsa_n);
#endif

		BIO_free_all(bio);
		RSA_free(rsa);

		return 0;
	}

	static int GetPrivateHexFromPEM(const unsigned char* prikey, std::string& hex_n, std::string& hex_d) {
		::RSA* rsa = NULL;
		BIO* bio = BIO_new_mem_buf(prikey, -1);
		rsa = PEM_read_bio_RSAPrivateKey(bio, &rsa, NULL, NULL);

#if OPENSSL_VERSION_NUMBER < 0x1010009fL
		hex_d = BN_bn2hex(rsa->d);
		hex_n = BN_bn2hex(rsa->n);
#else
		//fix to this
		const BIGNUM *rsa_e, *rsa_n, *rsa_d;
		RSA_get0_key(rsa, &rsa_n, &rsa_e, &rsa_d);
		hex_d = BN_bn2hex(rsa_d);
		hex_n = BN_bn2hex(rsa_n);
#endif

		BIO_free_all(bio);
		RSA_free(rsa);

		return 0;
	}

	static ::RSA* getPubKEY(const char* hex_n, const char* hex_e = "10001") {
		BIGNUM* n = BN_new();
		BN_hex2bn(&n, hex_n);

		BIGNUM* e = BN_new();
		BN_hex2bn(&e, hex_e);

		EVP_PKEY* pRsaKey = EVP_PKEY_new();
		::RSA* rsa = RSA_new();
#if OPENSSL_VERSION_NUMBER < 0x1010009fL
		rsa->n = n;
		rsa->e = e;
#else
		RSA_set0_key(rsa, n, e, NULL);
#endif

		return rsa;
	}

public:
	// for char*
	static int Test(const unsigned char* in, int bits = 1024, int padding = RSA_PKCS1_PADDING) {
		{
			///	std::string skeys[2];
			///	GenRSAKey(skeys, 512);
			///	GenRSAKey(skeys, 1024);
			///	GenRSAKey(skeys, 2048);
		}
		{
			int ilen = (int)strlen((const char*)in);
			if (ilen <= 0) return 0;

			std::string prikey, pubkey;
			if (bits == 512) { prikey = std::string((char*)test_prikey_512); pubkey = std::string((char*)test_pubkey_512); }
			else if (bits == 1024) { prikey = std::string((char*)test_prikey_1024); pubkey = std::string((char*)test_pubkey_1024); }
			else if (bits == 2048) { prikey = std::string((char*)test_prikey_2048); pubkey = std::string((char*)test_pubkey_2048); }
			else { return -1; }

			int rsa_len = bits / 8;
			int block_size = -1;
			if (padding == RSA_PKCS1_PADDING) { block_size = rsa_len - 11; }
			else { return -1; } // note, here 3 keys is pkcs1

			int olen = ((ilen + block_size - 1) / block_size) * rsa_len;

			unsigned char* out = new unsigned char[olen];// olen >= ilen > 0
			memset(out, 0, olen);
			int ret = PublicEncrypt((const unsigned char*)pubkey.c_str(), in, ilen, out, &olen, padding);
			if (ret != 0) { delete[] out; return ret; }
			{
				unsigned char* out2 = new unsigned char[olen];
				memset(out2, 0, olen);
				int olen2 = 0;
				int ret = PrivateDecrypt((const unsigned char*)prikey.c_str(), out, olen, out2, &olen2, padding);
				delete[] out;
				if (ret != 0) { delete[] out2; return ret; }
				ret = strcmp((const char*)in, (const char*)out2);
				delete[] out2;
				return ret;
			}
		}
		return 0;
	}

};

NS_PLATON_MPC_END
