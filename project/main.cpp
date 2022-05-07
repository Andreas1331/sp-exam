#include <iostream>
#include <fstream>

#include "json.hpp"
#include "ticker.hpp"

const std::string FILE_TO_OPEN {"../DANSKE.json"};

int main() {
    // Exercise 1 & 2)
    std::ifstream file {FILE_TO_OPEN};
    if(file){
        auto t = ticker{};
        file >> json_t{t};
        std::cout << t.name << std::endl;
    }
    else{
        std::cout << "No file found ..." << std::endl;
    }

    return 0;
}

