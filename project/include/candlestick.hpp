//
// Created by andre on 07-05-2022.
//

#ifndef PROJECT_CANDLESTICK_HPP
#define PROJECT_CANDLESTICK_HPP

#include <iostream>

struct trade_stamp; // Forward declaration to avoid circular-dependencies

class candlestick{
public:
    double highest, lowest;
    double opening_price, closing_price;

    static candlestick calculate_candlestick(const std::vector<trade_stamp> &trades);
    /** Prints the candlestick in a table format */
    std::ostream& print(std::ostream& os) const;
};

#endif //PROJECT_CANDLESTICK_HPP
