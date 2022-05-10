#include "benchmark.hpp"
#include <iostream>

/** I/O functions are slow, so they can be on its own translation unit */
std::ostream& ticker_benchmark::benchmark::print(std::ostream &os) const
{
    using std::chrono::duration;
    os << "Mean,PlusMinus,Benchmark\n";
    for (auto& m: measurements)
        os << m.mean() << ",\t" << m.half_conf_interval() << ",\t" << m.name << '\n';
    return os;
}