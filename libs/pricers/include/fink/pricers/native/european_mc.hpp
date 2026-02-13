/**
 * @file european_mc.hpp
 * @brief Monte Carlo pricing for European options.
 */
#pragma once

#include <cstddef>
#include <cstdint>

#include <fink/instruments/concepts.hpp>
#include <fink/math/online_stats.hpp>
#include <fink/math/discount.hpp>
#include <fink/models/gbm.hpp>
#include <fink/rng/normal_rng.hpp>
#include <fink/rng/pcg32.hpp>
#include <fink/pricers/pricer_result.hpp>

namespace fink::pricers::native
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
 * @brief Price a European option using Monte Carlo simulation.
 *
 * Estimates the price of a European-style option by simulating terminal
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
 * @tparam Option European option type satisfying european_option_like.
 *
 * @param opt   European option contract (e.g. call or put).
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
template <fink::instruments::european_option_like Option>
[[nodiscard]] inline pricer_result price_european_mc(
    const Option &opt,
    const fink::models::gbm_params &model,
    const mc_config &cfg) noexcept
{
    fink::rng::pcg32 urng(cfg.seed);
    fink::rng::normal_rng n(urng);
    fink::math::online_stats stats;

    for (std::size_t i = 0; i < cfg.paths; ++i)
    {
        const double Z = n();
        const double ST =
            fink::models::gbm_terminal_price(model, opt.expiry, Z);
        const double payoff = opt.payoff(ST);
        stats.add(payoff);
    }

    const double df = fink::math::discount_continuous(model.r, opt.expiry);

    return pricer_result{
        .price = df * stats.mean(),
        .std_error = df * stats.stderr(),
    };
}

} // namespace fink::pricers::native
