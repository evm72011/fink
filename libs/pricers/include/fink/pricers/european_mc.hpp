#pragma once

#include <utility>

#include <fink/mc/engine.hpp>
#include <fink/mc/reducer.hpp>
#include <fink/mc/result.hpp>
#include <fink/rng/normal_rng.hpp>

namespace fink::pricers
{

template <typename Backend, typename Instrument, typename Model>
[[nodiscard]] inline fink::mc::mc_result price_european_mc(
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

    // TODO:
    // Discount to present value logic here.
    // New struct pricer_result mean -> price
    // Add minimal std_err to mc_config to as additional condition for interruption
    // Optional: Benchmark info ?
    return fink::mc::run(cfg, std::forward<Backend>(backend), sample, reducer);
}

} // namespace fink::pricers