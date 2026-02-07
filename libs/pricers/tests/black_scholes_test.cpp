#include <fink/pricers/black_scholes.hpp>
#include <gtest/gtest.h>

#include <cmath>

using fink::pricers::bs_european_call;

namespace
{
constexpr double eps_tight = 1e-12;
constexpr double eps_ref = 1e-4;
} // namespace

TEST(Pricers_BlackScholes, AtTheMoneyReferenceValue)
{
    // Common reference case:
    // S0=100, K=100, r=5%, sigma=20%, T=1
    // Call price ≈ 10.4506
    const double price = bs_european_call(100.0, 100.0, 0.05, 0.2, 1.0);
    EXPECT_NEAR(price, 10.4506, eps_ref);
}

TEST(Pricers_BlackScholes, ZeroMaturityEqualsIntrinsicValue)
{
    // At T=0, European call equals intrinsic value: max(S0-K,0)
    EXPECT_DOUBLE_EQ(bs_european_call(120.0, 100.0, 0.05, 0.2, 0.0), 20.0);
    EXPECT_DOUBLE_EQ(bs_european_call(80.0, 100.0, 0.05, 0.2, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(bs_european_call(100.0, 100.0, 0.05, 0.2, 0.0), 0.0);
}

TEST(Pricers_BlackScholes, ZeroVolatilityBecomesDiscountedForwardIntrinsic)
{
    // If sigma=0, ST is deterministic under risk-neutral:
    // ST = S0 * exp(rT). Then:
    // C = exp(-rT) * max(ST - K, 0) = max(S0 - K*exp(-rT), 0)
    const double s0 = 100.0;
    const double k = 100.0;
    const double r = 0.05;
    const double T = 1.0;

    const double expected = std::max(s0 - k * std::exp(-r * T), 0.0);
    const double price = bs_european_call(s0, k, r, /*sigma=*/0.0, T);

    EXPECT_NEAR(price, expected, eps_tight);
}

TEST(Pricers_BlackScholes, DeepInTheMoneyApproachesForwardIntrinsic)
{
    // For very large S0 relative to K, call price approaches:
    // C ≈ S0 - K*exp(-rT)
    const double s0 = 1'000.0;
    const double k = 1.0;
    const double r = 0.03;
    const double sigma = 0.2;
    const double T = 2.0;

    const double approx = s0 - k * std::exp(-r * T);
    const double price = bs_european_call(s0, k, r, sigma, T);

    // Not exact, but should be very close in this regime.
    EXPECT_NEAR(price, approx, 1e-3);
}

TEST(Pricers_BlackScholes, MonotonicInSpot)
{
    const double k = 100.0;
    const double r = 0.05;
    const double sigma = 0.2;
    const double T = 1.0;

    const double c1 = bs_european_call(90.0, k, r, sigma, T);
    const double c2 = bs_european_call(100.0, k, r, sigma, T);
    const double c3 = bs_european_call(110.0, k, r, sigma, T);

    EXPECT_LT(c1, c2);
    EXPECT_LT(c2, c3);
}

TEST(Pricers_BlackScholes, MonotonicInVolatility)
{
    const double s0 = 100.0;
    const double k = 100.0;
    const double r = 0.05;
    const double T = 1.0;

    const double low = bs_european_call(s0, k, r, 0.10, T);
    const double mid = bs_european_call(s0, k, r, 0.20, T);
    const double high = bs_european_call(s0, k, r, 0.40, T);

    EXPECT_LT(low, mid);
    EXPECT_LT(mid, high);
}

TEST(Pricers_BlackScholes, MonotonicInMaturity)
{
    // Call price should be non-decreasing in T (no dividends).
    const double s0 = 100.0;
    const double k = 100.0;
    const double r = 0.05;
    const double sigma = 0.2;

    const double c_short = bs_european_call(s0, k, r, sigma, 0.25);
    const double c_mid = bs_european_call(s0, k, r, sigma, 1.0);
    const double c_long = bs_european_call(s0, k, r, sigma, 2.0);

    EXPECT_LE(c_short, c_mid);
    EXPECT_LE(c_mid, c_long);
}

TEST(Pricers_BlackScholes, NonNegativeAndUpperBoundedBySpot)
{
    const double s0 = 100.0;
    const double k = 80.0;
    const double r = 0.05;
    const double sigma = 0.3;
    const double T = 1.5;

    const double c = bs_european_call(s0, k, r, sigma, T);
    EXPECT_GE(c, 0.0);
    EXPECT_LE(c, s0);
}
