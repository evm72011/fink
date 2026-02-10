/**
 * @file concepts.hpp
 * @brief Concepts for Monte Carlo engine components.
 */
#pragma once

#include <concepts>
#include <cstddef>
#include <utility>

#include <fink/mc/config.hpp>
#include <fink/mc/result.hpp>

namespace fink::mc
{

template <typename SampleFn, typename Rng>
concept sample_fn = requires(SampleFn fn, std::size_t i, Rng &rng) {
    { fn(i, rng) } -> std::convertible_to<double>;
};

template <typename Reducer>
concept reducer = requires(Reducer r, double x) {
    { r.add(x) };
    { r.result() } -> std::same_as<mc_result>;
};

template <typename Backend, typename SampleFn, typename Reducer>
concept backend =
    requires(Backend b, const mc_config &cfg, SampleFn fn, Reducer r) {
        { b.run(cfg, fn, r) } -> std::same_as<mc_result>;
    };

} // namespace fink::mc