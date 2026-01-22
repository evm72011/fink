#include <fink/rng/normal_rng.hpp>
#include <fink/rng/pcg32.hpp>
#include <gtest/gtest.h>

using fink::rng::normal_rng;
using fink::rng::pcg32;

TEST(RNG_PCG32, Reproducibility)
{
    pcg32 r1(42);
    pcg32 r2(42);

    for (int i = 0; i < 1000; ++i)
        EXPECT_EQ(r1.next(), r2.next());
}

TEST(RNG_Normal, NormalMeanVariance)
{
    pcg32 r(123);
    normal_rng n(r);

    constexpr int N = 100000;
    double sum = 0.0, sum2 = 0.0;

    for (int i = 0; i < N; ++i)
    {
        const double x = n();
        sum += x;
        sum2 += x * x;
    }

    const double mean = sum / N;
    const double var = sum2 / N;

    EXPECT_NEAR(mean, 0.0, 1e-2);
    EXPECT_NEAR(var, 1.0, 1e-2);
}
