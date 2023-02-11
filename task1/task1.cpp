#include <iostream>
#include <vector>
#include <algorithm>
#include <thread>
#include <future>
#include <exception>


void getMinElement(const std::vector<int>::iterator& itrStart, std::vector<int>& vec, std::promise<std::vector<int>::iterator>& prom)
{
	if (vec.empty())
		throw std::runtime_error("Vector is empty!");
	prom.set_value(std::min_element(itrStart, vec.end()));
}

void printVec(std::vector<int>& vec)
{
	if (vec.empty())
		throw std::runtime_error("Vector is empty!");
	for (const auto& v : vec)
		std::cout << v << " ";
	std::cout << std::endl;
}

int main(int argc, char** argv)
{
	try
	{
		std::vector<int> vec(10);
		std::generate(vec.begin(), vec.end(), []() {return rand() % 10; });
		printVec(vec);
		auto itrStart = vec.begin();
		do
		{
			std::promise<std::vector<int>::iterator> pRes;
			auto fRes = pRes.get_future();
			std::future<void> result = std::async(getMinElement, ref(itrStart), ref(vec), ref(pRes));
			fRes.wait();
			std::swap(*itrStart, *fRes.get());
			++itrStart;
		} while (itrStart < vec.end());
		printVec(vec);
	}
	catch (std::runtime_error& ex) { std::cout << ex.what(); }
	catch (...) { std::cout << "Some exception has occurred!"; }

	return 0;
}