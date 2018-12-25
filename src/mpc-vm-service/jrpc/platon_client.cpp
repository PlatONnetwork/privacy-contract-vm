#include <iostream>
using namespace std;

#include "platon_client.h"
#include "platon_logger.h"


NS_PLATON_SDK_MPC_BEG

PlatonClient::PlatonClient(const std::string& url) :
    httpclient(url), client(httpclient, JSONRPC_CLIENT_V2) {
}

std::string PlatonClient::call(const Json::Value& param01, const std::string& param02) {
    LOGI("eth_call .....");
    std::string ret("");
    try {
        ret = client.eth_call(param01, param02);
    }
    catch (JsonRpcException &e) {
        LOGE("eth_call error:%d %s", e.GetCode(), e.what());
    }
    catch (exception &e) {
        LOGE("eth_call exception error:%d %s", e.what());
    }
    return ret;
}

std::string PlatonClient::getTransactionCount(const std::string& param01, const std::string& param02) {
    LOGI("eth_getTransactionCount .....");
    std::string ret("");
    try {
        ret = client.eth_getTransactionCount(param01, param02);
        LOGI("eth_getTransactionCount ...ret%s ..", ret.c_str());
    }
    catch (JsonRpcException &e) {
        LOGE("eth_getTransactionCount error:%d %s", e.GetCode(), e.what());
    }
    return ret;
}

std::string PlatonClient::sendRawTransaction(const std::string& param01) {
    LOGI("eth_sendRawTransaction .....");
    std::string transhash("");
    try {
        transhash = client.eth_sendRawTransaction(param01);
    }
    catch (JsonRpcException &e) {
        LOGE("eth_sendRawTransaction error:%d %s", e.GetCode(), e.what());
    }
    return transhash;
}

Json::Value PlatonClient::getTransactionReceipt(const std::string& param01) {
    LOGI("eth_getTransactionReceipt .....");
    Json::Value val;
    try {
        val = client.eth_getTransactionReceipt(param01);
    }
    catch (JsonRpcException &e) {
        LOGE("eth_getTransactionReceipt error:%d %s", e.GetCode(), e.what());
    }
    return val;
}
std::string  PlatonClient::setActor(const std::string& param01) {
    LOGI("eth_setActor .....");
    std::string ret("");
    try {
        ret = client.eth_setActor(param01);
    }
    catch (JsonRpcException &e) {
        LOGE("eth_setActor error:%d %s", e.GetCode(), e.what());
    }
    return ret;
}

/*
**
**
**
**
**
**
*/
void PlatonClient::set_result(const TransParams& transparams, const char *taskId, uint64_t status, const char *data) {
    call(transparams.toJsonValue(), "latest");
}

void PlatonClient::set_result(const TransParams& transparams) {
    call(transparams.toJsonValue(), "latest");
}

std::string PlatonClient::get_ir_data(const TransParams& transparams) {
    std::string ret = call(transparams.toJsonValue(), "latest");
    return ret;
}
std::string PlatonClient::get_participants(const TransParams& transparams) {
    std::string ret = call(transparams.toJsonValue(), "latest");
    return ret;
}
std::string PlatonClient::get_urls(const TransParams& transparams) {
    std::string ret = call(transparams.toJsonValue(), "latest");
    return ret;
}
std::string PlatonClient::get_url_by_id(const TransParams& transparams, const char* address) {
    std::string ret = call(transparams.toJsonValue(), "latest");
    return ret;
}
std::string PlatonClient::get_invitor(const TransParams& transparams) {
    std::string ret = call(transparams.toJsonValue(), "latest");
    return ret;
}
uint64_t PlatonClient::get_status(const TransParams& transparams, const char* taskid) {
    std::string ret = call(transparams.toJsonValue(), "latest");
    return 0;
}
uint64_t PlatonClient::get_fee(const TransParams& transparams, const char* method) {
    std::string ret = call(transparams.toJsonValue(), "latest");
    return 0;
}


NS_PLATON_SDK_MPC_END
