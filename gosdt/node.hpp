#ifndef GOSDT_REWRITE_NODE_HPP
#define GOSDT_REWRITE_NODE_HPP


#include "bitset.hpp"

namespace GOSDT {

    struct Node {

        Bitset identifier;
        Bitset features;

        f32 upper_bound;
        f32 lower_bound;

        Node(Bitset const &identifier, Bitset const &features);

    };

};

#endif //GOSDT_REWRITE_NODE_HPP
