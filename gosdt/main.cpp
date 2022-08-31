#include <fstream>
#include <iostream>

#include "gosdt.hpp"

int main(int argc, char * argv[]) {

    namespace fs = std::filesystem;
    using json = nlohmann::json;
    assert(argc == 3);
    fs::path dataset_path(argv[1]);

    json config_json = json::parse(std::ifstream(argv[2]));
    gosdt::Config config = gosdt::Config::configure_from_json(config_json);

    auto result = gosdt::run_from_path(config, dataset_path);
    std::cout << "[gosdt::Result]:\n\tTime: " << result.time << "ms\n\tSize: " << result.size << "\n\tIterations: "
                 << result.iterations << "\n\tBounds: [" << result.lower_bound << ", " << result.upper_bound
                 << "]\n\tModel Loss: " << result.model_loss << std::endl;
    return 0;
}
