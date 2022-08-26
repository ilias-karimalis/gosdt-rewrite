#pragma once

#include <optional>
#include <unordered_set>

#include <json/json.hpp>

#include "utilities/numeric_types.hpp"


namespace gosdt {

    struct Config {

        using json = nlohmann::json;

        /// Config struct members
        f32 uncertainty_tolerance;
        std::optional<u8> max_height;
        u32 time_limit;

        u64 regularization;


        /// Config functions
        static Config configure_from_stream(std::istream &stream_config);
        static Config configure_from_json(json json_config);
    };

}