#pragma once

#include <unordered_map>

#include "utilities/numeric_types.hpp"

#include "model.hpp"
#include "bitset.hpp"
#include "node.hpp"
#include "config.hpp"
#include "dataset.hpp"
#include "message.hpp"
#include "queue.hpp"

namespace gosdt {

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

        // When we go multithreaded these are the things that need to be protected
        Queue queue;
        std::unordered_map<Bitset, Node, std::hash<Bitset>> graph;
        u64 global_lower_bound = 0;
        u64 global_upper_bound = std::numeric_limits<u64>::max();

        std::vector<Bitset> bitset_storage;
        std::vector<Node> node_storage;

        u32 nodes_created = 0;
        u32 useless_nodes_created = 0;

        // Statistics produced by optimizing
        struct OptimizationStatistics {
            u64 time;
            u32 iterations;
            u64 upper_bound;
            u64 lower_bound;
            u64 graph_size;
        };

        Optimizer(const Dataset& dataset, const Config& config);

        OptimizationStatistics
        optimize();

        Result
        extract();

        void
        split_bitset_local(usize feature_index, const Bitset & capture_set);

        Node construct_node(const Bitset &identifier, const Bitset *parent_id);

        Node &find_or_create_local(Bitset &identifier, const Bitset *parent, usize lb_index);
    };

}

