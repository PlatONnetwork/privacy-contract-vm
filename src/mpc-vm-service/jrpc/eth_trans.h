#pragma once

#include <string>
#include <sstream>


#include "json/value.h"

#include "mpc_vm_service.h"

using std::string;
using std::stringstream;

NS_PLATON_SDK_MPC_BEG


/*
.json
{ "name": "eth_sendRawTransaction", "params": [""], "order": [], "returns": ""}

request:
{
  "jsonrpc": "2.0",
  "method": "eth_sendRawTransaction",
  "params": [
	{
	  "from": "0xb60e8dd61c5d32be8058bb8eb970870f07233155",
	  "to": "0xd46e8dd67c5d32be8058bb8eb970870f07244567",
	  "gas": "0x76c0",
	  "gasPrice": "0x9184e72a000",
	  "value": "0x9184e72a",
	  "data": "0xd46e8dd67c5d32be8d46e8dd67c5d32be8058bb8eb970870f072445675058bb8eb970870f072445675"
	}
  ],
  "id": 1
}
response
{
  "id": 1,
  "jsonrpc": "2.0",
  "result": "0xe670ec64341771606e55d6b4ca35a1a6b75ee3d5145a99d05921026d1527331"
}
*/

struct TransParams
{
	string from;
	string to;
	string gas;
	string gasPrice;
	string value;
	string data;

	string tag; // only for test


	TransParams() {

	}

	string toString() const {
		/*
		**	{
		**		"from": "0xb60e8dd61c5d32be8058bb8eb970870f07233155",
		**		"to": "0xd46e8dd67c5d32be8058bb8eb970870f07244567",
		**		"gas": "0x76c0",
		**		"gasPrice": "0x9184e72a000",
		**		"value": "0x9184e72a",
		**		"data": "0xd46e8dd67c5d32be8d46e8dd67c5d32be8058bb8eb970870f072445675058bb8eb970870f072445675"
		**	}
		*/
		stringstream sss;

		sss << "{"
			<< "\"from\":\"" << from << "\","
			<< "\"to\":\"" << to << "\","
			<< "\"gas\":\"" << gas << "\","
			<< "\"gasPrice\":\"" << gasPrice << "\","
			<< "\"value\":\"" << value << "\","
			<< "\"data\":\"" << data << "\""
			<< "}";
		return sss.str();
	}

	Json::Value toJsonValue() const {
		Json::Value root;
		root["from"] = from;
		root["to"] = to;
		if (gas != "")root["gas"] = gas;
		if (gasPrice != "")root["gasPrice"] = gasPrice;
		if (value != "")root["value"] = value;
		//if (tag != "")root["tag"] = tag;
		root["data"] = data;

		//std::string s = root.toStyledString();
		//std::cout << "toJsonValue : " << s << std::endl;

		return root;
	}

	void parseFrom(const Json::Value& root) {
		from = root.get("from", "").asString();
		to = root.get("to", "").asString();
		gas = root.get("gas", "").asString();
		gasPrice = root.get("gasPrice", "").asString();
		value = root.get("value", "").asString();
		tag = root.get("tag", "").asString();
		data = root.get("data", "").asString();
	}

};

NS_PLATON_SDK_MPC_END

