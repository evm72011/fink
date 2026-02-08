/**
 * @file gbm.hpp
 * @brief Geometric Brownian motion (GBM) model utilities.
 */
#pragma once

#include <cmath>

namespace fink::models
{

/**
 * @brief Parameters of a geometric Brownian motion (GBM).
 *
 * The asset price follows:
 *
 * \f[
 *   dS_t = r S_t\,dt + \sigma S_t\,dW_t
 * \f]
 *
 * under the risk-neutral measure.
 */
struct gbm_params
{
    double s0{};    ///< Initial spot price S₀
    double r{};     ///< Risk-free rate (continuous compounding)
    double sigma{}; ///< Volatility
};

/**
 * @brief Terminal asset price under GBM.
 *
 * Computes the closed-form solution:
 *
 * \f[
 *   S_T = S_0 \exp\left(
 *     \left(r - \tfrac{1}{2}\sigma^2\right)T
 *     + \sigma \sqrt{T}\, Z
 *   \right)
 * \f]
 *
 * where \f$Z \sim \mathcal{N}(0,1)\f$.
 *
 * @param p Model parameters.
 * @param T Time to maturity (in years).
 * @param Z Standard normal variate.
 */
[[nodiscard]] inline double gbm_terminal_price(const gbm_params &p,
                                               double T,
                                               double Z) noexcept
{
    return p.s0 * std::exp((p.r - 0.5 * p.sigma * p.sigma) * T +
                           p.sigma * std::sqrt(T) * Z);
}

} // namespace fink::models
