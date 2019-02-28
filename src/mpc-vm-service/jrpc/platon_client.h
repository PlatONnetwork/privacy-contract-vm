#pragma once

#include <map>
#include <string>

#include "stubclient.h"
#include <jsonrpccpp/client/connectors/httpclient.h>
using namespace jsonrpc;

#include "eth_trans.h"

#include "mpc_vm_service.h"
NS_PLATON_SDK_MPC_BEG

/*
jsonrpcstub platon.json --cpp-server=AbstractStubServer --cpp-client=StubClient
*/
class PlatonClient {
public:
	PlatonClient(const std::string& url);
    ~PlatonClient() {}
    bool ok() { return ok_; }

public:
	// eth_xxx
	std::string call(const Json::Value& param01, const std::string& param02);
	std::string getTransactionCount(const std::string& param01, const std::string& param02);
	std::string sendRawTransaction(const std::string& param01);
	Json::Value getTransactionReceipt(const std::string& param01);
	std::string setActor(const std::string& param01);

public:
	// abi
    void set_result(const TransParams& transparams, const char *taskId, uint64_t status, const char *data);
    void set_result(const TransParams& transparams);

    std::string get_ir_hash(const TransParams& transparams);
    std::string get_ir_path(const TransParams& transparams);
    std::string get_ir_data(const TransParams& transparams);
	std::string get_participants(const TransParams& transparams);
	std::string get_urls(const TransParams& transparams);
	std::string get_url_by_id(const TransParams& transparams, const char* address);
	std::string get_invitor(const TransParams& transparams);
	uint64_t get_status(const TransParams& transparams, const char* taskid);
	uint64_t get_fee(const TransParams& transparams, const char* method);

private:
    bool ok_ = true;
#ifndef RUN_WITHOUT_PLATON
	jsonrpc::HttpClient httpclient;
	StubClient client;
#endif
};

NS_PLATON_SDK_MPC_END
