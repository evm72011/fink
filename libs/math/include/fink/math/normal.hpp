#pragma once

#include <cmath>
#include <numbers>

namespace fink::math
{

/**
 * @brief Standard normal probability density function.
 *
 * Computes the probability density function (PDF) of a standard normal
 * random variable:
 *
 * \f[
 *   \varphi(x) = \frac{1}{\sqrt{2\pi}} e^{-x^2 / 2}
 * \f]
 *
 * where \f$x \in \mathbb{R}\f$.
 *
 * @param x Point at which to evaluate the density.
 *
 * @note
 * - The standard normal distribution has mean 0 and variance 1.
 * - This function is symmetric: \f$\varphi(x) = \varphi(-x)\f$.
 */
[[nodiscard]] constexpr double norm_pdf(double x) noexcept
{
    constexpr double inv_sqrt_2pi = 0.39894228040143267794; // 1/sqrt(2π)
    return inv_sqrt_2pi * std::exp(-0.5 * x * x);
}

/**
 * @brief Standard normal cumulative distribution function.
 *
 * Computes the cumulative distribution function (CDF) of a standard normal
 * random variable.
 *
 * The CDF is defined as:
 *
 * \f[
 *   \Phi(x) = \int_{-\infty}^{x} \varphi(t)\,dt
 * \f]
 *
 * which can be expressed in closed form using the error function:
 *
 * \f[
 *   \Phi(x) =
 *   \frac{1}{2}
 *   \left(
 *     1 + \operatorname{erf}\left(\frac{x}{\sqrt{2}}\right)
 *   \right)
 * \f]
 *
 * The error function \f$\operatorname{erf}(x)\f$ is defined by:
 *
 * \f[
 *   \operatorname{erf}(x)
 *   =
 *   \frac{2}{\sqrt{\pi}}
 *   \int_{0}^{x} e^{-t^{2}} \, dt
 * \f]
 *
 * @param x Point at which to evaluate the distribution function.
 *
 * @note
 * - This implementation relies on \c std::erf from the C++ standard library.
 * - \c std::erf is not \c constexpr, therefore this function cannot be
 *   \c constexpr either.
 * - For large \f$|x|\f$, numerical accuracy is determined by the quality
 *   of the underlying libm implementation.
 */
[[nodiscard]] inline double norm_cdf(double x) noexcept
{
    return 0.5 * (1.0 + std::erf(x / std::numbers::sqrt2));
}

} // namespace fink::math
