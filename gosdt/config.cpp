#include "config.hpp"

namespace GOSDT {

    Config Config::configure_from_stream(std::istream &stream_config) {
        json json_config;
        // This operation is defined by the nlohmann::json library
        stream_config >> json_config;
        return configure_from_json(json_config);
    }

    Config Config::configure_from_json(json json_config) {
        return {};
    }
}