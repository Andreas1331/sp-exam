//
// Created by andre on 08-05-2022.
//

#ifndef PROJECT_TRADESTRATEGY_HPP
#define PROJECT_TRADESTRATEGY_HPP

#include <tuple>
#include <vector>

#include "candlestick.hpp"

class tradestrategy {
protected:
    using vector_c = std::vector<candlestick>;
    using vector_t = std::tuple<std::vector<double>, std::vector<double>>;

    /** We do not allow stocks owned to be in fractions */
    double money;
    int stocks_amount{0}, purchases{0}, sales{0};
public:
    explicit tradestrategy(double money) : money{money} {}

    virtual void run_strategy(const vector_t &oscillators,
                              const std::vector<candlestick> &candles) = 0;

    /** Purchase as many stocks as we can */
    void buy(const double& unit_price) {
        int stocks_bought = money / unit_price;
        stocks_amount += stocks_bought;
        /** Design choice to avoid modulus to be as precise as possible */
        money -= unit_price * stocks_bought;
        purchases++;
    }

    void sell(const double& unit_price){
        money += unit_price * stocks_amount;
        stocks_amount = 0;
        sales++;
    }

    /** Prints the candlestick in a table format */
    std::ostream& print_result(std::ostream& os) const {
        return os;
    };
};

#endif //PROJECT_TRADESTRATEGY_HPP
