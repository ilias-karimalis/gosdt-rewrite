#include "optimizer.hpp"

namespace GOSDT {

    Optimizer::Optimizer(const Dataset &dataset, const Config &config)
    : config(config), dataset(dataset)
    {
        root = Bitset(dataset.n_rows, true);
    }

    Optimizer::Result
    GOSDT::Optimizer::optimize()
    {
        std::cout << "[Optimizer::optimize] Starting Optimization\n";
        queue.emplace(root, Message::Priority::ZERO);
        graph.find_or_create(root, {}, *this);

        while (global_upper_bound > global_lower_bound)
        {
            auto message = queue.top();
            queue.pop();
            // TODO This requires that message.bitset exists.
            auto& node = graph.find(message.bitset);

            if (node.lower_bound == node.upper_bound) continue;
            f32 lower_bound_prime = -std::numeric_limits<f32>::max();
            f32 upper_bound_prime = std::numeric_limits<f32>::max();
            for (usize feature = 0; feature < dataset.n_columns; feature++)
            {
                auto [left_id, right_id]
                    = split_bitset(feature, node.capture_set);
                auto& left_child = graph.find_or_create(left_id, &node, *this);
                auto& right_child = graph.find_or_create(right_id, &node, *this);

                // Create bounds as if feature were chosen for splitting
                lower_bound_prime = std::max(
                    lower_bound_prime,left_child.lower_bound + right_child.lower_bound);
                upper_bound_prime = std::min(
                    upper_bound_prime, left_child.upper_bound + right_child.upper_bound);
            }

            // Signal the parents if an update occurred
            if (node.lower_bound != lower_bound_prime
                || node.upper_bound != upper_bound_prime) {
                node.lower_bound = lower_bound_prime;
                node.upper_bound = upper_bound_prime;

                for (auto * parent : node.parents) {
                    if (parent == nullptr) {
                        continue;
                    }
                    queue.emplace(parent->capture_set, Message::Priority::ONE);
                }
            }

            // TODO this should be  a comparison less than some epsilon cause
            //      we're comparing floats hopefully this issue will just
            //      disappear under some integer formulation of this problem.
            if (node.lower_bound == node.upper_bound) continue;

            // Enqueue all children
            for (usize feature = 0; feature < dataset.n_columns; feature++) {
                auto [left_id, right_id]
                    = split_bitset(feature, node.capture_set);
                auto left = graph.find_or_create(left_id, &node, *this);
                auto right = graph.find_or_create(right_id, &node, *this);

                // TODO the paper pseudocode says that lb' is to be computed
                //      with a min here but that must be wrong because we'd
                //      never converge if that were the case
                // Create bounds as if feature were chosen for splitting
                lower_bound_prime = left.lower_bound + right.lower_bound;
                upper_bound_prime = left.upper_bound + right.upper_bound;
                if (lower_bound_prime < upper_bound_prime
                    && lower_bound_prime <= node.upper_bound) {
                    queue.emplace(std::move(left_id), Message::Priority::ZERO);
                    queue.emplace(std::move(right_id), Message::Priority::ZERO);
                }
            }
        }

        std::cout << "[Optimizer::optimize] Completed optimization\n";
        return {
            .time = 0.0,
            .iterations = 0,
            .model_loss = 0.0,
            .models = {}, // TODO extraction
        };
    }

    std::pair<Bitset, Bitset>
    Optimizer::split_bitset(usize feature_index, const Bitset &capture_set)
    {
        std::cout << "[Optimizer::split_bitset] splitting bitsexxt on feature index\n";
        Bitset left = Bitset::bit_and(
            dataset.features[feature_index],
            capture_set,
            true);     // subset of capture_set such that feature j is negative
        Bitset right = Bitset::bit_and(
            dataset.features[feature_index],
            capture_set,
            false);   // subset of capture_set such that feature j is positive

        return {std::move(left), std::move(right)};
    }

    std::tuple<f32, f32, f32, usize>
    Optimizer::calculate_bounds(const Bitset& capture_set) const{
        // TODO better error reporting.
        return dataset.calculate_bounds(capture_set);
    }


}
