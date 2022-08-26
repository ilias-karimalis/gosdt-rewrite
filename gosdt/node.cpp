#include "node.hpp"

namespace gosdt {

    Node::Node()
    : upper_bound(std::numeric_limits<u64>::max()), lower_bound(0)
    {}

    Node::Node(Bitset capture_set, Node *parent)
    : capture_set(std::move(capture_set))
    {
        if (parent != nullptr)
            parents.push_back(parent);

        upper_bound = std::numeric_limits<u64>::max();
        lower_bound = 0;
    }
}