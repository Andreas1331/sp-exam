#include <vector>
#include <iomanip>

#include "candlestick.hpp"

namespace ticker_essentials {
    candlestick candlestick::calculate_candlestick(const std::vector<trade_stamp> &trades) {
        candlestick stick{};
        stick.time = {trades[0].time};
        stick.opening_price = {trades[0].price};
        stick.closing_price = {trades[trades.size() - 1].price};
        /** Now find the highest and lowest price that the stock was purchased for in this range */
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

    std::ostream &candlestick::print(std::ostream &os) const {
        os << "Candle " << time.tm_mday << '/' << time.tm_mon << '/' << (1900 + time.tm_year) << std::endl;
        os << std::left << std::setw(CANDLE_PRINT_WIDTH) << "Lowest:" << std::setw(CANDLE_PRINT_WIDTH) << "Highest:"
           << std::setw(CANDLE_PRINT_WIDTH) << "Opening:" << std::setw(CANDLE_PRINT_WIDTH) << "Closing:" << '\n';
        os << std::left << std::setw(CANDLE_PRINT_WIDTH) << lowest << std::setw(CANDLE_PRINT_WIDTH) << highest
           << std::setw(CANDLE_PRINT_WIDTH) << opening_price << std::setw(CANDLE_PRINT_WIDTH) << closing_price << '\n';
        os << "__________________________________________________________________________________" << '\n';
        return os;
    }
}
