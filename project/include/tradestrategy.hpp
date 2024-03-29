#ifndef PROJECT_TRADESTRATEGY_HPP
#define PROJECT_TRADESTRATEGY_HPP

#include <tuple>
#include <vector>
#include <string>

#include "candlestick.hpp"
#include "tradestamp.hpp"

namespace ticker_strategies {
    class tradestrategy {
    private:
        static constexpr size_t STRAT_PRINT_WIDTH = 20;
    protected:
        using vector_c = std::vector<ticker_essentials::candlestick>;
        using vector_t = std::tuple<std::vector<double>, std::vector<double>>;

        /** We do not allow stocks owned to be in fractions */
        std::string strategy_name;
        double current_money, initial_money;
        int stock_amount{0}, purchases{0}, sales{0};
        std::vector<ticker_essentials::trade_stamp> trades{};

        /** Buy as many stocks as we can */
        void buy(const double &unit_price, const std::tm &time) {
            if (unit_price <= 0)
                return;
            int stocks_bought = current_money / unit_price;
            if (stocks_bought > 0) {
                stock_amount += stocks_bought;
                /** Design choice to avoid modulus to be as precise as possible */
                current_money -= unit_price * stocks_bought;
                purchases++;
                trades.push_back(ticker_essentials::trade_stamp{
                        time, unit_price, stocks_bought, 0, 1000, "adch18", "Marius"
                });
            }
        }

        /** Sell every single stock at the provided unit price */
        void sell(const double &unit_price, const std::tm &time) {
            if (unit_price <= 0)
                return;
            if (stock_amount > 0) {
                trades.push_back(ticker_essentials::trade_stamp{
                        time, unit_price, stock_amount, 0, 1000, "Marius", "adch18"
                });
                current_money += unit_price * stock_amount;
                stock_amount = 0;
                sales++;
            }
        }

    public:
        static constexpr int OVERBOUGHT_BOUNDARY = 80;
        static constexpr int OVERSOLD_BOUNDARY = 20;

        explicit tradestrategy(std::string &&name, double money) : strategy_name{name}, initial_money{money},
                                                                 current_money{money} {};

        void reset(){
            current_money = initial_money;
            stock_amount = 0, purchases = 0, sales = 0;
            trades.clear();
        }

        virtual void run_strategy(const vector_t &oscillators,
                                  const std::vector<ticker_essentials::candlestick> &candles) = 0;
        /** Prints the strategy in a 'table' format */
        std::ostream &print_result(std::ostream &os) const {
            std::cout << "DISPLAYING STRATEGY RESULT FOR \"" << strategy_name << "\":" << std::endl;
            for (const auto &t: trades) {
                t.print(std::cout);
            }
            os << "_____________ OVERVIEW _____________" << '\n';
            os << std::left << std::setw(STRAT_PRINT_WIDTH) << "Money left (DKK):" << std::setw(STRAT_PRINT_WIDTH) << "Difference (DKK):"
               << std::setw(STRAT_PRINT_WIDTH) << "Purchases#:" << std::setw(STRAT_PRINT_WIDTH) << "Sales#:" << '\n';
            os << std::left << std::setw(STRAT_PRINT_WIDTH) << current_money << std::setw(STRAT_PRINT_WIDTH)
               << (current_money - initial_money) << std::setw(STRAT_PRINT_WIDTH) << purchases
               << std::setw(STRAT_PRINT_WIDTH) << sales << '\n';
            os << "__________________________________________________________________________________" << '\n';
            return os;
        };
    };
}

#endif //PROJECT_TRADESTRATEGY_HPP
