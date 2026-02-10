#include <iostream>
#include <array>

#include <fink/examples/utils.hpp>
#include <fink/pricers/black_scholes.hpp>
#include <fink/pricers/native/european_mc.hpp>

using namespace fink::examples;

int main()
{
    std::cout << "Monte Carlo native\n\n";
    auto option = default_european_call();
    auto gbm_params = default_gbm_params();

    print_info(option, gbm_params);

    auto price_bs = fink::pricers::bs_european_call(option, gbm_params);
    print_row("Black-Scholes analytical price", price_bs);
    std::cout << '\n';

    const std::array<size_t, 4> paths_set {100'000, 1'000'000, 5'000'000, 10'000'000};
    for (auto paths : paths_set)
    {
        run_mc_benchmark(
            option,
            gbm_params,
            paths,
            [paths](auto const &opt, auto const &mdl) {
                fink::pricers::native::mc_config cfg{.paths = paths};
                auto result = fink::pricers::native::price_european_mc(opt, mdl, cfg);
                return result; // TODO map to examples result
            });
    }
}
