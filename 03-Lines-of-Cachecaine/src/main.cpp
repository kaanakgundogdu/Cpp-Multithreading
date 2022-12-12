#include <iostream>
#include <vector>
#include <array>
#include <ranges>
#include <random>
#include <cmath>
#include <limits>
#include <thread>
#include <mutex>
#include "ChiliTimer.h"

constexpr size_t DATA_SIZE = 5000000;

void ProcessDataSet(std::array<int, DATA_SIZE> &set, int &sum) {

    for (int x : set)
    {
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
    ChiliTimer timer;

    //making different cache line
    //64 byte
    struct sums_type{
        int value = 0;
        char padding[60];
    };

    sums_type sum[4];

    std::mutex mtx;

    for (auto &arr: datasets)
    {
        std::ranges::generate(arr, randon_number_engine);
    }

    timer.Mark();
    int i = 0;
    for ( auto &set: datasets)
    {
        workers.push_back(std::thread{ ProcessDataSet, std::ref(set), std::ref(sum[i].value)});
        //ProcessDataSet(set);
        i++;
    }

    for (auto &w : workers) {
        w.join();
    }

    auto time_passed= timer.Peek();

    std::cout << "Processing datasets took " << time_passed << " seconds" << std::endl;
    std::cout << "Sum of the all element is " << (sum[0].value+ sum[1].value + sum[2].value + sum[3].value ) << std::endl;

    return 0;
}

