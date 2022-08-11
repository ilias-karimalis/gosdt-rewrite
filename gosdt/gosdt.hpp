#ifndef GOSDT_REWRITE_GOSDT_HPP
#define GOSDT_REWRITE_GOSDT_HPP

#include <filesystem>

#include "utils.hpp"

#include "result.hpp"
#include "config.hpp"


namespace GOSDT {

    static Result run_from_path(Config, const std::filesystem::path& dataset_path);


};

#endif //GOSDT_REWRITE_GOSDT_HPP
