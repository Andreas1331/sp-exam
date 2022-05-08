#include <iostream>
#include <fstream>

#include "json.hpp"
#include "ticker.hpp"
#include "assignmentstrategy.hpp"

const std::string FILE_TO_OPEN{"../DANSKE.json"};

using namespace ticker_strategies;
using namespace ticker_essentials;

int main() {
    /** Assignment 1 & 2) */
    std::ifstream file{FILE_TO_OPEN};
    if (file) {
        auto t = ticker{};
        file >> json_t{t};
        /** Assignment 6) Makes use of lambda expression to sort the trades by their tm value converted to mktime */
        t.sort_trades();

        /** Assignment 3) The predicate will allow for control over the desired period that makes up the candlesticks.
         * The predicate below will aggregate all matching days into a set of candlesticks. */
        const auto candles = t.get_candlesticks(ticker::candle_intervals::day);
        for (const auto& stick: candles) {
            stick.print(std::cout);
        }
        /** Assignment 4) The ticker class can provide multiple different kinds of candlesticks for the stochastic indicators */
        const auto indicators = t.get_stochastic_indicators(candles, 14, 3);

        /** Assignment 5) assignment_strategy tried to follow the description provided in the exam */
        auto assign_strat = assignment_strategy(100000);
        assign_strat.run_strategy(indicators, candles);
        assign_strat.print_result(std::cout);


    } else {
        std::cout << "No file found ..." << std::endl;
    }

    return 0;
}

