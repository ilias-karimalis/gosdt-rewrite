#pragma once

#include <queue>
#include <unordered_set>

#include "bitset.hpp"
#include "message.hpp"

namespace gosdt
{

    struct Queue {

        // TODO THis is really slow and is grinding our runtime to a halt we need a better solution NOW
        //      extra copies of bitset are acceptable to avoid this, imo
        std::priority_queue<Message, std::vector<Message>, std::less<>> pq;
        std::unordered_set<Bitset> in_queue;


        Queue() = default;
        ~Queue() = default;

        void emplace(Bitset bitset, Message::Priority prio);
        [[nodiscard]] bool empty() const;
        [[nodiscard]] usize size() const;
        [[nodiscard]] const Bitset& top() const;
        void pop();
    };

}