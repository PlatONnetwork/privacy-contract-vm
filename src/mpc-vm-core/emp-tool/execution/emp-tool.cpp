#include "circuit_execution.h"
#include "protocol_execution.h"

//#ifndef THREADING
//thread_local platon::mpc::ProtocolExecution* platon::mpc::ProtocolExecution::prot_exec = nullptr;
thread_local platon::mpc::ProtocolExecutionProxy platon::mpc::ProtocolExecutionProxy::prot_exec;
//thread_local platon::mpc::CircuitExecution* platon::mpc::CircuitExecution::circ_exec = nullptr;
thread_local platon::mpc::CircuitExecutionProxy platon::mpc::CircuitExecutionProxy::circ_exec;
//#else
//__thread emp::ProtocolExecution* emp::ProtocolExecution::prot_exec = nullptr;
//__thread emp::CircuitExecution* emp::CircuitExecution::circ_exec = nullptr;
//#endif
