#include <algorithm>
#include <math.h>
#include <assert.h>
#include <cstdint>

//#include "emp-tool/circuits/number.h"
//#include "circuit_execution.h"
//#include "protocol_execution.h"
#include "integer_utils.h"
#include "bit.h"
#include "emp-tool/utils/utils.h"


using std::min;

namespace platon
{
	namespace mpc
	{
		void integer_bool_data(bool* data, size_t len, std::string str) {
			std::string bin = dec_to_bin(str);
			std::reverse(bin.begin(), bin.end());
			//		cout << "convert " <<str<<" "<<bin<<endl;
			int l = (bin.size() > (size_t)len ? len : bin.size());
			for (int i = 0; i < l; ++i)
				data[i] = (bin[i] == '1');
			for (size_t i = l; i < len; ++i)
				data[i] = data[l - 1];
		}

		void integer_bool_data(bool* data, size_t len, long long num) {
			integer_bool_data(data, len, std::to_string(num));
		}


		//https://github.com/samee/obliv-c/blob/obliv-c/src/ext/oblivc/obliv_bits.c#L1487
		void integer_add_full(Bit* dest, Bit * carryOut, const Bit * op1, const Bit * op2,
				const Bit * carryIn, int size) {
			Bit carry, bxc, axc, t;
			int skipLast;
			int i = 0;
			if (size == 0) {
				if (carryIn && carryOut)
					*carryOut = *carryIn;
				return;
			}
			if (carryIn)
				carry = *carryIn;
			else
				carry = false;
			// skip AND on last bit if carryOut==NULL
			skipLast = (carryOut == nullptr);
			while (size-- > skipLast) {
				axc = op1[i] ^ carry;
				bxc = op2[i] ^ carry;
				dest[i] = op1[i] ^ bxc;
				t = axc & bxc;
				carry = carry ^ t;
				++i;
			}
			if (carryOut != nullptr)
				*carryOut = carry;
			else
				dest[i] = carry ^ op2[i] ^ op1[i];
		}

		void integer_sub_full(Bit * dest, Bit * borrowOut, const Bit * op1, const Bit * op2,
				const Bit * borrowIn, int size) {
			Bit borrow, bxc, bxa, t;
			int skipLast; int i = 0;
			if (size == 0) {
				if (borrowIn && borrowOut)
					*borrowOut = *borrowIn;
				return;
			}
			if (borrowIn)
				borrow = *borrowIn;
			else
				borrow = false;
			// skip AND on last bit if borrowOut==NULL
			skipLast = (borrowOut == nullptr);
			while (size-- > skipLast) {
				bxa = op1[i] ^ op2[i];
				bxc = borrow ^ op2[i];
				dest[i] = bxa ^ borrow;
				t = bxa & bxc;
				borrow = borrow ^ t;
				++i;
			}
			if (borrowOut != nullptr) {
				*borrowOut = borrow;
			}
			else
				dest[i] = op1[i] ^ op2[i] ^ borrow;
		}

		void integer_mul_full(Bit * dest, const Bit * op1, const Bit * op2, int size) {
			//	OblivBit temp[MAX_BITS]={},sum[MAX_BITS]={};
			Bit * sum = new Bit[size];
			Bit * temp = new Bit[size];
			for (int i = 0; i < size; ++i)sum[i] = false;
			for (int i = 0; i < size; ++i) {
				for (int k = 0; k < size - i; ++k)
					temp[k] = op1[k] & op2[i];
				integer_add_full(sum + i, nullptr, sum + i, temp, nullptr, size - i);
			}
			memcpy(dest, sum, sizeof(Bit)*size);
			delete[] sum;
			delete[] temp;
		}

		void integer_ifThenElse(Bit * dest, const Bit * tsrc, const Bit * fsrc,
				int size, Bit cond) {
			Bit x, a;
			int i = 0;
			while (size-- > 0) {
				x = tsrc[i] ^ fsrc[i];
				a = cond & x;
				dest[i] = a ^ fsrc[i];
				++i;
			}
		}

		void integer_condNeg(Bit cond, Bit * dest, const Bit * src, int size) {
			int i;
			Bit c = cond;
			for (i = 0; i < size - 1; ++i) {
				dest[i] = src[i] ^ cond;
				Bit t = dest[i] ^ c;
				c = c & dest[i];
				dest[i] = t;
			}
			dest[i] = cond ^ c ^ src[i];
		}

		void integer_div_full(Bit * vquot, Bit * vrem, const Bit * op1, const Bit * op2,
			int size) {
			Bit * overflow = new Bit[size];
			Bit * temp = new Bit[size];
			Bit * rem = new Bit[size];
			Bit * quot = new Bit[size];
			Bit b;
			memcpy(rem, op1, size * sizeof(Bit));
			overflow[0] = false;
			for (int i = 1; i < size; ++i)
				overflow[i] = overflow[i - 1] | op2[size - i];
			// skip AND on last bit if borrowOut==NULL
			for (int i = size - 1; i >= 0; --i) {
				integer_sub_full(temp, &b, rem + i, op2, nullptr, size - i);
				b = b | overflow[i];
				integer_ifThenElse(rem + i, rem + i, temp, size - i, b);
				quot[i] = !b;
			}
			if (vrem != nullptr) memcpy(vrem, rem, size * sizeof(Bit));
			if (vquot != nullptr) memcpy(vquot, quot, size * sizeof(Bit));
			delete[] overflow;
			delete[] temp;
			delete[] rem;
			delete[] quot;
		}


		/*void integer_init(Bit * bits, const bool* b, int length, int party) {
			block * bbits = (block *)bits;
			if (party == PUBLIC) {
				block one = CircuitExecutionProxy::circ_exec.public_label(true);
				block zero = CircuitExecutionProxy::circ_exec.public_label(false);
				for (int i = 0; i < length; ++i)
					bbits[i] = b[i] ? one : zero;
			}
			else {
				ProtocolExecutionProxy::prot_exec.feed((block *)bits, party, b, length);
			}
		}*/

		string integer_bin2dec(const string& bin2)
		{
			return bin_to_dec(bin2);
		}

	}
}