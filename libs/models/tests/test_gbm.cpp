#include <fink/models/gbm.hpp>
#include <gtest/gtest.h>

#include <cmath>

using fink::models::gbm_params;
using fink::models::gbm_terminal_price;

namespace
{
constexpr double eps = 1e-12;
} // namespace

TEST(Models_GBM, SigmaZeroIsDeterministic)
{
    const gbm_params p{.s0 = 100.0, .r = 0.05, .sigma = 0.0};

    const double T = 2.0;
    const double Z = 0.0; // irrelevant if sigma=0

    const double ST = gbm_terminal_price(p, T, Z);
    const double expected = 100.0 * std::exp(0.05 * T);

    EXPECT_NEAR(ST, expected, eps);
}

TEST(Models_GBM, ZEqualsZero)
{
    const gbm_params p{.s0 = 100.0, .r = 0.03, .sigma = 0.2};

    const double T = 1.0;

    const double ST = gbm_terminal_price(p, T, 0.0);
    const double expected = 100.0 * std::exp((0.03 - 0.5 * 0.2 * 0.2) * T);

    EXPECT_NEAR(ST, expected, eps);
}
