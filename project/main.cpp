#include <iostream>
#include <fstream>
#include <future>

#include "json.hpp"
#include "ticker.hpp"
#include "assignmentstrategy.hpp"
#include "holdingstrategy.hpp"
#include "benchmark.hpp"

using namespace ticker_strategies;
using namespace ticker_essentials;
using namespace ticker_benchmark;

constexpr double STARTING_MONEY = 10000;
constexpr double BENCHMARK_TIMES = 100;

/** A select few of the JSON files for testing, path may change depending on from where execution takes place */
const std::string DANSKE_BANK{"../DANSKE.json"};
const std::string FLS{"../FLS.json"};
const std::string PNDORA{"../PNDORA.json"};

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

template<typename T1>
void save_data_to_json(const std::string &file_name, T1 data){
    std::ofstream file {file_name};
    file << json_t{data};
    file.close();
}

void analyze_ticker(const ticker &t){
    /** Assignment 3) The parameter will allow for control over the desired period that makes up the candlesticks.
     * The chosen parameter is 1-day as this is the most typical depending on what
     * kind of trader we're talking about. */
    const auto candles = t.get_candlesticks(ticker::candle_intervals::day);
    for (const auto& stick: candles) {
        stick.print(std::cout);
    }

    /** Assignment 4) The ticker class can take different kinds of candlesticks for the stochastic indicators
     * with varying trading periods used. */
    const auto indicators = t.get_stochastic_indicators(candles, 14, 3);

    /** I save some of the data to be loaded in Python later for plotting */
    //save_data_to_json("candle_data.JSON",candles);
    //save_data_to_json("indicators.JSON",indicators);

    /** Assignment 5) assignment_strategy tried to follow the description provided in the exam and holding_strategy
     * is a classic attempt at not selling. */
    auto assign_strat = assignment_strategy(STARTING_MONEY);
    auto hold_strat = holding_strategy(STARTING_MONEY);

    /** Assignment 9)
     * (Please take a look at the provided images in the ZIP folder for the printed results)
     * What I did to speed up the analyses: I changed the input for run_strategy(..) to be of call-by-reference,
     * as this will prevent calling the copy-constructor each time run_strategy(..) is invoked. This is much more efficient,
     * and quickly changed the runtime to be almost twice as fast. The confidence levels are sufficiently low for me to make,
     * the assumption that I've tested using enough iterations. */
    auto bm = benchmark{};
    size_t id = bm.add("Strategy-Measure-After-Fix");
    /** Execute the strategy and measure the time it took */
    for(int i = 0; i < BENCHMARK_TIMES; i++){
        assign_strat.run_strategy(indicators, candles);
        assign_strat.reset();
        bm.measure(id);
    }
    bm.print(std::cout);

    /** Run the strategies for assignment 5 */
    assign_strat.run_strategy(indicators, candles);
    assign_strat.print_result(std::cout);

    hold_strat.run_strategy(indicators, candles);
    hold_strat.print_result(std::cout);
}

int main() {
    /** Assignment 7) Start three asynchronously threads to load three tickers into memory at the same time */
    auto fut1 = std::async(std::launch::async, load_ticker, DANSKE_BANK);
    auto fut2 = std::async(std::launch::async, load_ticker, FLS);
    auto fut3 = std::async(std::launch::async, load_ticker, PNDORA);

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

