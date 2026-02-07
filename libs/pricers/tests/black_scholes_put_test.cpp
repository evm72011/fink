#include <fink/pricers/black_scholes.hpp>
#include <gtest/gtest.h>

#include <algorithm>
#include <cmath>

using fink::pricers::bs_european_call;
using fink::pricers::bs_european_put;

namespace
{
constexpr double eps_tight = 1e-12;
constexpr double eps_ref = 1e-4;
} // namespace

TEST(Pricers_BlackScholes_Put, AtTheMoneyReferenceValue)
{
    // Common reference case:
    // S0=100, K=100, r=5%, sigma=20%, T=1
    // Put price ≈ 5.5735
    const double price = bs_european_put(100.0, 100.0, 0.05, 0.2, 1.0);
    EXPECT_NEAR(price, 5.5735, eps_ref);
}

TEST(Pricers_BlackScholes_Put, ZeroMaturityEqualsIntrinsicValue)
{
    // At T=0, European put equals intrinsic value: max(K-S0,0)
    EXPECT_DOUBLE_EQ(bs_european_put(120.0, 100.0, 0.05, 0.2, 0.0), 0.0);
    EXPECT_DOUBLE_EQ(bs_european_put(80.0, 100.0, 0.05, 0.2, 0.0), 20.0);
    EXPECT_DOUBLE_EQ(bs_european_put(100.0, 100.0, 0.05, 0.2, 0.0), 0.0);
}

TEST(Pricers_BlackScholes_Put, ZeroVolatilityBecomesDiscountedForwardIntrinsic)
{
    // If sigma=0, ST is deterministic under risk-neutral:
    // ST = S0 * exp(rT). Then:
    // P = exp(-rT) * max(K - ST, 0) = max(K*exp(-rT) - S0, 0)
    const double s0 = 100.0;
    const double k = 100.0;
    const double r = 0.05;
    const double T = 1.0;

    const double expected = std::max(k * std::exp(-r * T) - s0, 0.0);
    const double price = bs_european_put(s0, k, r, /*sigma=*/0.0, T);

    EXPECT_NEAR(price, expected, eps_tight);
}

TEST(Pricers_BlackScholes_Put, DeepInTheMoneyApproachesDiscountedStrikeMinusSpot)
{
    // For very small S0 relative to K, put price approaches:
    // P ≈ K*exp(-rT) - S0
    const double s0 = 1.0;
    const double k = 1'000.0;
    const double r = 0.03;
    const double sigma = 0.2;
    const double T = 2.0;

    const double approx = k * std::exp(-r * T) - s0;
    const double price = bs_european_put(s0, k, r, sigma, T);

    EXPECT_NEAR(price, approx, 1e-3);
}

TEST(Pricers_BlackScholes_Put, MonotonicInSpot)
{
    // Put price decreases with spot.
    const double k = 100.0;
    const double r = 0.05;
    const double sigma = 0.2;
    const double T = 1.0;

    const double p1 = bs_european_put(90.0, k, r, sigma, T);
    const double p2 = bs_european_put(100.0, k, r, sigma, T);
    const double p3 = bs_european_put(110.0, k, r, sigma, T);

    EXPECT_GT(p1, p2);
    EXPECT_GT(p2, p3);
}

TEST(Pricers_BlackScholes_Put, MonotonicInVolatility)
{
    // Put price increases with volatility.
    const double s0 = 100.0;
    const double k = 100.0;
    const double r = 0.05;
    const double T = 1.0;

    const double low = bs_european_put(s0, k, r, 0.10, T);
    const double mid = bs_european_put(s0, k, r, 0.20, T);
    const double high = bs_european_put(s0, k, r, 0.40, T);

    EXPECT_LT(low, mid);
    EXPECT_LT(mid, high);
}

TEST(Pricers_BlackScholes_Put, MonotonicInMaturity)
{
    // Put price should be non-decreasing in T (no dividends).
    const double s0 = 100.0;
    const double k = 100.0;
    const double r = 0.05;
    const double sigma = 0.2;

    const double p_short = bs_european_put(s0, k, r, sigma, 0.25);
    const double p_mid = bs_european_put(s0, k, r, sigma, 1.0);
    const double p_long = bs_european_put(s0, k, r, sigma, 2.0);

    EXPECT_LE(p_short, p_mid);
    EXPECT_LE(p_mid, p_long);
}

TEST(Pricers_BlackScholes_Put, NonNegativeAndUpperBoundedByDiscountedStrike)
{
    const double s0 = 100.0;
    const double k = 140.0;
    const double r = 0.05;
    const double sigma = 0.3;
    const double T = 1.5;

    const double p = bs_european_put(s0, k, r, sigma, T);

    EXPECT_GE(p, 0.0);
    EXPECT_LE(p, k * std::exp(-r * T));
}

TEST(Pricers_BlackScholes_Put, PutCallParity)
{
    const double s0 = 100.0;
    const double k = 100.0;
    const double r = 0.05;
    const double sigma = 0.2;
    const double T = 1.0;

    const double c = bs_european_call(s0, k, r, sigma, T);
    const double p = bs_european_put(s0, k, r, sigma, T);

    const double rhs = s0 - k * std::exp(-r * T);
    EXPECT_NEAR(c - p, rhs, 1e-10);
}
