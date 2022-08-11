#ifndef DATASET_HPP
#define DATASET_HPP

#include <istream>

#include <matrix/matrix.hpp>

#include "bitset.hpp"
#include "config.hpp"
#include "encoder.hpp"

namespace GOSDT {

    /*
     *
     */
    struct Dataset {

//        Encoder encoder;    ///< The Dataset encoder, used to binarize the dataset and store the codex for the encoding

        // TODO as far as I can tell rows is only used in the creation of features and targets and is no longer needed.
        //      this might be an incomplete image though because perhaps encoder.binary_rows is needed somewhere else
        //      that's to be seen, keeping it for now.
        std::vector<Bitset> rows;       ///< Row view of the dataset in binary representation
        std::vector<Bitset> features;   ///< Column view of the dataset in binary representation
        std::vector<Bitset> targets;    ///< Column view of the original target column in binary representation

        usize n_rows;       ///< Number of rows in the Dataset
        usize n_columns;    ///< Number of columns in the Dataset
        usize n_targets;    ///< Number of classes in the target column of the Dataset


        // SECTION: Cost Matrices for different types of mis-prediction
        // NOTE: not sure if all of these are actually needed so I'll avoid writing these for now.
//        Matrix<f32> aggregate_cost_matrix;

        Dataset(Config config, std::istream & input_stream);
        ~Dataset() = default;

    };

}

#endif
