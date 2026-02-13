#pragma once

#include <utility>

#include <fink/mc/engine.hpp>
#include <fink/mc/reducer.hpp>
#include <fink/mc/result.hpp>
#include <fink/rng/normal_rng.hpp>
#include <fink/math/discount.hpp>
#include <fink/pricers/pricer_result.hpp>

namespace fink::pricers
{

template <typename Backend, typename Instrument, typename Model>
[[nodiscard]] inline pricer_result price_european_mc(
    const Instrument &inst,
    const Model &model,
    std::size_t paths,
    Backend &&backend)
{
    const fink::mc::mc_config cfg{paths};

    const fink::mc::online_stats_reducer reducer{};

    auto sample = [&](std::size_t /*i*/, auto &rng) -> double {
        fink::rng::normal_rng n(rng);
        const double Z = n();
        const double ST =
            fink::models::gbm_terminal_price(model, inst.expiry, Z);
        return inst.payoff(ST);
    };

    auto mc_result = fink::mc::run(cfg, std::forward<Backend>(backend), sample, reducer);
    const double df = fink::math::discount_continuous(model.r, inst.expiry);
    return pricer_result {
        .price = df * mc_result.mean,
        .std_error = df * mc_result.std_error,
    };
}

} // namespace fink::pricers