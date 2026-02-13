#include <fink/math/discount.hpp>
#include <gtest/gtest.h>

#include <numbers>
#include <array>
#include <vector>


using fink::math::discount_continuous;
using fink::math::discount_discrete;

TEST(Math_Discount_Continuous, ZeroTimeReturnsOne)
{
    EXPECT_DOUBLE_EQ(discount_continuous(0.05, 0.0), 1.0);
    EXPECT_DOUBLE_EQ(discount_continuous(-0.05, 0.0), 1.0);
}

TEST(Math_Discount_Continuous, ZeroRateReturnsOne)
{
    EXPECT_DOUBLE_EQ(discount_continuous(0.0, 1.0), 1.0);
    EXPECT_DOUBLE_EQ(discount_continuous(0.0, 10.0), 1.0);
}

TEST(Math_Discount_Continuous, PositiveRateDecreasesWithTime)
{
    const double r = 0.05;
    EXPECT_GT(discount_continuous(r, 1.0), discount_continuous(r, 2.0));
    EXPECT_GT(discount_continuous(r, 2.0), discount_continuous(r, 5.0));
}

TEST(Math_Discount_Continuous, NegativeRateIncreasesWithTime)
{
    const double r = -0.05;
    EXPECT_LT(discount_continuous(r, 1.0), discount_continuous(r, 2.0));
    EXPECT_LT(discount_continuous(r, 2.0), discount_continuous(r, 5.0));
}

TEST(Math_Discount_Continuous, KnownValues)
{
    // DF(t)=exp(-r*t)
    EXPECT_DOUBLE_EQ(discount_continuous(0.0, 3.0), 1.0);
    EXPECT_DOUBLE_EQ(discount_continuous(1.0, 0.0), 1.0);

    // exp(-ln(2)) = 0.5
    constexpr double ln2 = std::numbers::ln2;
    EXPECT_DOUBLE_EQ(discount_continuous(ln2, 1.0), 0.5);

    // exp(ln(2)) = 2.0 when r is negative
    EXPECT_DOUBLE_EQ(discount_continuous(-ln2, 1.0), 2.0);
}

TEST(Math_Discount_Discrete, ZeroTimeReturnsOne)
{
    EXPECT_DOUBLE_EQ(discount_discrete(0.05, 0.0), 1.0);
    EXPECT_DOUBLE_EQ(discount_discrete(-0.05, 0.0), 1.0);
}

TEST(Math_Discount_Discrete, ZeroRateReturnsOne)
{
    EXPECT_DOUBLE_EQ(discount_discrete(0.0, 1.0), 1.0);
    EXPECT_DOUBLE_EQ(discount_discrete(0.0, 10.0), 1.0);
}

TEST(Math_Discount_Discrete, PositiveRateDecreasesWithTime)
{
    const double r = 0.05;
    EXPECT_GT(discount_discrete(r, 1.0), discount_discrete(r, 2.0));
    EXPECT_GT(discount_discrete(r, 2.0), discount_discrete(r, 5.0));
}

TEST(Math_Discount_Discrete, NegativeRateIncreasesWithTime)
{
    const double r = -0.05;
    EXPECT_LT(discount_discrete(r, 1.0), discount_discrete(r, 2.0));
    EXPECT_LT(discount_discrete(r, 2.0), discount_discrete(r, 5.0));
}

TEST(Math_Discount_Discrete, KnownValues)
{
    // DF(t)=1/(1+r)^t
    EXPECT_DOUBLE_EQ(discount_discrete(0.0, 3.0), 1.0);

    // r=1, t=1 => 1/2
    EXPECT_DOUBLE_EQ(discount_discrete(1.0, 1.0), 0.5);

    // r=3, t=1 => 1/4
    EXPECT_DOUBLE_EQ(discount_discrete(3.0, 1.0), 0.25);

    // r=1, t=2 => 1/4
    EXPECT_DOUBLE_EQ(discount_discrete(1.0, 2.0), 0.25);

    // r=-0.5, t=1 => 1/(0.5)=2
    EXPECT_DOUBLE_EQ(discount_discrete(-0.5, 1.0), 2.0);
}

TEST(Math_Discount, ContinuousAndDiscreteAgreeAtT1WhenRIsContinuousEquivalent)
{
    // If discrete rate is r_eff, the equivalent continuous rate is ln(1+r_eff).
    // Then exp(-ln(1+r_eff)) == 1/(1+r_eff) for t=1.
    constexpr double r_eff = 0.10;
    const double r_cont = std::log(1.0 + r_eff);

    EXPECT_DOUBLE_EQ(discount_continuous(r_cont, 1.0),
                     discount_discrete(r_eff, 1.0));
}
