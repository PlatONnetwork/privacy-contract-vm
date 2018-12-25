//#include "emp-tool/utils/relic_ec_utils.h"
//#include "io_channel.h"
//#include "emp-tool/io/mem_io_channel.h"
//#include "emp-tool/utils/utils.h"
//#include <iostream>
//
//using namespace platon::emp;
//using namespace std;
//
//int main() {
//		initialize_relic();
//		const eb_t *gtable = eb_curve_get_tab();
//		MemIO io;
//		io.send_eb(gtable, RELIC_EB_TABLE_MAX);
//		uint64_t zero = 0ULL;
//		io.send_data(&zero, 4-io.size%4);
////		cout <<io.size<<endl;
//		cout <<"#ifndef PRE_EB_TABLE\n #define PRE_EB_TABLE\n";
//		uint32_t * data = (uint32_t*)(io.buffer);
//		cout << "uint32_t eb_curve_get_tab_data[] = {"<<data[0]<<"";
//		for (int i = 1; i < io.size/4; ++i) {
//			cout << ","<<data[i]<<"";
//		}
//		cout << "};\n#endif";
//}
