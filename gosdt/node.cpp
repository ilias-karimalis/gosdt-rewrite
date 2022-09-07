#include "node.hpp"

namespace gosdt {

    Node::Node()
    : upper_bound(std::numeric_limits<u64>::max()), lower_bound(0)
    {}

    Node::Node(const Bitset* parent)
    {
        if (parent != nullptr) {
            parents.insert(parent);
        }

        upper_bound = std::numeric_limits<u64>::max();
        lower_bound = 0;
    }
}
