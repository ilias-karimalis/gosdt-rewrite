#pragma once

#include <unordered_map>

#include "bitset.hpp"
#include "node.hpp"

namespace GOSDT {

    struct Graph {

        std::unordered_map<Bitset, Node *, BitsetHash> node_map;

        std::unordered_map<std::pair<Bitset, int>, Bitset *, ChildHash>;

        Graph() = default;
        ~Graph() = default;

        std::optional<Node *> find(Bitset & identifier);
        void insert(Bitset & identifier, Node * node);

        std::vector<Node *> parents(Node & node);

    };

    struct ChildHash {
        std::size_t operator()(const std::pair<Bitset, int> &key);
    };

}

