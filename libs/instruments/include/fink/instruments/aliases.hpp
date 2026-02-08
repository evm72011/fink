/**
 * @file aliases.hpp
 * @brief Convenience type aliases for common option types.
 */
#pragma once

#include <fink/instruments/options.hpp>

namespace fink::instruments
{

/**
 * @brief European call option type alias.
 *
 * Alias for a European-style option with a call payoff.
 *
 * This is a convenience name for:
 * option<european, call_payoff>
 *
 * @note
 * - This is a type alias, not a new type.
 * - It introduces no runtime or ABI overhead.
 * - Prefer this alias in pricing and user-facing code for readability.
 */
using european_call = option<european, call_payoff>;

/**
 * @brief European put option type alias.
 *
 * Alias for a European-style option with a put payoff.
 */
using european_put = option<european, put_payoff>;

/**
 * @brief American call option type alias.
 *
 * Alias for an American-style option with a call payoff.
 */
using american_call = option<american, call_payoff>;

/**
 * @brief American put option type alias.
 *
 * Alias for an American-style option with a put payoff.
 */
using american_put = option<american, put_payoff>;

} // namespace fink::instruments
