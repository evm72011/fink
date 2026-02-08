#include <iostream>
#include <random>
#include <string_view>
#include <iomanip>
#include <format>

#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>
#include <fink/rng/normal_rng.hpp>
#include <fink/rng/pcg32.hpp>
#include <fink/pricers/black_scholes.hpp>
#include <fink/pricers/european_mc.hpp>

using namespace fink::instruments;

void print_row(std::string_view label, double value)
{
    constexpr int w = 50;
    std::cout << std::left
              << std::setfill('.') << std::setw(w) << label
              << std::setfill(' ') << value << '\n';
}

std::string format_paths(size_t n)
{
    if (n >= 1'000'000)
        return std::to_string(n / 1'000'000) + "M";
    if (n >= 1'000)
        return std::to_string(n / 1'000) + "K";
    return std::to_string(n);
}

int main()
{
    const auto spot = 100.0; 
    const auto sigma = 0.1;
    const auto r = 0.05;    
    const auto T = 1.0;    

    print_row("Spot price", spot);
    print_row("Volatility (stddev)", sigma);
    print_row("Risk free rate", r);
    print_row("Time to maturity (years)", T);

    std::random_device rd;
    fink::rng::pcg32 pcg(rd());
    fink::rng::normal_rng normal_rng(pcg);

    fink::models::gbm_params gbm_params{
        .s0{spot},
        .r{r},
        .sigma{sigma},
    };
    auto z = normal_rng();
    auto price_spot = fink::models::gbm_terminal_price(gbm_params, T, z);
    std::cout << '\n';
    print_row("Possible spot price", price_spot);

    const auto strike = 120.0;
    print_row("Strike price", strike);
    std::cout << '\n';

    auto price_bs = fink::pricers::bs_european_call(spot, strike, r, sigma, T);
    print_row("Black-Sholes analytical price", price_bs);

    const european_call option{
        .expiry = T,
        .payoff = call_payoff{.strike = strike},
    };

    size_t paths[] { 100'000, 1'000'000, 5'000'000, 10'000'000};
    for (auto path : paths) {
            fink::pricers::mc_config mc_config {
                .paths=path,
            };
            auto price_mc_native = fink::pricers::price_european_mc(option, gbm_params, mc_config);
            auto label = "Monte Carlo native price [" + format_paths(path) + " paths]:";
            print_row(label, price_mc_native.price);
    }
}
