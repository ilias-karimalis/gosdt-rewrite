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
        mp_size_t n_blocks = 0; // number of usize blocks allocated and stored in data

        // External memory structure
        usize size; // number of booleans represented by this Bitset


        // TODO I think that depth_budget should be encapsulated under Nodes
        // not in the Bitset. The number of possible additional splits which
        // can be conducted. If it's -1 that signifies that there is no depth
        // budget

        // the number of bits used in the last used block
        u8 n_bits_used_last_block;

        // TODO maybe add a construct which does not fill and only bothers to
        //      allocate the memory for the Bitset. this could be used in
        //      bit_and where we know that we can avoid filling that Bitset.

        // Constructs a bitset from a single fill value and size
        Bitset(usize size, bool fill);

        [[nodiscard]] std::optional<bool>
        get(usize index) const;

        void
        set(usize index, bool value) const;

        // Assumes that the last block of memory is filled with zeros outside
        // of the n_used_last_block bytes.
        [[nodiscard]] usize
        count() const;


        // TODO add more functionality as we need it

        // flip: whether to treat the bits of b1 as flipped
        static Bitset
        bit_and(Bitset const& b1, Bitset const& b2, bool flip = false);

        [[nodiscard]] bool empty() const;
    };

}
