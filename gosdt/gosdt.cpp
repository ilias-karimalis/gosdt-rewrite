#include <fstream>
#include "gosdt.hpp"
#include "dataset.hpp"


// TODO This will be one of the main interfaces to the algorithm
GOSDT::Result GOSDT::run_from_path(GOSDT::Config config, const std::filesystem::path& dataset_path) {

    auto dataset_stream = std::ifstream(dataset_path);
    auto dataset = Dataset(config, dataset_stream);
    return {};
}
