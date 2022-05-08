//
// Created by andre on 08-05-2022.
//

#ifndef PROJECT_ASSIGNMENTSTRATEGY_HPP
#define PROJECT_ASSIGNMENTSTRATEGY_HPP

#include "tradestrategy.hpp"

namespace ticker_strategies{

    class assignment_strategy : public tradestrategy {
    public:
        explicit assignment_strategy(int money) : tradestrategy(money){};
        void run_strategy(const vector_t &oscillators,
                          const vector_c &candles) override {

            bool make_purchase {false};
            bool sell_everything {false};
            /** Go through each candle */
            for(int i = 0; i < candles.size(); i++){
                if(make_purchase){
                    buy(candles[i].opening_price, candles[i].time);
                }
                if(sell_everything){
                    sell(candles[i].opening_price);
                }

                /** Figure out whether to either buy or sell the following day */
                double K = (std::get<0>(oscillators)[i]);
                double D = (std::get<1>(oscillators)[i]);

                make_purchase = (K <= tradestrategy::OVERSOLD_BOUNDARY && K >= D);
                sell_everything = (K >= tradestrategy::OVERBOUGHT_BOUNDARY && K <= D);
            }

            sell(candles[candles.size() - 1].opening_price);
        };
    };
}

#endif //PROJECT_ASSIGNMENTSTRATEGY_HPP
