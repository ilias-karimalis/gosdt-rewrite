#pragma once

#include <vector>
#include <optional>

#include <gmp.h>

#include "utils.hpp"

namespace GOSDT {

    struct Bitset {

        // Not Portable. Requires that each 1 byte == 8 bits
        static const constexpr size_t BITS_PER_BLOCK = 8 * sizeof(usize);

        // Internal memory structure
        usize * data = nullptr;
        mp_size_t n_blocks; // number of usize blocks allocated and stored in data

        // External memory structure
        usize size; // number of bools represented by this Bitset


        // The number of possible additional splits which can be conducted
        // If it's -1 that signifies that there is no depth budget
        // std::optional<i8> depth_budget = std::nullopt;

        // the number of bits used in the last used block
        u8 n_bits_used_last_block;

        // Constructs a bitset from a single fill value and size
        Bitset(usize size, bool fill);

        [[nodiscard]] std::optional<bool> get(usize index) const;
        void set(usize index, bool value) const;

        [[nodiscard]] usize count() const;


        // TODO add more as we need it


        bool empty() const;
    };

}