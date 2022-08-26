#include "message.hpp"

namespace gosdt {

    Message::Message(Bitset bitset, Message::Priority priority)
    : bitset(std::move(bitset)), priority(priority)
    {}

    bool operator>(const Message &lhs, const Message &rhs) {
        return lhs.priority > rhs.priority ;
    }


}

