#include <chrono>
#include <format>
#include <iomanip>
#include <iostream>
#include <random>
#include <string_view>

#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>
#include <fink/pricers/black_scholes.hpp>
#include <fink/pricers/native/european_mc.hpp>
#include <fink/rng/normal_rng.hpp>
#include <fink/rng/pcg32.hpp>

using namespace fink::instruments;

void print_row(std::string_view label, double value)
{
    constexpr int w = 50;
    std::cout << std::left << std::setfill('.') << std::setw(w) << label
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

void run_mc_benchmark(const european_call &option,
                      const fink::models::gbm_params &model,
                      size_t paths)
{
    using clock = std::chrono::steady_clock;

    fink::pricers::native::mc_config cfg{.paths = paths};

    const auto t0 = clock::now();
    const auto res = fink::pricers::native::price_european_mc(option, model, cfg);
    const auto t1 = clock::now();

    const double sec = std::chrono::duration<double>(t1 - t0).count();

    const auto main_label =
        std::format("Monte Carlo native [{} paths]", format_paths(paths));

    print_row(main_label, res.price);

    const auto meta =
        std::format("[time(sec): {:.5f}, stderr: {:.6g}]", sec, res.stderr);

    std::cout << meta << "\n\n";
}

int main()
{
    const auto spot = 100.0;
    const auto sigma = 0.1;
    const auto r = 0.05;
    const auto T = 1.5;
    const auto strike = 120.0;

    print_row("Spot price", spot);
    print_row("Volatility (stddev)", sigma);
    print_row("Risk free rate", r);
    print_row("Time to maturity (years)", T);
    print_row("Strike price", strike);

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
    print_row("Simulated terminal price (GBM)", price_spot);
    std::cout << '\n';

    const european_call option{
        .expiry = T,
        .payoff = call_payoff{.strike = strike},
    };

    auto price_bs = fink::pricers::bs_european_call(option, gbm_params);
    print_row("Black-Scholes analytical price", price_bs);
    std::cout << '\n';

    size_t paths_set[]{100'000, 1'000'000, 5'000'000, 10'000'000};
    for (auto paths : paths_set)
    {
        run_mc_benchmark(option, gbm_params, paths);
    }
}
