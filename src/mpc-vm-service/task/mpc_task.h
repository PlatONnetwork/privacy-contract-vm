#pragma once

#include "platon_mpc_service.h"

#include <cstdio>

#include <map>
#include <list>
#include <string>
#include <vector>
#include <memory>

#include "eth_trans.h"
#include "simple_timer.h"
#include "platon_client.h"

using std::vector;
using std::string;
using std::list;
using std::map;
using std::atomic;

using ::platon::callback::TaskCallbackPrx;
using ::platon::callback::TransactionCallbackPrx;
using ::platon::MPCRole;
using ::platon::NodeCommunicateInfo;

#include "mpc_vm_service.h"
NS_PLATON_SDK_MPC_BEG


class PLATONMPCVM_SERVICE_DLL_API MPCTask
{
public:
    ///
    string taskId = ""; // (uuid) task id
    string pubkey = ""; // wallet public key
    string irhash = ""; // ir-data hash
    string method = ""; // mpc(ir) method name
    string extras = ""; // extra data, add a letter "s" only for align ...
    string txaddr = ""; // tx adddress who send this transaction-task
    string iraddr = ""; // ir address, also named mpc contract address
    string irpath = ""; // ir-data-file full path
    ///

    bool						invitor; // is invitor or not, for 2PC, alice is invitor and bob is invitee
    MPCRole						role; // ALICE or BOB
    string						method_hash; // hash(method)
    string						transhash; // transaction hash, return by eth_sendRawTransaction

    vector<string>				urls; // node urls, for ice
    map<string, string>			addr_url; // address -> url
    vector<NodeCommunicateInfo> nodeInfoList;

    ///
    vector<string>				participants; // participants in the mpc task
    vector<string>				receivers; // receiver of mpc result
    ///
    TaskCallbackPrx				taskCB;
    TransactionCallbackPrx		transactionCB;
    ///
    std::atomic<int>			readyState;// 0: not ready, not input, 1: not ready, input, 3: ready, input 
    bool                        already = false; // true if state == 0x03

    platon::mpc::SimpleTimer	timer;

    enum Status {
        INIT,// initialize
        PUSH_COMMIT,// push into commit queue
        POP_COMMIT,// pop from commit queue
        INVITE_OR_NOTIFY, // invite or notify
        PUSH_READY,// push into ready queue
        POP_READY,// pop from ready queue
        PROCESS_INIT, // process ...
        PROCESS_FAILED,
        PROCESS_SET_CALLBACK,
        PROCESS_GET_INPUT,
        PROCESS_RUN_FUNCTION,
        PROCESS_NOTIFY_RESULT,
        PROCESS_ENCRYPT,
        PROCESS_SIGN_TRANSACTION,
        PROCESS_SEND_TRANSACTION,
        PROCESS_OK,
        PROCESS_TIMEOUT,
        N // The number of status
    };
    Status                      status = Status::INIT;
    string                      errmsg = "OK"; // the error message in each status

public:
    string toString() {
        stringstream sss;
        sss << "\n==================================================="
            ///
            << "\n   " << "     taskId: " << taskId
            << "\n   " << "     pubkey: " << pubkey
            << "\n   " << "     irhash: " << irhash
            << "\n   " << "     method: " << method
            << "\n   " << "     extras: " << extras
            << "\n   " << "     txaddr: " << txaddr
            << "\n   " << "     iraddr: " << iraddr
            ///			      
            << "\n   " << "    invitor: " << std::string(invitor ? "yes" : "no")
            << "\n   " << "       role: " << std::string(role == MPCRole::PART_ALICE ? "ALICE" : "BOB")
            << "\n   " << "     irpath: " << irpath
            << "\n   " << "method_hash: " << method_hash
            << "\n   " << "  transhash: " << transhash
            << "\n   " << "     status: " << getStatus()
            << "\n   " << "     errmsg: " << errmsg
            << "\n===================================================\n";
        return sss.str();
    }
public:
    ~MPCTask() = default;
    MPCTask();
    MPCTask(
        const string& _taskid,
        const string& _pubkey,
        const string& _txaddr,
        const string& _iraddr,
        const string& _method,
        const string& _extras
    );

    MPCTask(const MPCTask& task)
    {
        ///
        taskId = task.taskId;
        pubkey = task.pubkey;
        irhash = task.irhash;
        method = task.method;
        extras = task.extras;
        txaddr = task.txaddr;
        iraddr = task.iraddr;
        ///
        irpath = task.irpath;

        transhash = task.transhash;

        participants = task.participants;
        receivers = task.receivers;
        nodeInfoList.assign(task.nodeInfoList.begin(), task.nodeInfoList.end());

        readyState = task.readyState.load(std::memory_order_relaxed);
        timer = task.timer;

        taskCB = task.taskCB;
        transactionCB = task.transactionCB;

        role = task.role;
        invitor = task.invitor;
        urls = task.urls;
        addr_url = task.addr_url;
        method_hash = task.method_hash;

        status = task.status;
        errmsg = task.errmsg;
    };

    MPCTask& operator=(const MPCTask& task)
    {
        if (this == &task)
            return *this;

        ///
        taskId = task.taskId;
        pubkey = task.pubkey;
        irhash = task.irhash;
        method = task.method;
        extras = task.extras;
        txaddr = task.txaddr;
        iraddr = task.iraddr;
        ///
        irpath = task.irpath;

        transhash = task.transhash;

        participants = task.participants;
        receivers = task.receivers;
        nodeInfoList.assign(task.nodeInfoList.begin(), task.nodeInfoList.end());

        readyState = task.readyState.load(std::memory_order_relaxed);
        timer = task.timer;

        taskCB = task.taskCB;
        transactionCB = task.transactionCB;

        role = task.role;
        invitor = task.invitor;
        urls = task.urls;
        addr_url = task.addr_url;

        method_hash = task.method_hash;

        status = task.status;
        errmsg = task.errmsg;

        return *this;
    };

public:
    void setNodeInfoList(const vector<NodeCommunicateInfo>& nodeList) { nodeInfoList = nodeList; };
    void setTransHash(const string& _transhash) { transhash = _transhash; }

    void setTaskCB(const TaskCallbackPrx& cb) { taskCB = cb; }
    void setTransactionCB(const TransactionCallbackPrx& cb) { transactionCB = cb; }


    void setTaskId(const    string& _taskId) { taskId = _taskId; };
    const string getTaskId() const { return taskId; };

    void towardsReady(int step = 1) { readyState.fetch_add(step); };
    void setbackReady(int step = 1) { readyState.fetch_sub(step); };

    void setReady(int step = 2) { readyState.store(step, std::memory_order_relaxed); };
    int getReady() { return readyState.load(std::memory_order_relaxed); };

    void start() { return timer.start(); };
    double stop() { return timer.stop(); };

    bool timeout() {
        return false;
        //return timer.elapse() > 80.0;
    }

    string getStatus() const;
    string getStatusErrDesc() const;
};

NS_PLATON_SDK_MPC_END
