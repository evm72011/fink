/**
 * @file config.hpp
 * @brief Configuration parameters for Monte Carlo simulation.
 */
#pragma once

#include <cstddef>
#include <cstdint>

namespace fink::mc
{

/**
 * @brief Configuration parameters for Monte Carlo simulation.
 */
struct mc_config
{
    /// Number of Monte Carlo simulation paths.
    std::size_t paths{100'000};

    /// Seed for the random number generator.
    std::uint64_t seed{42};
};

} // namespace fink::mc