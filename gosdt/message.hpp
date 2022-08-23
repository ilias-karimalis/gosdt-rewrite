#ifndef GOSDT_REWRITE_MESSAGE_HPP
#define GOSDT_REWRITE_MESSAGE_HPP

#include "bitset.hpp"

namespace GOSDT {

    struct Message {

        enum class Priority {
            ZERO,
            ONE,
        };

        Bitset bitset;
        Priority priority;

        Message(Bitset bitset, Priority priority);

        friend bool operator>(const Message& lhs, const Message& rhs);

    };

}

#endif //GOSDT_REWRITE_MESSAGE_HPP
