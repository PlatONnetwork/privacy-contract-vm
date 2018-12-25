#include <fstream>
#include <iostream>
using namespace std;

#include <boost/algorithm/string.hpp>
#include <libdevcore/RLP.h>
#include <libdevcore/SHA3.h>
#include <libdevcore/Function.h>
using namespace dev;

int main() {
	{
		cout << sha3("double(int256)").hex() << endl;
	}
	{
		RLPStream r;
		r.append("double(int256)");
		bytes b = r.out();
		cout << toHex(b) << endl;
		cout << sha3(b).hex() << endl;
	}
	{
		RLPStream r;
		r.append(1);
		bytes b = r.out();
		cout << toHex(b) << endl;
		cout << sha3(b).hex() << endl;
	}
	{
		std::vector<func_info_st> FuncInfos;
		int ret = GetFunctionInfo("E:/work/local/dev-mmpc-llvm/third-code/eth-simplest/mpcc.cpp.abi.json", FuncInfos);
		for (auto& fi : FuncInfos) {
			fi.print();
		}
	}

	return 0;
}