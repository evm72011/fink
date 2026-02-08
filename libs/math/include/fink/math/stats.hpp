/**
 * @file stats.hpp
 * @brief Basic statistical utilities for contiguous data sequences.
 */
#pragma once

#include <cstddef>
#include <span>

namespace fink::math
{
/**
 * @brief Compute the arithmetic mean of a sequence of values.
 *
 * Computes the arithmetic mean
 * \f[
 *   \mu = \frac{1}{n}\sum_{i=1}^{n} x_i
 * \f]
 * for a contiguous sequence of @p n values.
 *
 * @param data Pointer to the first element of the data sequence.
 * @param n    Number of elements in the sequence.
 *
 * @return Arithmetic mean of the values.
 *         Returns 0.0 if @p data is null or @p n == 0.
 *
 * @note
 * - This function does not allocate memory.
 * - This function does not throw exceptions.
 * - Numerical accuracy is sufficient for Monte Carlo statistics; no
 *   compensated summation (e.g. Kahan) is used.
 *
 * @warning
 * The caller is responsible for ensuring that @p data points to a valid
 * memory region of at least @p n elements.
 */
[[nodiscard]] double mean(const double *data, std::size_t n) noexcept;

/**
 * @brief Compute the arithmetic mean of a span of values.
 *
 * Convenience overload of mean(const double*, std::size_t) that operates
 * on a std::span.
 *
 * @param x Span referencing a contiguous sequence of values.
 *
 * @return Arithmetic mean of the values.
 *         Returns 0.0 if the span is empty.
 *
 * @note
 * - Passing std::span by value is inexpensive (pointer + size).
 * - This overload is preferred over raw pointer usage when possible.
 */
[[nodiscard]] double mean(std::span<const double> x) noexcept;

/**
 * @brief Compute the sample variance of a span of values.
 *
 * Computes the unbiased (sample) variance using normalization by (N - 1).
 * This is a convenience overload that operates on a std::span.
 *
 * @param x Span referencing a contiguous sequence of values.
 *
 * @return Sample variance of the values.
 *         Returns 0.0 if the span contains fewer than two elements.
 *
 * @note
 * - This function computes the sample variance (normalization by N - 1).
 * - For x.size() < 2, the variance is not defined; this function returns 0.0.
 * - Passing std::span by value is inexpensive (pointer + size).
 * - This overload is preferred over raw pointer usage when possible.
 */
[[nodiscard]] double variance(std::span<const double> x) noexcept;

/**
 * @brief Compute the sample variance of a sequence of values.
 *
 * Computes the unbiased (sample) variance using normalization by (N - 1).
 *
 * @param data Pointer to the first element of a contiguous sequence.
 * @param n    Number of elements in the sequence.
 *
 * @return Sample variance of the values.
 *         Returns 0.0 if n < 2.
 *
 * @note
 * - This function computes the sample variance (normalization by N - 1).
 * - For n < 2, the variance is not defined; this function returns 0.0.
 * - Prefer the std::span overload when possible.
 */
[[nodiscard]] double variance(const double *data, std::size_t n) noexcept;

/**
 * @brief Compute the sample standard deviation of a span of values.
 *
 * Computes the square root of the sample variance.
 *
 * @param x Span referencing a contiguous sequence of values.
 *
 * @return Sample standard deviation of the values.
 *         Returns 0.0 if the span contains fewer than two elements.
 *
 * @note
 * - This function computes the sample standard deviation.
 * - For x.size() < 2, the standard deviation is not defined; this function returns 0.0.
 * - Passing std::span by value is inexpensive (pointer + size).
 */
[[nodiscard]] double stddev(std::span<const double> x) noexcept;

/**
 * @brief Compute the sample standard deviation of a sequence of values.
 *
 * Computes the square root of the sample variance using normalization by (N - 1).
 *
 * @param data Pointer to the first element of a contiguous sequence.
 * @param n    Number of elements in the sequence.
 *
 * @return Sample standard deviation of the values.
 *         Returns 0.0 if n < 2.
 *
 * @note
 * - This function computes the sample standard deviation (normalization by N - 1).
 * - For n < 2, the standard deviation is not defined; this function returns 0.0.
 * - Prefer the std::span overload when possible.
 */
[[nodiscard]] double stddev(const double *data, std::size_t n) noexcept;
} // namespace fink::math
