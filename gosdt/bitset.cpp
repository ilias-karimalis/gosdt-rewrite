#include "bitset.hpp"

namespace GOSDT {

    Bitset::Bitset(::usize size, bool fill)
    : size(size)
    {
        /* Allocate memory for the Bitset */

        // Find number of usize blocks that need to be allocated
        n_bits_used_last_block = size % BITS_PER_BLOCK;
        if (size == 0) {
            n_blocks = 0;
            data = nullptr;
            return;
        } else {
            n_blocks = (size / BITS_PER_BLOCK);
            // Add extra block that is lost to integer division above
            n_blocks += n_bits_used_last_block != 0;
        }

        // TODO For now we're using the default c++ allocator, but this might
        //      need to be changed in the future. Our default implementation
        //      used a tbb::scalable_allocator<usize>, it might make sense to
        //      use a slab allocator as all the bitsets we're producing are the
        //      same size.
        data = new usize[n_blocks];

        // Fill the data with the specific value.
        if (fill) {

            for (auto i = 0; i < n_blocks; ++i) {
                data[i] = ~((usize) 0);
            }

            // We need to set to 0 all values in the last byte which are unused
            // which we do by right shifting the number of bits that we want to
            // keep.
            auto mask = ~((usize) 0) >>
                (BITS_PER_BLOCK - n_bits_used_last_block);
            data[n_blocks - 1] &= mask;

        } else {
            for (auto i = 0; i < n_blocks; ++i) {
                data[i] = 0;
            }
        }

    }

    std::optional<bool>
    Bitset::get(usize index) const
    {
        if (size == 0 || index >= size || data == nullptr) return std::nullopt;

        auto block_index = index / BITS_PER_BLOCK;
        auto bit_index = index % BITS_PER_BLOCK;
        auto block = data[block_index];
        return (bool)((block >> bit_index) % 2);
    }

    void
    Bitset::set(usize index, bool value) const
    {
        if (size == 0 || index >= size || data == nullptr) return;

        auto block_index = index / BITS_PER_BLOCK;
        auto bit_index = index % BITS_PER_BLOCK;
        usize mask = (usize)(1) << bit_index;

        if (value) data[block_index] |= mask;
        else data[block_index] &= ~mask;
    }

    usize
    Bitset::count() const
    {
        return mpn_popcount(data, n_blocks);
    }

    Bitset
    Bitset::bit_and(Bitset const& b1, Bitset const& b2, bool flip)
    {
        assert(b1.size == b2.size);

        auto ret_val = Bitset(b1.size, true);
        if (!flip) {
            mpn_and_n(ret_val.data, b1.data, b2.data, ret_val.n_blocks);
        } else {
            mpn_nior_n(ret_val.data, b2.data, b2.data, ret_val.n_blocks);
            mpn_nior_n(ret_val.data, b1.data, ret_val.data, ret_val.n_blocks);
        }
        return ret_val;
    }

    bool
    Bitset::empty() const
    {
        return mpn_zero_p(data, n_blocks);
    }


}
