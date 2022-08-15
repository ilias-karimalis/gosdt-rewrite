#ifndef GOSDT_REWRITE_GRAPH_HPP
#define GOSDT_REWRITE_GRAPH_HPP

#include <unordered_map>

#include "node.hpp"

namespace GOSDT {

    struct Graph {

        std::unordered_map<Bitset, Node *> node_map;
        std::unordered_map<Node, std::vector<Node *>> parent_map;

        Graph() = default;
        ~Graph() = default;

        std::optional<Node *> find(Bitset & identifier);
        void insert(Bitset & identifier, Node * node);

        std::vector<Node *> parents(Node & node);

    };

}

#endif //GOSDT_REWRITE_GRAPH_HPP
