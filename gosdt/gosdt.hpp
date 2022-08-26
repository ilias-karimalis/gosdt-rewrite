#pragma once

#include <filesystem>

#include "utilities/numeric_types.hpp"
#include "result.hpp"
#include "config.hpp"

namespace gosdt {

    Result
    run_from_path(Config config, const std::filesystem::path& dataset_path);

};

