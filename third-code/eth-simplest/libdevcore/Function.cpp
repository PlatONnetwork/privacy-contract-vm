#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
using namespace std;

#include "Function.h"

#include "json/value.h"
#include "json/reader.h"

#include "SHA3.h"


namespace dev
{

	int GetFunctionInfo(const char* filename,
		std::vector<func_info_st>& vecFuncInfos, std::map<std::string, func_info_st>& mapFuncInfos)
	{
		Json::Reader reader;
		Json::Value root;

		std::ifstream is;
		is.open(filename, std::ios::binary);
		if (!is.good()) {
            cerr << "cannot open " << filename << "!" << endl;
			return -1;
		}
		if (reader.parse(is, root, false)) {
			for (int i = 0; i < root.size(); i++) {
				func_info_st fi;

				Json::Value arr = root[i];

				std::string name = arr.get("name", "null").asString();
				std::string constant = arr.get("constant", "null").asString();
				std::string type = arr.get("type", "null").asString();

				if (type != "function") {
					continue;
				}

				fi.func_name = name;
				fi.func_prot += name;
				fi.func_prot += "(";

				Json::Value inputs = arr["inputs"];
				for (int j = 0; j < inputs.size(); j++) {
					Json::Value arg = inputs[j];
					std::string name = arg.get("name", "null").asString();
					std::string type = arg.get("type", "null").asString();
					fi.func_prot += type;
					fi.func_prot += ",";
				}
				Json::Value outputs = arr["outputs"];
				for (int j = 0; j < outputs.size(); j++) {
					Json::Value ret = outputs[j];
					std::string name = ret.get("name", "null").asString();
					std::string type = ret.get("type", "null").asString();
					fi.func_full += type;
					fi.func_full += " ";
				}
				if (inputs.size() > 0) {
					fi.func_prot.pop_back();
				}
				fi.func_prot += ")";
				fi.func_full += fi.func_prot;
				fi.func_sha3 = dev::sha3(fi.func_prot).hex();

				{
					for (int k = 0; k < name.length(); k++) {
						fi.func_byte.push_back(name[k]);
					}
				}


				vecFuncInfos.push_back(fi);
				mapFuncInfos[name] = fi;
			}

		}
		is.close();

		return 0;
	}

	int GetFunctionInfo(const char* filename, std::vector<func_info_st>& vecFuncInfos) {
		std::map<std::string, func_info_st> mapFuncInfos;
		return GetFunctionInfo(filename, vecFuncInfos, mapFuncInfos);
	}

	int GetFunctionInfo(const char* filename, std::map<std::string, func_info_st>& mapFuncInfos) {
		std::vector<func_info_st> vecFuncInfos;
		return GetFunctionInfo(filename, vecFuncInfos, mapFuncInfos);
	}

}