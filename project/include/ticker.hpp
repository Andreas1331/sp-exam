//
// Created by andre on 06-05-2022.
//

#ifndef PROJECT_TICKER_HPP
#define PROJECT_TICKER_HPP

#include <string>
#include <vector>

struct trade_stamp {
    int price;
    int amount;
    std::string buyer;
    std::string seller;
    int seq;
    int code;
    //trades [{time, price, amount, buyer, seller, seq, code}]

    /** visitor support with full access, e.g. for reading-in */
    template <typename Visitor>
    void accept_writer(Visitor&& visit)
    {
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
class ticker
{
public:
    std::string name;
    std::string url;
    std::string tag;
    std::string isin;
    std::string market;
    std::string sector;
    std::string segment;
    int shares;
    std::vector<trade_stamp> trades;

    /** visitor support with full access, e.g. for reading-in */
    template <typename Visitor>
    void accept_writer(Visitor&& visit)
    {
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
};

#endif //PROJECT_TICKER_HPP
