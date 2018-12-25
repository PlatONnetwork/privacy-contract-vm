#include "gencode.h"
#include <stdio.h>
using namespace std;


void SFUNC(IR& ir, std::string _hash_name, std::string _hash_prot, std::string _name, std::string _prot, std::string _rett, std::string _arg1, std::string _arg2) {
	IR::func f;
	f.rett = _rett;
	f.name = _name;
	f.prot = _prot;
	f.argtypes.push_back(_arg1);
	f.argtypes.push_back(_arg2);
	f.hash_prot = _hash_prot;
	f.hash_name = _hash_name;
	ir.funcs.push_back(f);
}

int main(int argc, char* argv[]) {

	{
		IR ir;
		ir.name = "IntTest";
		ir.hash = "abff8381224c0fa01ea0cd264b393c92"; // testint.ll

		SFUNC(ir, "3f5a75d123bb5ef2cbc5ee421ead09b9", "20ce2cac12e0cd224c50da01c89cb157", "ListMsg", "ListMsg(_<>int,int)", "i32", "i32", "i32");

		gen_all(ir, "E:/work/local/dev-mmpc-llvm/tools/gencode");
	}

	{
		IR ir;
		ir.name = "Samples";
		ir.hash = "11111111111111111111111111111111"; // MpcSamples

		SFUNC(ir, "777777777770cd224c50da01c89cb101", "3f5a75d128888888888888881ead0901", "f01", "f01(_<>bool,bool)", "i1", "i1", "i1");
		SFUNC(ir, "777777777770cd224c50da01c89cb102", "3f5a75d128888888888888881ead0902", "f02", "f02(_<>int,int)", "i32", "i32", "i32");
		SFUNC(ir, "777777777770cd224c50da01c89cb103", "3f5a75d128888888888888881ead0903", "f03", "f03(_<>long,long)", "i64", "i64", "i64");
		SFUNC(ir, "777777777770cd224c50da01c89cb104", "3f5a75d128888888888888881ead0904", "f04", "f04(_<>float,float)", "float", "float", "float");
		SFUNC(ir, "777777777770cd224c50da01c89cb105", "3f5a75d128888888888888881ead0905", "f05", "f05(_<>double,double)", "double", "double", "double");
		SFUNC(ir, "777777777770cd224c50da01c89cb106", "3f5a75d128888888888888881ead0906", "f06", "f06(_<>Foo,Foo)", "%\"class.demo::Bar\"*", "%\"class.demo::Foo\"*", "%\"class.demo::Foo\"*");
		SFUNC(ir, "777777777770cd224c50da01c89cb107", "3f5a75d128888888888888881ead0907", "f07", "f07(_<>Bar,Foo)", "%\"class.demo::Foo\"*", "%\"class.demo::Bar\"*", "%\"class.demo::Foo\"*");
		SFUNC(ir, "777777777770cd224c50da01c89cb108", "3f5a75d128888888888888881ead0908", "f08", "f08(_<>string,string)", "%\"class.std::__cxx11::basic_string\"*", "%\"class.std::__cxx11::basic_string\"*", "%\"class.std::__cxx11::basic_string\"*");
		//SFUNC(ir, "777777777770cd224c50da01c89cb109", "3f5a75d128888888888888881ead0909", "f09", "f09(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb110", "3f5a75d128888888888888881ead0910", "f10", "f10(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb111", "3f5a75d128888888888888881ead0911", "f11", "f11(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb112", "3f5a75d128888888888888881ead0912", "f12", "f12(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb113", "3f5a75d128888888888888881ead0913", "f13", "f13(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb114", "3f5a75d128888888888888881ead0914", "f14", "f14(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb115", "3f5a75d128888888888888881ead0915", "f15", "f15(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb116", "3f5a75d128888888888888881ead0916", "f16", "f16(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb117", "3f5a75d128888888888888881ead0917", "f17", "f17(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb118", "3f5a75d128888888888888881ead0918", "f18", "f18(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb119", "3f5a75d128888888888888881ead0919", "f19", "f19(int,int)", "i32", "i32", "i32");
		//SFUNC(ir, "777777777770cd224c50da01c89cb120", "3f5a75d128888888888888881ead0920", "f20", "f20(int,int)", "i32", "i32", "i32");

		gen_all(ir, "E:/work/local/dev-mmpc-llvm/tools/gencode");
	}

	return 0;
}