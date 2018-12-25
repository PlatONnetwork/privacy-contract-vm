#pragma once


#include <sstream>
#include <cstddef>//https://gcc.gnu.org/gcc-4.9/porting_to.html
#include <chrono>
#include <string>
//#include "block.h"
#include "emp-tool/utils/prg.h"
//#include <gmp>
#include <iomanip>
#include <bitset>


#define macro_xstr(a) macro_str(a)
#define macro_str(a) #a

#define PUBLIC 0
#define ALICE 1
#define BOB 2



using std::string;
using std::chrono::time_point;
using std::chrono::high_resolution_clock;
using std::stringstream;
using std::bitset;



namespace platon
{
	namespace mpc
	{


		template<typename T>
		void delete_array_null(T * ptr)
		{
			if (ptr != nullptr) {
				delete[] ptr;
				ptr = nullptr;
			}
		}

		void inline error(const char * s, int line = 0, const char * file = nullptr)
		{
			fprintf(stderr, s, "\n");
			if (file != nullptr) {
				fprintf(stderr, "at %d, %s\n", line, file);
			}
			exit(1);
		}

		template<class... Ts>
		void run_function(void *function, const Ts&... args)
		{
			reinterpret_cast<void(*)(Ts...)>(function)(args...);
		}

		inline void parse_party_and_port(char ** arg, int argc, int * party, int * port)
		{
			if (argc == 1)
				error("ERROR: argc = 1, need two argsm party ID {1,2} and port.");

			*party = atoi(arg[1]);
			*port = atoi(arg[2]);
		}

		//deprecate soon
		inline void parse_party_and_port(char ** arg, int * party, int * port)
		{
			parse_party_and_port(arg, 2, party, port);
		}


		inline std::string PartyRole(int p)
		{
			if (p == ALICE)
				return "ALICE";
			else if (p == BOB)
				return "BOB";
			else return "PUBLIC";
		}


		template <typename T = uint64_t>
		std::string m128i_to_string(const block var)
		{
			std::stringstream sstr;
			const T* values = (const T*)&var;
			for (unsigned int i = 0; i < sizeof(block) / sizeof(T); i++)
			{
				sstr << "0x" << std::hex << values[i] << " ";
			}
			return sstr.str();
		}

		//inline double wallClock() 
		//{
		//	struct timespec t;
		//	clock_gettime(CLOCK_REALTIME,&t);
		//	return t.tv_sec+1e-9*t.tv_nsec;
		//}


		//inline uint64_t timeStamp() 
		//{
		//	struct timespec t;
		//	clock_gettime(CLOCK_REALTIME,&t);
		//	return (t.tv_sec*1000*1000+t.tv_nsec/1000);
		//}


		inline std::chrono::time_point<std::chrono::high_resolution_clock> clock_start()
		{
			return std::chrono::high_resolution_clock::now();
		}

		inline long long time_from(const std::chrono::time_point<std::chrono::high_resolution_clock>& s)
		{
			return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now() - s).count();
		}


		template<typename t>
		t bool_to_int(const bool * data, size_t len /*=0*/)
		{
			if (len != 0) len = (len > sizeof(t) * 8 ? sizeof(t) * 8 : len);
			else len = sizeof(t) * 8;
			t res = 0;
			for (size_t i = 0; i < len - 1; ++i) {
				if (data[i])
					res |= (1LL << i);
			}
			if (data[len - 1]) return -1 * res;
			else return res;
		}

		uint64_t bool_to64(const bool * data);

		block bool_to128(const bool * data);

		void int64_to_bool(bool * data, uint64_t input, int length);


		//static const char* hex_char_to_bin(char c);

		std::string hex_to_binary(std::string hex);

		//static string intToStr(int a, int base);
		//
		//static int strToInt(string s, int base);

		std::string trans_base(std::string str, int old_base, int new_base);

		std::string dec_to_bin(const std::string& dec);

		std::string bin_to_dec(const std::string& bin2);

		// convert number of decimal format to binary format
		std::string decimal_2_binary(const std::string& dec);

		void ulong_2_bits(bool *res, uint64_t in, int length);

		uint64_t bits_2_ulong(const bool *data);

		block bits_2_block(const bool *data);

	}

}