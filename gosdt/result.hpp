#pragma once

#include <unordered_set>

#include <json/json.hpp>

#include "utilities/numeric_types.hpp"
#include "model.hpp"



namespace gosdt {

    struct Result {
        // Elapsed time in seconds
        f64 time;
        u32 size;
        u32 iterations;
        f32 lower_bound;
        f32 upper_bound;
        f32 model_loss;

        f32 ru_utime;
        f32 ru_stime;
        usize ru_maxrss;
        usize ru_nswap;
        usize ru_nivcw;

        std::vector<Model> models;
    };

}