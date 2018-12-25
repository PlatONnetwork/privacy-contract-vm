#pragma once

#include "emp-sh2pc/semihonest_gen.h"
#include "emp-sh2pc/semihonest_eva.h"
#include "io_channel.h"

#ifdef OT_NP_USE_MIRACL
#include "emp-tool/utils/sm2_params.h"
#else
#include "emp-tool/utils/relic_ec_utils.h"
#endif//


namespace platon
{
	namespace mpc
	{
		//template<typename IO>
		inline void setup_semi_honest(IOChannel* io, int party) {
#ifdef OT_NP_USE_MIRACL
			SM2_Init();
#else
			initialize_relic();
#endif//
			CircuitExecutionProxy::circ_exec;
			ProtocolExecutionProxy::prot_exec;

			if (party == ALICE) {
				CircuitExecution* cir = new HalfGateGen(io);
				ProtocolExecution* prot = new SemiHonestGen(io);

				CircuitExecutionProxy::circ_exec.setup(cir);
				ProtocolExecutionProxy::prot_exec.setup(prot);
			}
			else {
				CircuitExecution* cir = new HalfGateEva(io);
				ProtocolExecution* prot = new SemiHonestEva(io);

				CircuitExecutionProxy::circ_exec.setup(cir);
				ProtocolExecutionProxy::prot_exec.setup(prot);
			}
		}
	}
}

