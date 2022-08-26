#include "config.hpp"

namespace gosdt {

    Config Config::configure_from_stream(std::istream &stream_config) {
        json json_config;
        // This operation is defined by the nlohmann::json library
        stream_config >> json_config;
        return configure_from_json(std::move(json_config));
    }

#define CONFIGURE(JSON, CONFIG, X) if ((JSON).contains(#X)) { (CONFIG).X = (JSON)[#X]; }


    Config Config::configure_from_json(json json_config) {
        Config config;
        CONFIGURE(json_config, config, uncertainty_tolerance)
        CONFIGURE(json_config, config, time_limit)
        CONFIGURE(json_config, config, max_height)
        CONFIGURE(json_config, config, regularization)

        return config;
    }
}
