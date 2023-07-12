#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <Windows.h>
// Функция для расчета суммы двух векторов в определенном диапазоне
void sumVectorsRange(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, int start, int end)
{
	for (int i = start; i < end; ++i)
	{
		result[i] = a[i] + b[i];
	}
}

// функция для рассчета суммы двух векторов с использованием задонного колличества потоков

std::vector<int> sumVectors(const std::vector<int>& a, const std::vector<int>& b, int threadCount) {
	int size = a.size();
	std::vector<int> result(size);

	std::vector<std::thread> threads;
	int range = size / threadCount;

	for (int i = 0; i < threadCount; ++i) {
		int start = i * range;
		int end = (i == threadCount - 1) ? size : (i + 1) * range;

		threads.emplace_back(sumVectorsRange, std::ref(a), std::ref(b), std::ref(result), start, end);
	}

	for (auto& thread : threads) {
		thread.join();
	}
	return result;	
}

int main() {
	setlocale(LC_ALL, "Russian");

	//Количество доступных ядер
	int num_Threads = std::thread::hardware_concurrency();
	std::cout << "Доступное количество ядер: " << num_Threads << std::endl;

	std::vector<int> sizeVector = { 1000, 10000, 100000, 1000000 };
	std::vector<int> threadCounts = { 1, 2, 4, 8, 16 };

	std::vector<std::vector<double>> resultPrint(threadCounts.size(),std::vector<double>(sizeVector.size()));

	for (size_t i = 0; i < sizeVector.size(); ++i) {
		int size = sizeVector.size();
		std::vector<int> a(size, 3);
		std::vector<int> b(size, 5);

		for (size_t j = 0; j < threadCounts.size(); ++j) {
			int tCount = threadCounts[j];
			auto start = std::chrono::high_resolution_clock::now();
			std::vector<int> result = sumVectors(a, b, tCount );
			auto end = std::chrono::high_resolution_clock::now();

			auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
			double durationSec = duration.count() / 100000.0;

			resultPrint[j][i] = durationSec;

		}
	}

	// Вывод результата
	for (size_t i = 0; i < sizeVector.size(); ++i) {
		std::cout << "\t\t" << sizeVector[i];
	}
	std::cout << std::endl;
	for (size_t i = 0; i < threadCounts.size(); ++i) {
		int threadCount = threadCounts[i];
		std::cout << threadCount << " потоков \t";
		for (size_t j = 0; j < sizeVector.size(); ++j) {
			double dur = resultPrint[i][j];
			std::cout << dur << "s \t";
		}
		std::cout << std::endl;
	}
	
}