#include "node.hpp"

namespace GOSDT {

    Node::Node(Bitset const &identifier, Bitset const &features)
        : identifier(identifier), features(features)
    {
        bool is_terminal_node = (identifier.count() <= 1) || features.empty();

        // TODO the rest of this

    }

}