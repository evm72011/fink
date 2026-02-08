/**
 * @file options.hpp
 * @brief Core option contract types and payoff definitions.
 */
#pragma once

namespace fink::instruments
{

/**
 * @brief Option right (call or put).
 *
 * Defines whether the option payoff corresponds to a call or a put.
 */
enum class option_right // NOLINT(performance-enum-size)
{
    call,
    put
};

/**
 * @brief Exercise style of an option.
 *
 * Indicates when the option can be exercised.
 */
enum class exercise_style // NOLINT(performance-enum-size)
{
    european,
    american
};

/**
 * @brief European exercise style tag.
 *
 * Used as a compile-time tag to indicate European-style exercise.
 */
struct european
{
    static constexpr exercise_style style = exercise_style::european;
};

/**
 * @brief American exercise style tag.
 *
 * Used as a compile-time tag to indicate American-style exercise.
 */
struct american
{
    static constexpr exercise_style style = exercise_style::american;
};

/**
 * @brief Call option payoff.
 *
 * Payoff function for a European/American call option:
 * max(spot - strike, 0).
 */
struct call_payoff
{
    double strike{}; ///< Strike price of the option.

    /**
     * @brief Compute payoff at expiry or exercise.
     *
     * @param spot Spot price of the underlying.
     * @return Payoff value.
     */
    double operator()(double spot) const noexcept
    {
        return spot > strike ? (spot - strike) : 0.0;
    }
};

/**
 * @brief Put option payoff.
 *
 * Payoff function for a European/American put option:
 * max(strike - spot, 0).
 */
struct put_payoff
{
    double strike{}; ///< Strike price of the option.

    /**
     * @brief Compute payoff at expiry or exercise.
     *
     * @param spot Spot price of the underlying.
     * @return Payoff value.
     */
    double operator()(double spot) const noexcept
    {
        return spot < strike ? (strike - spot) : 0.0;
    }
};

/**
 * @brief Generic option contract.
 *
 * Represents an option as a combination of:
 * - an exercise style (European or American),
 * - a payoff function,
 * - a time to maturity.
 *
 * This type models the *instrument contract only*.
 * Pricing logic is implemented separately in pricing engines.
 *
 * @tparam Exercise Exercise style tag (e.g. european, american).
 * @tparam Payoff   Payoff type providing operator()(double).
 */
template <class Exercise, class Payoff>
struct option
{
    double expiry{}; ///< Time to maturity in years.
    Payoff payoff{}; ///< Payoff function object.

    /// Exercise style of the option (compile-time constant).
    static constexpr exercise_style style = Exercise::style;
};

} // namespace fink::instruments
