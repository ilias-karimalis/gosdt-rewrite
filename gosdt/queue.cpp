#include "queue.hpp"

namespace GOSDT {


    bool Queue::push(Message message) {
        // TODO
        // Need to figure out how to not have multiple copies of the same message in the queue.
        return false;
    }

    std::optional<Queue::Message> pop() {
        // TODO
        return {};
    }

    bool operator>(const Queue::Message &lhs, const Queue::Message &rhs) {
        return lhs.priority > rhs.priority;
    }

    std::optional<Queue::Message> Queue::pop()
    {
        return {};
    }

}