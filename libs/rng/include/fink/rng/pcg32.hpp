/**
 * @file pcg32.hpp
 * @brief PCG32 pseudo-random number generator.
 */
#pragma once

#include <cstdint>

namespace fink::rng
{
/**
 * @brief 32-bit PCG (Permuted Congruential Generator).
 *
 * A small, fast, statistically sound pseudo-random number generator
 * suitable for Monte Carlo simulations and general-purpose numerical work.
 *
 * This implementation produces 32-bit uniformly distributed integers and
 * provides a helper function to generate uniform doubles in the interval (0,1).
 *
 * @note
 * - Not cryptographically secure.
 * - Not thread-safe if shared between threads.
 */
class pcg32
{
public:
    using result_type = std::uint32_t;

    /**
     * @brief Construct generator with a given seed.
     *
     * @param seed Initial state of the generator.
     */
    explicit pcg32(std::uint64_t seed = 0x853c49e6748fea9bULL) noexcept
        : state_(seed)
    {
        advance();
    }

    /**
     * @brief Advance the generator state once without using the value.
     */
    void advance() noexcept
    {
        static_cast<void>(next());
    }

    /**
     * @brief Generate the next 32-bit pseudo-random integer.
     *
     * @return A uniformly distributed 32-bit integer.
     */
    [[nodiscard]] result_type next() noexcept
    {
        const std::uint64_t old = state_;
        state_ = old * 6364136223846793005ULL + inc_;

        auto xorshifted =
            static_cast<std::uint32_t>(((old >> 18u) ^ old) >> 27u);
        auto rot = static_cast<std::uint32_t>(old >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((0u - rot) & 31));
    }

    /**
     * @brief Generate a uniform random number in the interval (0,1).
     *
     * @return Double precision value strictly between 0 and 1.
     *
     * @note
     * The midpoint offset (+0.5) reduces bias when converting
     * integers to floating-point values.
     */
    [[nodiscard]] double uniform() noexcept
    {
        return (static_cast<double>(next()) + 0.5) / 4294967296.0;
    }

private:
    std::uint64_t state_;
    std::uint64_t inc_{0xda3e39cb94b95bdbULL};
};

} // namespace fink::rng
