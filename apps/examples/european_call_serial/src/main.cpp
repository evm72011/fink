#include <chrono>
#include <format>
#include <iostream>

#include <fink/examples/utils.hpp>
#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>
#include <fink/mc/config.hpp>
#include <fink/backends/cpu/mc_backend_serial.hpp>
#include <fink/pricers/black_scholes.hpp>
#include <fink/pricers/european_mc.hpp>

using namespace fink::examples;

void run_mc_benchmark(const fink::instruments::european_call &option,
                      const fink::models::gbm_params &model,
                      size_t paths)
{
    using clock = std::chrono::steady_clock;

    fink::mc::mc_config cfg{};
    cfg.paths = paths;

    fink::backends::cpu::mc_backend_serial backend{};

    const auto t0 = clock::now();
    const auto res = fink::pricers::price_european_mc(option, model, cfg.paths, backend);
    const auto t1 = clock::now();

    const double sec = std::chrono::duration<double>(t1 - t0).count();

    const auto label =
        std::format("Monte Carlo serial [{} paths]", format_paths(paths));

    print_row(label, res.mean);

    const auto meta =
        std::format("[time(sec): {:.5f}, stderr: {:.6g}]", sec, res.std_error);

    std::cout << meta << "\n\n";
}

int main()
{
    auto option = default_european_call();
    auto gbm_params = default_gbm_params();
    
    print_info(option, gbm_params);

    auto price_bs = fink::pricers::bs_european_call(option, gbm_params);
    print_row("Black-Scholes analytical price", price_bs);
    std::cout << '\n';

    size_t paths_set[]{100'000, 1'000'000, 5'000'000, 10'000'000};
    for (auto paths : paths_set)
    {
        run_mc_benchmark(option, gbm_params, paths);
    }
}
