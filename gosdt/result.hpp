#ifndef GOSDT_REWRITE_RESULT_HPP
#define GOSDT_REWRITE_RESULT_HPP

#include <unordered_set>

#include <json/json.hpp>

#include "utils.hpp"

#include "model.hpp"



namespace GOSDT {

    struct Result {
        f32 time;
        u32 size;
        u32 iterations;
        f32 lower_bound;
        f32 upper_bound;
        f32 model_loss;

        f32 ru_time;
        f32 rs_time
        usize ru_maxrss;
        usize ru_nswap;
        usize ru_nivcw;

        std::vector<Model> models;
    };

}

#endif //GOSDT_REWRITE_RESULT_HPP
