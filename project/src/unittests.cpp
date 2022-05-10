#ifndef PROJECT_UNITTESTS_CPP
#define PROJECT_UNITTESTS_CPP

#include <doctest.h>
#include "assignmentstrategy.hpp"
#include "meta.hpp"
#include "candlestick.hpp"

template<typename T>
bool is_tm_wrapper(T obj){
    return is_tm_v<T>;
}

TEST_CASE("meta_tm_type")
{
    /** tm object for testing */
    const std::tm t {};
    CHECK(is_tm_v<std::tm> == true);
    CHECK(is_tm_wrapper(t) == true);
    CHECK(is_tm_v<bool> == false);
}

TEST_CASE("strategy"){
    /** Not a whole lot to test due to accessible level of member functions */
    auto strat = ticker_strategies::assignment_strategy{100000};
    SUBCASE("constant_values"){
        CHECK(strat.OVERBOUGHT_BOUNDARY == 80);
        CHECK(strat.OVERSOLD_BOUNDARY == 20);
    }
}

TEST_CASE("candlestick"){
    /** Create a few tradestamps for testing */
    std::vector<ticker_essentials::trade_stamp> trades {};
    trades.push_back(ticker_essentials::trade_stamp{std::tm{2, 42, 6, 30, 12, 100}, 99, 120, 0, 0, "Nordnet", "SAXO"});
    trades.push_back(ticker_essentials::trade_stamp{std::tm{4, 32, 7, 30, 12, 100}, 102, 1300, 0, 0, "Nordnet", "SAXO"});
    trades.push_back(ticker_essentials::trade_stamp{std::tm{56, 32, 14, 30, 12, 100}, 100, 54, 0, 0, "Nordnet", "SAXO"});

    const auto candle = ticker_essentials::candlestick::calculate_candlestick(trades);

    /** Based on the provided tradestamps the values are expected to be as follows: */
    CHECK(candle.opening_price == 99);
    CHECK(candle.closing_price == 100);
    CHECK(candle.highest == 102);
    CHECK(candle.lowest == 99);
}
#endif //PROJECT_UNITTESTS_CPP
