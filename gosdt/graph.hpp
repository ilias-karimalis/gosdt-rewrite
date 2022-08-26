#pragma once

#include <unordered_set>

#include "bitset.hpp"
#include "node.hpp"

namespace gosdt {

    // Pre declaration of Optimizer;
    struct Optimizer;

    struct Graph {

        // NOTE the Graph will own both the Bitset and Nodes that they contain!!
//        std::unordered_map<Bitset, Node, BitsetHash> node_map;
        std::unordered_set<Node, NodeHasher, std::equal_to<>> node_set;

        Graph() = default;
        ~Graph() = default;

        Node&
        find_or_create(Bitset identifier, Node* parent, const Optimizer& optimizer);

        bool
        contains(const Bitset& identifier) const;

        Node&
        find(const Bitset& identifier);

    };

}

