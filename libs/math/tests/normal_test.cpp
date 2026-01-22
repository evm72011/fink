#include <fink/math/normal.hpp>
#include <gtest/gtest.h>

#include <cmath>

using fink::math::norm_cdf;
using fink::math::norm_pdf;

namespace
{

constexpr double eps = 1e-6;

} // namespace

TEST(Math_Normal_PDF, Zero)
{
    EXPECT_NEAR(norm_pdf(0.0), 0.3989422804014327, eps);
}

TEST(Math_Normal_PDF, Symmetry)
{
    EXPECT_NEAR(norm_pdf(1.0), norm_pdf(-1.0), eps);
}

TEST(Math_Normal_CDF, Zero)
{
    EXPECT_NEAR(norm_cdf(0.0), 0.5, eps);
}

TEST(Math_Normal_CDF, One)
{
    EXPECT_NEAR(norm_cdf(1.0), 0.8413447460685429, eps);
}

TEST(Math_Normal_CDF, MinusOne)
{
    EXPECT_NEAR(norm_cdf(-1.0), 0.1586552539314571, eps);
}

TEST(Math_Normal_CDF, Two)
{
    EXPECT_NEAR(norm_cdf(2.0), 0.9772498680518208, eps);
}

TEST(Math_Normal_CDF, MinusTwo)
{
    EXPECT_NEAR(norm_cdf(-2.0), 0.02275013194817921, eps);
}
