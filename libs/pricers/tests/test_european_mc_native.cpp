#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>
#include <fink/pricers/black_scholes.hpp>
#include <fink/pricers/native/european_mc.hpp>

#include <gtest/gtest.h>

#include <cmath>

using fink::instruments::call_payoff;
using fink::instruments::european_call;
using fink::instruments::european_put;
using fink::instruments::put_payoff;

using fink::models::gbm_params;

using fink::pricers::bs_european_call;
using fink::pricers::bs_european_put;
using fink::pricers::native::mc_config;
using fink::pricers::native::price_european_mc;

namespace
{
constexpr double eps_tight = 1e-12;
} // namespace

TEST(Pricers_European_MC, DeterministicWhenSigmaZero_CallMatchesClosedForm)
{
    const european_call opt{.expiry = 1.0,
                            .payoff = call_payoff{.strike = 100.0}};
    const gbm_params model{.s0 = 100.0, .r = 0.05, .sigma = 0.0};
    const mc_config cfg{.paths = 10'000, .seed = 123};

    const auto mc = price_european_mc(opt, model, cfg);
    const double bs = bs_european_call(opt, model);

    EXPECT_NEAR(mc.price, bs, eps_tight);
    EXPECT_NEAR(mc.std_error, 0.0, eps_tight);
}

TEST(Pricers_European_MC, DeterministicWhenSigmaZero_PutMatchesClosedForm)
{
    const european_put opt{.expiry = 1.0,
                           .payoff = put_payoff{.strike = 100.0}};
    const gbm_params model{.s0 = 100.0, .r = 0.05, .sigma = 0.0};
    const mc_config cfg{.paths = 10'000, .seed = 123};

    const auto mc = price_european_mc(opt, model, cfg);
    const double bs = bs_european_put(opt, model);

    EXPECT_NEAR(mc.price, bs, eps_tight);
    EXPECT_NEAR(mc.std_error, 0.0, eps_tight);
}

TEST(Pricers_European_MC, ZeroMaturityEqualsIntrinsicValue_CallAndZeroError)
{
    const european_call opt{.expiry = 0.0,
                            .payoff = call_payoff{.strike = 100.0}};
    const gbm_params model{.s0 = 120.0, .r = 0.05, .sigma = 0.2};
    const mc_config cfg{.paths = 1000, .seed = 123};

    const auto mc = price_european_mc(opt, model, cfg);

    EXPECT_NEAR(mc.price, 20.0, eps_tight);
    EXPECT_NEAR(mc.std_error, 0.0, eps_tight);
}

TEST(Pricers_European_MC, ReproducibleForSameSeed)
{
    const european_call opt{.expiry = 1.0,
                            .payoff = call_payoff{.strike = 100.0}};
    const gbm_params model{.s0 = 100.0, .r = 0.05, .sigma = 0.2};

    const mc_config cfg1{.paths = 50'000, .seed = 123};
    const mc_config cfg2{.paths = 50'000, .seed = 123};

    const auto a = price_european_mc(opt, model, cfg1);
    const auto b = price_european_mc(opt, model, cfg2);

    EXPECT_DOUBLE_EQ(a.price, b.price);
    EXPECT_DOUBLE_EQ(a.std_error, b.std_error);
}

TEST(Pricers_European_MC, DifferentSeedsUsuallyGiveDifferentEstimates)
{
    const european_call opt{.expiry = 1.0,
                            .payoff = call_payoff{.strike = 100.0}};
    const gbm_params model{.s0 = 100.0, .r = 0.05, .sigma = 0.2};

    const mc_config cfg1{.paths = 50'000, .seed = 1};
    const mc_config cfg2{.paths = 50'000, .seed = 2};

    const auto a = price_european_mc(opt, model, cfg1);
    const auto b = price_european_mc(opt, model, cfg2);

    // Extremely unlikely to be exactly equal; allow tiny tolerance to avoid flakiness.
    EXPECT_GT(std::abs(a.price - b.price), 1e-15);
}

TEST(Pricers_European_MC, StandardErrorDecreasesWithMorePaths)
{
    const european_call opt{.expiry = 1.0,
                            .payoff = call_payoff{.strike = 100.0}};
    const gbm_params model{.s0 = 100.0, .r = 0.05, .sigma = 0.2};

    const auto small =
        price_european_mc(opt, model, mc_config{.paths = 20'000, .seed = 123});
    const auto large =
        price_european_mc(opt, model, mc_config{.paths = 200'000, .seed = 123});

    EXPECT_LT(large.std_error, small.std_error);

    const double expected_ratio =
        std::sqrt(20'000.0 / 200'000.0); // sqrt(N_small/N_large)
    const double ratio = large.std_error / small.std_error;

    EXPECT_NEAR(ratio, expected_ratio, 0.15 * expected_ratio);
}

TEST(Pricers_European_MC, PutCallParityHoldsWithinMonteCarloError)
{
    const double K = 100.0;
    const double T = 1.0;

    const european_call call{.expiry = T, .payoff = call_payoff{.strike = K}};
    const european_put put{.expiry = T, .payoff = put_payoff{.strike = K}};

    const gbm_params model{.s0 = 100.0, .r = 0.05, .sigma = 0.2};
    const mc_config cfg{.paths = 400'000, .seed = 123};

    const auto c = price_european_mc(call, model, cfg);
    const auto p = price_european_mc(put, model, cfg);

    const double rhs = model.s0 - K * std::exp(-model.r * T);

    const double tol = 5.0 * (c.std_error + p.std_error);
    EXPECT_NEAR(c.price - p.price, rhs, tol);
}