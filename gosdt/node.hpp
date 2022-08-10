#ifndef GOSDT_REWRITE_NODE_HPP
#define GOSDT_REWRITE_NODE_HPP


#include "bitset.hpp"

namespace GOSDT {

    class Node {

        Bitset identifier;
        Bitset features;

        u32 upper_bound;
        u32 lower_bound;

        Node(Bitset const &identifier, Bitset const &features);

    };

};

#endif //GOSDT_REWRITE_NODE_HPP
