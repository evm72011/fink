#include <fink/pricers/black_scholes.hpp>
#include <fink/pricers/european_mc.hpp>
#include <gtest/gtest.h>

using fink::instruments::call_payoff;
using fink::instruments::european_call;
using fink::models::gbm_params;
using fink::pricers::bs_european_call;
using fink::pricers::mc_config;
using fink::pricers::price_european_call_mc;

TEST(Pricers_European_MC, ConvergesToBlackScholes)
{
    const european_call opt{.expiry = 1.0,
                            .payoff = call_payoff{.strike = 100.0}};

    const gbm_params model{.s0 = 100.0, .r = 0.05, .sigma = 0.2};

    const mc_config cfg{.paths = 300'000, .seed = 123};

    const auto mc = price_european_call_mc(opt, model, cfg);
    const double bs =
        bs_european_call(model.s0, 100.0, model.r, model.sigma, opt.expiry);

    // 3-sigma rule
    EXPECT_NEAR(mc.price, bs, 3.0 * mc.stderr);
}
