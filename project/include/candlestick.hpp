#ifndef PROJECT_CANDLESTICK_HPP
#define PROJECT_CANDLESTICK_HPP

#include <iostream>
#include "tradestamp.hpp"

namespace ticker_essentials{
    class candlestick{
    private:
        static constexpr size_t CANDLE_PRINT_WIDTH = 12;
    public:
        std::tm time;
        double highest, lowest;
        double opening_price, closing_price;

        static candlestick calculate_candlestick(const std::vector<ticker_essentials::trade_stamp> &trades);
        /** Prints the candlestick in a 'table' format */
        std::ostream& print(std::ostream& os) const;
    };
}

#endif //PROJECT_CANDLESTICK_HPP
