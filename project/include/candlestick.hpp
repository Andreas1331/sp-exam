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

        /** visitor support with read-only access, e.g. for writing-out */
        template <typename Visitor>
        void accept_reader(Visitor&& visit) const
        {
            visit("time", time);
            visit("highest", highest);
            visit("lowest", lowest);
            visit("opening_price", opening_price);
            visit("closing_price", closing_price);
        }

        /** Prints the candlestick in a 'table' format */
        std::ostream& print(std::ostream& os) const;

        static candlestick calculate_candlestick(const std::vector<ticker_essentials::trade_stamp> &trades);
    };
}

#endif //PROJECT_CANDLESTICK_HPP
