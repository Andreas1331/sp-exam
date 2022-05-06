//
// Created by andre on 06-05-2022.
//

#ifndef PROJECT_TICKER_HPP
#define PROJECT_TICKER_HPP

/** A custom class to contain all information related to a specific ticker e.g name, trades.
 * The class also implements the visitor pattern to allow for JSON parsing */
class ticker
{ /** public access is just for easy structured initialization in tests */
    bool b;
    int x;
    double y;
    std::string z;
    std::vector<int> w;
    /** visitor support with read-only access, e.g. for writing-out */
    template <typename Visitor>
    void accept_reader(Visitor&& visit) const
    {
        visit("b", b);
        visit("x", x);
        visit("y", y);
        visit("z", z);
        visit("w", w);
    }
    /** visitor support with full access, e.g. for reading-in */
    template <typename Visitor>
    void accept_writer(Visitor&& visit)
    {
        visit("b", b);
        visit("x", x);
        visit("y", y);
        visit("z", z);
        visit("w", w);
    }
    /** equality operator to support testing */
    friend bool operator==(const aggregate_t& a1, const aggregate_t& a2)
    {
        return (a1.b == a2.b) && (a1.x == a2.x) && (a1.y == a2.y) && (a1.z == a2.z) && (a1.w == a2.w);
    }
};

#endif //PROJECT_TICKER_HPP
