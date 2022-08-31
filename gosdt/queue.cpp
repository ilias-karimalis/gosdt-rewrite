#include "queue.hpp"

void
gosdt::Queue::emplace(gosdt::Bitset bitset, gosdt::Message::Priority prio)
{
    switch(prio) {

        case Message::Priority::ZERO:
            if (std::find(pzero.begin(), pzero.end(), bitset) == pzero.end())
                pzero.push_back(std::move(bitset));
            break;

        case Message::Priority::ONE:
            if (std::find(pone.begin(), pone.end(), bitset) == pone.end())
                pone.push_back(std::move(bitset));
            break;
    }
}

bool
gosdt::Queue::empty() const
{
    return pzero.empty() && pone.empty();
}

usize
gosdt::Queue::size() const
{
    return pzero.size() + pone.size();
}

const gosdt::Bitset&
gosdt::Queue::top() const {
    if (!pzero.empty()) {
        return pzero.front();
    }
    return pone.front();
}

void
gosdt::Queue::pop() {
    if (!pzero.empty()) {
        pzero.pop_front();
        return;
    }
    pone.pop_front();
}



