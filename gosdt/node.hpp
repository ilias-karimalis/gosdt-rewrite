#pragma once

#include <vector>
#include <limits>

#include "bitset.hpp"

#include "utilities/numeric_types.hpp"
#include "utilities/optional.hpp"

namespace gosdt {

    // This is a hack!
    struct Optimizer;

    struct Node {

        std::vector<const Bitset *> parents;

        u64 upper_bound;
        u64 lower_bound;

        usize optimal_feature;

        // This should only be used for the
        Node();
        explicit Node(const Bitset* parent);

    };

}
