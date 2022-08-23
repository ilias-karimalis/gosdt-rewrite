#pragma once

#include <vector>
#include <optional>

#include <gmp.h>

#include "utils.hpp"

namespace GOSDT {

    struct Bitset {
        // Not Portable. Requires that each 1 byte == 8 bits
        static const constexpr size_t BITS_PER_BLOCK = 8 * sizeof(usize);

        // pointer to bistet content
        usize * data = nullptr;
        // number of usize blocks allocated and stored in data
        mp_size_t n_blocks = 0;
        // number of booleans represented by this Bitset
        usize size;
        // the number of bits used in the last used block
        u8 n_bits_used_last_block;

        // TODO maybe add a construct which does not fill and only bothers to
        //      allocate the memory for the Bitset. this could be used in
        //      bit_and where we know that we can avoid filling that Bitset.

        // Constructs a bitset from a single fill value and size
        Bitset();
        Bitset(usize size, bool fill);
        Bitset(const Bitset& other);
        Bitset(Bitset&& other) noexcept;
        ~Bitset();

        Bitset& operator=(const Bitset& other);
        Bitset& operator=(Bitset&& other) noexcept;

        bool friend operator==(const Bitset& left, const Bitset& right);

        [[nodiscard]] bool
        get(usize index) const;

        void
        set(usize index, bool value) const;

        // Assumes that the last block of memory is filled with zeros outside
        // the n_used_last_block bytes.
        [[nodiscard]] usize
        count() const;

        // flip: whether to treat the bits of b1 as flipped
        static Bitset
        bit_and(Bitset const& b1, Bitset const& b2, bool flip = false);

        [[nodiscard]] bool
        empty() const;

        usize
        scan(usize first, bool value);
    };

}

namespace std {

    template<>
    struct hash<GOSDT::Bitset> {

        size_t
        operator()(const GOSDT::Bitset& bitset) const
        {
            // TODO: calculate the hash value of this bitset
            return 0;
        }

    };

}
