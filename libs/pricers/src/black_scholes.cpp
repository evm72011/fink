#include <cmath>
#include <fink/math/normal.hpp>
#include <fink/pricers/black_scholes.hpp>

namespace fink::pricers
{

double bs_call(double s0, double k, double r, double sigma, double T) noexcept
{
    if (T <= 0.0) {
        return std::max(s0 - k, 0.0);
    }

    if (sigma <= 0.0) {
        // Deterministic terminal under risk-neutral: ST = S0 * exp(rT)
        // Call = exp(-rT) * max(ST - K, 0) = max(S0 - K*exp(-rT), 0)
        return std::max(s0 - k * std::exp(-r * T), 0.0);
    }

    const double vol_sqrt_T = sigma * std::sqrt(T);
    const double d1 =
        (std::log(s0 / k) + (r + 0.5 * sigma * sigma) * T) / vol_sqrt_T;
    const double d2 = d1 - vol_sqrt_T;

    return s0 * fink::math::norm_cdf(d1) -
           k * std::exp(-r * T) * fink::math::norm_cdf(d2);
}

} // namespace fink::pricers
