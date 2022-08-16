#include "dataset.hpp"

namespace gosdt {

    Dataset::Dataset(std::istream &input_stream)
    {

        /*
         * We start by creating the Encoder object and then extracting the various
         * parts of the binarized dataset.
         * TODO Maybe we don't even keep the encoder object around and instead
         *      std::move all the things we do need and let it free itself once it
         *      leaves the scope of this constructor.
         */
        auto encoder = Encoder(input_stream);

        n_rows    = encoder.n_binary_rows;
        n_columns = encoder.n_binary_columns;
        n_targets = encoder.n_targets;

        // Avoid copying vectors by moving them out of the encoder object.
        // Note that after the fact, encode.binary_rows is empty
        rows = std::move(encoder.binary_rows);

        features.resize(n_columns, Bitset(n_rows, false));
        targets.resize(n_targets, Bitset(n_rows, false));

        for (usize i = 0; i < n_rows; i++)
        {
            // This is here with the hope that this aids cache access by trying to
            // bring this Bitset into cache, we'll see how well that works later.
            auto row_i = rows[i];

            for (usize j = 0; j < n_columns; j++)
            {
                // Will crash if the optional doesn't hold the value
                // Not sure if this is the best way to do this, will need to ask
                // Jerry once he's less busy
                features[i].set(j, row_i.get(j).value());
            }

            for (usize t = 0; t < n_targets; t++)
            {
                // Crashes exactly as the one above would.
                targets[i].set(t, row_i.get(t + n_columns).value());
            }
        }

        // At this point we've constructed the bitmasks representing the dataset.
        // TODO below we're creating the costs matrix which I'm not sure it's
        //      actually useful.
        costs = {n_targets, n_targets, 0.0f};
        for (usize i = 0; i < n_targets; i++)
        {
            for (usize j = 0; j < n_targets; j++)
            {
                if (i == j)
                    costs(i, j) = 0.0f;
                else
                    costs(i, j) = 1.0f / ((f32) n_rows);
            }
        }

        auto max_costs = std::vector<f32>(n_targets, -std::numeric_limits<f32>::max());
        auto min_costs = std::vector<f32>(n_targets,  std::numeric_limits<f32>::max());
        for (usize i = 0; i < n_targets; i++)
        {
            for (usize j = 0; j < n_targets; j++)
            {
                max_costs[i] = std::max(max_costs[i], costs(j, i));
                min_costs[i] = std::min(min_costs[i], costs(j, i));
            }
        }

        for (auto i = 0; i < n_targets; i++)
        {
            diff_costs[i] = max_costs[i] - min_costs[i];
        }

    }

    std::tuple<f32, f32>
    calculate_bounds(Bitset * capture_set)
    {
        Bitset buffer;
        // Compute the distribution of each of the target values in waht is captured by the capture_set
        std::vector<u32> distribution(n_targets);
        for (auto t = 0; t < n_targets; t++)
        {
            distribution[t] = Bitset::bit_and(targets[t], capture_set).count();
        }

        // Find which prediction incurs the lowest cost
        f32 min_cost = std::numeric_limits<f32>::max();
        usize cost_minimizer = 0;
        for (auto i = 0; i < n_targets; i++)
        {
            auto cost = 0.0
            for (auto j = 0; j < n_targets; j++)
            {
                cost += costs(i, j) * distribution[j];
            }
            // Track the prediction that minimizes cost
            if (cost < min_cost) {
                min_cost = cost;
                cost_minimizer = i;
            }
        }

        // Compute the equivalent point loss for the capture set
        f32 eqp_loss = 0.0, max_cost_reduction = 0.0;
        f32 support = (float)(capture_set.count()) / (float)(n_rows);
        for (auto t = 0; i < n_targets; t++)
        {
            max_cost_reduction += diff_costs[t] * distribution[t];
            // We now need a copy constructor for Bitset
        }

}
