#pragma once

#include <cstdio>

#include <map>
#include <string>
#include <iostream>

#include "abstractstubserver.h"
#include <jsonrpccpp/server/connectors/httpserver.h>

using namespace jsonrpc;
using std::string;
using std::cout;
using std::endl;
using std::cerr;

//#include "EthTrans.h"
//using namespace platon::sdk::mpc;

/*
this is a simulator.
*/
#include "abi.h"



class Server : public AbstractStubServer
{
public:
	Server(jsonrpc::AbstractServerConnector &conn) :AbstractStubServer(conn) {}

	virtual std::string eth_call2(const Json::Value& param01, const std::string& param02);
	virtual std::string eth_call(const Json::Value& param01, const std::string& param02) {
		std::cout << __FUNCTION__ << " p1:" << param01 << "p2:" << param02 << std::endl;
		return eth_call2(param01, param02);
	}
	virtual std::string eth_getTransactionCount(const std::string& param01, const std::string& param02) {
		std::cout << __FUNCTION__ << " p1:" << param01 << "p2:" << param02 << std::endl;
		return std::string("eth_getTransactionCount");
	}
	virtual std::string eth_sendRawTransaction(const std::string& param01) {
		std::cout << __FUNCTION__ << " p1:" << param01 << std::endl;
		return std::string("0x1234567890abcdef000000000000");
	}
	virtual Json::Value eth_getTransactionReceipt(const std::string& param01) {
		std::cout << __FUNCTION__ << " p1:" << param01 << std::endl;
		Json::Value val;
		return val;
	}
	virtual std::string eth_setActor(const std::string& param01) {
		std::cout << __FUNCTION__ << " p1:" << param01 << std::endl;
		return std::string("");
	}

};

int SimulatePlatONNode(int port) {
	try {
		HttpServer server(port);
		Server serv(server);
		if (serv.StartListening()) {
			std::cout << "Server started successfully, listen on port:" << port<< std::endl;
			getchar();
			serv.StopListening();
		}
		else {
			std::cout << "Error starting Server" << std::endl;
		}
	}
	catch (jsonrpc::JsonRpcException &e) {
		std::cerr << e.what() << std::endl;
		return -1;
	}
	return 0;
}


std::string Server::eth_call2(const Json::Value& param01, const std::string& param02) {
	//TransParams transparams;
	//transparams.parseFrom(param01);

	string tag = param01.get("tag", "").asString();
	if (tag == "") {
		return "";
	}

	std::string p("");
	size_t pos = tag.find("|");
	if (pos != std::string::npos) {
		p = tag.substr(pos + 1, tag.length() - pos - 1);
	}

	if (false) {}
	else if (tag == "get_ir_data") { return std::string(get_ir_data()); }
	else if (tag == "get_participants") { return std::string(get_participants()); }
	else if (tag == "get_urls") { return std::string(get_urls()); }
	else if (tag == "get_url_by_id") { return std::string(get_url_by_id(p.c_str())); }
	else if (tag == "get_invitor") { return std::string(get_invitor()); }
	else if (tag == "get_status") { return std::to_string(get_status(p.c_str())); }
	else if (tag == "get_fee") { return std::to_string(get_fee(p.c_str())); }
	else return "";
}