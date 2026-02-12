/**
 * @file mc_backend_parallel.hpp
 * @brief Parallel CPU backend for Monte Carlo simulation.
 */
#pragma once

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <thread>
#include <vector>

#include <fink/mc/config.hpp>
#include <fink/mc/result.hpp>
#include <fink/rng/pcg32.hpp>

namespace fink::backends::cpu
{

/**
 * @brief Parallel CPU Monte Carlo backend (thread-local reducers + merge).
 */
class mc_backend_parallel
{
public:
    explicit mc_backend_parallel(std::size_t threads = std::thread::hardware_concurrency()) noexcept
        : threads_(threads == 0 ? 1 : threads)
    {
    }

    template <typename SampleFn, typename Reducer>
    [[nodiscard]] fink::mc::mc_result run(const fink::mc::mc_config& cfg,
                                          SampleFn&& sample,
                                          Reducer reducer) const
    {
        if (cfg.paths == 0)
            return reducer.result();

        const std::size_t n_threads =
            std::min<std::size_t>(threads_, cfg.paths);

        std::vector<std::thread> workers;
        workers.reserve(n_threads);

        std::vector<Reducer> locals;
        locals.resize(n_threads, reducer);

        const std::size_t total = cfg.paths;
        const std::size_t block = (total + n_threads - 1) / n_threads;

        for (std::size_t t = 0; t < n_threads; ++t)
        {
            const std::size_t begin = t * block;
            const std::size_t end = std::min(total, begin + block);

            workers.emplace_back([&, t, begin, end] {
                // Seed per thread (first parallel version; deterministic for fixed thread count).
                fink::rng::pcg32 rng(cfg.seed + static_cast<std::uint64_t>(t));

                auto& local = locals[t];

                for (std::size_t i = begin; i < end; ++i)
                {
                    const double x = sample(i, rng);
                    local.add(x);
                }
            });
        }

        for (auto& th : workers)
            th.join();

        for (std::size_t t = 0; t < n_threads; ++t)
            reducer.merge(locals[t]);

        return reducer.result();
    }

private:
    std::size_t threads_{1};
};

} // namespace fink::backends::cpu