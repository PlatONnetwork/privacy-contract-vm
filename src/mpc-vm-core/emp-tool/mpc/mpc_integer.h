#pragma once


#include <cstdint>
#include <cstddef>
#include "emp-tool/circuits/bit.h"

//using emp::Bit;

namespace platon
{
	namespace mpc
	{
		enum MPCRoleType
		{
			ROLE_PUBLIC = 0,
			ROLE_ALICE = 1,
			ROLE_BOB = 2,
			ROLE_NOBODY = 3
		};

		class MPCInteger
		{
		public:
			MPCInteger(int32_t v, MPCRoleType role = ROLE_PUBLIC);
			MPCInteger(MPCInteger&& in);
			MPCInteger(const MPCInteger& in);

			MPCInteger(int len, const void * b);

			MPCInteger(int length, const string& str, int party = PUBLIC);
			MPCInteger(int length, long long input, int party = PUBLIC);
			MPCInteger() : length_(0), bits_(nullptr) { };

			MPCInteger& operator= (MPCInteger rhs);

			~MPCInteger();

			/*   ------       arithmetic oprators: + - * / %  implements    */
			MPCInteger operator<<(int steps) const;
			MPCInteger operator>>(int steps) const;
			MPCInteger operator<<(const MPCInteger& steps) const;
			MPCInteger operator>>(const MPCInteger& steps) const;

			MPCInteger operator+(const MPCInteger& rhs) const;
			MPCInteger operator-(const MPCInteger& rhs) const;
			MPCInteger operator-() const;
			MPCInteger operator*(const MPCInteger& rhs) const;
			MPCInteger operator/(const MPCInteger& rhs) const;
			MPCInteger operator%(const MPCInteger& rhs) const;
			MPCInteger operator&(const MPCInteger& rhs) const;
			MPCInteger operator|(const MPCInteger& rhs) const;


			//Comparable
			Bit geq(const MPCInteger & rhs) const;
			Bit equal(const MPCInteger & rhs) const;

			//Swappable
			MPCInteger select(const Bit & sel, const MPCInteger & rhs) const;
			MPCInteger operator^(const MPCInteger& rhs) const;

			int size() const;

			template<typename O>
			O reveal(int party = PUBLIC) const;

			MPCInteger abs() const;
			MPCInteger& resize(int length, bool signed_extend = true);
			MPCInteger modExp(MPCInteger p, MPCInteger q);
			MPCInteger leading_zeros() const;
			MPCInteger hamming_weight() const;

			Bit& operator[](int index);
			const Bit & operator[](int index) const;

			//batcher
			template<typename... Args>
			static size_t bool_size(size_t size, Args... args) {
				return size;
			}

			/*
			@desc convert data to bool, with every bit present by a boolean value: true or false
			*/
			static void bool_data(bool* data, size_t len, long long num) {
				bool_data(data, len, std::to_string(num));
			};

			static void bool_data(bool* data, size_t len, string str);

		private:
			Bit* bits_ = nullptr;
			int	 length_ = 0;
			MPCRoleType	role_ = ROLE_PUBLIC;
		};
	}
}//platon
