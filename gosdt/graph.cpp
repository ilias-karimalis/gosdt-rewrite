#include "graph.hpp"
#include "optimizer.hpp"

namespace GOSDT {

    Node&
    Graph::find_or_create(Bitset identifier, Node* parent, const Optimizer& optimizer)
    {

        if (node_set.contains(identifier)) {
            return const_cast<Node &>(* node_set.find(identifier));
        }
        auto [ub, lb, mcr, cm] = optimizer.calculate_bounds(identifier);
        // Here we're creating a copy of identifier in calling Node ctor.
        auto node = Node(std::move(identifier), ub, lb, mcr, cm, parent);
        return const_cast<Node &>(* node_set.emplace(std::move(node)).first);
    }

    bool
    Graph::contains(const Bitset &identifier)
    {
        return node_set.contains(identifier);
    }

    Node&
    Graph::find(const Bitset &identifier)
    {
        return const_cast<Node &>(* node_set.find(identifier));
    }
}
