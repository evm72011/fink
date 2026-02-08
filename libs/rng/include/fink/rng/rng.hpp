/**
 * @file concepts.hpp
 * @brief Concepts for random number generators.
 */
#pragma once
#include <concepts>

namespace fink::rng
{
/**
 * @brief Concept for uniform random number generators.
 *
 * Requires a member function uniform() returning a value convertible
 * to double and guaranteed not to throw.
 */
template <class R>
concept uniform_rng = requires(R &r) {
    { r.uniform() } noexcept -> std::convertible_to<double>;
};

/**
 * @brief Concept for standard normal random number generators.
 *
 * Requires a callable object producing values convertible to double,
 * typically representing samples from N(0,1).
 */
template <class R>
concept normal_rng_like = requires(R &r) {
    { r() } noexcept -> std::convertible_to<double>;
};

} // namespace fink::rng
