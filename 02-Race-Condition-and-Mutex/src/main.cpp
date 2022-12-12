#include <iostream>
#include <vector>
#include <array>
#include <ranges>
#include <random>
#include <cmath>
#include <limits>
#include <thread>
#include <mutex>
#include "ChiliTimer2.h"

constexpr size_t DATA_SIZE = 5000000;

void ProcessDataSet(std::array<int, DATA_SIZE> &set, int &sum, std::mutex &mtx) {

    for (int x : set)
    {
        std::lock_guard g { mtx };
        constexpr auto limit = (double)std::numeric_limits<int> ::max();
        const auto y = (double)x / limit;
        sum+= int(std::sin(std::cos(y)) * limit);
    }
}


int main()
{
    std::minstd_rand randon_number_engine;
    std::vector<std::array<int, DATA_SIZE>> datasets{ 4 };
    std::vector<std::thread> workers;
    ChiliTimer2 timer;
    int sum = 0;
    std::mutex mtx;

    for (auto &arr: datasets)
    {
        std::ranges::generate(arr, randon_number_engine);
    }

    timer.Mark();
        
    for ( auto &set: datasets)
    {
        workers.push_back(std::thread{ ProcessDataSet, std::ref(set), std::ref(sum), std::ref(mtx)});
        //ProcessDataSet(set);
    }

    for (auto &w : workers) {
        w.join();
    }

    auto time_passed= timer.Peek();

    std::cout << "Processing datasets took " << time_passed << " seconds" << std::endl;
    std::cout << "Sum of the all element is " << sum << std::endl;

    return 0;
}

