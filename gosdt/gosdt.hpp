#ifndef GOSDT_REWRITE_GOSDT_HPP
#define GOSDT_REWRITE_GOSDT_HPP

#include <filesystem>

#include "utils.hpp"

#include "result.hpp"
#include "config.hpp"


namespace GOSDT {

    Result
    run_from_path(Config config, const std::filesystem::path& dataset_path);

};

#endif //GOSDT_REWRITE_GOSDT_HPP
