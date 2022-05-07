#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <iterator>

#include "json.hpp"
#include "ticker.hpp"
#include "candlestick.hpp"

const std::string FILE_TO_OPEN{"../DANSKE.json"};

template<typename T, typename Pred>
auto FilterRangesCopyIf(const std::vector<T> &vec, Pred p) {
    std::vector<T> out;
    std::ranges::copy_if(vec, std::back_inserter(out), p);
    return out;
}

int main() {
    /*
    std::vector<int> numbers {1,2,3,4,4,4,5,6,7,1};
    auto& first = numbers[3];
    auto n2 = std::vector<int>{};
    for(auto p : numbers){
        if(p == first)
            n2.push_back(p);
    }
    std::cout << numbers.size() << std::endl;
    numbers.erase(std::remove_if(numbers.begin(), numbers.end(), [](int p){return p == 4;}), numbers.end());
    std::cout << numbers.size() << std::endl;
    for(auto k : numbers){
        std::cout << k << std::endl;
    }
     */
    //const int& first = numbers[0];
    //std::cout << first << std::endl;
    //auto n2 = FilterRangesCopyIf(numbers, [](int v){return v == 1;});
    //std::vector<int>::iterator n2 = std::find_if(numbers.begin(), numbers.end(), [first](const int& i) {
    //  return i == first;
    //});
    //std::cout << n2[9] << std::endl;


    // Exercise 1 & 2)
    std::ifstream file{FILE_TO_OPEN};
    if (file) {
        auto t = ticker{};
        file >> json_t{t};
        std::cout << t.name << std::endl;
        t.sort_trades();


        auto data = t.get_candlestick([](const trade_stamp &t1, const trade_stamp &t2) {
            //std::cout << t2.time.tm_mon << t2.time.tm_year << t2.time.tm_mon << std::endl;
            return (t1.time.tm_year == t2.time.tm_year &&
                   t1.time.tm_mday == t2.time.tm_mday &&
                   t1.time.tm_mon == t2.time.tm_mon);
        });
        for (auto stick: data) {
            std::cout << "------------------------------" << std::endl;
            std::cout << stick.lowest << std::endl;
            std::cout << stick.highest << std::endl;
            std::cout << stick.opening_price << std::endl;
            std::cout << stick.closing_price << std::endl;
            std::cout << "------------------------------" << std::endl;
        }
    } else {
        std::cout << "No file found ..." << std::endl;
    }

    return 0;
}

