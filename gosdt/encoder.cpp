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

#include "encoder.hpp"
#include "utilities/logging.hpp"

#include <cstdlib>

namespace gosdt {

    const std::regex Encoder::integral_regex(R"(^[+-]?\d+$)");
    const std::regex Encoder::rational_regex(R"(^[+-]?\d+\.\d*$)");

    Encoder::Encoder(std::istream &input_stream)
    {
        DVOUT << "[Encoder::Encoder] Tokenizing csv data file\n";
        static const char CSV_SEPARATOR = ',';

        // Tokenize the csv input.
        io::LineReader line_reader("", input_stream);
        // Used all over the place as a placeholder for an extracted token
        std::string header_token;

        // Tokenize Header
        const char * header_row_string = line_reader.next_line();
        std::stringstream header_row_stream(header_row_string);
        while (header_row_stream.good())
        {
            std::getline(header_row_stream, header_token, CSV_SEPARATOR);
            header.push_back(header_token);
            n_columns = header.size();
        }

        DASSERT(n_columns > 0);
        // Tokenize the rest of the csv file
        n_rows = 0;
        while (const char * row_string = line_reader.next_line()) {
            // TODO I shouldn't have to create a new stringstream everytime, there must be a more efficient solution.
            std::stringstream row_stream(row_string);
            std::string token;
            std::vector<std::string> row;
            while (row_stream.good()) {
                std::getline(row_stream, token, CSV_SEPARATOR);
                row.push_back(token);
            }
            DASSERT(row.size() == n_columns);

            tokens.push_back(row);
            n_rows += 1;
        }

        // Parse the Tokenized CSV input
        DVOUT << "[Encoder::Encoder] Determining the cell types of the dataset\n";
        values_per_column.resize(n_columns);
        type_per_column.resize(n_columns, Type::UNDEFINED);

        // Columns in the original dataset which should be tossed away for
        // various reasons
        std::vector<bool> skip_column;
        skip_column.resize(n_columns, false);
        for (usize i = 0; i < n_rows; i++)
        {
            for (usize j = 0; j < n_columns; j++)
            {
                auto element = tokens[i][j];
                auto type = type_per_column[j];

                // Null type elements
                if (element.empty() || element == "NULL" || element == "null"
                    || element == "Null" || element == "NA" || element == "na"
                    || element == "NaN") {
                    type_per_column[j] = Type::NULL_TYPE;
                }
                // Integer type elements
                else if ((type == Type::UNDEFINED || type == Type::INTEGRAL)
                    && (std::regex_match(element, integral_regex))) {
                    type_per_column[j] = Type::INTEGRAL;
                }
                // Rational type elements
                else if ((type == Type::UNDEFINED || type == Type::INTEGRAL
                    || type == Type::RATIONAL)
                    && std::regex_match(element, rational_regex)) {
                    type_per_column[j] = Type::RATIONAL;
                }
                // We default to equality based Categorical elements
                else {
                    type_per_column[j] = Type::CATEGORICAL;
                }

                values_per_column[j].insert(element);
                if (j == n_columns - 1) {
                    target_concentration[element] += 1;
                }
            }
        }

        // We can do further type specification based on column statistics
        DVOUT << "[Encoder::Encoder] Performing further type specification based on column statistics\n";
        for (usize j = 0; j < n_columns; j++) {
            auto cardinality = values_per_column[j].size();
            // There's only 1 type in this feature so it's useless for
            // classification so we will mark it as redundant and ignore it
            // when building the binarized dataset
            if (cardinality <= 1) {
                type_per_column[j] = Type::REDUNDANT;
            }
            // Regardless of the inferred value type, if there are only two
            // elements of that feature then we can already treat it as
            // binarized (if this column is not already invalid)
            else if (cardinality == 2 && !skip_column[j]) {
                type_per_column[j] = Type::BINARY;
            }
        }
        // We set the target column as Categorical so that we ensure equality
        // constraints.
        type_per_column[n_columns - 1] = Type::CATEGORICAL;
        // At this point we can also count how many classes this categorical
        // type has.
        n_targets = values_per_column[n_columns - 1].size();
        DOUT << "[Encoder::Encoder] [nRows, nFeatureColumns, nTargets]: [" << n_rows << ", " << n_columns-1 << ", " << n_targets << "]\n";


        DVOUT << "[Encoder::Encoder] Constructing the set of encoding rules\n";
        std::vector<std::tuple<usize, Type, Relation, std::string>> codex;
        for (usize i = 0; i < n_columns; i++)
        {
            auto type = type_per_column[i];
            auto initial_size = codex.size();
            auto value_set = values_per_column[i];
            n_binary_columns = codex.size();
            switch (type)
            {
                case Type::BINARY:
                    DASSERT(value_set.size() == 2);
                    codex.emplace_back(i, Type::BINARY, Relation::EQ, *value_set.begin());
                    break;

                case Type::CATEGORICAL:
                    for (const auto& value: value_set)
                    {
                        codex.emplace_back(i, Type::CATEGORICAL, Relation::EQ, value);
                    }
                    break;

                case Type::INTEGRAL:
                    for (const auto& value: value_set)
                    {
                        codex.emplace_back(i, Type::INTEGRAL, Relation::GEQ, std::to_string(atoi(value.c_str())));
                    }
                    break;

                case Type::RATIONAL: {
                    std::set<f64> parsed_value_set;
                    for (const auto& value: value_set) {
                        parsed_value_set.emplace(std::strtod(value.c_str(), nullptr));
                    }
                    f64 base_value = *parsed_value_set.begin();
                    for (auto parsed_value: parsed_value_set) {
                        f64 threshold = 0.5 * (parsed_value + base_value);
                        base_value = parsed_value;
                        codex.emplace_back(i, Type::RATIONAL, Relation::GEQ, std::to_string(threshold));
                    }
                }
                break;

                default:
                    continue;
            }

            if (i == n_columns - 1) {
                n_binary_targets = codex.size() - initial_size;
            }
        }
        n_binary_columns = codex.size() - n_binary_targets;

        DVOUT << "[Encoder::Encoder] Encoding the tokenized data using codex\n";
        for (usize i = 0; i < n_rows; i++)
        {
            // This is a place where we wish this wasn't filled.
            Bitset row(n_binary_columns + n_binary_targets, false);
            for (usize j = 0; j < n_binary_columns + n_binary_targets; j++)
            {
                auto& [index, type, relation, reference] = codex[j];
                bool value;
                switch (type)
                {
                    case Type::INTEGRAL:
                        value = atoi(tokens[i][index].c_str()) >=
                                atoi(reference.c_str());
                        break;

                    case Type::RATIONAL:
                        value = atof(tokens[i][index].c_str()) >=
                                atof(reference.c_str());
                        break;

                    default:
                        value = tokens[i][index] == reference;
                        break;
                }
                row.set(j, value);
            }
            binary_rows.push_back(std::move(row));
        }

        DOUT << "[Encoder::Encoder] [nRows, nBinaryColumns, nBinaryTargets]: [" << n_rows << ", " << n_binary_columns << ", " << n_binary_targets << "]\n";

    }

    std::ostream & operator<<(std::ostream &os, Encoder::Type type) {
        switch (type) {
            case Encoder::Type::RATIONAL: os << "Rational"; break;
            case Encoder::Type::INTEGRAL: os << "Integral"; break;
            case Encoder::Type::BINARY: os << "Binary"; break;
            case Encoder::Type::CATEGORICAL: os << "Categorical"; break;
            case Encoder::Type::REDUNDANT: os << "Redundant"; break;
            case Encoder::Type::NULL_TYPE: os << "Null"; break;
            case Encoder::Type::UNDEFINED: os << "Undefined"; break;
        }
        return os;
    }


}
