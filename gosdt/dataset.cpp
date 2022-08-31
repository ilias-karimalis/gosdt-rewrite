#include "dataset.hpp"
#include "utilities/logging.hpp"

namespace gosdt {

    Dataset::Dataset(std::istream &input_stream, const Config& configuration)
    : config(configuration)
    {
        DOUT << "[Dataset::Dataset] Constructing Dataset\n";

        DVOUT << "[Dataset::Dataset] Constructing Bitmasks\n";
        construct_bitmasks(input_stream);
        DVOUT << "[Dataset::Dataset] Constructing Cost Matrices\n";
        construct_cost_matrices();
        DVOUT << "[Dataset::Dataset] Constructing Majority Bitmask\n";
        construct_majority_bitmask();
    }

    std::tuple<u64, u64, u64, usize>
    Dataset::calculate_bounds(const Bitset& capture_set) const
    {
        DVOUT << "[Dataset::calculate_bounds] Computing Bounds for Capture Set\n";
        // Compute the distribution of each of the target values in what is captured by the capture_set
        auto work_buffer = Bitset(capture_set.size);
        std::vector<u32> distribution(n_targets);
        for (usize t = 0; t < n_targets; t++)
        {
            // z_u-, z_u+ is distribution!
            Bitset::bit_and(targets[t], capture_set, work_buffer, false);
            distribution[t] = work_buffer.count();
        }

        DVOUT << "[Dataset::calculate_bounds] Computing the Upper Bound of a Capture Set\n";
        DVOUT << "Distributions: " << distribution[0] << " " << distribution[1] << std::endl;
        DVOUT << "Costs: " << costs(0, 0) << " " << costs(0, 1) << " " << costs(1, 0) << " " << costs(1, 1) << std::endl;
        // Find which prediction incurs the lowest cost
        u64 max_loss = std::numeric_limits<u64>::max();
        usize cost_minimizer = 0;
        for (auto i = 0u; i < n_targets; i++)
        {
            u64 cost = 0;
            for (auto j = 0u; j < n_targets; j++)
            {
                // One of these will be 0 when i == j
                cost += costs(i, j) * distribution[j];
            }
            // Track the prediction that minimizes cost
            if (cost < max_loss) {
                max_loss = cost;
                cost_minimizer = i;
            }
            // Max loss is the min(...) in GOSDT Equation 10 (scaled by N/lambda)
        }

        DVOUT << "[Dataset::calculate_bounds] Computing the Lower Bound of a Capture Set\n";
        // Compute the equivalent point loss for the capture set
        u64 eqp_loss = 0, max_cost_reduction = 0;
        for (usize t = 0; t < n_targets; t++)
        {
            max_cost_reduction += diff_costs[t] * distribution[t];
            // Captured majority points
            Bitset::bit_and(majority, capture_set, work_buffer, false);
            Bitset::bit_and(targets[t], work_buffer, work_buffer, false);
            eqp_loss += match_costs[t] * work_buffer.count();

            // Captured minority points
            Bitset::bit_and(majority, capture_set, work_buffer, true);
            Bitset::bit_and(targets[t], work_buffer, work_buffer, false);
            eqp_loss += mismatch_costs[t] * work_buffer.count();
        }

        auto min_loss = eqp_loss;
        if (min_loss > max_loss)
            min_loss = max_loss;

        DVOUT << "[Dataset::calculate_bounds] Min Loss: " << min_loss << std::endl
                << "Max Loss: " << max_loss << std::endl
                << "Max Cost Reduction: " << max_cost_reduction << std::endl
                << "Cost Minimizing Target: " << cost_minimizer << std::endl;
        // Returning {min_loss, max_loss, max_cost_reduction, cost_minimizer}
        return {min_loss, max_loss, max_cost_reduction, cost_minimizer};
    }

    void
    Dataset::construct_bitmasks(std::istream &input_stream)
    {
        /*
        * We start by creating the Encoder object and then extracting the various
        * parts of the binarized dataset.
        * TODO Maybe we don't even keep the encoder object around and instead
        *      std::move all the things we do need and let it free itself once it
        *      leaves the scope of this constructor.
        */
        auto encoder = Encoder(input_stream);

        n_rows    = encoder.n_rows;
        n_columns = encoder.n_binary_columns;
        n_targets = encoder.n_binary_targets;

        // Avoid copying vectors by moving them out of the encoder object.
        // Note that after the fact, encode.binary_rows is empty
        rows = std::move(encoder.binary_rows);

        // We're crashing on this line right now.
        features.resize(n_columns, Bitset(n_rows, false));
        targets.resize(n_targets, Bitset(n_rows, false));

        for (usize i = 0; i < n_columns; i++) {
            for (usize j = 0; j < n_rows; j++) {
                features[i].set(j, rows[j].get(i));
            }
        }

        for (usize i = 0; i < n_targets; i++) {
            for (usize j = 0; j < n_rows; j++) {
                targets[i].set(j, rows[j].get(i + n_columns));
            }
        }
    }

    void
    Dataset::construct_cost_matrices() {
        costs = {n_targets, n_targets, 0};
        for (usize i = 0; i < n_targets; i++)
        {
            for (usize j = 0; j < n_targets; j++)
            {
                if (i != j) {
                    costs(i, j) = config.regularization;
                }
            }
        }

        auto max_costs = std::vector<u64>(n_targets, 0);
        auto min_costs = std::vector<u64>(n_targets,  std::numeric_limits<u64>::max());
        match_costs = std::vector<u64>(n_targets, 0);
        mismatch_costs = std::vector<u64>(n_targets, std::numeric_limits<u64>::max());
        diff_costs = std::vector<u64>(n_targets, std::numeric_limits<u64>::max());
        for (usize i = 0; i < n_targets; i++)
        {
            for (usize j = 0; j < n_targets; j++)
            {
                max_costs[i] = std::max(max_costs[i], costs(j, i));
                min_costs[i] = std::min(min_costs[i], costs(j, i));
                if (i == j)
                    match_costs[i] = costs(j, i);
                else
                    mismatch_costs[i] = std::min(mismatch_costs[j], costs(j, i));
            }
        }

        for (usize i = 0; i < n_targets; i++)
        {
            diff_costs[i] = max_costs[i] - min_costs[i];
        }
    }

    void
    Dataset::construct_majority_bitmask() {

        auto distributions = Matrix<u32>(n_rows, n_targets, 0);
        for (usize i = 0; i < n_rows; i++)
        {
            for (usize j = 0; j < n_targets; j++)
            {
                distributions(i, j) += rows[i].get(j + n_columns);
            }
        }

        auto minimizers = std::vector<usize>(n_rows);
        for (usize i = 0; i < n_rows; i++)
        {
            auto minimum = std::numeric_limits<f32>::max();
            usize minimizer = 0;
            for (usize j = 0; j < n_targets; j++)
            {
                auto cost = 0;
                for (usize k = 0; k < n_targets; k++)
                {
                    cost += costs(j, k) * (f32)distributions(i, k);
                }
                
                if (cost < minimum) {
                    minimum = cost;
                    minimizer = j;
                }
            }
            minimizers[i] = minimizer;
        }

        majority = {n_rows, false};
        for (usize i = 0; i < n_rows; i++)
        {
            auto minimizer = minimizers[i];
            auto label = rows[i].scan(n_columns, true) - n_columns;
            majority.set(i, minimizer == label);
        }
    }

}