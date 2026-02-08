/**
 * @file european_mc.hpp
 * @brief Monte Carlo pricing for European options.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>

namespace fink::pricers
{

/**
 * @brief Configuration parameters for Monte Carlo pricing.
 */
struct mc_config
{
    /// Number of Monte Carlo simulation paths.
    std::size_t paths{100'000};

    /// Seed for the random number generator.
    std::uint64_t seed{42};
};

/**
 * @brief Result of a Monte Carlo pricing run.
 */
struct mc_result
{
    /// Estimated option price.
    double price{};

    /// Standard error of the estimator.
    double stderr{};
};

/**
 * @brief Price a European call option using Monte Carlo simulation.
 *
 * Estimates the price of a European call option by simulating terminal
 * asset prices under a geometric Brownian motion (GBM) model and
 * discounting the expected payoff.
 *
 * The underlying model follows:
 *
 * \f[
 *   dS_t = r S_t\,dt + \sigma S_t\,dW_t
 * \f]
 *
 * under the risk-neutral measure.
 *
 * @param opt   European call option contract.
 * @param model Parameters of the GBM model.
 * @param cfg   Monte Carlo configuration (paths, seed).
 *
 * @return Monte Carlo pricing result containing the estimated price and
 *         its standard error.
 *
 * @note
 * - This function simulates only terminal prices \f$S_T\f$.
 * - Variance reduction techniques are not applied (baseline MC).
 * - Intended as a reference implementation and for comparison with
 *   analytic Black–Scholes pricing.
 */
[[nodiscard]] mc_result price_european_call_mc(
    const fink::instruments::european_call &opt,
    const fink::models::gbm_params &model,
    const mc_config &cfg) noexcept;

} // namespace fink::pricers
