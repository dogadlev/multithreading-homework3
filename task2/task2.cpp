#include <iostream>
#include <algorithm>
#include <iterator>
#include <future>


template <class Iterator, class Function>
void parallel_for_each(Iterator first, Iterator last, Function func)
{
	unsigned long long int const length = std::distance(first, last);
	unsigned long long int const maxChunkSize = 25;
	if (length <= maxChunkSize)
	{
		for_each(first, last, func);
	}
	else
	{
		Iterator midPoint = first;
		std::advance(midPoint, length / 2);
		std::future<void> firstHalfResult = std::async(std::launch::async, parallel_for_each<Iterator, Function>, first, midPoint, func);
		parallel_for_each(midPoint, last, func);
	}
}

void printVec(std::vector<int>& vec)
{
	for (const auto& v : vec)
		std::cout << v << " ";
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	std::vector<int> vec(10);
	std::generate(vec.begin(), vec.end(), []() {return rand() % 10; });
	printVec(vec);
	parallel_for_each(vec.begin(), vec.end(), [](auto& el) { return el *= el; });
	printVec(vec);

	return 0;
}