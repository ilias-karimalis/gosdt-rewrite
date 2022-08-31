#include "message.hpp"

namespace gosdt {

    Message::Message(Bitset bitset, Message::Priority priority)
    : bitset(std::move(bitset)), priority(priority)
    {}

}

