#ifndef GOSDT_REWRITE_ENCODER_HPP
#define GOSDT_REWRITE_ENCODER_HPP

#include <sstream>
#include <iostream>
#include <regex>
#include <set>
#include <map>

#include <csv/csv.h>
#include <matrix/matrix.hpp>
#include "utils.hpp"
#include "bitset.hpp"

namespace GOSDT {

    struct Encoder {

        enum class Type {
            RATIONAL,
            INTEGRAL,
            ENUMERABLE [[maybe_unused]], // I don't like this one... and I also don't think it's necessary
            BINARY,
            CATEGORICAL,
            REDUNDANT,
            NULL_TYPE,
            UNDEFINED,
        };

        enum class Relation {
            G,      // >
            L,      // <
            GEQ,    // >=
            LEQ,    // <=
            EQ,     // ==
        };

        static const std::regex integral_regex;         ///< RegEx for parsing integer types
        static const std::regex rational_regex;         ///< RegEx for parsing rational types

        std::vector<std::string> header;                ///< header of the ingested csv file
        std::vector<std::vector<std::string>> tokens;   ///< strings representing each entry of the ingested csv file

        usize n_rows;       ///< number of rows in the PRE-binarized dataset
        usize n_columns;    ///< number of columns in the PRE-binarized dataset
        usize n_targets;    ///< number of classes in the PRE-binarized dataset

        std::vector<std::set<std::string>> values_per_column;
        std::vector<Type> type_per_column;
        // Maybe add cardinality_per_column later, not needed as far as I can tell.

        // Maps from Target values (found in the last column) to their concentration
        // in the dataset
        std::map<std::string, u32> target_concentration;


        explicit Encoder(std::istream & input_stream);

        std::vector<Bitset> binary_rows;

        usize n_binary_rows;        ///< number of rows in the binarized dataset
        usize n_binary_columns;     ///< number of columns in the binarized dataset
    };

}

#endif //GOSDT_REWRITE_ENCODER_HPP
