#include "optimizer.hpp"
#include "utilities/logging.hpp"
#include "utilities/time.hpp"

namespace gosdt {

    Optimizer::Optimizer(const Dataset &dataset, const Config &config)
    : config(config), dataset(dataset)
    {
        root = Bitset(dataset.n_rows, true);
        bitset_storage.resize(dataset.n_columns * 2);
        node_storage.resize(dataset.n_columns * 2);
        for (usize i = 0; i < dataset.n_columns * 2; i++) {
            bitset_storage[i] = Bitset(dataset.n_rows);
        }
    }

    Optimizer::OptimizationStatistics
    Optimizer::optimize()
    {
        std::cout << "[gosdt::Optimizer::optimize] Starting Optimization\n";
        auto start_time = std::chrono::steady_clock::now();
        queue.emplace(root, Message::Priority::ZERO);
        auto root_node = construct_node(root, nullptr);
        graph.emplace(root, std::move(root_node));

        u32 n_iterations = 0;
        while (global_upper_bound - global_lower_bound > config.optimality_gap
            && !queue.empty()
            && !time_limit_exceeded<ch::steady_clock, ch::seconds>(start_time, config.time_limit))
        {
            DOUT << "[gosdt::Optimizer::optimize] [ITERATION, QUEUE_SIZE, GRAPH_SIZE]: [" << n_iterations
                 << ", " << queue.size() << ", " << graph.size() << "]\n";

            n_iterations++;

            // NOTE: This requires that message.bitset exists.
            auto message_bitset = queue.top();
            auto graph_iterator = graph.find(message_bitset);
            queue.pop();

            DASSERT(graph_iterator != graph.end());
            const Bitset& bitset = graph_iterator->first;
            Node& node = graph_iterator->second;
            DVOUT << "Node UB: " << node.upper_bound << " LB: " << node.lower_bound << std::endl;

            u64 lower_bound_prime = 0;
            u64 upper_bound_prime = std::numeric_limits<u64>::max();
            for (usize feature = 0; feature < dataset.n_columns; feature++)
            {
                split_bitset_local(feature, message_bitset);

                // There should be:
                //      - Minimum Support Bound
                //      - Incremental Accuracy
                //      - Leaf Accuracy Bound -> Never split this leaf
                // When we create each of these children, we should do so locally, using a local buffer of size 2*n_features
                // which holds <Bitset, Node> pairs, and then we can decide whether to add them into the graph after the fact
                // based on whether they're good enough to be elements of a final tree.
                // TODO also we should create the nodes in the local storage buffer.
                if (bitset_storage[2*feature].empty() || bitset_storage[2*feature + 1].empty())
                    continue;

                auto& lchild = find_or_create_local(bitset_storage[2*feature], &bitset, 2*feature);
                auto& rchild = find_or_create_local(bitset_storage[2*feature + 1], &bitset, 2*feature+1);

                // Create bounds as if feature were chosen for splitting
                lower_bound_prime = std::max(
                    lower_bound_prime,lchild.lower_bound + rchild.lower_bound);
                upper_bound_prime = std::min(
                    upper_bound_prime, lchild.upper_bound + rchild.upper_bound);
            }

            // Signal the parents if an update occurred
            if (node.lower_bound != lower_bound_prime
                || node.upper_bound != upper_bound_prime) {

                node.lower_bound = lower_bound_prime;
                node.upper_bound = upper_bound_prime;
                for (auto parent : node.parents) {
                    if (parent == nullptr) {
                        continue;
                    }
                    queue.emplace(*parent, Message::Priority::ONE);
                }
            }

            if (node.lower_bound >= node.upper_bound) {
                node.lower_bound = node.upper_bound;
                continue;
            }

            for (usize feature = 0; feature < dataset.n_columns; feature++) {

                if (bitset_storage[2*feature].empty() || bitset_storage[2*feature + 1].empty())
                    continue;

                auto& lchild = node_storage[2*feature];
                auto& rchild = node_storage[2*feature + 1];

                // Create bounds as if feature were chosen for splitting
                lower_bound_prime = lchild.lower_bound + rchild.lower_bound;
                upper_bound_prime = lchild.upper_bound + rchild.upper_bound;
                if (lower_bound_prime < upper_bound_prime
                    && lower_bound_prime <= node.upper_bound) {
                    graph.emplace(bitset_storage[2*feature], std::move(node_storage[2*feature]));
                    graph.emplace(bitset_storage[2*feature + 1], std::move(node_storage[2*feature + 1]));
                    queue.emplace(bitset_storage[2*feature], Message::Priority::ZERO);
                    queue.emplace(bitset_storage[2*feature + 1], Message::Priority::ZERO);
                }
            }

            if (bitset == root) {
                global_upper_bound = node.upper_bound;
                global_lower_bound = node.lower_bound;
            }
        }

        auto end_time = std::chrono::steady_clock::now();
        root_node = graph.find(root)->second;
        std::cout << "[gosdt::Optimizer::optimize] Completed optimization\n";
        global_upper_bound = root_node.upper_bound;
        global_lower_bound = root_node.lower_bound;

        std::cout << "Nodes Created: " << nodes_created << " Bad Nodes: " << useless_nodes_created << std::endl;

        return {
            .time = duration<ch::steady_clock, ch::milliseconds>(start_time, end_time),
            .iterations = n_iterations,
            .upper_bound = global_upper_bound,
            .lower_bound = global_lower_bound,
            .graph_size = graph.size()
        };
    }

    Optimizer::Result
    Optimizer::extract() {
        return {
            .time = 0,
            .iterations = 0,
            .model_loss = 0,
            .models = {},
        };
    }

    void
    Optimizer::split_bitset_local(usize feature_index, const Bitset &capture_set)
    {
        DVOUT << "[Optimizer::split_bitset_local] splitting bitset on feature index: " << feature_index << std::endl;
        // subset of capture_set such that feature j is negative
        Bitset::bit_and(dataset.features[feature_index], capture_set, bitset_storage[2*feature_index], true);
        // subset of capture_set such that feature j is positive
        Bitset::bit_and(dataset.features[feature_index], capture_set, bitset_storage[2*feature_index + 1], false);
    }

    Node
    Optimizer::construct_node(const Bitset& identifier, const Bitset* parent_id)
    {
        auto node = Node(parent_id);
        auto [lower_bound, upper_bound, optimal_feature] = dataset.calculate_bounds(identifier);
        node.lower_bound = lower_bound;
        node.upper_bound = upper_bound;
        node.optimal_feature = optimal_feature;

        nodes_created++;
        // Check if we fail some bounds.
        auto incremental_accuracy = (node.upper_bound - node.lower_bound) <= dataset.n_rows;
        auto leaf_accuracy = node.upper_bound <= 2 * dataset.n_rows;
        if (incremental_accuracy || leaf_accuracy) {
            node.lower_bound = node.upper_bound;
            useless_nodes_created++;
        }
        return node;
    }

    Node&
    Optimizer::find_or_create_local(Bitset &identifier, const Bitset *parent, usize lb_index)
    {
        if (graph.contains(identifier)) {
            return graph.find(identifier)->second;
        }
        node_storage[lb_index] = construct_node(identifier, parent);
        return node_storage[lb_index];

    }
}
