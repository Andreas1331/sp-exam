//
// Created by andre on 07-05-2022.
//

#include <vector>

#include "candlestick.hpp"
#include "ticker.hpp"

candlestick candlestick::calculate_candlestick(const std::vector<trade_stamp> &trades) {
    candlestick stick{};
    stick.opening_price = {trades[0].price};
    stick.closing_price = {trades[trades.size() - 1].price};
    // Now find the highest and lowest price that the stock was purchased for in this range
    stick.lowest = {
            std::min_element(trades.begin(), trades.end(), [](const trade_stamp &t1, const trade_stamp &t2) {
                return t1.price < t2.price;
            })->price};
    stick.highest = {
            std::max_element(trades.begin(), trades.end(), [](const trade_stamp &t1, const trade_stamp &t2) {
                return t1.price < t2.price;
            })->price};

    return stick;
}