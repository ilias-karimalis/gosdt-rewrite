#ifndef GOSDT_REWRITE_QUEUE_HPP
#define GOSDT_REWRITE_QUEUE_HPP

#include <queue>

#include "utils.hpp"
#include "bitset.hpp"

namespace GOSDT {

    struct Queue {

        struct Message {

            enum class Priority {
                ZERO,
                ONE,
                };

            Bitset bitset;
            Priority priority;

            friend bool operator>(const Message & lhs, const Message & rhs);
        };

        std::priority_queue<Message, std::vector<Message>, std::greater<>> internal_queue;

        bool push(Message message);
        std::optional<Message> pop();

    };

}

#endif
