#pragma once

#include <string>
#include "bit.h"

using std::string;


namespace platon
{
	namespace mpc
	{

		void integer_bool_data(bool* data, size_t len, std::string str);

		void integer_bool_data(bool* data, size_t len, long long num);

		void integer_add_full(Bit* dest, Bit * carryOut, const Bit * op1, const Bit * op2, const Bit * carryIn, int size);

		void integer_sub_full(Bit * dest, Bit * borrowOut, const Bit * op1, const Bit * op2,
			const Bit * borrowIn, int size);

		void integer_mul_full(Bit * dest, const Bit * op1, const Bit * op2, int size);

		void integer_ifThenElse(Bit * dest, const Bit * tsrc, const Bit * fsrc,
			int size, Bit cond);

		void integer_condNeg(Bit cond, Bit * dest, const Bit * src, int size);

		void integer_div_full(Bit * vquot, Bit * vrem, const Bit * op1, const Bit * op2,
			int size);

		string integer_bin2dec(const string& bin2);
	}
}