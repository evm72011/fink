#include <array>
#include <iostream>

#include <fink/backends/cpu/mc_backend_parallel.hpp>
#include <fink/examples/defaults.hpp>
#include <fink/examples/formatters.hpp>
#include <fink/examples/utils.hpp>
#include <fink/mc/config.hpp>
#include <fink/pricers/black_scholes.hpp>
#include <fink/pricers/european_mc.hpp>
#include <fink/pricers/native/european_mc.hpp>

using namespace fink::examples;

int main()
{
    std::cout << "Monte Carlo parallel\n\n";
    
    auto option = default_european_call();
    print(option);
    
    auto gbm_params = default_gbm_params();
    print(gbm_params);

    auto price_bs = fink::pricers::bs_european_call(option, gbm_params);
    print_row("Black-Scholes analytical price", price_bs);
    std::cout << '\n';

    const std::array<size_t, 4> paths_set{100'000,
                                          1'000'000,
                                          5'000'000,
                                          10'000'000};
    for (auto paths : paths_set)
    {
        run_mc_benchmark(  // TODO the aim this code - example. But this wrapper makes it not clear - how exactly to use the lib. 
            option,
            gbm_params,
            paths,
            [paths](auto const &opt, auto const &mdl) {
                const fink::mc::mc_config cfg{.paths = paths};
                const auto backend = fink::backends::cpu::mc_backend_parallel(10);
                auto result = fink::pricers::price_european_mc(opt,
                                                               mdl,
                                                               cfg.paths,
                                                               backend);
                const double df = std::exp(-mdl.r * opt.expiry);

                return example_result{
                    .price = result.mean * df,
                    .stderr = result.std_error * df
                };
            });
    }
}
