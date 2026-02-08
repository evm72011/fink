/**
 * @file normal_rng.hpp
 * @brief Standard normal random number generator.
 */
#pragma once

#include <cmath>
#include <fink/rng/pcg32.hpp>
#include <functional>
#include <numbers>

namespace fink::rng
{
/**
 * @brief Standard normal random number generator.
 *
 * Produces independent samples from the standard normal distribution
 * \f$\mathcal{N}(0,1)\f$ using the Box–Muller transform.
 *
 * The implementation caches one of the generated values, so every second
 * call avoids recomputing logarithm, square root, and trigonometric
 * functions.
 *
 * @note
 * - Not thread-safe if shared between threads.
 * - Relies on an underlying uniform RNG providing values in (0,1).
 */
class normal_rng
{
public:
    /**
     * @brief Construct a normal generator using an underlying uniform RNG.
     *
     * @param rng Uniform random number generator used as the entropy source.
     */
    explicit normal_rng(pcg32 &rng) noexcept : rng_(rng)
    {
    }

    /* @brief Generate a standard normal random value.
     *
     * @return A sample from \f$\mathcal{N}(0,1)\f$.
     */
    [[nodiscard]] double operator()() noexcept
    {
        if (has_spare_)
        {
            has_spare_ = false;
            return spare_;
        }
        auto &rng = rng_.get();

        double u1{}, u2{};
        do // NOLINT(cppcoreguidelines-avoid-do-while)
        {
            u1 = rng.uniform();
        } while (u1 <= 0.0);

        u2 = rng.uniform();

        const double r = std::sqrt(-2.0 * std::log(u1));
        const double theta = 2.0 * std::numbers::pi * u2;

        spare_ = r * std::sin(theta);
        has_spare_ = true;
        return r * std::cos(theta);
    }

private:
    std::reference_wrapper<pcg32> rng_;
    bool has_spare_{false};
    double spare_{0.0};
};

} // namespace fink::rng
