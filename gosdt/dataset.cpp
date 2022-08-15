#include "dataset.hpp"

GOSDT::Dataset::Dataset(std::istream &input_stream)
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
        // bring this Bitmask into cache, we'll see how well that works later.
        auto row_i = rows[i];

        for (usize j = 0; j < n_columns; j++)
        {
            // Will crash if the optional doesn't hold the value
            // Not sure if this is the best way to do this, will need to ask
            // Jerry once he's less busy
            auto rows_entry = row_i.get(j).value();
            features[i].set(j, rows_entry);
        }

        for (usize t = 0; t < n_targets; t++)
        {
            // Crashes exactly as the one above would.
            auto rows_entry = row_i.get(t + n_columns).value();
            targets[i].set(t, rows_entry);
        }
    }

    // At this point we've constructed the bitmasks representing the dataset.
    // TODO below we're creating the costs matrix which I'm not sure it's
    // actually useful.
    costs = {n_targets, n_targets, 0.0f};
    for (usize i = 0; i < n_targets; i++)
    {
        for (usize j = 0; j < n_targets; j++)
        {
            if (i == j) {costs(i, j) = 0.0f;
            else costs(i, j) = 1.0f / ((f32) n_rows);
        }
    }

    for (usize i = 0; i < n_targets; i++)
    {
        for (usize j = 0; j < n_targets; j++)
        {
            std::cout << costs(i, j);
        }
    }

}
