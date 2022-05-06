#include <iostream>

#include "json.hpp"
#include "ticker.hpp"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

int main() {
    //auto dateStr = std::string{"2012-10-31T09:01:00.000+0100"};
    //auto time = getEpochTime(dateStr);
    //std::cout << time.tm_hour << std::endl;
    //std::cout << std::asctime(std::gmtime(&time)) << std::endl;

    auto is = std::istringstream{R"({"tag":"CARL A",
"isin":"DK0010181676",
"shares":33699252,
"name":"Carlsberg Breweries A/S",
"url":"https://www.carlsberggroup.com/",
"market":"Nasdaq Copenhagen",
"sector":"Food, Beverage and Tobacco",
"segment":"Large",
"trades":[
{"time":"2012-10-31T09:01:00.000+0100",
"price":527,
"amount":35,
"buyer":"DDB",
"seller":"NDA",
"seq":1,
"code":0}
]})"};
    auto t = ticker{};
    is >> json_t{t};
    for (const auto trade : t.trades){
        std::cout << trade.time.tm_hour << std::endl;
    }
    return 0;
}

