#pragma once

#include <vector>

#include "bitset.hpp"

namespace GOSDT {

    struct Optimizer;

    struct Node {

        Bitset capture_set;
        std::vector<Node *> parents;

        f32 upper_bound;
        f32 lower_bound;

        std::optional<f32> max_cost_reduction;
        std::optional<usize> cost_minimizer;

        // This should only be used for the
        Node();
        Node(Bitset capture_set, f32 ub, f32 lb, f32 mcr, usize cm, Node * parent);

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
