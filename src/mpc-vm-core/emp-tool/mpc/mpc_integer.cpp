#include "emp-tool/mpc/mpc_integer.h"
#include "emp-tool/utils/utils.h"
#include <algorithm>
#include <cassert>
#include <utility>

using namespace emp;

namespace platon
{
	void MPCInteger::bool_data(bool* data, size_t len, string str) {
		string bin = dec_to_bin(str);
		std::reverse(bin.begin(), bin.end());
		//		cout << "convert " <<str<<" "<<bin<<endl;
		int l = (bin.size() > (size_t)len ? len : bin.size());
		for (int i = 0; i < l; ++i)
			data[i] = (bin[i] == '1');
		for (size_t i = l; i < len; ++i)
			data[i] = data[l - 1];
	}

	void static init_integer(Bit * bits, const bool* b, int length, int party) {
		block * bbits = (block *)bits;
		if (party == PUBLIC) {
			block one = CircuitExecution::circ_exec->public_label(true);
			block zero = CircuitExecution::circ_exec->public_label(false);
			for (int i = 0; i < length; ++i)
				bbits[i] = b[i] ? one : zero;
		}
		else {
			ProtocolExecution::prot_exec->feed((block *)bits, party, b, length);
		}
	}

	MPCInteger::MPCInteger(int32_t v, MPCRoleType role)
	{
	}

	MPCInteger::~MPCInteger()
	{
		if (bits_ != nullptr) delete[] bits_;
	}

	MPCInteger::MPCInteger(MPCInteger&& in) : length_(in.length_) {
		bits_ = in.bits_;
		in.bits_ = nullptr;
	}
	MPCInteger::MPCInteger(const MPCInteger& in) : length_(in.length_) {
		bits_ = new Bit[length_];
		memcpy(bits_, in.bits_, sizeof(Bit)*length_);
	}
	
	MPCInteger& MPCInteger::operator= (MPCInteger rhs) {
		length_ = rhs.length_;
		std::swap(bits_, rhs.bits_);//kelvin note: should copy bits !
		return *this;
	}
	
	MPCInteger::MPCInteger(int len, const void * b) : length_(len) {
		bits_ = new Bit[len];
		memcpy(bits_, b, sizeof(Bit)*len);
	}

	//https://github.com/samee/obliv-c/blob/obliv-c/src/ext/oblivc/obliv_bits.c#L1487

	static void add_full(Bit* dest, Bit * carryOut, const Bit * op1, const Bit * op2, const Bit * carryIn, int size) 
	{
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

	static void sub_full(Bit * dest, Bit * borrowOut, const Bit * op1, const Bit * op2,
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

	static void mul_full(Bit * dest, const Bit * op1, const Bit * op2, int size) {
		//	OblivBit temp[MAX_BITS]={},sum[MAX_BITS]={};
		Bit * sum = new Bit[size];
		Bit * temp = new Bit[size];
		for (int i = 0; i < size; ++i)sum[i] = false;
		for (int i = 0; i < size; ++i) {
			for (int k = 0; k < size - i; ++k)
				temp[k] = op1[k] & op2[i];
			add_full(sum + i, nullptr, sum + i, temp, nullptr, size - i);
		}
		memcpy(dest, sum, sizeof(Bit)*size);
		delete[] sum;
		delete[] temp;
	}


	static void ifThenElse(Bit * dest, const Bit * tsrc, const Bit * fsrc,
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

	static void condNeg(Bit cond, Bit * dest, const Bit * src, int size) {
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


	static void div_full(Bit * vquot, Bit * vrem, const Bit * op1, const Bit * op2, int size) 
	{
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
			sub_full(temp, &b, rem + i, op2, nullptr, size - i);
			b = b | overflow[i];
			ifThenElse(rem + i, rem + i, temp, size - i, b);
			quot[i] = !b;
		}
		if (vrem != nullptr) 
			memcpy(vrem, rem, size * sizeof(Bit));
		if (vquot != nullptr) 
			memcpy(vquot, quot, size * sizeof(Bit));

		delete[] overflow;
		delete[] temp;
		delete[] rem;
		delete[] quot;
	}

	void bool_data(bool* data, size_t len, string str) {
		string bin = dec_to_bin(str);
		std::reverse(bin.begin(), bin.end());
		//		cout << "convert " <<str<<" "<<bin<<endl;
		int l = (bin.size() > (size_t)len ? len : bin.size());
		for (int i = 0; i < l; ++i)
			data[i] = (bin[i] == '1');
		for (size_t i = l; i < len; ++i)
			data[i] = data[l - 1];
	}

	

	MPCInteger::MPCInteger(int len, const string& str, int party) : length_(len) {
		bool* b = new bool[len];
		bool_data(b, len, str);
		bits_ = new Bit[length_];
		init_integer(bits_, b, length_, party);
		delete[] b;
	}

	
	MPCInteger::MPCInteger(int len, long long input, int party)
		: MPCInteger(len, std::to_string(input), party) {
	}

	
	MPCInteger MPCInteger::select(const Bit & select, const MPCInteger & a) const {
		MPCInteger res(*this);
		for (int i = 0; i < size(); ++i)
			res[i] = bits_[i].select(select, a[i]);
		return res;
	}

	
	Bit& MPCInteger::operator[](int index) {
		return bits_[std::min(index, size() - 1)];
	}

	
	const Bit &MPCInteger::operator[](int index) const {
		return bits_[min(index, size() - 1)];
	}

	template<>
	
	string MPCInteger::reveal<string>(int party) const {
		bool * b = new bool[length_];
		ProtocolExecution::prot_exec->reveal(b, party, (block *)bits_, length_);
		string bin = "";
		for (int i = length_ - 1; i >= 0; --i)
			bin += (b[i] ? '1' : '0');
		delete[] b;
		return bin_to_dec(bin);
	}

	template<>
	
	int MPCInteger::reveal<int>(int party) const {
		string s = reveal<string>(party);
		return stoi(s);
	}
	template<>
	
	uint32_t MPCInteger::reveal<uint32_t>(int party) const {
		MPCInteger tmp = *this;
		tmp.resize(tmp.size() + 1, false);
		string s = tmp.reveal<string>(party);
		return stoi(s);
	}


	template<>
	
	long long MPCInteger::reveal<long long>(int party) const {
		string s = reveal<string>(party);
		return stoll(s);
	}



	
	int MPCInteger::size() const {
		return length_;
	}

	//circuits
	MPCInteger MPCInteger::abs() const {
		MPCInteger res(*this);
		for (int i = 0; i < size(); ++i)
			res[i] = bits_[size() - 1];
		return ((*this) + res) ^ res;
	}

	
	MPCInteger& MPCInteger::resize(int len, bool signed_extend) 
	{
		Bit * old = bits_;
		bits_ = new Bit[len];
		memcpy(bits_, old, min(len, size()) * sizeof(Bit));
		Bit extended_bit = old[length_ - 1] & signed_extend;
		for (int i = min(len, size()); i < len; ++i)
			bits_[i] = extended_bit;
		this->length_ = len;
		delete[] old;
		return *this;
	}

	//Logical operations
	
	MPCInteger MPCInteger::operator^(const MPCInteger& rhs) const {
		MPCInteger res(*this);
		for (int i = 0; i < size(); ++i)
			res.bits_[i] = res.bits_[i] ^ rhs.bits_[i];
		return res;
	}

	
	MPCInteger MPCInteger::operator|(const MPCInteger& rhs) const {
		MPCInteger res(*this);
		for (int i = 0; i < size(); ++i)
			res.bits_[i] = res.bits_[i] | rhs.bits_[i];
		return res;
	}

	MPCInteger MPCInteger::operator&(const MPCInteger& rhs) const {
		MPCInteger res(*this);
		for (int i = 0; i < size(); ++i)
			res.bits_[i] = res.bits_[i] & rhs.bits_[i];
		return res;
	}

	
	MPCInteger MPCInteger::operator<<(int shamt) const {
		MPCInteger res(*this);
		if (shamt > size()) {
			for (int i = 0; i < size(); ++i)
				res.bits_[i] = false;
		}
		else {
			for (int i = size() - 1; i >= shamt; --i)
				res.bits_[i] = bits_[i - shamt];
			for (int i = shamt - 1; i >= 0; --i)
				res.bits_[i] = false;
		}
		return res;
	}

	
	MPCInteger MPCInteger::operator>>(int shamt) const {
		MPCInteger res(*this);
		if (shamt > size()) {
			for (int i = 0; i < size(); ++i)
				res.bits_[i] = false;
		}
		else {
			for (int i = shamt; i < size(); ++i)
				res.bits_[i - shamt] = bits_[i];
			for (int i = size() - shamt; i < size(); ++i)
				res.bits_[i] = false;
		}
		return res;
	}

	
	MPCInteger MPCInteger::operator<<(const MPCInteger& shamt) const {
		MPCInteger res(*this);
		for (int i = 0; i < min(int(ceil(log2(size()))), shamt.size() - 1); ++i)
			res = res.select(shamt[i], res << (1 << i));
		return res;
	}

	
	MPCInteger MPCInteger::operator>>(const MPCInteger& shamt) const {
		MPCInteger res(*this);
		for (int i = 0; i < min(int(ceil(log2(size()))), shamt.size() - 1); ++i)
			res = res.select(shamt[i], res >> (1 << i));
		return res;
	}

	//Comparisons
	
	Bit MPCInteger::geq(const MPCInteger& rhs) const {
		assert(size() == rhs.size());
		/*	Bit res(false);
			for(int i = 0; i < size(); ++i) {
				res = ((bits_[i]^res)&(rhs[i]^res))^bits_[i];
			}
			return res;
		*/
		MPCInteger tmp = (*this) - rhs;
		return !tmp[tmp.size() - 1];
	}

	
	Bit MPCInteger::equal(const MPCInteger& rhs) const {
		assert(size() == rhs.size());
		Bit res(true);
		for (int i = 0; i < size(); ++i)
			res = res & (bits_[i] == rhs[i]);
		return res;
	}

	/* Arithmethics
	 */
	
	MPCInteger MPCInteger::operator+(const MPCInteger & rhs) const {
		assert(size() == rhs.size());
		MPCInteger res(*this);
		add_full(res.bits_, nullptr, bits_, rhs.bits_, nullptr, size());
		return res;
	}

	
	MPCInteger MPCInteger::operator-(const MPCInteger& rhs) const {
		assert(size() == rhs.size());
		MPCInteger res(*this);
		sub_full(res.bits_, nullptr, bits_, rhs.bits_, nullptr, size());
		return res;
	}


	
	MPCInteger MPCInteger::operator*(const MPCInteger& rhs) const {
		assert(size() == rhs.size());
		MPCInteger res(*this);
		mul_full(res.bits_, bits_, rhs.bits_, size());
		return res;
	}

	
	MPCInteger MPCInteger::operator/(const MPCInteger& rhs) const {
		assert(size() == rhs.size());
		MPCInteger res(*this);
		MPCInteger i1 = abs();
		MPCInteger i2 = rhs.abs();
		Bit sign = bits_[size() - 1] ^ rhs[size() - 1];
		div_full(res.bits_, nullptr, i1.bits_, i2.bits_, size());
		condNeg(sign, res.bits_, res.bits_, size());
		return res;
	}
	
	MPCInteger MPCInteger::operator%(const MPCInteger& rhs) const {
		assert(size() == rhs.size());
		MPCInteger res(*this);
		MPCInteger i1 = abs();
		MPCInteger i2 = rhs.abs();
		Bit sign = bits_[size() - 1];
		div_full(nullptr, res.bits_, i1.bits_, i2.bits_, size());
		condNeg(sign, res.bits_, res.bits_, size());
		return res;
	}

	
	MPCInteger MPCInteger::operator-() const {
		return MPCInteger(size(), 0, PUBLIC) - (*this);
	}

	//Others
	
	MPCInteger MPCInteger::leading_zeros() const {
		MPCInteger res = *this;
		for (int i = size() - 2; i >= 0; --i)
			res[i] = res[i + 1] | res[i];

		for (int i = 0; i < res.size(); ++i)
			res[i] = !res[i];
		return res.hamming_weight();
	}

	
	MPCInteger MPCInteger::hamming_weight() const {
		vector<MPCInteger> vec;
		for (int i = 0; i < size(); i++) {
			MPCInteger tmp(2, 0, PUBLIC);
			tmp[0] = bits_[i];
			vec.push_back(tmp);
		}

		while (vec.size() > 1) {
			int j = 0;
			for (size_t i = 0; i < vec.size() - 1; i += 2) {
				vec[j++] = vec[i] + vec[i + 1];
			}
			if (vec.size() % 2 == 1) {
				vec[j++] = vec[vec.size() - 1];
			}
			for (int i = 0; i < j; ++i)
				vec[i].resize(vec[i].size() + 1, false);
			int vec_size = vec.size();
			for (int i = j; i < vec_size; ++i)
				vec.pop_back();
		}
		return vec[0];
	}
	
	MPCInteger MPCInteger::modExp(MPCInteger p, MPCInteger q) {
		// the value of q should be less than half of the MAX_INT
		MPCInteger base = *this;
		MPCInteger res(1, size());
		for (int i = 0; i < p.size(); ++i) {
			MPCInteger tmp = (res * base) % q;
			res.select(p[i], tmp);
			base = (base*base) % q;
		}
		return res;
	}


}