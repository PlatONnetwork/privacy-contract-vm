#ifndef _MPC_BIT_H__
#define _MPC_BIT_H__

#include "circuit_execution.h"
#include "protocol_execution.h"
#include "block.h"
#include "swappable.h"

namespace platon
{
	namespace mpc
	{
		class MPCBit : public Swappable<MPCBit>
		{
		public:
			block bit_;

			MPCBit(bool _b = false, int party = PUBLIC);
			MPCBit(const block& a);

			//template<typename O = bool>
			//O reveal(int party = PUBLIC) const;

			MPCBit operator!=(const MPCBit& rhs) const;
			MPCBit operator==(const MPCBit& rhs) const;
			MPCBit operator &(const MPCBit& rhs) const;
			MPCBit operator |(const MPCBit& rhs) const;
			MPCBit operator !() const;

			//swappable
			MPCBit select(const MPCBit & select, const MPCBit & new_v) const;
			MPCBit operator ^(const MPCBit& rhs) const;

			//batcher
			template<typename... Args>
			static size_t bool_size(Args&&... args) {
				return 1;
			}

			static void bool_data(bool *b, bool data) {
				b[0] = data;
			}

			template<typename O>
			O reveal(int party) const {
				O res;
				ProtocolExecution::prot_exec->reveal(&res, party, &bit_, 1);
				return res;
			}

			template<>
			string reveal<string>(int party) const {
				bool res;
				ProtocolExecution::prot_exec->reveal(&res, party, &bit_, 1);
				return res ? "true" : "false";
			}
		};

	}
}//platon

#endif//_MPC_BIT_H__
