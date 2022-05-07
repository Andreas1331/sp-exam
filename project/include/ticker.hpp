//
// Created by andre on 06-05-2022.
//

#ifndef PROJECT_TICKER_HPP
#define PROJECT_TICKER_HPP

#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <math.h>
#include <tuple>

#include "candlestick.hpp"

struct trade_stamp {
    std::tm time;
    double price;
    int amount, seq, code;
    std::string buyer, seller;

    /** visitor support with full access, e.g. for reading-in */
    template<typename Visitor>
    void accept_writer(Visitor &&visit) {
        visit("time", time);
        visit("price", price);
        visit("amount", amount);
        visit("buyer", buyer);
        visit("seller", seller);
        visit("seq", seq);
        visit("code", code);
    }
};

/** A custom class to contain all information related to a specific ticker e.g name, trades.
 * The class also implements the visitor pattern to allow for JSON parsing */
class ticker {
public:
    std::string name, url, tag, isin, market, sector, segment;
    int shares;
    std::vector<trade_stamp> trades;

    /** visitor support with full access, e.g. for reading-in */
    template<typename Visitor>
    void accept_writer(Visitor &&visit) {
        visit("tag", tag);
        visit("isin", isin);
        visit("shares", shares);
        visit("name", name);
        visit("url", url);
        visit("market", market);
        visit("sector", sector);
        visit("segment", segment);
        visit("trades", trades);
    }

    void sort_trades() {
        std::sort(trades.begin(), trades.end(), [](trade_stamp &t1, trade_stamp &t2) {
            return std::mktime(&t1.time) < std::mktime(&t2.time);
        });
    }

    std::tuple<std::vector<double>, std::vector<double>>
    get_stochastic_indicators(const std::vector<candlestick> &candlesticks, const int &&trading_period_X,
                              const int &&number_of_periods_Y) const {
        // Pre-allocate enough storage for our values
        std::vector<double> blueCurve(candlesticks.size());
        std::vector<double> redCurve(candlesticks.size());

        // Calculate the fast-moving oscillator
        for (int i = 0; i < candlesticks.size(); i++) {
            int start_index = (i - trading_period_X);
            start_index = (start_index < 0) ? 0
                                            : start_index; // Make sure we do not grab unknown values from our sticks vector
            int most_recent_index = i - 1;
            most_recent_index = (most_recent_index < 0) ? 0 : most_recent_index;

            const double L = {std::min_element(candlesticks.begin() + start_index, candlesticks.begin() + i,
                                               [](const candlestick &c1, const candlestick &c2) {
                                                   return c1.lowest < c2.lowest;
                                               })->lowest};

            const double H = {std::max_element(candlesticks.begin() + start_index, candlesticks.begin() + i,
                                               [](const candlestick &c1, const candlestick &c2) {
                                                   return c1.highest < c2.highest;
                                               })->highest};
            const double C = candlesticks[most_recent_index].closing_price;

            double K = ((C - L) / (H - L) * 100);

            K = std::isnan(K) ? 0
                              : K; // As the data provided in the assignment is dirty, values will sometimes become NaN
            blueCurve[i] = std::move(K);
        }
        

        return std::make_tuple(blueCurve, redCurve);
    }

    template<typename StickPredicate>
    std::vector<candlestick> get_candlesticks(StickPredicate pred) {
        std::vector<candlestick> candlesticks{};
        std::vector<trade_stamp> tmp_trades{trades}; // For now, make a copy..

        while (!tmp_trades.empty()) {
            const trade_stamp t = tmp_trades[0];
            std::vector<trade_stamp> candle_stamps{};
            // Find all matching trades using the predicate
            for (const auto &t_tmp: tmp_trades) {
                if (pred(t_tmp, t)) {
                    // TODO: Try to remove the t_tmp from tmp_trades here instead of waiting until outside the for-loop
                    candle_stamps.push_back(t_tmp);
                }
            }

            candlesticks.push_back(candlestick::calculate_candlestick(candle_stamps));
            // Now remove the same elements from the tmp_trades
            tmp_trades.erase(std::remove_if(tmp_trades.begin(), tmp_trades.end(),
                                            [&t, &pred](const auto &v) { return pred(v, t); }), tmp_trades.end());
        }

        return candlesticks;
    }
};

#endif //PROJECT_TICKER_HPP
