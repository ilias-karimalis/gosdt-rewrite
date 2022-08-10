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
        u32 status;
//        std::unordered_set<Model> models;
    };

}

#endif //GOSDT_REWRITE_RESULT_HPP
