// GOSDT algorithm
//
//
// MIT License
//
// Copyright (c) 2022 Systopia Lab, Computer Science, University of British Columbia
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

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