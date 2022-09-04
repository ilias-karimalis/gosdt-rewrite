#include <fstream>
#include <iostream>
#include <array>

#include "gosdt.hpp"

int main(int argc, char * argv[]) {

    namespace fs = std::filesystem;
    using json = nlohmann::json;
//    assert(argc == 3);
//    fs::path dataset_path(argv[1]);

    json config_json = json::parse(std::ifstream(argv[1]));
    gosdt::Config config = gosdt::Config::configure_from_json(config_json);
    std::array<fs::path, 3> dataset_paths{{
        "datasets/monk_1/train.csv",
        "datasets/monk_2/train.csv",
        "datasets/monk_3/train.csv"
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
