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
        CONFIGURE(json_config, config, optimality_gap)
        CONFIGURE(json_config, config, regularization)

        CONFIGURE(json_config, config, time_limit)
        CONFIGURE(json_config, config, model_limit)

        CONFIGURE(json_config, config, max_height)

        return config;
    }
}
