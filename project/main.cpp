#include <iostream>

int main() {
    std::cout << "Hello, World!" << std::endl;
    return 0;
}

//auto is = std::istringstream{R"({"b":true,"x":3,"y":3.14,"z":"hello","w":[7,11]})"};
//auto v = aggregate_t{};
//static_assert(accepts_writer_v<aggregate_t, json_writer_t>, "does not accept writer");
//is >> json_t{v};
//CHECK(is);
//CHECK(v == aggregate_t{true, 3, 3.14, "hello", {7, 11}});