#include "emp-tool/execution/plain_prot.h"
#include "emp-tool/execution/plain_circ.h"



namespace platon
{
	namespace mpc
	{
		void setup_plain_prot(bool print, string filename)
		{
			CircuitExecution* cir = new PlainCircExec(print, filename);
			ProtocolExecution* prot = new PlainProt(print, filename);

			CircuitExecutionProxy::circ_exec.setup(cir);
			ProtocolExecutionProxy::prot_exec.setup(prot);
		}

		void finalize_plain_prot()
		{
			PlainCircExec * cast_circ_exec = static_cast<PlainCircExec *> (CircuitExecutionProxy::circ_exec.circ_exec_);
			PlainProt * cast_prot_exec = static_cast<PlainProt*> (ProtocolExecutionProxy::prot_exec.prot_exec_);
			int64_t z_index = cast_circ_exec->gid++;
			cast_circ_exec->fout << 2 << " " << 1 << " " << 0 << " " << 0 << " " << z_index << " XOR" << endl;
			for (auto v : cast_prot_exec->output_vec) {
				cast_circ_exec->fout << 2 << " " << 1 << " " << z_index << " " << v << " " << cast_circ_exec->gid++ << " XOR" << endl;
			}
			cast_circ_exec->gates += (1 + cast_prot_exec->output_vec.size());
			cast_circ_exec->finalize();

			ProtocolExecutionProxy::prot_exec.finalize();
			CircuitExecutionProxy::circ_exec.finalize();
		}

	}
}