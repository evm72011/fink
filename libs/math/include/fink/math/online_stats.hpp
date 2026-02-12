/**
 * @file online_stats.hpp
 * @brief Online mean/variance estimator (Welford algorithm).
 */
#pragma once

#include <cmath>
#include <cstddef>

namespace fink::math
{
/**
 * @brief Online estimation of mean and variance (Welford algorithm).
 *
 * Computes the sample mean and variance of a stream of values without
 * storing the full dataset. Each observation is processed in O(1) time.
 *
 * The algorithm updates the statistics incrementally. For a sequence
 * of observations \f$x_1, x_2, \dots, x_n\f$, the update step is:
 *
 * \f[
 *   \mu_n = \mu_{n-1} + \frac{x_n - \mu_{n-1}}{n}
 * \f]
 *
 * \f[
 *   M2_n = M2_{n-1} + (x_n - \mu_{n-1})(x_n - \mu_n)
 * \f]
 *
 * The (unbiased) sample variance is computed as:
 *
 * \f[
 *   s^2 = \frac{M2_n}{n-1}, \quad n > 1
 * \f]
 *
 * and the standard error of the mean:
 *
 * \f[
 *   \text{stderr} = \frac{s}{\sqrt{n}}
 * \f]
 *
 * This estimator is numerically stable and suitable for Monte Carlo
 * simulations and streaming data processing.
 *
 * @par Numerical properties
 * The Welford algorithm is more numerically stable than the naive
 * two-pass formula and avoids catastrophic cancellation when the
 * variance is small relative to the mean.
 * 
 * @note
 * - Does not allocate memory.
 * - Processes observations in constant time.
 * - Suitable for large Monte Carlo runs where storing all samples
 *   would be impractical.
 * - Uses double precision arithmetic; no compensated summation is used.
 *
 * @warning
 * The variance is undefined for fewer than two observations.
 * In this implementation, variance() returns 0.0 when n < 2.
 */
class online_stats
{
public:
    /**
     * @brief Add a new observation to the statistics.
     *
     * Updates the running mean and variance using the Welford method.
     *
     * @param x New sample value.
     *
     * @note
     * - Does not throw exceptions.
     * - Complexity: O(1).
     */
    void add(double x) noexcept
    {
        ++n_;
        const double delta = x - mean_;
        mean_ += delta / static_cast<double>(n_);
        const double delta2 = x - mean_;
        m2_ += delta * delta2;
    }

    /**
     * @brief Merge statistics from another accumulator.
     *
     * Combines two independent Welford accumulators.
     *
     * @param other Another online_stats instance.
     */
    void merge(const online_stats& other) noexcept      // TODO Unit tests for merge
    {
        if (other.n_ == 0)
            return;

        if (n_ == 0)
        {
            *this = other;
            return;
        }

        const double n1 = static_cast<double>(n_);
        const double n2 = static_cast<double>(other.n_);
        const double n = n1 + n2;

        const double delta = other.mean_ - mean_;

        mean_ = (n1 * mean_ + n2 * other.mean_) / n;
        m2_ = m2_ + other.m2_ + delta * delta * (n1 * n2 / n);
        n_ += other.n_;
    }

    /**
     * @brief Number of samples processed.
     */
    [[nodiscard]] std::size_t count() const noexcept
    {
        return n_;
    }

    /**
     * @brief Sample mean.
     */
    [[nodiscard]] double mean() const noexcept
    {
        return mean_;
    }

    /**
     * @brief Unbiased sample variance.
     *
     * Returns 0.0 if fewer than two samples were added.
     */
    [[nodiscard]] double variance() const noexcept
    {
        return (n_ > 1) ? (m2_ / static_cast<double>(n_ - 1)) : 0.0;
    }

    /**
     * @brief Sample standard deviation.
     */
    [[nodiscard]] double stddev() const noexcept
    {
        return std::sqrt(variance());
    }

    /**
     * @brief Standard error of the mean.
     *
     * Defined as:
     * \f[
     *   \frac{s}{\sqrt{n}}
     * \f]
     */
    [[nodiscard]] double stderr() const noexcept
    {
        return (n_ > 0) ? stddev() / std::sqrt(static_cast<double>(n_)) : 0.0;
    }

private:
    std::size_t n_{0};
    double mean_{0.0};
    double m2_{0.0};
};
} // namespace fink::math