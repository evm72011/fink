/**
 * @file black_scholes.hpp
 * @brief Black–Scholes analytic pricing formulas.
 */
#pragma once

#include <cmath>
#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>

namespace fink::pricers
{
/**
 * @brief Black–Scholes price of a European call option.
 *
 * Computes the closed-form Black–Scholes price for a European call option
 * under the risk-neutral measure.
 *
 * The formula is:
 *
 * \f[
 *   C = S_0 \Phi(d_1) - K e^{-rT} \Phi(d_2)
 * \f]
 *
 * where
 *
 * \f[
 *   d_1 = \frac{\ln(S_0/K) + (r + \tfrac{1}{2}\sigma^2)T}{\sigma \sqrt{T}},
 *   \quad
 *   d_2 = d_1 - \sigma \sqrt{T}
 * \f]
 *
 * and \f$\Phi(\cdot)\f$ is the standard normal cumulative distribution function.
 *
 * @param opt     European call option (strike, expiry).
 * @param params  GBM parameters (spot, rate, volatility).
 *
 * @return Black–Scholes price of the European call option.
 *
 * @note
 * - Assumes no dividends.
 * - Useful as an analytic benchmark for Monte Carlo pricers.
 */
[[nodiscard]] double bs_european_call(
    const instruments::european_call &opt,
    const models::gbm_params &params) noexcept;

/**
 * @brief Black–Scholes price of a European put option.
 *
 * Computes the closed-form Black–Scholes price for a European put option
 * under the risk-neutral measure.
 *
 * The formula is:
 *
 * \f[
 *   P = K e^{-rT} \Phi(-d_2) - S_0 \Phi(-d_1)
 * \f]
 *
 * where
 *
 * \f[
 *   d_1 = \frac{\ln(S_0/K) + (r + \tfrac{1}{2}\sigma^2)T}{\sigma \sqrt{T}},
 *   \quad
 *   d_2 = d_1 - \sigma \sqrt{T}
 * \f]
 *
 * and \f$\Phi(\cdot)\f$ is the standard normal cumulative distribution function.
 *
 * @param opt     European put option (strike, expiry).
 * @param params  GBM parameters (spot, rate, volatility).
 *
 * @return Black–Scholes price of the European put option.
 *
 * @note
 * - Assumes no dividends.
 * - Useful as an analytic benchmark for Monte Carlo pricers.
 */
[[nodiscard]] double bs_european_put(const instruments::european_put &opt,
                                     const models::gbm_params &params) noexcept;
} // namespace fink::pricers
