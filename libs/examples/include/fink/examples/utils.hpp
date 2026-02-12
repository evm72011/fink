#pragma once

#include <chrono>
#include <functional>
#include <string_view>

#include <fink/examples/formatters.hpp>
#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>

namespace fink::examples
{
struct example_result
{
    double price{0.0};
    double stderr{0.0};
};

template <class Pricer>
void run_mc_benchmark(const fink::instruments::european_call &option,
                      const fink::models::gbm_params &model,
                      size_t paths,
                      Pricer &&pricer)
{
    using clock = std::chrono::steady_clock;


    const auto t0 = clock::now();
    example_result res =
        std::invoke(std::forward<Pricer>(pricer), option, model);
    const auto t1 = clock::now();

    const double sec = std::chrono::duration<double>(t1 - t0).count();

    const auto main_label = std::format("[{} paths]", format_paths(paths));

    print_row(main_label, res.price);

    const auto meta =
        std::format("[time(sec): {:.5f}, stderr: {:.6g}]", sec, res.stderr);

    std::cout << meta << "\n\n";
}
} // namespace fink::examples
