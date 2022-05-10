#ifndef PROJECT_BENCHMARK_HPP
#define PROJECT_BENCHMARK_HPP

#include <iosfwd>
#include <vector>
#include <chrono>

#include <cmath>
#include <string>

namespace ticker_benchmark{
    struct measure_t
    {
        const std::string name;
        size_t count{0};
        double sum{0}; // the sum of timings
        double sumsq{0}; // the sum of squared timings
        void add(double time) {
            ++count;
            sum += time;
            sumsq += time * time;
        }
        [[nodiscard]] double mean() const { return sum / count; }
        [[nodiscard]] double half_conf_interval() const {
            auto standard_error = std::sqrt((sumsq - sum*sum/count) / (count - 1) / count);
            return standard_error * 1.960147;
        }
    };

    class benchmark
    {
        // These are just private fields
        using clk = std::chrono::high_resolution_clock;
        using time_point = clk::time_point;
        time_point last;
        std::vector<measure_t> measurements;
    public:
        /** Creates a benchmark with pre-allocated number of events */
        benchmark(size_t measure_count = 10) {
            measurements.reserve(measure_count); // preallocate so no time is wasted for allocations
        }
        void reset() { last = clk::now(); }
        /** registers a new benchmark */
        size_t add(const std::string &name) {
            auto res = measurements.size(); // This will return the index of the new element
            measurements.push_back({name});
            reset();
            return res;
        }
        /** Measures the time point of the named event */
        void measure(size_t id) {
            auto t = clk::now();
            measurements[id].add(std::chrono::duration<double, std::milli>(t-last).count());
            last = t;
        }
        /** Prints the time-table of events */
        std::ostream& print(std::ostream& os) const;
    };
}

#endif //PROJECT_BENCHMARK_HPP
