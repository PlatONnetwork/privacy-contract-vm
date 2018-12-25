#include "mpc_task.h"

NS_PLATON_SDK_MPC_BEG


MPCTask::MPCTask() : readyState(0) {};

MPCTask::MPCTask(
	const string& _taskid,
	const string& _pubkey,
	const string& _txaddr,
	const string& _iraddr,
	const string& _method,
	const string& _extras
) :
	taskId(_taskid),
	pubkey(_pubkey),
	txaddr(_txaddr),
	iraddr(_iraddr),
	method(_method),
	extras(_extras),
	readyState(0)
{
};


string MPCTask::getStatus() const {
    switch (status)
    {
#define GETSTATUS(S) case Status::S: return std::string(#S)
        GETSTATUS(INIT);
        GETSTATUS(PUSH_COMMIT);
        GETSTATUS(POP_COMMIT);
        GETSTATUS(INVITE_OR_NOTIFY);
        GETSTATUS(PUSH_READY);
        GETSTATUS(POP_READY);
        GETSTATUS(PROCESS_INIT);
        GETSTATUS(PROCESS_FAILED);
        GETSTATUS(PROCESS_SET_CALLBACK);
        GETSTATUS(PROCESS_GET_INPUT);
        GETSTATUS(PROCESS_RUN_FUNCTION);
        GETSTATUS(PROCESS_NOTIFY_RESULT);
        GETSTATUS(PROCESS_ENCRYPT);
        GETSTATUS(PROCESS_SIGN_TRANSACTION);
        GETSTATUS(PROCESS_SEND_TRANSACTION);
        GETSTATUS(PROCESS_OK);
        GETSTATUS(PROCESS_TIMEOUT);
#undef GETSTATUS
    default:return std::to_string(static_cast<int>(status));
    }
}

string MPCTask::getStatusErrDesc() const {
    switch (status)
    {
#define GETSTATUSERRDESC(S) case Status::S: return std::string(#S" FAILED")
        GETSTATUSERRDESC(INIT);
        GETSTATUSERRDESC(PUSH_COMMIT);
        GETSTATUSERRDESC(POP_COMMIT);
        GETSTATUSERRDESC(INVITE_OR_NOTIFY);
        GETSTATUSERRDESC(PUSH_READY);
        GETSTATUSERRDESC(POP_READY);
        GETSTATUSERRDESC(PROCESS_INIT);
        GETSTATUSERRDESC(PROCESS_FAILED);
        GETSTATUSERRDESC(PROCESS_SET_CALLBACK);
        GETSTATUSERRDESC(PROCESS_GET_INPUT);
        GETSTATUSERRDESC(PROCESS_RUN_FUNCTION);
        GETSTATUSERRDESC(PROCESS_NOTIFY_RESULT);
        GETSTATUSERRDESC(PROCESS_ENCRYPT);
        GETSTATUSERRDESC(PROCESS_SIGN_TRANSACTION);
        GETSTATUSERRDESC(PROCESS_SEND_TRANSACTION);
    case Status::PROCESS_OK: return std::string("OK");
    case Status::PROCESS_TIMEOUT: return std::string("PROCESS TIMEOUT");
#undef GETSTATUSERRDESC
    default:return std::to_string(static_cast<int>(status));
    }
}

NS_PLATON_SDK_MPC_END
