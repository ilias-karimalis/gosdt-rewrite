#pragma once

#include "bitset.hpp"

namespace gosdt {

    struct Message {

        enum class Priority {
            ZERO,
            ONE,
        };

        Bitset bitset;
        Priority priority;

        Message(Bitset bitset, Priority priority);

    };

}
