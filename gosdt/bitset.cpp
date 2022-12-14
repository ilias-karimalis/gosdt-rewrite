// GOSDT algorithm
//
//
// MIT License
//
// Copyright (c) 2022 Systopia Lab, Computer Science, University of British Columbia
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include "bitset.hpp"
#include "utilities/logging.hpp"

namespace gosdt {

    Bitset::Bitset()
    : data(nullptr), n_blocks(0), size(0), n_bits_used_last_block(0)
    {}

    Bitset::Bitset(usize size)
    : size(size)
    {
        n_bits_used_last_block = size & BITS_PER_BLOCK;
        if (size == 0) {
            n_blocks = 0;
            data = nullptr;
            return;
        }

        n_blocks = (mp_size_t)(size / BITS_PER_BLOCK);
        n_blocks += n_bits_used_last_block != 0;
        data = new usize[n_blocks];
    }

    Bitset::Bitset(usize size, bool fill)
    : size(size)
    {
        // Find number of usize blocks that need to be allocated
        n_bits_used_last_block = size % BITS_PER_BLOCK;
        if (size == 0) {
            n_blocks = 0;
            data = nullptr;
            return;
        }

        n_blocks = (mp_size_t)(size / BITS_PER_BLOCK);
        // Add extra block that is lost to integer division above
        n_blocks += n_bits_used_last_block != 0;
        // TODO For now we're using the default c++ allocator, but this might
        //      need to be changed in the future. Our default implementation
        //      used a tbb::scalable_allocator<usize>, it might make sense to
        //      use a slab allocator as all the bitsets we're producing are the
        //      same size.
        data = new usize[n_blocks];

        // Fill the data with the specific value.
        if (fill) {
            for (auto i = 0; i < n_blocks; ++i)
            {
                data[i] = ~((usize) 0);
            }

            auto mask = ~((usize) 0) >> (BITS_PER_BLOCK - n_bits_used_last_block);
            data[n_blocks - 1] &= mask;

        } else {
            for (auto i = 0; i < n_blocks; ++i)
            {
                data[i] = 0;
            }
        }
    }

    Bitset::Bitset(usize size, usize *data, bool fill)
    : data(data), size(size)
    {
        n_bits_used_last_block = size % BITS_PER_BLOCK;
        n_blocks = (mp_size_t)(size / BITS_PER_BLOCK);
        n_blocks += n_bits_used_last_block != 0;

        if (fill) {
            for (auto i = 0; i < n_blocks; ++i)
            {
                data[i] = ~((usize) 0);
            }
            auto mask = ~((usize)0) >> (BITS_PER_BLOCK  - n_bits_used_last_block);
            data[n_blocks-1] &= mask;
        }  else {
            for (auto i = 0; i < n_blocks; ++i)
            {
                data[i] = 0;
            }
        }
    }

    Bitset::Bitset(const Bitset& other)
    : data(new usize[other.n_blocks]), n_blocks(other.n_blocks), size(other.size),
    n_bits_used_last_block(other.n_bits_used_last_block)
    {
        for (mp_size_t i = 0; i < n_blocks; i++)
        {
            data[i] = other.data[i];
        }
    }

    Bitset::Bitset(Bitset && other) noexcept
    {
        // Instantiate this
        data = other.data;
        n_blocks = other.n_blocks;
        size = other.size;
        n_bits_used_last_block = other.n_bits_used_last_block;

        // Remove values from other
        other.data = nullptr;
        other.n_blocks = 0;
        other.size = 0;
        other.n_blocks = 0;
    }

    Bitset::~Bitset()
    {
        if (data == nullptr) return;
        delete[] data;
    }

    Bitset&
    Bitset::operator=(const Bitset &other)
    {
        if (this == &other) return *this;
        delete[] data;
        data = new usize[other.n_blocks];
        n_blocks = other.n_blocks;
        size = other.size;
        n_bits_used_last_block = other.n_bits_used_last_block;
        for (mp_size_t i = 0; i < n_blocks; i++)
        {
            data[i] = other.data[i];
        }
        return *this;
    }

    Bitset&
    Bitset::operator=(Bitset &&other) noexcept
    {
        delete[] data;
        data = other.data;
        n_blocks = other.n_blocks;
        size = other.size;
        n_bits_used_last_block = other.n_bits_used_last_block;

        other.data = nullptr;
        other.n_blocks = 0;
        other.size = 0;
        other.n_bits_used_last_block = 0;
        return *this;
    }

    bool
    Bitset::get(usize index) const
    {
        if (size == 0 || index >= size || data == nullptr)
            throw std::out_of_range("Oh no!");

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

//    Bitset
//    Bitset::bit_and(Bitset const& b1, Bitset const& b2, bool flip)
//    {
//        DASSERT(b1.size == b2.size);
//
//        auto ret_val = Bitset(b1.size, true);
//        if (!flip) {
//            mpn_and_n(ret_val.data, b1.data, b2.data, ret_val.n_blocks);
//        } else {
//            mpn_nior_n(ret_val.data, b2.data, b2.data, ret_val.n_blocks);
//            mpn_nior_n(ret_val.data, b1.data, ret_val.data, ret_val.n_blocks);
//        }
//        // Clean the last block to only keep bits that are in use
//        auto mask = ~(usize)0 >> (BITS_PER_BLOCK - ret_val.n_bits_used_last_block);
//        ret_val.data[ret_val.n_blocks - 1] &= mask;
//
//        return ret_val;
//    }

    void
    Bitset::bit_and(const Bitset &b1, const Bitset &b2, Bitset &out, bool flip)
    {
        DASSERT(b1.size == b2.size && b2.size == out.size);
        if (!flip) {
            mpn_and_n(out.data, b1.data, b2.data, out.n_blocks);
        } else {
            mpn_nior_n(out.data, b2.data, b2.data, out.n_blocks);
            mpn_nior_n(out.data, b1.data, out.data, out.n_blocks);
        }

        // TODO we need to figure out if we need to mask!
        //      as far as I can tell we don't need to mask out.
        out.data[out.n_blocks - 1] &= ~(usize)0 >> (BITS_PER_BLOCK -out.n_bits_used_last_block);
    }

    bool
    Bitset::empty() const
    {
        return mpn_zero_p(data, n_blocks);
    }

    usize Bitset::scan(usize first, bool value) {
        DASSERT(first < size);
        for (usize i = first; i < size; i++)
        {
            if (get(i) == value) {
                return i;
            }
        }
        // TODO find a better failure return value
        return size;
    }


    bool
    operator==(const Bitset& left, const Bitset& right)
    {
        bool ret = left.n_blocks == right.n_blocks;
        ret &= left.size == right.size;
        ret &= left.n_bits_used_last_block == right.n_bits_used_last_block;
        if (!ret) return ret;

        return mpn_cmp(left.data, right.data, left.n_blocks) == 0;
    }

}
