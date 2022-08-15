#include "optimizer.hpp"

namespace GOSDT {

    void
    GOSDT::Optimizer::load_dataset(GOSDT::Dataset dataset)
    {
        this->dataset = dataset;
    }

    void
    GOSDT::Optimizer::load_configuration(GOSDT::Config configuration)
    {
        auto last_configuration = this->configuration;
        this->configuration = configuration;

        // We might need to reload the Dataset here if the configuration
        // changes certain specific config flags
    }

    void
    GOSDT::Optimizer::optimize()
    {

        Queue::Message first_problem = {
                .bitset = root.value(),
                .priority = Queue::Message::Priority::ZERO
        };

        queue.push(first_problem);

        while (global_upper_bound >= global_lower_bound)
        {
            auto message = queue.pop().value();
            // TODO This should be a reference
            auto node = graph.find_or_create(message.bitset);

            if (node.lower_bound == node.upper_bound) continue;
            f32 lower_bound_prime = -std::numeric_limits<f32>::max();
            f32 upper_bound_prime = std::numeric_limits<f32>::max();
            for (usize feature = 0; feature < node.features.size; feature++)
            {
                if (node.features.get(feature) == false) continue;

                auto [lchild_id, rchild_id]
                    = split_bitset(feature, node.identifier);
                auto lchild = find_or_create(lchild_id);
                auto rchild = find_or_create(rchild_id);

                // TODO the paper pseudocode says that lb' is to be computed
                //      with a min here but that must be wrong because we'd
                //      never converge if that were the case
                // Create bounds as if feature were chosen for splitting
                lower_bound_prime = std::max(lower_bound_prime, lchild.lower_bound + rchild.lower_bound);
                upper_bound_prime = std::min(upper_bound_prime, lchild.upper_bound + rchild.upper_bound);
            }

            // Signal the parents if an update occurred
            if (node.lower_bound != lower_bound_prime
                || node.upper_bound != upper_bound_prime) {
                node.lower_bound = lower_bound_prime;
                node.upper_bound = upper_bound_prime;

                for (auto parent : graph.parents(node)) {
                    queue.push({
                        .bitset = parent.identifier,
                        .priority = Queue::Message::Priority::ONE
                    });
                }
            }

            // TODO this should be  a comparison less than some epsilon cause
            //      we're comparing floats hopefully this issue will just
            //      disappear under some integer formulation of this problem.
            if (node.lower_bound == node.upper_bound) continue;

            // Enqueue all children
            for (usize feature = 0; feature < node.features.size; feature++) {
                auto [lchild_id, rchild_id]
                    = split_bitset(feature, node.identifier);
                auto lchild = graph.find_or_create(lchild_id);
                auto rchild = graph.find_or_create(rchild_id);

                // TODO the paper pseudocode says that lb' is to be computed
                //      with a min here but that must be wrong because we'd
                //      never converge if that were the case
                // Create bounds as if feature were chosen for splitting
                lower_bound_prime = lchild.lower_bound + rchild.lower_bound;
                upper_bound_prime = lchild.upper_bound + rchild.upper_bound;
                if (lower_bound_prime < upper_bound_prime
                    && lower_bound_prime <= node.upper_bound) {
                    queue.push({
                        .bitset = lchild_id,
                        .priority = Queue::Message::Priority::ZERO
                    });
                    queue.push({
                        .bitset = rchild_id,
                        .priority = Queue::Message::Priority::ZERO
                    });
                }
            }

            // Once we get here we've solved the problemm
            // TODO implement all the extra TODOs as well as options such as
            // timeouts...

        }



    }

    std::pair<Bitset, Bitset>
    Optimizer::split_bitset(usize feature_index, Bitset capture_set)
    {
        Bitset left = Bitset::bit_and(
            dataset->features[feature_index],
            capture_set,
            true);     // subset of capture_set such that feature j is negative
        Bitset right = Bitset::bit_and(
            dataset->features[feature_index],
            capture_set,
            false);   // subset of capture_set such that feature j is positive

        return std::make_pair(left, right);
    }

    Node
    Optimizer::find_or_create(Bitset &identifier)
    {
        auto result_from_graph = graph.find(identifier);

        if (result_from_graph.has_value()) {
            return result_from_graph.value();
        }
        // TODO implement this.
        // We need to create the Node to return
        Node()
        return Node(GOSDT::Bitset(), GOSDT::Bitset());
    }

    Optimizer::OptimizerResult
    Optimizer::extract()
    {
        return Optimizer::OptimizerResult();
    }


}
