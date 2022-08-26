#include <fstream>
#include <sys/resource.h>
#include <sys/time.h>

#include <json/json.hpp>

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
        auto start = std::chrono::high_resolution_clock::now();

        // Load the dataset
        auto dataset_stream = std::ifstream(dataset_path);
        auto dataset = Dataset(dataset_stream, config);

        std::cout << "[gosdt::run_from_path] Completed Dataset and Config Creation\n";
        auto optimizer = Optimizer(dataset, config);

        // Perform the optimization
        auto optimizer_result = optimizer.optimize();

        auto end = std::chrono::high_resolution_clock::now();

        result.model_loss = optimizer_result.model_loss;
        result.iterations = optimizer_result.iterations;
        result.models = optimizer_result.models;

        // END resource usage statistics and timing
        result.time =
            std::chrono::duration_cast<std::chrono::seconds>(end - start).count();
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