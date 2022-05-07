//
// Created by andre on 06-05-2022.
//

#ifndef PROJECT_TICKER_HPP
#define PROJECT_TICKER_HPP

#include <string>
#include <vector>
#include <time.h>
#include <candlestick.hpp>

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

    template<typename StickPredicate>
    std::vector<candlestick> get_candlestick(StickPredicate pred) {
        std::vector<candlestick> candlesticks{};
        std::vector<trade_stamp> tmp_trades{trades}; // For now, make a copy..

        while (!tmp_trades.empty()) {
            const trade_stamp t = tmp_trades[0];
            std::vector<trade_stamp> candle_stamps{};
            // Find all matching trades using the predicate
            for (const auto &t_tmp: tmp_trades) {
                if (pred(t_tmp, t)) {
                    candle_stamps.push_back(t_tmp);
                }
            }

            candlesticks.push_back(calculate_candlestick(candle_stamps));
            // Now remove the same elements from the tmp_trades
            tmp_trades.erase(std::remove_if(tmp_trades.begin(), tmp_trades.end(),
                                            [&t, &pred](const auto &v) { return pred(v, t); }), tmp_trades.end());
        }

        return candlesticks;
    }

private:
    candlestick calculate_candlestick(const std::vector<trade_stamp> &trades) {
        std::cout << "candle length: " << trades.size() << std::endl;
        candlestick stick{};
        stick.opening_price = {trades[0].price};
        stick.closing_price = {trades[trades.size() - 1].price};
        // Now find the highest and lowest price that the stock was purchased for in this range
        stick.lowest = {
                std::min_element(trades.begin(), trades.end(), [](const trade_stamp &t1, const trade_stamp &t2) {
                    return t1.price < t2.price;
                })->price};
        stick.highest = {
                std::max_element(trades.begin(), trades.end(), [](const trade_stamp &t1, const trade_stamp &t2) {
                    return t1.price < t2.price;
                })->price};

        return stick;
    }
};

#endif //PROJECT_TICKER_HPP
