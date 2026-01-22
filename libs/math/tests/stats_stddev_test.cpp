#include <fink/math/stats.hpp>
#include <gtest/gtest.h>

#include <array>
#include <cmath>
#include <vector>

using fink::math::stddev;

namespace
{
constexpr double eps = 1e-12;
}

TEST(Math_Stats_Stddev, EmptyInputReturnsZero)
{
    EXPECT_DOUBLE_EQ(stddev(nullptr, 0), 0.0);

    std::vector<double> v;
    EXPECT_DOUBLE_EQ(stddev(v.data(), v.size()), 0.0);
}

TEST(Math_Stats_Stddev, SingleElementReturnsZero)
{
    const double x = 5.0;
    EXPECT_DOUBLE_EQ(stddev(&x, 1), 0.0);
}

TEST(Math_Stats_Stddev, TwoElements)
{
    std::array<double, 2> v{1.0, 3.0};
    EXPECT_NEAR(stddev(v.data(), v.size()), std::sqrt(2.0), eps);
}

TEST(Math_Stats_Stddev, SimpleValues)
{
    std::array<double, 4> v{1.0, 2.0, 3.0, 4.0};
    EXPECT_NEAR(stddev(v.data(), v.size()), std::sqrt(5.0 / 3.0), eps);
}

TEST(Math_Stats_Stddev, NegativeValues)
{
    std::array<double, 4> v{-1.0, -2.0, -3.0, -4.0};
    EXPECT_NEAR(stddev(v.data(), v.size()), std::sqrt(5.0 / 3.0), eps);
}

TEST(Math_Stats_Stddev, MixedValues)
{
    std::array<double, 4> v{-1.0, 1.0, -1.0, 1.0};
    EXPECT_NEAR(stddev(v.data(), v.size()), std::sqrt(4.0 / 3.0), eps);
}

TEST(Math_Stats_Stddev, SpanOverload)
{
    std::vector<double> v{2.0, 4.0, 6.0, 8.0};
    EXPECT_NEAR(stddev(std::span<const double>(v)), std::sqrt(20.0 / 3.0), eps);
}

TEST(Math_Stats_Stddev, LargeValuesNoOverflow)
{
    std::array<double, 2> v{1e12, 1e12};
    EXPECT_DOUBLE_EQ(stddev(v.data(), v.size()), 0.0);
}
