/**
 * @file result.hpp
 * @brief Result of a Monte Carlo simulation.
 */
#pragma once

#include <cstddef>

namespace fink::mc
{

/**
 * @brief Aggregated statistics of a Monte Carlo run.
 */
struct mc_result
{
    /// Estimated mean value.
    double mean{0.0};

    /// Estimated variance.
    double variance{0.0};

    /// Standard error of the mean.
    double std_err{0.0};

    /// Number of simulated paths used in the estimate.
    std::size_t paths{0};
};

} // namespace fink::mc