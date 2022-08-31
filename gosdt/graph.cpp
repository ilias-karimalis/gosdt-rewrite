//#include "graph.hpp"
//#include "optimizer.hpp"
//#include "utilities/logging.hpp"
//
//namespace gosdt {
//
//    Node&
//    Graph::find_or_create(Bitset identifier, std::pair<Bitset*, Node*> parent, const Optimizer& optimizer)
//    {
//
//        if (node_map.contains(identifier)) {
////            return node_set.find(identifier);
//            return node_map.find(identifier)->second;
//        }
//        auto [min_loss, max_loss, mcr, cm] = optimizer.calculate_bounds(identifier);
//        // Here we're creating a copy of identifier in calling Node ctor
//        auto node = Node(parent);
//        node.upper_bound = max_loss + optimizer.dataset.n_rows;
//        node.lower_bound = std::min(node.upper_bound, min_loss + 2 * optimizer.dataset.n_rows);
//
//        // Check if we fail bounds
//        auto incremental_accuracy = (node.upper_bound - node.lower_bound) <= optimizer.dataset.n_rows;
//        auto leaf_accuracy = node.upper_bound <= 2 * optimizer.dataset.n_rows;
//        if (parent.second != nullptr) {
//            DASSERT(node.upper_bound <= parent.second->upper_bound);
//            DASSERT(node.lower_bound >= parent.second->lower_bound || node.lower_bound == node.upper_bound);
//        }
//
//        if (incremental_accuracy || leaf_accuracy || identifier.empty()) {
//            DVOUT << "Node should not be split again" << std::endl;
//            node.lower_bound = node.upper_bound;
//        }
//        return node_map.emplace(std::move(identifier), std::move(node)).first->second;
//    }
//
//    bool
//    Graph::contains(const Bitset &identifier) const
//    {
//        return node_map.contains(identifier);
//    }
//
//    std::pair<const Bitset, Node>&
//    Graph::find(const Bitset &identifier)
//    {
//        return *node_map.find(identifier);
//    }
//}
