#include <fink/math/online_stats.hpp>
#include <gtest/gtest.h>

#include <array>
#include <vector>

using fink::math::online_stats;

TEST(Math_OnlineStats, EmptyStats)
{
    online_stats s;

    EXPECT_EQ(s.count(), 0u);
    EXPECT_DOUBLE_EQ(s.mean(), 0.0);
    EXPECT_DOUBLE_EQ(s.variance(), 0.0);
    EXPECT_DOUBLE_EQ(s.stddev(), 0.0);
    EXPECT_DOUBLE_EQ(s.stderr(), 0.0);
}

TEST(Math_OnlineStats, SingleValue)
{
    online_stats s;
    s.add(5.0);

    EXPECT_EQ(s.count(), 1u);
    EXPECT_DOUBLE_EQ(s.mean(), 5.0);
    EXPECT_DOUBLE_EQ(s.variance(), 0.0);
    EXPECT_DOUBLE_EQ(s.stddev(), 0.0);
    EXPECT_DOUBLE_EQ(s.stderr(), 0.0);
}

TEST(Math_OnlineStats, SimpleSequence)
{
    online_stats s;

    std::array<double, 4> v{1.0, 2.0, 3.0, 4.0};
    for (double x : v)
        s.add(x);

    EXPECT_EQ(s.count(), 4u);
    EXPECT_DOUBLE_EQ(s.mean(), 2.5);
    EXPECT_NEAR(s.variance(), 1.6666666666666667, 1e-12);
}

TEST(Math_OnlineStats, NegativeValues)
{
    online_stats s;

    std::array<double, 4> v{-1.0, -2.0, -3.0, -4.0};
    for (double x : v)
        s.add(x);

    EXPECT_EQ(s.count(), 4u);
    EXPECT_DOUBLE_EQ(s.mean(), -2.5);
    EXPECT_NEAR(s.variance(), 1.6666666666666667, 1e-12);
}

TEST(Math_OnlineStats, MixedValuesZeroMean)
{
    online_stats s;

    std::array<double, 4> v{-1.0, 1.0, -1.0, 1.0};
    for (double x : v)
        s.add(x);

    EXPECT_EQ(s.count(), 4u);
    EXPECT_DOUBLE_EQ(s.mean(), 0.0);
    EXPECT_NEAR(s.variance(), 1.3333333333333333, 1e-12);
}

TEST(Math_OnlineStats, MatchesBatchComputation)
{
    std::vector<double> v{2.0, 4.0, 6.0, 8.0};

    online_stats s;
    for (double x : v)
        s.add(x);

    const double expected_mean = 5.0;
    const double expected_variance = 6.666666666666667;

    EXPECT_DOUBLE_EQ(s.mean(), expected_mean);
    EXPECT_NEAR(s.variance(), expected_variance, 1e-12);
}

TEST(Math_OnlineStats, LargeValuesStable)
{
    online_stats s;

    s.add(1e12);
    s.add(1e12);

    EXPECT_DOUBLE_EQ(s.mean(), 1e12);
    EXPECT_DOUBLE_EQ(s.variance(), 0.0);
}
