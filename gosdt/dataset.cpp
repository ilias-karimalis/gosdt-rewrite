#include "dataset.hpp"


GOSDT::Dataset::Dataset(std::istream &input_stream)
{
    std::cout << "[Dataset::Dataset] Constructing Bitmasks\n";
    construct_bitmasks(input_stream);

    std::cout << "[Dataset::Dataset] Constructing Cost Matrices\n";
    construct_cost_matrices();

    std::cout << "[Dataset::Dataset] Constructing Majority Bitmask\n";
    construct_majority_bitmask();
}

std::tuple<f32, f32, f32, usize>
GOSDT::Dataset::calculate_bounds(const Bitset& capture_set) const
{
    std::cout << "[Dataset::calculate_bounds] Computing Bounds for Capture Set\n";
    // Compute the distribution of each of the target values in what is captured by the capture_set
    std::vector<u32> distribution(n_targets);
    for (auto t = 0; t < n_targets; t++)
    {
        distribution[t] = Bitset::bit_and(targets[t], capture_set).count();
    }

    std::cout << "[Dataset::calculate_bounds] Computing the Upper Bound of a Capture Set\n";
    // Find which prediction incurs the lowest cost
    f32 upper_bound = std::numeric_limits<f32>::max();
    usize cost_minimizer = 0;
    for (auto i = 0u; i < n_targets; i++)
    {
        f32 cost = 0.0;
        for (auto j = 0u; j < n_targets; j++)
        {
            cost += costs(i, j) * (f32)distribution[j];
        }
        // Track the prediction that minimizes cost
        if (cost < upper_bound) {
            upper_bound = cost;
            cost_minimizer = i;
        }
    }

    std::cout << "[Dataset::calculate_bounds] Computing the Lower Bound of a Capture Set\n";
    // Compute the equivalent point loss for the capture set
    f32 eqp_loss = 0.0, max_cost_reduction = 0.0;
    for (usize t = 0; t < n_targets; t++)
    {
        max_cost_reduction += diff_costs[t] * (f32)distribution[t];
        // Captured majority points
        auto buffer = Bitset::bit_and(majority, capture_set);
        buffer = Bitset::bit_and(targets[t], buffer);
        eqp_loss += match_costs[t] * (f32)buffer.count();

        // Captured minority points
        buffer = Bitset::bit_and(majority, capture_set, true);
        buffer = Bitset::bit_and(targets[t], buffer);
        eqp_loss += mismatch_costs[t] * (f32)buffer.count();
    }

    auto lower_bound = eqp_loss;
    if (lower_bound > upper_bound)
        lower_bound = upper_bound;

    // Returning {lower_bound, upper_bound, max_cost_reduction, cost_minimizer}
    return {lower_bound, upper_bound, max_cost_reduction, cost_minimizer};
}

void
GOSDT::Dataset::construct_bitmasks(std::istream &input_stream)
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
GOSDT::Dataset::construct_cost_matrices() {
    costs = {n_targets, n_targets, 0.0f};
    for (usize i = 0; i < n_targets; i++)
    {
        for (usize j = 0; j < n_targets; j++)
        {
            if (i == j)
                costs(i, j) = 0.0f;
            else
                costs(i, j) = 1.0f / ((f32) n_rows * targets[j].count());
        }
    }

    auto max_costs = std::vector<f32>(n_targets, -std::numeric_limits<f32>::max());
    auto min_costs = std::vector<f32>(n_targets,  std::numeric_limits<f32>::max());
    match_costs = std::vector<f32>(n_targets, 0.0);
    mismatch_costs = std::vector<f32>(n_targets, std::numeric_limits<f32>::max());
    diff_costs = std::vector<f32>(n_targets, std::numeric_limits<f32>::max());
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
GOSDT::Dataset::construct_majority_bitmask() {

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
            auto cost = 0.0f;
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

