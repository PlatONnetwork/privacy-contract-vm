#pragma once

namespace platon
{
	namespace mpc
	{
		template<typename T>
		class Comparable {
		public:
			Bit operator>=(const T&rhs) const {
				return static_cast<const T*>(this)->geq(rhs);
			}
			Bit operator<(const T& rhs) const {
				return !((*static_cast<const T*>(this)) >= rhs);
			}

			Bit operator<=(const T& rhs) const {
				return rhs >= *static_cast<const T*>(this);
			}

			Bit operator>(const T& rhs) const {
				return !(rhs >= *static_cast<const T*>(this));
			}

			Bit operator==(const T& rhs) const {
				return static_cast<const T*>(this)->equal(rhs);
			}
			Bit operator!=(const T& rhs) const {
				return !(*static_cast<const T*>(this) == rhs);
			}
		};
	}
}
