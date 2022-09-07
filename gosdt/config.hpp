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
        /// One step look ahead bound (enabled through the use of scopes)
        bool one_step_lookahead = true;


        /// Config functions
        static Config configure_from_stream(std::istream &stream_config);
        static Config configure_from_json(json json_config);
    };

}