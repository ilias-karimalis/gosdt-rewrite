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

#include <fstream>
#include <iostream>
#include <array>

#include "gosdt.hpp"

#include "node.hpp"

int main(int argc, char * argv[]) {

    namespace fs = std::filesystem;
    using json = nlohmann::json;
//    assert(argc == 3);
//    fs::path dataset_path(argv[1]);

    json config_json = json::parse(std::ifstream(argv[1]));
    gosdt::Config config = gosdt::Config::configure_from_json(config_json);
    std::array<fs::path, 4> dataset_paths{{
        "datasets/monk_1/train.csv",
        "datasets/monk_2/train.csv",
        "datasets/monk_3/train.csv",
        "datasets/fico/fico-binary.csv"
    }};

    for (const auto& path : dataset_paths) {
        std::cout << "Dataset Path: " << path << "\n";
        auto result = gosdt::run_from_path(config, path);
        std::cout << "[gosdt::Result]:\n\tTime: " << result.time << "ms\n\tSize: " << result.size << "\n\tIterations: "
                  << result.iterations << "\n\tBounds: [" << result.lower_bound << ", " << result.upper_bound
                  << "]\n\tModel Loss: " << result.model_loss << "\n\n\n";

    }
    return 0;
}
