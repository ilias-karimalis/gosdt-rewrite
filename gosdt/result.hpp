#pragma once

#include <unordered_set>

#include <json/json.hpp>

#include "utilities/numeric_types.hpp"
#include "model.hpp"



namespace gosdt {

    struct Result {
        // Elapsed time in seconds
        u64 time;
        u32 size;
        u32 iterations;
        u64 lower_bound;
        u64 upper_bound;
        u64 model_loss;

        f32 ru_utime;
        f32 ru_stime;
        usize ru_maxrss;
        usize ru_nswap;
        usize ru_nivcw;

        std::vector<Model> models;

    };

}