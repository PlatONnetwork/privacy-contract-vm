#include <gtest/gtest.h>
#include "safequeue.h"
#include "simple_timer.h"
#include <thread>
#include <iostream>
#include <random>

using namespace std;
using namespace platon::mpc;

static SafeQueue<std::string> que;

void pop()
{
	std::string word;
	int ret = 0;
	int count = 15;
	while (count--)
	{
		SimpleTimer timer;
		ret = que.pop(word, 1000);
		if (ret == 0)
			printf("cost time: %lf , pop a word: %s \n", timer.stop(), word.data());
		else if (ret == -2)
			printf("cost time: %lf , timeouot pop a word: %s \n", timer.stop(), word.data());
		else
			printf("cost time: %lf , failed pop a word: %s \n", timer.stop(), word.data());
	}

	printf("** pop ending !\n");
}

void push()
{
	int count = 10;
	while (count--)
	{
		std::string word = "hello-" + std::to_string(count );
		SimpleTimer timer;
		this_thread::sleep_for(std::chrono::milliseconds(100 * (rand() % 10)));
		que.push(word);
		printf("push cost time:  %lf, word: %s \n", timer.stop(), word.data());
	}

	printf("** push ending !\n");
}

TEST(TEST_QUEUE, TestTimeQue)
{
	srand(1000);
	thread th1(pop);
	thread th2(push);

	th1.join();
	th2.join();
	cout << "****   ending !!" << endl;
}