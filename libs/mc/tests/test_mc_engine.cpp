#include <gtest/gtest.h>

#include <cstddef>
#include <cstdint>
#include <utility>

#include <fink/mc/config.hpp>
#include <fink/mc/engine.hpp>
#include <fink/mc/reducer.hpp>

namespace
{
struct dummy_rng
{
    std::uint64_t s{0};
};

struct test_backend
{
    template <typename SampleFn, typename Reducer>
    [[nodiscard]] fink::mc::mc_result run(const fink::mc::mc_config &cfg,
                                          SampleFn &&sample,
                                          Reducer reducer) const
    {
        dummy_rng rng{cfg.seed};

        for (std::size_t i = 0; i < cfg.paths; ++i)
        {
            const double x = std::forward<SampleFn>(sample)(i, rng);
            reducer.add(x);
        }

        return reducer.result();
    }
};
} // namespace

TEST(mc_engine, constant_sample_mean_variance_std_err)
{
    const fink::mc::mc_config cfg{.paths = 10, .seed = 42};

    const test_backend backend{};
    const fink::mc::online_stats_reducer reducer{};

    auto sample = [](std::size_t /*i*/, dummy_rng & /*rng*/) -> double {
        return 3.0;
    };

    const auto r = fink::mc::run(cfg, backend, sample, reducer);

    EXPECT_EQ(r.paths, 10u);
    EXPECT_DOUBLE_EQ(r.mean, 3.0);
    EXPECT_DOUBLE_EQ(r.variance, 0.0);
    EXPECT_DOUBLE_EQ(r.std_err, 0.0);
}

TEST(mc_engine, sample_uses_index_and_seed_is_passed)
{
    const fink::mc::mc_config cfg{.paths = 5, .seed = 7};

    const test_backend backend{};
    const fink::mc::online_stats_reducer reducer{};

    // x = i + seed
    auto sample = [](std::size_t i, dummy_rng &rng) -> double {
        return static_cast<double>(i) + static_cast<double>(rng.s);
    };

    const auto r = fink::mc::run(cfg, backend, sample, reducer);

    // values: 7,8,9,10,11 => mean = (7+11)/2 = 9
    EXPECT_EQ(r.paths, 5u);
    EXPECT_DOUBLE_EQ(r.mean, 9.0);
    EXPECT_GT(r.variance, 0.0);
    EXPECT_GT(r.std_err, 0.0);
}