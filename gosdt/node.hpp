#pragma once

#include <vector>

#include "bitset.hpp"

namespace gosdt {

    struct Optimizer;

    struct Node {

        Bitset capture_set;
        std::vector<Node *> parents;

        u64 upper_bound;
        u64 lower_bound;

        std::optional<u64> max_cost_reduction;
        std::optional<usize> cost_minimizer;

        // This should only be used for the
        Node();
        Node(Bitset capture_set, Node * parent);

        friend bool operator==(const Node& lhs, const Node& rhs)
        {
            return lhs.capture_set == rhs.capture_set;
        }

        friend bool operator==(const Node& lhs, const Bitset& rhs)
        {
            return lhs.capture_set == rhs;
        }
    };


    struct NodeHasher {

        using is_transparent = void;

        size_t
        operator()(const Node& node) const
        {
            return std::hash<Bitset>{}(node.capture_set);
        }

        size_t
        operator()(const Bitset& bitset) const
        {
            return std::hash<Bitset>{}(bitset);
        }
    };

};
