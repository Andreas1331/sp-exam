#include <iostream>
#include <fstream>
#include <future>

#include "json.hpp"
#include "ticker.hpp"
#include "assignmentstrategy.hpp"
#include "holdingstrategy.hpp"
#include "benchmark.hpp"

/** A select few of the JSON files for testing, path may change depending on from where execution takes place */
const std::string DANSKE_BANK{"../DANSKE.json"};
const std::string CARL_B{"../CARLB.json"};
const std::string NOVOZYMES{"../NZYMB.json"};

using namespace ticker_strategies;
using namespace ticker_essentials;

constexpr int STARTING_MONEY = 10000;
constexpr int BENCHMARK_TIMES = 100;

ticker load_ticker(const std::string &path){
    /** Assignment 1 & 2) */
    std::ifstream file{path};
    if (file) {
        auto t = ticker{};
        file >> json_t{t};
        /** Assignment 6) Makes use of lambda expression to sort the trades by their tm value converted to mktime */
        t.sort_trades();

        return t;
    } else {
        throw std::logic_error("File " + path + " not found");
    }

}

void analyze_ticker(const ticker &t){
    /** Assignment 3) The predicate will allow for control over the desired period that makes up the candlesticks.
     * The predicate below will aggregate all matching days into a set of candlesticks. */
    const auto candles = t.get_candlesticks(ticker::candle_intervals::day);
    /*
    for (const auto& stick: candles) {
        stick.print(std::cout);
    }
     */
    /** Assignment 4) The ticker class can provide multiple different kinds of candlesticks for the stochastic indicators */
    const auto indicators = t.get_stochastic_indicators(candles, 14, 3);

    /** Assignment 5) assignment_strategy tried to follow the description provided in the exam and holding_strategy
     * is a classic attempt at not selling. */
    auto assign_strat = assignment_strategy(STARTING_MONEY);
    auto hold_strat = holding_strategy(STARTING_MONEY);
    auto bm = benchmark{};
    size_t id = bm.add("Strategy-Measure-Before-Fix");
    /** Execute the strategy and measure the time it took */
    for(int i = 0; i < BENCHMARK_TIMES; i++){
        assign_strat.run_strategy(indicators, candles);
        bm.measure(id);
    }
    bm.print(std::cout);
    return;
    assign_strat.print_result(std::cout);

    hold_strat.run_strategy(indicators, candles);
    hold_strat.print_result(std::cout);
}

int main() {
    /** Assignment 7) Start three asynchronously threads to load three tickers into memory at the same time */
    auto fut1 = std::async(std::launch::async, load_ticker, DANSKE_BANK);
    auto fut2 = std::async(std::launch::async, load_ticker, CARL_B);
    auto fut3 = std::async(std::launch::async, load_ticker, NOVOZYMES);

    /** Store the futures and go through them one by one retrieving the results afterwards */
    auto futures = std::vector<std::future<ticker>>();
    futures.push_back(std::move(fut1));
    futures.push_back(std::move(fut2));
    futures.push_back(std::move(fut3));

    for(auto &future : futures){
        const ticker t = future.get();
        std::cout << "Loaded ticker " << t.name << std::endl;

        /** Only analyze one of the stocks - I just picked Danske Bank */
        if (t.name == "Danske Bank A/S")
            analyze_ticker(t);
    }

    return 0;
}

