#include <iostream>
#include <fstream>

#include "json.hpp"
#include "ticker.hpp"

const std::string FILE_TO_OPEN{"../DANSKE.json"};

int main() {
    trade_stamp l {};

    // Exercise 1 & 2)
    std::ifstream file{FILE_TO_OPEN};
    if (file) {
        auto t = ticker{};
        file >> json_t{t};
        std::cout << t.name << std::endl;
        t.sort_trades();


        auto data = t.get_candlesticks([](const trade_stamp &t1, const trade_stamp &t2) {
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

