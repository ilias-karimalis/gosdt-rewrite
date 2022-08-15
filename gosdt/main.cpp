#include <iostream>
#include <variant>

#include <matrix/matrix.hpp>

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

    auto n_targets = 10u;
    Matrix<f32> costs(n_targets, n_targets, 0.0f);
    for (usize i = 0; i < n_targets; i++) {
        for (usize j = 0; j < n_targets; j++) {
            if (i == j) {
                costs(i, j) = 0.0f;
            } else {
                costs(i, j) = 1.0f;
            }
        }
    }

    for (usize i = 0; i < n_targets; i++) {
        for (usize j = 0; j < n_targets; j++) {
            std::cout << costs(i, j) << ", ";
        }
        std::cout << std::endl;
    }

    return 0;
}
