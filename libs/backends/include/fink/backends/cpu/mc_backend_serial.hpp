/**
 * @file mc_backend.hpp
 * @brief Serial CPU backend for Monte Carlo simulation.
 */
#pragma once

#include <cstddef>

#include <fink/mc/config.hpp>
#include <fink/mc/result.hpp>
#include <fink/rng/pcg32.hpp>

namespace fink::backends::cpu
{

/**
 * @brief Serial CPU Monte Carlo backend.
 */
class mc_backend_serial
{
public:
    template <typename SampleFn, typename Reducer>
    [[nodiscard]] fink::mc::mc_result run(const fink::mc::mc_config &cfg,
                                          SampleFn &&sample,
                                          Reducer reducer) const
    {
        fink::rng::pcg32 rng{cfg.seed};

        for (std::size_t i = 0; i < cfg.paths; ++i)
        {
            const double x = sample(i, rng);
            reducer.add(x);
        }

        return reducer.result();
    }
};

} // namespace fink::backends::cpu