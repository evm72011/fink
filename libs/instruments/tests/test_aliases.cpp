#include <fink/instruments/aliases.hpp>
#include <gtest/gtest.h>

#include <type_traits>

using namespace fink::instruments;

TEST(Instruments_Aliases, EuropeanCallIsCorrectAlias)
{
    static_assert(std::is_same_v<european_call, option<european, call_payoff>>);

    SUCCEED();
}

TEST(Instruments_Aliases, EuropeanPutIsCorrectAlias)
{
    static_assert(std::is_same_v<european_put, option<european, put_payoff>>);

    SUCCEED();
}

TEST(Instruments_Aliases, AmericanCallIsCorrectAlias)
{
    static_assert(std::is_same_v<american_call, option<american, call_payoff>>);

    SUCCEED();
}

TEST(Instruments_Aliases, AmericanPutIsCorrectAlias)
{
    static_assert(std::is_same_v<american_put, option<american, put_payoff>>);

    SUCCEED();
}

TEST(Instruments_Aliases, AliasesPreserveExerciseStyle)
{
    EXPECT_EQ(european_call::style, exercise_style::european);
    EXPECT_EQ(european_put::style, exercise_style::european);
    EXPECT_EQ(american_call::style, exercise_style::american);
    EXPECT_EQ(american_put::style, exercise_style::american);
}

TEST(Instruments_Aliases, AliasesAreTriviallyCopyable)
{
    static_assert(std::is_trivially_copyable_v<european_call>);
    static_assert(std::is_trivially_copyable_v<american_put>);
    SUCCEED();
}
