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

#ifndef DATASET_HPP
#define DATASET_HPP

#include <istream>

#include <matrix/matrix.hpp>

#include "bitset.hpp"
#include "encoder.hpp"
#include "config.hpp"

namespace gosdt {

    struct Dataset {

        // TODO as far as I can tell rows is only used in the creation of
        //      features and targets and is no longer needed. This might be an
        //      incomplete image though because perhaps encoder.binary_rows is
        //      needed somewhere else that's to be seen, keeping it for now.

        /// Configuration
        const Config& config;

        /// Row view of the dataset in binary representation
        std::vector<Bitset> rows;
        /// Column view of the dataset in binary representation
        std::vector<Bitset> features;
        /// Column view of the original target column in binary representation
        std::vector<Bitset> targets;
        /// TODO what is this??
        Bitset majority;

        usize n_rows;       ///< Number of rows in the Dataset
        usize n_columns;    ///< Number of columns in the Dataset
        usize n_targets;    ///< Number of classes in the Dataset

        // SECTION: Cost Matrices for different types of mis-prediction
        // NOTE: not sure if all of these are actually needed, so I'll avoid
        //       writing these for now.
        Matrix<u64> costs;
        [[maybe_unused]] std::vector<u64> diff_costs;
        std::vector<u64> match_costs;
        std::vector<u64> mismatch_costs;

        Dataset(std::istream & input_stream, const Config& configuration);
        ~Dataset() = default;

        // Helper functions for the ctor.
        void construct_bitmasks(std::istream & input_stream);
        void construct_cost_matrices();
        void construct_majority_bitmask();


        struct Bounds {
            u64 lower_bound;
            u64 upper_bound;
            usize optimal_feature;
        };
        [[nodiscard]] Bounds calculate_bounds(const Bitset& capture_set) const;

    };

}

#endif
