#pragma once

#include <concepts>
#include <type_traits>

#include <fink/instruments/options.hpp>

namespace fink::instruments
{

/**
 * @brief Concept for payoff function objects.
 *
 * A type models payoff_like if it can be invoked with a spot price and
 * returns a value convertible to double. The call must be noexcept.
 *
 * @tparam P Payoff type.
 *
 * @note
 * - Payoffs are intended to be lightweight value types.
 * - noexcept is required to keep instrument contracts non-throwing.
 */
template <class P>
concept payoff_like = requires(const P &p, double spot) {
    { p(spot) } noexcept -> std::convertible_to<double>;
};

/**
 * @brief Concept for option contract types.
 *
 * A type models option_like if it provides:
 * - an expiry value (time to maturity) accessible as a data member,
 * - a payoff object accessible as a data member (itself payoff_like),
 * - a compile-time exercise style (O::style).
 *
 * @tparam O Option-like type.
 *
 * @note
 * - This is a structural (duck-typed) contract; no inheritance is required.
 * - expiry is treated as time-to-maturity (e.g. in years), but units are up to the caller.
 * - The concept checks for presence and basic type compatibility only; it does not validate values
 *   (e.g. expiry > 0).
 */
template <class O>
concept option_like = payoff_like<decltype(std::declval<const O &>().payoff)> &&
                      requires(const O &o) {
                          { o.expiry } noexcept -> std::convertible_to<double>;
                          { O::style } -> std::convertible_to<exercise_style>;
                      };

/**
 * @brief Concept for European-style option contracts.
 *
 * Refinement of option_like requiring O::style == exercise_style::european.
 *
 * @tparam O Option-like type.
 */
template <class O>
concept european_option_like =
    option_like<O> && (O::style == exercise_style::european);

/**
 * @brief Concept for American-style option contracts.
 *
 * Refinement of option_like requiring O::style == exercise_style::american.
 *
 * @tparam O Option-like type.
 */
template <class O>
concept american_option_like =
    option_like<O> && (O::style == exercise_style::american);

} // namespace fink::instruments
