#include <fink/pricers/european_mc.hpp>

#include <cmath>

#include <fink/rng/normal_rng.hpp>
#include <fink/rng/pcg32.hpp>

namespace fink::pricers
{

mc_result price_european_call_mc(const fink::instruments::european_call &opt,
                                 const fink::models::gbm_params &model,
                                 const mc_config &cfg) noexcept
{
    fink::rng::pcg32 urng(cfg.seed);
    fink::rng::normal_rng n(urng);

    double sum = 0.0;
    double sum2 = 0.0;

    for (std::size_t i = 0; i < cfg.paths; ++i)
    {
        const double Z = n();
        const double ST =
            fink::models::gbm_terminal_price(model, opt.expiry, Z);
        const double payoff = opt.payoff(ST);
        sum += payoff;      // TODO Welford
        sum2 += payoff * payoff;
    }

    const auto npaths = static_cast<double>(cfg.paths);
    const double mean = sum / npaths;

    // sample variance of payoff (unbiased)
    const double var = (sum2 / npaths - mean * mean) * npaths / (npaths - 1.0);

    const double df = std::exp(-model.r * opt.expiry);

    mc_result res;
    res.price = df * mean;
    res.stderr = df * std::sqrt(var / npaths);
    return res;
}

} // namespace fink::pricers
