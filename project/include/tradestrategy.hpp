//
// Created by andre on 08-05-2022.
//

#ifndef PROJECT_TRADESTRATEGY_HPP
#define PROJECT_TRADESTRATEGY_HPP

#include <tuple>
#include <vector>

#include "candlestick.hpp"
#include "tradestamp.hpp"

namespace ticker_strategies {
    class tradestrategy {
    protected:
        using vector_c = std::vector<ticker_essentials::candlestick>;
        using vector_t = std::tuple<std::vector<double>, std::vector<double>>;

        /** We do not allow stocks owned to be in fractions */
        double current_money, initial_money;
        int stocks_amount{0}, purchases{0}, sales{0};
        std::vector<ticker_essentials::trade_stamp> trades {};

        /** Purchase as many stocks as we can */
        void buy(const double &unit_price, const std::tm &time) {
            if(unit_price <= 0)
                return;
            int stocks_bought = current_money / unit_price;
            if (stocks_bought > 0) {
                stocks_amount += stocks_bought;
                /** Design choice to avoid modulus to be as precise as possible */
                current_money -= unit_price * stocks_bought;
                purchases++;
                trades.push_back(ticker_essentials::trade_stamp{
                    time, unit_price, stocks_bought, 0, 1000, "adch18", "Marius"
                });
            }
        }

        /** Sell every single stock at the provided unit price */
        void sell(const double &unit_price) {
            if(unit_price <= 0)
                return;
            if (stocks_amount > 0) {
                current_money += unit_price * stocks_amount;
                stocks_amount = 0;
                sales++;
            }
        }

    public:
        static constexpr int OVERBOUGHT_BOUNDARY = 80;
        static constexpr int OVERSOLD_BOUNDARY = 20;

        explicit tradestrategy(double money) : initial_money{money}, current_money{money} {};

        virtual void run_strategy(const vector_t &oscillators,
                                  const std::vector<ticker_essentials::candlestick> &candles) = 0;

        /** Prints the strategy in a 'table' format */
        std::ostream &print_result(std::ostream &os) const {
            for(const auto &t : trades){
                t.print(std::cout);
            }
            os << "Money" << "\t\t" << "Purchases" << '\t' << "Sales" << '\n';
            os << current_money << "\t\t" << purchases << '\t' << sales << '\n';
            os << "__________________________________________________________________________________" << '\n';
            return os;
        };
    };
}

#endif //PROJECT_TRADESTRATEGY_HPP
