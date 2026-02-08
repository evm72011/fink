#include <cmath>

#include <fink/math/normal.hpp>
#include <fink/pricers/black_scholes.hpp>

namespace fink::pricers::detail
{

struct bs_args
{
    double s0;
    double k;
    double r;
    double sigma;
    double T;
};

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

[[nodiscard]] inline bs_args
extract(const instruments::european_call& opt,
        const models::gbm_params& params) noexcept
{
    return {
        .s0 = params.s0,
        .k = opt.payoff.strike,
        .r = params.r,
        .sigma = params.sigma,
        .T = opt.expiry};
}

[[nodiscard]] inline bs_args
extract(const instruments::european_put& opt,
        const models::gbm_params& params) noexcept
{
    return {
        .s0 = params.s0,
        .k = opt.payoff.strike,
        .r = params.r,
        .sigma = params.sigma,
        .T = opt.expiry};
}

[[nodiscard]] inline bs_d1d2
compute_d1d2(double s0,
             double k,
             double r,
             double sigma,
             double T) noexcept
{
    const double disc_k = k * std::exp(-r * T);

    const double sqrt_T = std::sqrt(T);
    const double vol_sqrt_T = sigma * sqrt_T;

    const double var = sigma * sigma;
    const double d1 =
        (std::log(s0 / k) + (r + 0.5 * var) * T) / vol_sqrt_T;

    return {.disc_k = disc_k, .d1 = d1, .d2 = d1 - vol_sqrt_T};
}

[[nodiscard]] inline double
price_from_args(const bs_args& a, bool is_call) noexcept
{
    const double s0 = a.s0;
    const double k = a.k;
    const double r = a.r;
    const double sigma = a.sigma;
    const double T = a.T;

    if (expired(T))
    {
        return is_call ? std::max(s0 - k, 0.0)
                       : std::max(k - s0, 0.0);
    }

    const double disc_k = k * std::exp(-r * T);

    if (deterministic(sigma))
    {
        return is_call ? std::max(s0 - disc_k, 0.0)
                       : std::max(disc_k - s0, 0.0);
    }

    const auto in = compute_d1d2(s0, k, r, sigma, T);

    if (is_call)
    {
        return s0 * fink::math::norm_cdf(in.d1) -
               in.disc_k * fink::math::norm_cdf(in.d2);
    }

    return in.disc_k * fink::math::norm_cdf(-in.d2) -
           s0 * fink::math::norm_cdf(-in.d1);
}

} // namespace fink::pricers::detail


namespace fink::pricers
{

double bs_european_call(const instruments::european_call& opt,
                        const models::gbm_params& params) noexcept
{
    const auto args = detail::extract(opt, params);
    return detail::price_from_args(args, /*is_call=*/true);
}

double bs_european_put(const instruments::european_put& opt,
                       const models::gbm_params& params) noexcept
{
    const auto args = detail::extract(opt, params);
    return detail::price_from_args(args, /*is_call=*/false);
}

} // namespace fink::pricers