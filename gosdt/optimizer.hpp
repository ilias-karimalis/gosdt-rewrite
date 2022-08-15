#pragma once

#include "utils.hpp"
#include "bitset.hpp"
#include "dataset.hpp"
#include "queue.hpp"
#include "graph.hpp"
#include "model.hpp"

namespace GOSDT {

    struct Optimizer {

        struct OptimizerResult {
            f32 time;
            u32 iterations;
            f32 model_loss;

            std::vector<Model> models;
        };

        std::optional<Bitset> root;
        std::optional<Config> configuration;
        std::optional<Dataset> dataset;

        // TODO the graph and queue have not been implemented yet.
        Graph graph;
        Queue queue;

        std::optional<f32> global_boundary;
        std::optional<f32> global_lower_bound;
        std::optional<f32> global_upper_bound;

        Optimizer() = default;

        void
        load_dataset(Dataset dataset);

        void
        load_configuration(Config configuration);

        void
        optimize();

        OptimizerResult
        extract();

        std::pair<Bitset, Bitset>
        split_bitset(usize feature_index, Bitset capture_set);

        Node
        find_or_create(Bitset& identifier);

    };

}

