#pragma once

#include <cmath>
#include <fink/rng/pcg32.hpp>
#include <functional>
#include <numbers>

namespace fink::rng
{

class normal_rng
{
public:
    explicit normal_rng(pcg32 &rng) noexcept : rng_(rng)
    {
    }

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
