//
// Created by andre on 08-05-2022.
//

#ifndef PROJECT_TRADESTAMP_HPP
#define PROJECT_TRADESTAMP_HPP

#include <ctime>
#include <string>

namespace ticker_essentials {
    struct trade_stamp {
    public:
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

        /** Prints the tradestamp in a 'table' format */
        std::ostream& print(std::ostream &os) const {
            os << "Date" << "\t\t" << "Price (DKK)" << '\t' << "Amount" << '\t' << "Buyer" << '\t' << "Seller" << '\n';
            os << time.tm_mday << '/' << time.tm_mon << '/' << (1900 + time.tm_year) << "\t\t"
               << price << "\t" << amount << '\t' << buyer << '\t' << seller << '\n';
            return os;
        };
    };
}

#endif //PROJECT_TRADESTAMP_HPP
