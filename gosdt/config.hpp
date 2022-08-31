#pragma once

#include <unordered_set>

#include <json/json.hpp>

#include "utilities/numeric_types.hpp"
#include "utilities/optional.hpp"


namespace gosdt {

    struct Config {

        using json = nlohmann::json;

        /// Config struct members

        /// The maximum allowed global optimality gap allowed before the optimization can terminate
        u64 optimality_gap = 0;
        /// The penalty incurred for each leaf in the model
        u64 regularization = 10;

        /// The maximum allowed runtime (seconds)
        opt<u32> time_limit = std::nullopt;
        /// The maximum number of models extracted
        opt<u32> model_limit = std::nullopt;

        /// The maximum tree depth for solutions
        opt<u8> max_height = std::nullopt;



        /// Config functions
        static Config configure_from_stream(std::istream &stream_config);
        static Config configure_from_json(json json_config);
    };

}