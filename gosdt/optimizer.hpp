#pragma once

#include <queue>

#include "utils.hpp"
#include "model.hpp"
#include "bitset.hpp"
#include "config.hpp"
#include "dataset.hpp"
#include "message.hpp"
#include "graph.hpp"

namespace GOSDT {

    struct Optimizer {

        struct Result {
            f32 time;
            u32 iterations;
            f32 model_loss;

            std::vector<Model> models;
        };

        Bitset root;
        const Config& config;
        const Dataset& dataset;

        // TODO the graph and queue have not been implemented yet.
        Graph graph;
        std::priority_queue<Message, std::vector<Message>, std::greater<>> queue;

        std::optional<f32> global_lower_bound;
        std::optional<f32> global_upper_bound;

        Optimizer(const Dataset& dataset, const Config& config);

        Result
        optimize();

        std::pair<Bitset, Bitset>
        split_bitset(usize feature_index, const Bitset & capture_set);

        [[nodiscard]] std::tuple<f32, f32, f32, usize>
        calculate_bounds(const Bitset& capture_set) const;

    };

}

