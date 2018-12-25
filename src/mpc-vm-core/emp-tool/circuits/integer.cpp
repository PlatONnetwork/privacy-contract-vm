#include "platon_integer.h"
#include <algorithm>
#include <vector>
#include <climits>
#include "emp-tool/circuits/integer_utils.h"
#include "emp-tool/execution/circuit_execution.h"
#include "emp-tool/execution/protocol_execution.h"
#include <cmath>


using std::min;
using std::ceil;
using std::log2;

namespace platon
{
	namespace mpc
	{
		void Integer::bool_data(bool* data, size_t len, std::string str)
		{
			integer_bool_data(data, len, str);
		}


		void Integer::bool_data(bool* data, size_t len, long long num)
		{
			integer_bool_data(data, len, num);
		}


		void Integer::init(Bit * bits, const bool* b, int length, int party) {
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
		}


		void Integer::init(int32_t length, const std::string& value, uint8_t party/* = PUBLIC*/)
		{
			bool* b = new bool[length];
			integer_bool_data(b, length, value);
			bits_ = new Bit[length];
			Integer::init(bits_, b, length, party);
			delete[] b;

		}


		void Integer::init(int32_t length, int32_t value, uint8_t party/* = PUBLIC*/)
		{
			init(length, std::to_string(value), party);
		}


		void Integer::init(int32_t length, int64_t value, uint8_t party/* = PUBLIC*/)
		{
			init(length, std::to_string(value), party);
		}


		Integer::Integer() : length_(0), bits_(nullptr) { }


		Integer::Integer(Integer&& in) : length_(in.length_) {
			bits_ = in.bits_;
			in.bits_ = nullptr;
		}


		Integer::Integer(const Integer& in) : length_(in.length_) {
			bits_ = new Bit[length_];
			memcpy(bits_, in.bits_, sizeof(Bit)*length_);
		}


		Integer& Integer::operator= (Integer rhs) {
			length_ = rhs.length_;
			std::swap(bits_, rhs.bits_);//kelvin note: should copy bits_ !
			return *this;
		}


		Integer::Integer(unsigned int len, const void * b) : length_(len) {
			bits_ = new Bit[len];
			memcpy(bits_, b, sizeof(Bit)*len);
		}


		Integer::~Integer() {
			if (bits_ != nullptr) delete[] bits_;
		}

		Integer::Integer(const int8_t& value, int party/* = PUBLIC*/) : Integer(8 * sizeof(value), std::to_string(value), party){}
		Integer::Integer(const int16_t& value, int party/* = PUBLIC*/) : Integer(8 * sizeof(value), std::to_string(value), party) {}
		Integer::Integer(const int32_t& value, int party/* = PUBLIC*/) : Integer(8 * sizeof(value), std::to_string(value), party) {}
		Integer::Integer(const int64_t& value, int party/* = PUBLIC*/) : Integer(8 * sizeof(value), std::to_string(value), party) {}

		Integer::Integer(unsigned int length, const std::string& value, int party) : length_(length) {
			bool* b = new bool[length];
			integer_bool_data(b, length, value);
			bits_ = new Bit[length];
			init(bits_, b, length, party);
			delete[] b;
		}

		/*Integer::Integer(int32_t length, int32_t value, uint8_t party)
			: Integer(length, std::to_string(value), party) {
		}

		Integer::Integer(int32_t length, int64_t value, uint8_t party)
			: Integer(length, std::to_string(value), party) {
		}*/


		Integer Integer::select(const Bit & select, const Integer & a) const {
			Integer res(*this);
			for (int i = 0; i < size(); ++i)
				res[i] = bits_[i].select(select, a[i]);
			return res;
		}


		Bit& Integer::operator[](int index) {
			return bits_[min(index, size() - 1)];
		}


		const Bit &Integer::operator[](int index) const {
			return bits_[min(index, size() - 1)];
		}


		int Integer::size() const {
			return length_;
		}

		//circuits

		Integer Integer::abs() const {
			Integer res(*this);
			for (int i = 0; i < size(); ++i)
				res[i] = bits_[size() - 1];
			return ((*this) + res) ^ res;
		}


		Integer& Integer::resize(int len, bool signed_extend) {
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


		std::string Integer::reveal_string(int party) const {
			bool * b = new bool[length_];
			ProtocolExecutionProxy::prot_exec.reveal(b, party, (block *)bits_, length_);
			std::string bin = "";
			for (int i = length_ - 1; i >= 0; --i)
				bin += (b[i] ? '1' : '0');
			delete[] b;
			return integer_bin2dec(bin);
		}

		//
		//T Integer::reveal_int(int party) const {
		//	std::string s = reveal_string(party);
		//	return stoi(s);
		//}


		int64_t Integer::reveal_int(int party) const {
			Integer tmp = *this;
			tmp.resize(tmp.size() + 1, true);
			std::string s = tmp.reveal_string(party);

			long long v = stoll(s);
			if (LLONG_MAX == v || LLONG_MIN == v)
				return -1;

			return v;
		}

		//Logical operations
		Integer Integer::operator^(const Integer& rhs) const {
			Integer res(*this);
			for (int i = 0; i < size(); ++i)
				res.bits_[i] = res.bits_[i] ^ rhs.bits_[i];
			return res;
		}

		Integer Integer::operator|(const Integer& rhs) const {
			Integer res(*this);
			for (int i = 0; i < size(); ++i)
				res.bits_[i] = res.bits_[i] | rhs.bits_[i];
			return res;
		}

		Integer Integer::operator&(const Integer& rhs) const {
			Integer res(*this);
			for (int i = 0; i < size(); ++i)
				res.bits_[i] = res.bits_[i] & rhs.bits_[i];
			return res;
		}

		Integer Integer::operator<<(int shamt) const {
			Integer res(*this);
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

		Integer Integer::operator>>(int shamt) const {
			Integer res(*this);
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


		Integer Integer::operator<<(const Integer& shamt) const {
			Integer res(*this);
			for (int i = 0; i < min(int(ceil(log2(size()))), shamt.size() - 1); ++i)
				res = res.select(shamt[i], res << (1 << i));
			return res;
		}


		Integer Integer::operator>>(const Integer& shamt) const {
			Integer res(*this);
			for (int i = 0; i < min(int(ceil(log2(size()))), shamt.size() - 1); ++i)
				res = res.select(shamt[i], res >> (1 << i));
			return res;
		}

		//Comparisons
		Bit Integer::geq(const Integer& rhs) const {
			Integer(size() == rhs.size());
			/*	Bit res(false);
				for(int i = 0; i < size(); ++i) {
					res = ((bits_[i]^res)&(rhs[i]^res))^bits_[i];
				}
				return res;
			*/
			Integer tmp = (*this) - rhs;
			return !tmp[tmp.size() - 1];
		}

		Bit Integer::equal(const Integer& rhs) const {
			//assert(size() == rhs.size());
			Bit res(true);
			for (int i = 0; i < size(); ++i)
				res = res & (bits_[i] == rhs[i]);
			return res;
		}

		/* Arithmethics
		 */
		Integer Integer::operator+(const Integer & rhs) const {
			//assert(size() == rhs.size());
			Integer res(*this);
			integer_add_full(res.bits_, nullptr, bits_, rhs.bits_, nullptr, size());
			return res;
		}

		Integer Integer::operator-(const Integer& rhs) const {
			//assert(size() == rhs.size());
			Integer res(*this);
			integer_sub_full(res.bits_, nullptr, bits_, rhs.bits_, nullptr, size());
			return res;
		}

		Integer Integer::operator*(const Integer& rhs) const {
			//assert(size() == rhs.size());
			Integer res(*this);
			integer_mul_full(res.bits_, bits_, rhs.bits_, size());
			return res;
		}

		Integer Integer::operator/(const Integer& rhs) const {
			//assert(size() == rhs.size());
			Integer res(*this);
			Integer i1 = abs();
			Integer i2 = rhs.abs();
			Bit sign = bits_[size() - 1] ^ rhs[size() - 1];
			integer_div_full(res.bits_, nullptr, i1.bits_, i2.bits_, size());
			integer_condNeg(sign, res.bits_, res.bits_, size());
			return res;
		}

		Integer Integer::operator%(const Integer& rhs) const {
			//assert(size() == rhs.size());
			Integer res(*this);
			Integer i1 = abs();
			Integer i2 = rhs.abs();
			Bit sign = bits_[size() - 1];
			integer_div_full(nullptr, res.bits_, i1.bits_, i2.bits_, size());
			integer_condNeg(sign, res.bits_, res.bits_, size());
			return res;
		}

		Integer Integer::operator-() const {
			return Integer(size(), 0, PUBLIC) - (*this);
		}

		//Others
		Integer Integer::leading_zeros() const {
			Integer res = *this;
			for (int i = size() - 2; i >= 0; --i)
				res[i] = res[i + 1] | res[i];

			for (int i = 0; i < res.size(); ++i)
				res[i] = !res[i];
			return res.hamming_weight();
		}

		Integer Integer::hamming_weight() const {
			std::vector<Integer> vec;
			for (int i = 0; i < size(); i++) {
				Integer tmp(2, 0, PUBLIC);
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
				
				auto vec_size = vec.size();
				for (size_t i = j; i < vec_size; ++i)
					vec.pop_back();
			}
			return vec[0];
		}

		Integer Integer::modExp(Integer p, Integer q) {
			// the value of q should be less than half of the MAX_INT
			Integer base = *this;
			Integer res(1, size());
			for (int i = 0; i < p.size(); ++i) {
				Integer tmp = (res * base) % q;
				res.select(p[i], tmp);
				base = (base*base) % q;
			}
			return res;
		}
	}
}
