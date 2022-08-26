#include "graph.hpp"
#include "optimizer.hpp"
#include "utilities/logging.hpp"

namespace gosdt {

    Node&
    Graph::find_or_create(Bitset identifier, Node* parent, const Optimizer& optimizer)
    {

        if (node_set.contains(identifier)) {
            return const_cast<Node &>(* node_set.find(identifier));
        }
        auto [min_loss, max_loss, mcr, cm] = optimizer.calculate_bounds(identifier);
        // Here we're creating a copy of identifier in calling Node ctor
        auto node = Node(std::move(identifier), parent);
        node.upper_bound = max_loss + optimizer.dataset.n_rows;
        node.lower_bound = std::min(node.upper_bound,  min_loss + 2 * optimizer.dataset.n_rows);

        // Check if we fail bounds
        auto incremental_accuracy = (node.upper_bound - node.lower_bound) <= optimizer.dataset.n_rows;
        auto leaf_accuracy = node.upper_bound <= 2 * optimizer.dataset.n_rows;
        if (incremental_accuracy || leaf_accuracy) {
            DVOUT << "Node should not be split again" << std::endl;
            node.lower_bound = node.upper_bound;
        }

        return const_cast<Node &>(* node_set.emplace(std::move(node)).first);
    }

    bool
    Graph::contains(const Bitset &identifier) const
    {
        return node_set.contains(identifier);
    }

    Node&
    Graph::find(const Bitset &identifier)
    {
        return const_cast<Node &>(* node_set.find(identifier));
    }
}
