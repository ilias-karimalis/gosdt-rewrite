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



