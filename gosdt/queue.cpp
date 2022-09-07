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

#include "queue.hpp"

void
gosdt::Queue::emplace(gosdt::Bitset bitset, gosdt::Message::Priority prio)
{
    if (in_queue.contains(bitset)) return;

    in_queue.emplace(bitset);
    pq.emplace(std::move(bitset), prio);
}

bool
gosdt::Queue::empty() const
{
    return pq.empty();
}

usize
gosdt::Queue::size() const
{
    return pq.size();
}

const gosdt::Bitset&
gosdt::Queue::top() const {
    return pq.top().bitset;
}

void
gosdt::Queue::pop() {
    in_queue.erase(pq.top().bitset);
    pq.pop();
}



