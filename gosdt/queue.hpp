#pragma once

#include <queue>
#include <set>

#include "bitset.hpp"
#include "message.hpp"

namespace gosdt
{

    struct Queue {

        // TODO this will work for now, but I should refelect with Jerry to pick a better solution
        std::deque<Bitset> pzero;
        std::deque<Bitset> pone;

        Queue() = default;
        ~Queue() = default;

        void emplace(Bitset bitset, Message::Priority prio);
        [[nodiscard]] bool empty() const;
        [[nodiscard]] usize size() const;
        [[nodiscard]] const Bitset& top() const;
        void pop();
    };

}