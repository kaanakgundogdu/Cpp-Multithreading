#include <iostream>
#include <vector>
#include <array>
#include <random>
#include <ranges>
#include <cmath>
#include <limits>
#include <thread>
#include "ChiliTimer.h"

constexpr size_t DATA_SIZE = 5000000;

void ProcessDataSet(std::array<int, DATA_SIZE> &set) {

    for (int x : set)
    {
        constexpr auto limit = (double)std::numeric_limits<int> ::max();
        const auto y = (double)x / limit;
        set[0] += int(std::sin(std::cos(y)) * limit);
    }
}


int main()
{
    std::minstd_rand randon_number_engine;
    std::vector<std::array<int, DATA_SIZE>> datasets{ 4 };
    std::vector<std::thread> workers;
    ChiliTimer timer;

    for (auto &arr: datasets)
    {
        std::ranges::generate(arr, randon_number_engine);
    }

    timer.Mark();
        
    for ( auto &set: datasets)
    {
        workers.push_back(std::thread{ ProcessDataSet, std::ref(set) });
        //ProcessDataSet(set);
    }

    for (auto &w : workers) {
        w.join();
    }

    auto time_passed= timer.Peek();

    std::cout << "Processing datasets took " << time_passed << " seconds" << std::endl;
    return 0;
}

