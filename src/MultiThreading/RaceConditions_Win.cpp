/*
		Race Condition and Mutex example in Windows Operating System using C++
		You have to include TimerClass header and source file which is given in the same repo.!!
*/

#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <thread>
#include <assert.h>
#include <mutex>
#include "TimerClass.h"

constexpr size_t DATASETS_SIZE = 10000000;

void ProcessDataSet(std::array<int, DATASETS_SIZE>& set,int& sum,std::mutex& mtx)
{
	for (int x : set)
	{
		mtx.lock();
		constexpr auto limit = (double)std::numeric_limits<int>::max();
		const auto y = (double)x / limit;
		sum += int(std::sin(std::cos(y)) * limit);
		mtx.unlock();
	}
}

int main()
{
	std::mt19937 rng;
	std::uniform_int_distribution<int> rand(-100,100);
	TimerClass timer;
	std::vector<std::thread> threads;
	std::vector<std::array<int, DATASETS_SIZE>> datasets{4};

	timer.Mark();
	for (auto& a : datasets)	
	{
		for (int& x : a)
		{
			x = rand( rng );
		}
	}

	int sum {0};
	std::mutex mtx;

	for (auto& set : datasets)
	{	
		threads.push_back(std::thread(ProcessDataSet,std::ref(set),std::ref(sum),std::ref(mtx)));
	}

	for (auto& t: threads)
	{
		t.join();
	}

	auto t = timer.Peek();

	std::cout << "It took " << t << " seconds\n";
	std::cout << "Result is " << sum << "\n";

	return 0;
}
