#include <iostream>

#include "json.hpp"
#include "ticker.hpp"

int main() {
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