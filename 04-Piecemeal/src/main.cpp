#include <iostream>
#include <vector>
#include <array>
#include <ranges>
#include <random>
#include <cmath>
#include <limits>
#include <thread>
#include <mutex>
#include <span>
#include "ChiliTimer.h"

constexpr size_t DATA_SIZE = 50'000'000;

int Do_Whole_Data();
int Do_Smaller_Sets();

int main(int argc, char** argv) {
    if (argc > 1 && std::string{ argv[1] } == "--small")
    {
        return Do_Smaller_Sets();
    }
    return Do_Whole_Data();
}

void ProcessDataSet(std::span<int> set, int &sum) {

    for (int x : set)
    {
        constexpr auto limit = (double)std::numeric_limits<int> ::max();
        const auto y = (double)x / limit;
        sum+= int(std::sin(std::cos(y)) * limit);
    }
}

std::vector<std::array<int, DATA_SIZE>> Generate_Datasets() {
    std::minstd_rand randon_number_engine;
    std::vector<std::array<int, DATA_SIZE>> datasets{ 4 };

    for (auto& arr : datasets)
    {
        std::ranges::generate(arr, randon_number_engine);
    }
    return datasets;
}


int Do_Whole_Data()
{
    auto datasets = Generate_Datasets();
    std::vector<std::thread> workers;
    ChiliTimer timer;

    //making different cache line,64 byte
    struct sums_type{
        int value = 0;
        char padding[60];
    };

    sums_type sum[4];

    timer.Mark();
    for ( size_t i=0; i<4; i++)
    {
        workers.push_back(std::thread{ ProcessDataSet, std::span(datasets[i]), std::ref(sum[i].value)});
    }

    for (auto &w : workers) {
        w.join();
    }

    auto time_passed= timer.Peek();

    std::cout << "Processing datasets took " << time_passed << " seconds" << std::endl;
    std::cout << "Sum of the all element is " << (sum[0].value+ sum[1].value + sum[2].value + sum[3].value ) << std::endl;

    return 0;
}

int Do_Smaller_Sets() {
    auto datasets = Generate_Datasets();
    //this time used jthread look google for more info
    std::vector<std::jthread> workers;
    ChiliTimer timer;

    struct sums_type {
        int value = 0;
        char padding[60];
    };

    sums_type sum[4];

    timer.Mark();
    constexpr const int subset_size = DATA_SIZE / 10'000;

    int grand_total = 0;
    for (size_t i = 0; i < DATA_SIZE; i+= subset_size)
    {
        for (size_t j = 0; j < 4; j++)
        {
            workers.push_back(std::jthread{ ProcessDataSet, std::span( &datasets[j][i], subset_size), std::ref(sum[j].value)});
        }
        workers.clear();
        grand_total = sum[0].value + sum[1].value + sum[2].value + sum[3].value;

    }
    const auto time_passed = timer.Peek();
    std::cout << "Processing datasets took " << time_passed << " seconds" << std::endl;
    std::cout << "Sum of the all element is " << grand_total<< std::endl;

    return 0;
}
