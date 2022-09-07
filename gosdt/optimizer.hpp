// GOSDT algorithm
//
//
// MIT License
//
// Copyright (c) 2022 Systopia Lab, Computer Science, University of British Columbia
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#pragma once

#include <unordered_map>

#include "utilities/numeric_types.hpp"

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
        u32 new_parent_found = 0;

        // Statistics produced by optimizing
        struct OptimizationStatistics {
            u64 time;
            u32 iterations;
            u64 upper_bound;
            u64 lower_bound;
            u64 graph_size;
        };

        Optimizer(const Dataset& dataset, const Config& config);

        OptimizationStatistics optimize();

        Result extract();

        void split_bitset_local(usize feature_index, const Bitset & capture_set);

        Node construct_node(const Bitset &identifier, const Bitset *parent_id);

        Node construct_scoped_node(const Bitset &identifier, const Bitset *parent_id);

        Node &find_or_create_local(Bitset &identifier, const Bitset *parent, usize lb_index);
    };

}

