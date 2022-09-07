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

#pragma once

#include <sstream>
#include <iostream>
#include <regex>
#include <set>
#include <map>

#include <csv/csv.h>
#include <matrix/matrix.hpp>
#include "utilities/numeric_types.hpp"
#include "bitset.hpp"

namespace gosdt {

    struct Encoder {

        enum class Type {
            RATIONAL,
            INTEGRAL,
            BINARY,
            CATEGORICAL,
            REDUNDANT,
            NULL_TYPE,
            UNDEFINED,
        };

        friend std::ostream &operator<<(std::ostream &os, Type type);

        enum class Relation {
            GEQ,    // >=
            EQ,     // ==
        };

        /// RegEx for parsing integer types
        static const std::regex integral_regex;
        /// RegEx for parsing rational types
        static const std::regex rational_regex;

        /// header of the ingested csv file
        std::vector<std::string> header;
        /// strings representing each entry of the ingested csv file
        std::vector<std::vector<std::string>> tokens;

        usize n_rows;       ///< number of rows in the dataset
        usize n_columns;    ///< number of columns in the PRE-binarized dataset (Including Target column)
        usize n_targets;    ///< number of classes in the PRE-binarized dataset

        std::vector<std::set<std::string>> values_per_column;
        std::vector<Type> type_per_column;
        // Maybe add cardinality_per_column later, not needed as far as I can
        // tell.

        // Maps from Target values (found in the last column) to their
        // concentration in the dataset
        std::map<std::string, u32> target_concentration;


        explicit Encoder(std::istream & input_stream);

        std::vector<Bitset> binary_rows;

        usize n_binary_columns; ///< number of columns in the binarized dataset (Discarding Target columns)
        usize n_binary_targets; ///< number of target columns in the binarized dataset
    };

}

