#ifndef GOSDT_REWRITE_CONFIG_HPP
#define GOSDT_REWRITE_CONFIG_HPP

#include <optional>
#include <unordered_set>

#include <json/json.hpp>

#include "utils.hpp"


namespace GOSDT {

    class Config {

        using json = nlohmann::json;

        /// Config struct members
        f32 uncertainty_tolerance;
        std::optional<u8> max_height;
        u32 time_limit;


        /// Config functions
        static Config configure_from_stream(std::istream &stream_config);
        static Config configure_from_json(json json_config);

    };

}

#endif //GOSDT_REWRITE_CONFIG_HPP
