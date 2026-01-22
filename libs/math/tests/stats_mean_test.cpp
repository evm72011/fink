#include <fink/math/stats.hpp>
#include <gtest/gtest.h>

#include <array>
#include <vector>

using fink::math::mean;

TEST(Math_Stats_Mean, EmptyInputReturnsZero)
{
    EXPECT_DOUBLE_EQ(mean(nullptr, 0), 0.0);

    std::vector<double> v;
    EXPECT_DOUBLE_EQ(mean(v.data(), v.size()), 0.0);
}

TEST(Math_Stats_Mean, SingleElement)
{
    const double x = 5.0;
    EXPECT_DOUBLE_EQ(mean(&x, 1), 5.0);
}

TEST(Math_Stats_Mean, SimpleValues)
{
    std::array<double, 4> v{1.0, 2.0, 3.0, 4.0};
    EXPECT_DOUBLE_EQ(mean(v.data(), v.size()), 2.5);
}

TEST(Math_Stats_Mean, NegativeValues)
{
    std::array<double, 4> v{-1.0, -2.0, -3.0, -4.0};
    EXPECT_DOUBLE_EQ(mean(v.data(), v.size()), -2.5);
}

TEST(Math_Stats_Mean, MixedValues)
{
    std::array<double, 4> v{-1.0, 1.0, -1.0, 1.0};
    EXPECT_DOUBLE_EQ(mean(v.data(), v.size()), 0.0);
}

TEST(Math_Stats_Mean, SpanOverload)
{
    std::vector<double> v{2.0, 4.0, 6.0, 8.0};
    EXPECT_DOUBLE_EQ(mean(std::span<const double>(v)), 5.0);
}

TEST(Math_Stats_Mean, LargeValuesNoOverflow)
{
    std::array<double, 2> v{1e12, 1e12};
    EXPECT_DOUBLE_EQ(mean(v.data(), v.size()), 1e12);
}
