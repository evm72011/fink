#include <fink/instruments/aliases.hpp>

#include <gtest/gtest.h>

#include <type_traits>

using namespace fink::instruments;

TEST(Instruments_Option, StoresExpiryAndPayoff)
{
    constexpr european_call c{
        .expiry = 1.5,
        .payoff = call_payoff{.strike = 100.0},
    };

    EXPECT_DOUBLE_EQ(c.expiry, 1.5);
    EXPECT_DOUBLE_EQ(c.payoff.strike, 100.0);
    EXPECT_DOUBLE_EQ(c.payoff(120.0), 20.0);
}

TEST(Instruments_Option, EuropeanStyleIsSetByTag)
{
    EXPECT_EQ(european_call::style, exercise_style::european);
    EXPECT_EQ(european_put::style, exercise_style::european);
}

TEST(Instruments_Option, AmericanStyleIsSetByTag)
{
    EXPECT_EQ(american_call::style, exercise_style::american);
    EXPECT_EQ(american_put::style, exercise_style::american);
}

TEST(Instruments_Option, IsTriviallyCopyableWhenPayoffIsTriviallyCopyable)
{
    static_assert(std::is_trivially_copyable_v<call_payoff>);
    static_assert(std::is_trivially_copyable_v<european_call>);
    SUCCEED();
}
