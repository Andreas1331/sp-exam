//
// Created by andre on 10-05-2022.
//

#ifndef PROJECT_HOLDINGSTRATEGY_HPP
#define PROJECT_HOLDINGSTRATEGY_HPP

#include "tradestrategy.hpp"

namespace ticker_strategies{
    class holding_strategy : public tradestrategy {
    public:
        explicit holding_strategy(int money) : tradestrategy("Holding strategy", money){};

        void run_strategy(const vector_t &oscillators,
                          const vector_c &candles) override {

            /** It is said to be impossible to beat the market, and instead just buy and hold. Let us see. */
            buy(candles[0].opening_price, candles[0].time);
            sell(candles[candles.size() - 1].opening_price, candles[candles.size() - 1].time);
        };
    };
}


#endif //PROJECT_HOLDINGSTRATEGY_HPP
