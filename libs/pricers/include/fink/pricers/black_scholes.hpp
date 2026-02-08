/**
 * @file black_scholes.hpp
 * @brief Black–Scholes analytic pricing formulas.
 */
#pragma once

#include <cmath>
#include <fink/math/normal.hpp>

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
 * @param s0    Spot price of the underlying asset.
 * @param k     Strike price.
 * @param r     Risk-free continuously compounded interest rate.
 * @param sigma Volatility of the underlying asset.
 * @param T     Time to maturity in years.
 *
 * @return Black–Scholes price of the European call option.
 *
 * @note
 * - This function assumes no dividends.
 * - This is an analytic benchmark, useful for validation of Monte Carlo pricers.
 */
[[nodiscard]] double bs_european_call(double s0,
                                      double k,
                                      double r,
                                      double sigma,
                                      double T) noexcept;


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
 * @param s0    Spot price of the underlying asset.
 * @param k     Strike price.
 * @param r     Risk-free continuously compounded interest rate.
 * @param sigma Volatility of the underlying asset.
 * @param T     Time to maturity in years.
 *
 * @return Black–Scholes price of the European put option.
 *
 * @note
 * - This function assumes no dividends.
 * - This is an analytic benchmark, useful for validation of Monte Carlo pricers.
 */
[[nodiscard]] double bs_european_put(double s0,
                                     double k,
                                     double r,
                                     double sigma,
                                     double T) noexcept;
} // namespace fink::pricers
