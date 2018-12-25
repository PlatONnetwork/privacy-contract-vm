#include "platon_integer.h"
#include "emp-tool/circuits/batcher.h"
#include "emp-tool/execution/plain_circ.h"
#include "emp-tool/execution/plain_prot.h"
#include "emp-tool/io/net_io_channel.h"
#include "emp-tool/utils/utils.h"
#include "emp-sh2pc/semihonest.h"
#include "emp-tool/circuits/number.h"
//#include <algorithm>

using namespace platon::mpc;
using namespace std;

void test_millionare(int party, int number) {
	Integer a(number, ALICE);
	Integer b(number, BOB);

	cout << "ALICE Input:\t"<<a.reveal_int(PUBLIC)<<endl;
	cout << "BOB Input:\t"<<b.reveal_int(PUBLIC)<<endl;
	cout << "ALICE larger?\t"<< (a>b).reveal()<<endl;
}

void test_sort(int party) {
	int size = 10;
	Batcher batcher1, batcher2;
	Integer *A = new Integer[size];
	for(int i = 0; i < size; ++i) {
		batcher1.add<Integer>(32, rand()%1024);
		batcher2.add<Integer>(32, rand()%1024);
	}

	batcher1.make_semi_honest(ALICE);
	batcher2.make_semi_honest(BOB);

	for (int i = 0; i < size; ++i)
	{
		(A[i]) = batcher1.next<Integer>() ^ batcher2.next<Integer>();
	}

	sort(A, size);
	for (int i = 0; i < size; ++i)
	{
		cout << A[i].reveal_string(PUBLIC) << endl;
	}
	delete[]A;
}

static void test_threads(long long a, int port)
{
	std::thread* threads[2]{ nullptr, nullptr };
	IOChannel* channels[2]{ nullptr, nullptr };

	int results[2]{ 0, 0 };
	for (int i = 0; i < 2; ++i)
	{
		threads[i] = new std::thread([&](IOChannel** _io, int _role, long long _a,  int* _results) {
			if (!*_io)
				*_io = new NetIO(_role == ALICE ? nullptr : "127.0.0.1", port);

			setup_semi_honest(*_io, _role);
			test_sort(_role);
			//test_full(_role);
		}, &(channels[i]), i, a, results + i);
	}

	for (int i = 0; i < 2; ++i)
	{
		threads[i]->join();
		delete threads[i];

		if (channels[i])
		{
			delete channels[i];
			channels[i] = nullptr;
		}
	}
}
int main(int argc, char** argv) {
	int port, party;
	parse_party_and_port(argv, &party, &port);
	if (argc > 3)
	{
		test_threads(1, port);
	}
	else
	{
		NetIO * io = new NetIO(party == ALICE ? nullptr : "127.0.0.1", port);

		setup_semi_honest(io, party);
		test_millionare(party, atoi(argv[3]));
		test_sort(party);
		delete io;
	}
	
}
