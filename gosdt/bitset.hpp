#pragma once

#include <vector>

#include <gmp.h>

#include "utilities/numeric_types.hpp"
#include "utilities/constants.hpp"

namespace gosdt
{

    struct Bitset {
        // TODO figure out if there's a more portable way to compute BITS_PER_BLOCK
        // Not Portable. Requires that each 1 byte == 8 bits
        static const constexpr size_t BITS_PER_BLOCK = 8 * sizeof(usize);

        // pointer to bistet content
        usize *data = nullptr;
        // number of usize blocks allocated and stored in data
        mp_size_t n_blocks = 0;
        // number of booleans represented by this Bitset
        usize size;
        // the number of bits used in the last used block
        u8 n_bits_used_last_block;

        // TODO maybe add a construct which does not fill and only bothers to
        //      allocate the memory for the Bitset. this could be used in
        //      bit_and where we know that we can avoid filling that Bitset.

        Bitset();

        // This constructor allocates enough memory to store #size number of booleans,
        // but does not initialize the values of that memory.
        explicit Bitset(usize size);

        // Constructs a bitset from a single fill value and size
        Bitset(usize size, bool fill);

        Bitset(const Bitset &other);

        Bitset(Bitset &&other) noexcept;

        Bitset(usize size, usize *data, bool fill);

        ~Bitset();

        Bitset &operator=(const Bitset &other);

        Bitset &operator=(Bitset &&other) noexcept;

        bool friend operator==(const Bitset &left, const Bitset &right);

        [[nodiscard]] bool
        get(usize index) const;

        void
        set(usize index, bool value) const;

        // Assumes that the last block of memory is filled with zeros outside
        // the n_used_last_block bytes.
        [[nodiscard]] usize
        count() const;

        // flip: whether to treat the bits of b1 as flipped
//        static Bitset
//        bit_and(Bitset const &b1, Bitset const &b2, bool flip = false);

        static void
        bit_and(const Bitset& b1, const Bitset& b2, Bitset& out, bool flip);

        [[nodiscard]] bool
        empty() const;

        usize
        scan(usize first, bool value);
    };

} // namespace gosdt

namespace std
{
    template<>
    struct hash<gosdt::Bitset> {

        size_t
        operator()(const gosdt::Bitset& bitset) const
        {
            // TODO: calculate the hash value of this bitset
            size_t hash = bitset.size;

            for (mp_size_t i = 0; i < bitset.n_blocks; i++)
            {
                hash ^= bitset.data[i] + GOLDEN_RATIO_HASHING_CONSTANT + (hash << 6) + (hash >> 2);
            }
            return hash;
        }

    };

} // namespace std
