#pragma once


#include "protocol_execution.h"
#include "circuit_execution.h"
#include "emp-tool/execution/plain_circ.h"
#include <iostream>
#include <fstream>

using std::endl;
using std::fstream;
using std::vector;
using std::string;

namespace platon
{
	namespace mpc
	{
		class PlainProt : public ProtocolExecution
		{
		public:
			int64_t n1 = 0, n2 = 0, n3 = 0;

			bool print;
			std::string filename;
			//PlainCircExec *cast_circ_exec;
			vector<int64_t>output_vec;

			void setup(IOChannel* _io)
			{
				//nothing
			}

			PlainProt(bool _print, std::string _filename) : print(_print),
				filename(_filename) {
				//cast_circ_exec = static_cast<PlainCircExec*> (CircuitExecutionProxy::circ_exec.circ_exec_);
			}

			~PlainProt()
			{

			}

			void finalize() {
				if (print) {
					fstream fout(filename, std::fstream::in | std::fstream::out);
					int gates = (static_cast<PlainCircExec*> (CircuitExecutionProxy::circ_exec.circ_exec_))->gates;
					int gid = (static_cast<PlainCircExec*> (CircuitExecutionProxy::circ_exec.circ_exec_))->gid;
					fout << gates << " " << gid<< std::endl;
					fout << n1 << " " << n2 << " " << n3 << std::endl;
					fout.close();
				}
			}

			void feed(block * label, int party, const bool* b, int length) override {
				for (int i = 0; i < length; ++i)
					label[i] = (static_cast<PlainCircExec*> (CircuitExecutionProxy::circ_exec.circ_exec_))->private_label(b[i]);

				if (party == ALICE) n1 += length;
				else n2 += length;
			}

			void reveal(bool* b, int party, const block * label, int length) override {
				for (int i = 0; i < length; ++i) {
					uint64_t *arr = (uint64_t*)(&label[i]);
					output_vec.push_back(arr[1]);
					b[i] = (static_cast<PlainCircExec*> (CircuitExecutionProxy::circ_exec.circ_exec_))->get_value(label[i]);
				}
				n3 += length;
			}
		};

		void setup_plain_prot(bool print, std::string filename);

		void finalize_plain_prot();

	}
}
