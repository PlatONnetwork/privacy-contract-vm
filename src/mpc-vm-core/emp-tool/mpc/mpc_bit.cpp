#include "emp-tool/mpc/mpc_bit.h"
#include "emp-tool/execution/protocol_execution.h"
#include "emp-tool/execution/circuit_execution.h"
#include "emp-tool/utils/utils.h"

using namespace emp;

namespace platon
{
	MPCBit::MPCBit(const block& a) {
		memcpy(&bit_, &a, sizeof(block));
	}

	MPCBit::MPCBit(bool b, int party) {
		if (party == PUBLIC)
			bit_ = CircuitExecution::circ_exec->public_label(b);
		else ProtocolExecution::prot_exec->feed(&bit_, party, &b, 1);
	}

	MPCBit MPCBit::select(const MPCBit & select, const MPCBit & new_v) const {
		MPCBit tmp = *this;
		tmp = tmp ^ new_v;
		tmp = tmp & select;
		return *this ^ tmp;
	}

	MPCBit MPCBit::operator==(const MPCBit& rhs) const {
		return !(*this ^ rhs);
	}

	MPCBit MPCBit::operator!=(const MPCBit& rhs) const {
		return (*this) ^ rhs;
	}

	MPCBit MPCBit::operator &(const MPCBit& rhs) const {
		MPCBit res;
		res.bit_ = CircuitExecution::circ_exec->and_gate(bit_, rhs.bit_);
		return res;
	}
	MPCBit MPCBit::operator ^(const MPCBit& rhs) const {
		MPCBit res;
		res.bit_ = CircuitExecution::circ_exec->xor_gate(bit_, rhs.bit_);
		return res;
	}

	MPCBit MPCBit::operator |(const MPCBit& rhs) const {
		return (*this ^ rhs) ^ (*this & rhs);
	}

	MPCBit MPCBit::operator!() const {
		return CircuitExecution::circ_exec->not_gate(bit_);
	}

}//platon
