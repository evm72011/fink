#include <array>
#include <iostream>

#include <fink/backends/cpu/mc_backend_serial.hpp>
#include <fink/examples/formatters.hpp>
#include <fink/mc/config.hpp>
#include <fink/pricers/black_scholes.hpp>
#include <fink/pricers/european_mc.hpp>
#include <fink/pricers/native/european_mc.hpp>

using namespace fink::instruments;
using namespace fink::examples;

int main()
{
    const double spot = 100.0;
    const double sigma = 0.1;
    const double r = 0.05;
    const double T = 1.5;
    const double K = 120.0;
    const size_t paths = 1'000'000;

    const european_call option{
        .expiry = T,
        .payoff = call_payoff{.strike = K},
    };

    const fink::models::gbm_params gbm_params{
        .s0 = spot,
        .r = r,
        .sigma = sigma,
    };

    print(gbm_params);
    print(option);

    auto price_bs = fink::pricers::bs_european_call(option, gbm_params);
    print_row("Black-Scholes analytical price", price_bs);
    std::cout << '\n';

    const fink::mc::mc_config cfg{.paths = paths};
    const fink::backends::cpu::mc_backend_serial backend{};
    auto result = fink::pricers::price_european_mc(option,
                                                   gbm_params,
                                                   cfg.paths,
                                                   backend);

    std::cout << "Monte Carlo serial\n";
    print_row("paths:", format_paths(paths));
    print_row("price:", result.price);
    print_row("stderr:", result.stderr);
}
