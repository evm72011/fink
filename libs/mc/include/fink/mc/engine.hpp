/**
 * @file engine.hpp
 * @brief Monte Carlo engine entry point.
 */
#pragma once

#include <utility>

#include <fink/mc/concepts.hpp>
#include <fink/mc/config.hpp>
#include <fink/mc/result.hpp>

namespace fink::mc
{

/**
 * @brief Run Monte Carlo simulation using the provided backend.
 *
 * @tparam Backend Execution backend type.
 * @tparam SampleFn Sample generator callable.
 * @tparam Reducer Reducer type used to aggregate samples.
 *
 * @param cfg Monte Carlo configuration.
 * @param backend Execution backend instance.
 * @param sample Sample function: sample(i, rng) -> double.
 * @param reducer Reducer instance.
 */
template <typename Backend, typename SampleFn, typename Reducer>
    requires backend<Backend, SampleFn, Reducer>
[[nodiscard]] mc_result run(const mc_config &cfg,
                            Backend &&backend,
                            SampleFn &&sample,
                            Reducer &&reducer)
{
    return std::forward<Backend>(backend).run(cfg,
                                              std::forward<SampleFn>(sample),
                                              std::forward<Reducer>(reducer));
}

} // namespace fink::mc

// NOTE: Currently this function only forwards execution to the backend.
// Parameter validation and benchmarking hooks may be added here later.