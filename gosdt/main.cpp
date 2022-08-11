#include <iostream>
#include <variant>

#include "bitset.hpp"


int main(int argc, char * argv[]) {

    auto bitset = GOSDT::Bitset(10, true);

    for (auto i = 0; i < 11; ++i) {
        auto get_opt = bitset.get(i);
        if (get_opt.has_value()) {
            std::cout << get_opt.value() << std::endl;
        } else {
            std::cout << "Oh no we have an issue" << std::endl;
        }
    }

    std::cout << "Bitset count: " << bitset.count() << std::endl;

    return 0;
}
