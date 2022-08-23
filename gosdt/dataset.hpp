#ifndef DATASET_HPP
#define DATASET_HPP

#include <istream>

#include <matrix/matrix.hpp>

#include "bitset.hpp"
#include "encoder.hpp"

namespace GOSDT {

    /*
     *
     */
    struct Dataset {

        // TODO as far as I can tell rows is only used in the creation of
        //      features and targets and is no longer needed. This might be an
        //      incomplete image though because perhaps encoder.binary_rows is
        //      needed somewhere else that's to be seen, keeping it for now.

        /// Row view of the dataset in binary representation
        std::vector<Bitset> rows;
        /// Column view of the dataset in binary representation
        std::vector<Bitset> features;
        /// Column view of the original target column in binary representation
        std::vector<Bitset> targets;
        /// TODO
        Bitset majority;

        usize n_rows;       ///< Number of rows in the Dataset
        usize n_columns;    ///< Number of columns in the Dataset
        usize n_targets;    ///< Number of classes in the Dataset

        // SECTION: Cost Matrices for different types of mis-prediction
        // NOTE: not sure if all of these are actually needed so I'll avoid
        //       writing these for now.
        Matrix<f32> costs;
        std::vector<f32> diff_costs;
        std::vector<f32> match_costs;
        std::vector<f32> mismatch_costs;

        explicit Dataset(std::istream & input_stream);
        ~Dataset() = default;

        std::tuple<f32, f32, f32, usize>
        calculate_bounds(const Bitset& capture_set) const;

        // Helper functions for the ctor.
        void
        construct_bitmasks(std::istream & input_stream);

        void
        construct_cost_matrices();

        void
        construct_majority_bitmask();

    };

}

#endif
