#include "graph.hpp"

namespace GOSDT {

    std::optional<Node *> Graph::find(Bitset &identifier) {

        // TODO make Bitset hashable.
        auto found_node = node_map.find(identifier);
        if (found_node == node_map.end()) {
            // Node was not found in graph.
            return std::nullopt;
        }

        return found_node->second;

    }


    void Graph::insert(Bitset & identifier, Node * node) {

        auto is_in_graph = node_map.find(identifier) == node_map.end();
        if (is_in_graph) return;

        node_map.insert({identifier, node});
    }

    std::vector<Node *> Graph::parents(Node & node) {

        auto found_parents = parent_map.find(node);
        if (found_parents != parent_map.end())
            return {};

        return found_parents->second;
    }


}
