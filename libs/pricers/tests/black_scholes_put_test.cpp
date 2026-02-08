#include <fink/pricers/black_scholes.hpp>
#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>

#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>

using fink::instruments::call_payoff;
using fink::instruments::put_payoff;
using fink::instruments::european_call;
using fink::instruments::european_put;
using fink::models::gbm_params;

using fink::pricers::bs_european_call;
using fink::pricers::bs_european_put;

namespace
{
constexpr double eps_tight = 1e-12;
constexpr double eps_ref   = 1e-4;

inline european_call make_call(double strike, double T)
{
    return european_call{
        .expiry = T,
        .payoff = call_payoff{.strike = strike},
    };
}

inline european_put make_put(double strike, double T)
{
    return european_put{
        .expiry = T,
        .payoff = put_payoff{.strike = strike},
    };
}

inline gbm_params make_gbm(double s0, double r, double sigma)
{
    return gbm_params{.s0 = s0, .r = r, .sigma = sigma};
}
} // namespace

TEST(Pricers_BlackScholes_Put, AtTheMoneyReferenceValue)
{
    // S0=100, K=100, r=5%, sigma=20%, T=1
    // Put price ≈ 5.5735
    const auto opt   = make_put(100.0, 1.0);
    const auto model = make_gbm(100.0, 0.05, 0.2);

    const double price = bs_european_put(opt, model);
    EXPECT_NEAR(price, 5.5735, eps_ref);
}

TEST(Pricers_BlackScholes_Put, ZeroMaturityEqualsIntrinsicValue)
{
    // At T=0: max(K-S0,0)
    {
        const auto opt   = make_put(100.0, 0.0);
        const auto model = make_gbm(120.0, 0.05, 0.2);
        EXPECT_DOUBLE_EQ(bs_european_put(opt, model), 0.0);
    }
    {
        const auto opt   = make_put(100.0, 0.0);
        const auto model = make_gbm(80.0, 0.05, 0.2);
        EXPECT_DOUBLE_EQ(bs_european_put(opt, model), 20.0);
    }
    {
        const auto opt   = make_put(100.0, 0.0);
        const auto model = make_gbm(100.0, 0.05, 0.2);
        EXPECT_DOUBLE_EQ(bs_european_put(opt, model), 0.0);
    }
}

TEST(Pricers_BlackScholes_Put, ZeroVolatilityBecomesDiscountedForwardIntrinsic)
{
    // If sigma=0:
    // P = max(K*exp(-rT) - S0, 0)
    const double s0 = 100.0;
    const double k  = 100.0;
    const double r  = 0.05;
    const double T  = 1.0;

    const auto opt   = make_put(k, T);
    const auto model = make_gbm(s0, r, /*sigma=*/0.0);

    const double expected = std::max(k * std::exp(-r * T) - s0, 0.0);
    const double price    = bs_european_put(opt, model);

    EXPECT_NEAR(price, expected, eps_tight);
}

TEST(Pricers_BlackScholes_Put,
     DeepInTheMoneyApproachesDiscountedStrikeMinusSpot)
{
    // For very small S0 relative to K:
    // P ≈ K*exp(-rT) - S0
    const double s0    = 1.0;
    const double k     = 1'000.0;
    const double r     = 0.03;
    const double sigma = 0.2;
    const double T     = 2.0;

    const auto opt   = make_put(k, T);
    const auto model = make_gbm(s0, r, sigma);

    const double approx = k * std::exp(-r * T) - s0;
    const double price  = bs_european_put(opt, model);

    EXPECT_NEAR(price, approx, 1e-3);
}

TEST(Pricers_BlackScholes_Put, MonotonicInSpot)
{
    // Put decreases with spot.
    const double k     = 100.0;
    const double r     = 0.05;
    const double sigma = 0.2;
    const double T     = 1.0;

    const auto opt = make_put(k, T);

    const double p1 = bs_european_put(opt, make_gbm(90.0,  r, sigma));
    const double p2 = bs_european_put(opt, make_gbm(100.0, r, sigma));
    const double p3 = bs_european_put(opt, make_gbm(110.0, r, sigma));

    EXPECT_GT(p1, p2);
    EXPECT_GT(p2, p3);
}

TEST(Pricers_BlackScholes_Put, MonotonicInVolatility)
{
    // Put increases with volatility.
    const double s0 = 100.0;
    const double k  = 100.0;
    const double r  = 0.05;
    const double T  = 1.0;

    const auto opt = make_put(k, T);

    const double low  = bs_european_put(opt, make_gbm(s0, r, 0.10));
    const double mid  = bs_european_put(opt, make_gbm(s0, r, 0.20));
    const double high = bs_european_put(opt, make_gbm(s0, r, 0.40));

    EXPECT_LT(low, mid);
    EXPECT_LT(mid, high);
}

TEST(Pricers_BlackScholes_Put, MonotonicInMaturity)
{
    // Put non-decreasing in T (no dividends).
    const double s0    = 100.0;
    const double k     = 100.0;
    const double r     = 0.05;
    const double sigma = 0.2;

    const auto model = make_gbm(s0, r, sigma);

    const double p_short = bs_european_put(make_put(k, 0.25), model);
    const double p_mid   = bs_european_put(make_put(k, 1.0),  model);
    const double p_long  = bs_european_put(make_put(k, 2.0),  model);

    EXPECT_LE(p_short, p_mid);
    EXPECT_LE(p_mid, p_long);
}

TEST(Pricers_BlackScholes_Put, NonNegativeAndUpperBoundedByDiscountedStrike)
{
    const double s0    = 100.0;
    const double k     = 140.0;
    const double r     = 0.05;
    const double sigma = 0.3;
    const double T     = 1.5;

    const auto opt   = make_put(k, T);
    const auto model = make_gbm(s0, r, sigma);

    const double p = bs_european_put(opt, model);

    EXPECT_GE(p, 0.0);
    EXPECT_LE(p, k * std::exp(-r * T));
}

TEST(Pricers_BlackScholes_Put, PutCallParity)
{
    const double s0    = 100.0;
    const double k     = 100.0;
    const double r     = 0.05;
    const double sigma = 0.2;
    const double T     = 1.0;

    const auto call_opt = make_call(k, T);
    const auto put_opt  = make_put(k, T);
    const auto model    = make_gbm(s0, r, sigma);

    const double c = bs_european_call(call_opt, model);
    const double p = bs_european_put(put_opt, model);

    const double rhs = s0 - k * std::exp(-r * T);
    EXPECT_NEAR(c - p, rhs, 1e-10);
}
