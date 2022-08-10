#include "encoder.hpp"

namespace GOSDT {

    const std::regex Encoder::integral_regex(R"(^[+-]?\d+$)");
    const std::regex Encoder::rational_regex(R"(^[+-]?\d+\.\d*$)");

    Encoder::Encoder(std::istream &input_stream) {

        static const char CSV_SEPARATOR = ',';

        // Tokenize the csv input.
        io::LineReader line_reader("", input_stream);
        std::string token; // Used all over the place as a placeholder for an extracted token


        // Tokenize Header
        const char * header_row_string = line_reader.next_line();
        std::stringstream header_row_stream(header_row_string);
        while (header_row_stream.good()) {
            std::getline(header_row_stream, token, CSV_SEPARATOR);
            header.push_back(token);
            n_columns = header.size();
        }

        // TODO if n_columns = 0, we have an issue and should just crash.
        // TODO Figure out a better error reporting system rather than just using asserts.
        assert(n_columns > 0);

        // Tokenize the rest of the csv file
        n_rows = 0;
        while (const char * row_string = line_reader.next_line()) {
            std::stringstream row_stream(row_string);
            static std::vector<std::string> row;

            while (row_stream.good()) {
                std::getline(row_stream, token, CSV_SEPARATOR);
                row.push_back(token);
            }
            assert(row.size() == n_columns);

            tokens.push_back(row);
            n_rows += 1;
        }

        // Parse the Tokenized CSV input
        values_per_column.resize(n_columns);
        type_per_column.resize(n_columns, Type::UNDEFINED);

        // Columns in the original dataset which should be tossed away for various reasons
        std::vector<bool> skip_column;
        skip_column.resize(n_columns, false);

        for (usize i = 0; i < n_rows; i++) {
            for (usize j = 0; j < n_columns; j++) {
                auto element = tokens[i][j];
                auto type = type_per_column[j];

                // Null type elements
                if (element.empty() || element == "NULL" || element == "null" || element == "Null" || element == "NA"
                    || element == "na" || element == "NaN") {
                    type_per_column[j] = Type::NULLTYPE;
                }
                // Integer type elements
                else if ((type == Type::UNDEFINED || type == Type::INTEGRAL)
                    && (std::regex_match(element, integral_regex))) {
                    type_per_column[j] = Type::INTEGRAL;
                }
                // Rational type elements
                else if ((type == Type::UNDEFINED || type == Type::INTEGRAL || type == Type::RATIONAL)
                    && (std::regex_match(element, rational_regex))) {
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
        for (usize j = 0; j < n_columns; j++) {
            auto cardinality = values_per_column[j].size();

            // There's only 1 type in this feature so it's useless for classification
            // so we will mark it as redundant and ignore it when building the binarized dataset
            if (cardinality <= 1) {
                type_per_column[j] = Type::REDUNDANT;
            }
            // Regardless of the inferred value type, if there are only two elements of that feature
            // then we can already treat it as binarized (if this column is not already invalid)
            else if (cardinality == 2 && !skip_column[j]) {
                type_per_column[j] = Type::BINARY;
            }
        }

        // We set the target column as Categorical so that we ensure equality constraints.
        type_per_column[n_columns - 1] = Type::CATEGORICAL;

        // TODO Build a set of encoding rules for each original feature
        // TODO Encode the tokenized data using the encoding rules
    }

}