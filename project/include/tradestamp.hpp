//
// Created by andre on 08-05-2022.
//

#ifndef PROJECT_TRADESTAMP_HPP
#define PROJECT_TRADESTAMP_HPP

#include <iomanip>
#include <ctime>
#include <string>


namespace ticker_essentials {
    struct trade_stamp {
    private:
        static constexpr int PRINT_WIDTH = 14;
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
        std::ostream &print(std::ostream &os) const {
            os << std::left << std::setw(PRINT_WIDTH) << "Date:" << std::setw(PRINT_WIDTH) << "Price (DKK):"
               << std::setw(PRINT_WIDTH) << "Amount#:" << std::setw(PRINT_WIDTH) << "Buyer:" << std::setw(PRINT_WIDTH)
               << "Seller:" << '\n';
            os << std::setw(PRINT_WIDTH) << std::to_string(time.tm_mday) + "/" + std::to_string(time.tm_mon) + '/' +
                                            std::to_string((1900 + time.tm_year)) << std::setw(PRINT_WIDTH)
               << price << std::setw(PRINT_WIDTH) << amount << std::setw(PRINT_WIDTH) << buyer << std::setw(PRINT_WIDTH)
               << seller << '\n';
            return os;
        };
    };
}

#endif //PROJECT_TRADESTAMP_HPP
