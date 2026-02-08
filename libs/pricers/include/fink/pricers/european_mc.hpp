#pragma once

#include <fink/mc/engine.hpp>
#include <fink/mc/reducer.hpp>

namespace fink::pricers
{

template <typename Backend,
          typename Instrument,
          typename Model,
          typename RngNormal>
double european_mc(const Instrument& inst,
                   const Model& model,
                   std::size_t paths,
                   Backend&& backend)
{
    fink::mc::mc_config cfg{paths};

    fink::mc::online_stats_reducer reducer{};

    auto sample = [&](std::size_t /*i*/, auto& rng) -> double
    {
        // TODO
        // 1. sampling Z
        // 2. S_T
        // 3. payoff
        // 4. discount
        return 0.0;
    };

    auto result = fink::mc::run(cfg, backend, sample, reducer);

    return result.mean;
}

}