/**
 * @file reducer.hpp
 * @brief Reducers for Monte Carlo aggregation.
 */
#pragma once

#include <fink/math/online_stats.hpp>
#include <fink/mc/result.hpp>

namespace fink::mc
{

/**
 * @brief Reducer based on fink::math::online_stats.
 */
class online_stats_reducer
{
public:
    /// Add a new sample value.
    void add(double x) noexcept
    {
        stats_.add(x);
    }

    void merge(const online_stats_reducer &other) noexcept
    {
        stats_.merge(other.stats_);
    }

    /// Produce the final Monte Carlo result.
    [[nodiscard]] mc_result result() const noexcept
    {
        mc_result r;
        r.mean = stats_.mean();
        r.variance = stats_.variance();
        r.std_error = stats_.stderr();
        r.paths = stats_.count();
        return r;
    }

private:
    fink::math::online_stats stats_{};
};

} // namespace fink::mc