#include <iostream>

#include "json.hpp"
#include "ticker.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

// Converts UTC time string to a time_t value.
std::tm getEpochTime(const std::string& dateTime)
{
    // Let's consider we are getting all the input in
    // this format: '2014-07-25T20:17:22Z' (T denotes
    // start of Time part, Z denotes UTC zone).
    // A better approach would be to pass in the format as well.
    static const std::string dateTimeFormat{ "%Y-%m-%dT%H:%M:%S" };

    // Create a stream which we will use to parse the string,
    // which we provide to constructor of stream to fill the buffer.
    std::istringstream ss{ dateTime };

    // Create a tm object to store the parsed date and time.
    std::tm dt;

    // Now we read from buffer using get_time manipulator
    // and formatting the input appropriately.
    ss >> std::get_time(&dt, dateTimeFormat.c_str());

    // Convert the tm structure to time_t value and return.
    return dt;
    //return std::mktime(&dt);
}

int main() {
    auto dateStr = std::string{"2012-10-31T09:01:00.000+0100"};
    auto time = getEpochTime(dateStr);
    //std::cout << time.tm_mday << std::endl;
    //std::cout << std::asctime(std::gmtime(&time)) << std::endl;

    auto is = std::istringstream{R"({"tag":"CARL A","isin":"DK0010181676","shares":33699252,"name":"Carlsberg Breweries A/S", "url":"https://www.carlsberggroup.com/","market":"Nasdaq Copenhagen","sector":"Food, Beverage and Tobacco","segment":"Large","trades":[{"price":527,"amount":35,"buyer":"DDB","seller":"NDA","seq":1,"code":0}]})"};
    auto t = ticker{};
    is >> json_t{t};
    std::cout << t.name << std::endl;
    for (const auto trade : t.trades){
        std::cout << trade.amount << std::endl;
    }
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

//auto is = std::istringstream{R"({"b":true,"x":3,"y":3.14,"z":"hello","w":[7,11]})"};
//auto v = aggregate_t{};
//static_assert(accepts_writer_v<aggregate_t, json_writer_t>, "does not accept writer");
//is >> json_t{v};
//CHECK(is);
//CHECK(v == aggregate_t{true, 3, 3.14, "hello", {7, 11}});