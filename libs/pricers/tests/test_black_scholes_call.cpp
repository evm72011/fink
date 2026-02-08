#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>
#include <fink/pricers/black_scholes.hpp>

#include <cmath>
#include <gtest/gtest.h>

using fink::instruments::call_payoff;
using fink::instruments::european_call;
using fink::models::gbm_params;
using fink::pricers::bs_european_call;

namespace
{
constexpr double eps_tight = 1e-12;
constexpr double eps_ref = 1e-4;

inline european_call make_call(double strike, double T)
{
    return european_call{
        .expiry = T,
        .payoff = call_payoff{.strike = strike},
    };
}

inline gbm_params make_gbm(double s0, double r, double sigma)
{
    return gbm_params{.s0 = s0, .r = r, .sigma = sigma};
}
} // namespace

TEST(Pricers_BlackScholes_Call, AtTheMoneyReferenceValue)
{
    // Reference case:
    // S0=100, K=100, r=5%, sigma=20%, T=1
    // Call price ≈ 10.4506
    const auto opt = make_call(/*strike=*/100.0, /*T=*/1.0);
    const auto model = make_gbm(/*s0=*/100.0, /*r=*/0.05, /*sigma=*/0.2);

    const double price = bs_european_call(opt, model);
    EXPECT_NEAR(price, 10.4506, eps_ref);
}

TEST(Pricers_BlackScholes_Call, ZeroMaturityEqualsIntrinsicValue)
{
    // At T=0: max(S0-K,0)
    {
        const auto opt = make_call(100.0, 0.0);
        const auto model = make_gbm(120.0, 0.05, 0.2);
        EXPECT_DOUBLE_EQ(bs_european_call(opt, model), 20.0);
    }
    {
        const auto opt = make_call(100.0, 0.0);
        const auto model = make_gbm(80.0, 0.05, 0.2);
        EXPECT_DOUBLE_EQ(bs_european_call(opt, model), 0.0);
    }
    {
        const auto opt = make_call(100.0, 0.0);
        const auto model = make_gbm(100.0, 0.05, 0.2);
        EXPECT_DOUBLE_EQ(bs_european_call(opt, model), 0.0);
    }
}

TEST(Pricers_BlackScholes_Call, ZeroVolatilityBecomesDiscountedForwardIntrinsic)
{
    // If sigma=0:
    // C = max(S0 - K*exp(-rT), 0)
    const double s0 = 100.0;
    const double k = 100.0;
    const double r = 0.05;
    const double T = 1.0;

    const auto opt = make_call(k, T);
    const auto model = make_gbm(s0, r, /*sigma=*/0.0);

    const double expected = std::max(s0 - k * std::exp(-r * T), 0.0);
    const double price = bs_european_call(opt, model);

    EXPECT_NEAR(price, expected, eps_tight);
}

TEST(Pricers_BlackScholes_Call, DeepInTheMoneyApproachesForwardIntrinsic)
{
    // For very large S0 relative to K:
    // C ≈ S0 - K*exp(-rT)
    const double s0 = 1'000.0;
    const double k = 1.0;
    const double r = 0.03;
    const double sigma = 0.2;
    const double T = 2.0;

    const auto opt = make_call(k, T);
    const auto model = make_gbm(s0, r, sigma);

    const double approx = s0 - k * std::exp(-r * T);
    const double price = bs_european_call(opt, model);

    EXPECT_NEAR(price, approx, 1e-3);
}

TEST(Pricers_BlackScholes_Call, MonotonicInSpot)
{
    const double k = 100.0;
    const double r = 0.05;
    const double sigma = 0.2;
    const double T = 1.0;

    const auto opt = make_call(k, T);

    const double c1 = bs_european_call(opt, make_gbm(90.0, r, sigma));
    const double c2 = bs_european_call(opt, make_gbm(100.0, r, sigma));
    const double c3 = bs_european_call(opt, make_gbm(110.0, r, sigma));

    EXPECT_LT(c1, c2);
    EXPECT_LT(c2, c3);
}

TEST(Pricers_BlackScholes_Call, MonotonicInVolatility)
{
    const double s0 = 100.0;
    const double k = 100.0;
    const double r = 0.05;
    const double T = 1.0;

    const auto opt = make_call(k, T);

    const double low = bs_european_call(opt, make_gbm(s0, r, 0.10));
    const double mid = bs_european_call(opt, make_gbm(s0, r, 0.20));
    const double high = bs_european_call(opt, make_gbm(s0, r, 0.40));

    EXPECT_LT(low, mid);
    EXPECT_LT(mid, high);
}

TEST(Pricers_BlackScholes_Call, MonotonicInMaturity)
{
    // Call is non-decreasing in T (no dividends).
    const double s0 = 100.0;
    const double k = 100.0;
    const double r = 0.05;
    const double sigma = 0.2;

    const auto model = make_gbm(s0, r, sigma);

    const double c_short = bs_european_call(make_call(k, 0.25), model);
    const double c_mid = bs_european_call(make_call(k, 1.0), model);
    const double c_long = bs_european_call(make_call(k, 2.0), model);

    EXPECT_LE(c_short, c_mid);
    EXPECT_LE(c_mid, c_long);
}

TEST(Pricers_BlackScholes_Call, NonNegativeAndUpperBoundedBySpot)
{
    const double s0 = 100.0;
    const double k = 80.0;
    const double r = 0.05;
    const double sigma = 0.3;
    const double T = 1.5;

    const auto opt = make_call(k, T);
    const auto model = make_gbm(s0, r, sigma);

    const double c = bs_european_call(opt, model);
    EXPECT_GE(c, 0.0);
    EXPECT_LE(c, s0);
}
