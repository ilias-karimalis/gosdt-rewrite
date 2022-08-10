#ifndef GOSDT_REWRITE_ENCODER_HPP
#define GOSDT_REWRITE_ENCODER_HPP

#include <sstream>
#include <iostream>
#include <regex>
#include <set>
#include <map>

#include <csv/csv.h>
#include "utils.hpp"

namespace GOSDT {

    struct Encoder {

        enum class Type {
            RATIONAL,
            INTEGRAL,
            ENUMERABLE, // I don't like this one... and I also don't think it's necessary
            BINARY,
            CATEGORICAL,
            REDUNDANT,
            NULLTYPE,
            UNDEFINED,
        };

        enum class Relation {
            G,      // >
            L,      // <
            GEQ,    // >=
            LEQ,    // <=
            EQ,     // ==
        };

        // RegExs for parsing integers and rationals
        static const std::regex integral_regex;
        static const std::regex rational_regex;

        // The header of the ingested csv file
        std::vector<std::string> header;
        // The strings representing each entry in the ingested csv file
        std::vector<std::vector<std::string>> tokens;
        usize n_columns;
        usize n_rows;

        std::vector<std::set<std::string>> values_per_column;
        std::vector<Type> type_per_column;
        // Maybe add cardinality_per_column later, not needed as far as I can tell.

        // Maps from Target values (found in the last column) to their concentration
        // in the dataset
        std::map<std::string, u32> target_concentration;



        explicit Encoder(std::istream & input_streamer);

    };

}

#endif //GOSDT_REWRITE_ENCODER_HPP
