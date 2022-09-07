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
#include <sys/resource.h>
#include <sys/time.h>

#include "gosdt.hpp"
#include "dataset.hpp"
#include "optimizer.hpp"

namespace gosdt {

    // NOTE: This will be one of the main interfaces to the algorithm
    Result
    run_from_path(gosdt::Config config, const std::filesystem::path& dataset_path)
    {
        Result result{};

        // START resource usage statistics and timing
        static struct rusage usage_start, usage_end;
        if (getrusage(RUSAGE_SELF, &usage_start)) {}

        // Load the dataset
        auto dataset_stream = std::ifstream(dataset_path);
        auto dataset = Dataset(dataset_stream, config);

        std::cout << "[gosdt::run_from_path] Completed Dataset and Config Creation\n";
        auto optimizer = Optimizer(dataset, config);

        // Perform the optimization
        auto optimization_result = optimizer.optimize();
        auto extraction_result = optimizer.extract();

        result.upper_bound = optimization_result.upper_bound;
        result.lower_bound = optimization_result.lower_bound;
        result.iterations = optimization_result.iterations;
        result.size = optimization_result.graph_size;
        result.time = optimization_result.time;
        result.model_loss = ((f64) result.upper_bound) / ((f64) dataset.n_rows * config.regularization);

        // END resource usage statistics and timing
        getrusage(RUSAGE_SELF, &usage_end);
        struct timeval delta{};
        timersub(&usage_end.ru_utime, &usage_start.ru_utime, &delta);
        result.ru_utime = (float)delta.tv_sec + (((float)delta.tv_usec) / 1000000);
        timersub(&usage_end.ru_stime, &usage_start.ru_stime, &delta);
        result.ru_stime = (float)delta.tv_sec + (((float)delta.tv_usec) / 1000000);
        result.ru_maxrss = usage_end.ru_maxrss;
        result.ru_nswap = usage_end.ru_nswap - usage_start.ru_nswap;
        result.ru_nivcw = usage_end.ru_nivcsw - usage_start.ru_nivcsw;

        return result;
    }

}