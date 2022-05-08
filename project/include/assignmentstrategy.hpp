//
// Created by andre on 08-05-2022.
//

#ifndef PROJECT_ASSIGNMENTSTRATEGY_HPP
#define PROJECT_ASSIGNMENTSTRATEGY_HPP

#include "tradestrategy.hpp"

class assignment_strategy : protected tradestrategy {
public:
    explicit assignment_strategy(int money) : tradestrategy(money){};
    void run_strategy(const vector_t &oscillators,
                      const vector_c &candles) override {

        bool make_purchase {false};
        bool sell_everything {false};
        /** Go through each candle */
        for(int i = 0; i < candles.size(); i++){
            if(make_purchase){
                buy(candles[i].opening_price);
            }else{
                /** Figure out whether to buy or not the following day */

                make_purchase = true;
            }

            if(sell_everything){
                sell(candles[i].opening_price);
            }
        }
    };
};

#endif //PROJECT_ASSIGNMENTSTRATEGY_HPP
