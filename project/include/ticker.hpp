//
// Created by andre on 06-05-2022.
//

#ifndef PROJECT_TICKER_HPP
#define PROJECT_TICKER_HPP

#include <string>
#include <vector>
#include <ctime>
#include <iostream>
#include <cmath>
#include <tuple>
#include <numeric>
#include <functional>

#include "candlestick.hpp"
#include "tradestamp.hpp"

/** Will invoke the action for each index and provide the start index of the desired window size as well */
template<typename T1, typename T2>
void slide_window(const std::vector<T1> &in, const T2 action, const int &window_size) {
    int start_index, index_before_i;
    for (int i = 0; i < in.size(); i++) {
        start_index = (i - window_size);
        start_index = (start_index < 0) ? 0
                                        : start_index; // Make sure we do not use negative indices
        index_before_i = i - 1;
        index_before_i = (index_before_i < 0) ? 0 : index_before_i;

        action(start_index, index_before_i, i);
    }
}

template<typename T>
T get_average(std::vector<T> const &v, const int &total_elements, const int &start_offset, const int &end_offset,
              const T &&default_val) {
    if (v.empty()) {
        return default_val;
    }

    return std::reduce(v.begin() + start_offset, v.begin() + end_offset, default_val) / total_elements;
}

namespace ticker_essentials {
/** A custom class to contain all information related to a specific ticker e.g name, trades.
 * The class also implements the visitor pattern to allow for JSON parsing */
    class ticker {
    private:
        template<typename Comparer>
        [[nodiscard]] std::vector<candlestick> find_candlesticks(const Comparer &compare) const {
            std::vector<candlestick> candlesticks{};

            /** Set the current trade stamp and look for similar trades based on the comparer */
            trade_stamp current = trades[0];
            std::vector<trade_stamp> current_trades{};

            for (const auto &trade: trades) {
                if (compare(trade.time, current.time)) {
                    /** We accept copy-constructing as it is most likely not going to be a whole lot of objects */
                    current_trades.push_back({trade});
                } else {
                    candlesticks.push_back(candlestick::calculate_candlestick(current_trades));
                    current_trades.clear();
                    current = trade;
                    current_trades.push_back(current);
                }
            }

            return candlesticks;
        }

    public:
        std::string name, url, tag, isin, market, sector, segment;
        int shares;
        std::vector<trade_stamp> trades;

        enum class candle_intervals {
            hour, day, month, year
        };

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
        static get_stochastic_indicators(const std::vector<candlestick> &candlesticks, const int &&trading_period_X,
                                         const int &&number_of_periods_Y) {
            /** Pre-allocate enough storage for our values */
            std::vector<double> blueCurve(candlesticks.size());
            std::vector<double> redCurve(candlesticks.size());

            /** Calculate the fast-moving oscillator */
            slide_window(candlesticks,
                         [&blueCurve, &candlesticks](const auto start, const auto index_before_i, const auto index) {
                             const double L = {
                                     std::min_element(candlesticks.begin() + start, candlesticks.begin() + index,
                                                      [](const candlestick &c1, const candlestick &c2) {
                                                          return c1.lowest < c2.lowest;
                                                      })->lowest};

                             const double H = {
                                     std::max_element(candlesticks.begin() + start, candlesticks.begin() + index,
                                                      [](const candlestick &c1, const candlestick &c2) {
                                                          return c1.highest < c2.highest;
                                                      })->highest};
                             const double C = candlesticks[index_before_i].closing_price;

                             double K = ((C - L) / (H - L) * 100);

                             K = std::isnan(K) ? 0
                                               : K; // As the data provided in the assignment is dirty, values will sometimes become NaN
                             blueCurve[index] = {K};
                         }, trading_period_X);
            /** Calculate the slow-moving oscillator */
            slide_window(blueCurve,
                         [&redCurve, &blueCurve, &number_of_periods_Y](const auto start, const auto index_before_i,
                                                                       const auto index) {
                             double D = get_average(blueCurve, number_of_periods_Y, start, index, 0.0);
                             redCurve[index] = {D};
                         }, number_of_periods_Y);

            return std::make_tuple(blueCurve, redCurve);
        }

        std::vector<candlestick> get_candlesticks(const candle_intervals &&interval) {
            switch (interval) {
                case candle_intervals::hour: {
                    return find_candlesticks([](const std::tm &t1, const std::tm &t2) {
                        return t1.tm_year == t2.tm_year &&
                               t1.tm_mon == t2.tm_mon &&
                               t1.tm_mday == t2.tm_mday &&
                               t1.tm_hour == t2.tm_hour;
                    });
                }
                case candle_intervals::day: {
                    return find_candlesticks([](const std::tm &t1, const std::tm &t2) {

                        return t1.tm_year == t2.tm_year &&
                               t1.tm_mon == t2.tm_mon &&
                               t1.tm_mday == t2.tm_mday;
                    });
                }
                case candle_intervals::month: {
                    return find_candlesticks([](const std::tm &t1, const std::tm &t2) {
                        return t1.tm_year == t2.tm_year &&
                               t1.tm_mon == t2.tm_mon;
                    });
                }
                case candle_intervals::year: {
                    return find_candlesticks([](const std::tm &t1, const std::tm &t2) {
                        return t1.tm_year == t2.tm_year;
                    });
                }
                default:
                    return std::vector<candlestick>{};
            }
        }
    };
}

#endif //PROJECT_TICKER_HPP
