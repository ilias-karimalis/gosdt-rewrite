#include <fstream>

#include "gosdt.hpp"

int main(int argc, char * argv[]) {

    namespace fs = std::filesystem;
    using json = nlohmann::json;
    assert(argc == 3);
    fs::path dataset_path(argv[1]);

    json config_json = json::parse(std::ifstream(argv[2]));
    gosdt::Config config = gosdt::Config::configure_from_json(config_json);

    auto res = gosdt::run_from_path(config, dataset_path);
    return 0;
}
