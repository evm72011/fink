#include <cmath>
#include <fink/math/normal.hpp>
#include <fink/pricers/black_scholes.hpp>

namespace
{
struct bs_d1d2
{
    double disc_k;
    double d1;
    double d2;
};

[[nodiscard]] inline bool expired(double T) noexcept
{
    return T <= 0.0;
}

[[nodiscard]] inline bool deterministic(double sigma) noexcept
{
    return sigma <= 0.0;
}

[[nodiscard]] inline bs_d1d2 compute_d1d2(double s0,
                                          double k,
                                          double r,
                                          double sigma,
                                          double T) noexcept
{
    const double disc_k = k * std::exp(-r * T);

    const double sqrt_T = std::sqrt(T);
    const double vol_sqrt_T = sigma * sqrt_T;

    const double var = sigma * sigma;
    const double d1 = (std::log(s0 / k) + (r + 0.5 * var) * T) / vol_sqrt_T;

    return {.disc_k = disc_k, .d1 = d1, .d2 = d1 - vol_sqrt_T};
}
} // namespace


namespace fink::pricers
{

double bs_european_call(double s0,
                        double k,
                        double r,
                        double sigma,
                        double T) noexcept
{
    if (expired(T))
    {
        return std::max(s0 - k, 0.0);
    }

    const double disc_k = k * std::exp(-r * T);

    if (deterministic(sigma))
    {
        // Call = max(S0 - K*exp(-rT), 0)
        return std::max(s0 - disc_k, 0.0);
    }

    const auto in = compute_d1d2(s0, k, r, sigma, T);

    return s0 * fink::math::norm_cdf(in.d1) -
           in.disc_k * fink::math::norm_cdf(in.d2);
}

double bs_european_put(double s0,
                       double k,
                       double r,
                       double sigma,
                       double T) noexcept
{
    if (expired(T))
    {
        return std::max(k - s0, 0.0);
    }

    const double disc_k = k * std::exp(-r * T);

    if (deterministic(sigma))
    {
        // Put = max(K*exp(-rT) - S0, 0)
        return std::max(disc_k - s0, 0.0);
    }

    const auto in = compute_d1d2(s0, k, r, sigma, T);

    return in.disc_k * fink::math::norm_cdf(-in.d2) -
           s0 * fink::math::norm_cdf(-in.d1);
}
} // namespace fink::pricers
