#include <fink/instruments/options.hpp>
#include <gtest/gtest.h>

using namespace fink::instruments;

TEST(Instruments_CallPayoff, SpotBelowStrikeReturnsZero)
{
    const call_payoff p{.strike = 100.0};
    EXPECT_DOUBLE_EQ(p(99.0), 0.0);
}

TEST(Instruments_CallPayoff, SpotEqualStrikeReturnsZero)
{
    const call_payoff p{.strike = 100.0};
    EXPECT_DOUBLE_EQ(p(100.0), 0.0);
}

TEST(Instruments_CallPayoff, SpotAboveStrikeReturnsIntrinsic)
{
    const call_payoff p{.strike = 100.0};
    EXPECT_DOUBLE_EQ(p(120.0), 20.0);
}

TEST(Instruments_PutPayoff, SpotAboveStrikeReturnsZero)
{
    const put_payoff p{.strike = 100.0};
    EXPECT_DOUBLE_EQ(p(101.0), 0.0);
}

TEST(Instruments_PutPayoff, SpotEqualStrikeReturnsZero)
{
    const put_payoff p{.strike = 100.0};
    EXPECT_DOUBLE_EQ(p(100.0), 0.0);
}

TEST(Instruments_PutPayoff, SpotBelowStrikeReturnsIntrinsic)
{
    const put_payoff p{.strike = 100.0};
    EXPECT_DOUBLE_EQ(p(80.0), 20.0);
}
