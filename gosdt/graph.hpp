//#pragma once
//
//#include <unordered_set>
//#include <unordered_map>
//
//#include "bitset.hpp"
//#include "node.hpp"
//
//namespace gosdt {
//
//    // Pre declaration of Optimizer;
//    struct Optimizer;
//
//    struct Graph {
//
//        // NOTE the Graph will own both the Bitset and Nodes that they contain!!
//        std::unordered_map<Bitset, Node, std::hash<Bitset>> node_map;
//
////        std::unordered_set<Node, NodeHasher, std::equal_to<>> node_set;
//
//        Graph() = default;
//
//        ~Graph() = default;
//
//        Node&
//        find_or_create(Bitset identifier, std::pair<Bitset*, Node*> parent, const Optimizer& optimizer);
//
//        bool
//        contains(const Bitset& identifier) const;
//
//        std::pair<const Bitset, Node>&
//        find(const Bitset& identifier);
//
//    };
//
//}
//