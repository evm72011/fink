/**
 * @file discount.hpp
 * @brief Discount factor utilities.
 */
#pragma once
#include <cmath>

namespace fink::math
{

/**
 * @brief Continuous-time discounting.
 *
 * Computes the discount factor under continuous compounding:
 *
 * \f[
 *   DF(t) = e^{-r t}
 * \f]
 *
 * where:
 * - \f$r\f$ is the continuously compounded rate,
 * - \f$t\f$ is the time to maturity in years.
 *
 * @param r Continuously compounded interest rate.
 * @param t Time to maturity in years.
 */
[[nodiscard]] double discount_continuous(double r, double t) noexcept
{
    return std::exp(-r * t);
}


/**
 * @brief Discrete-time discounting with effective annual compounding.
 *
 * Computes the discount factor assuming an effective annual interest rate:
 *
 * \f[
 *   DF(t) = \frac{1}{(1 + r)^t}
 * \f]
 *
 * where:
 * - \f$r\f$ is the effective annual interest rate,
 * - \f$t\f$ is the time to maturity in years.
 *
 * @param r Annual effective interest rate.
 * @param t Time to maturity in years.
 *
 * @note
 * - This corresponds to effective annual compounding (m = 1).
 * - Other compounding frequencies (monthly, quarterly, etc.)
 *   should be converted to an effective or continuous rate before use.
 */
[[nodiscard]] double discount_discrete(double r, double t) noexcept
{
    return 1.0 / std::pow(1.0 + r, t);
}
} // namespace fink::math
