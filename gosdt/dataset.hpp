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
        // NOTE: not sure if all of these are actually needed so I'll avoid
        //       writing these for now.
        Matrix<u64> costs;
        std::vector<u64> diff_costs;
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
