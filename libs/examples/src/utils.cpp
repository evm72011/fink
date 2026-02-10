#include <iomanip>
#include <iostream>

#include <fink/examples/utils.hpp>

namespace fink::examples
{

fink::models::gbm_params default_gbm_params()
{
    fink::models::gbm_params result{
        .s0{100.0},
        .r{0.05},
        .sigma{0.1},
    };
    return result;
}

fink::instruments::european_call default_european_call()
{
    using namespace fink::instruments;
    european_call result{
        .expiry = 1.5,
        .payoff = call_payoff{.strike = 120.0},
    };
    return result;
}

void print_row(std::string_view label, double value)
{
    constexpr int w = 50;
    std::cout << std::left << std::setfill('.') << std::setw(w) << label
              << std::setfill(' ') << value << '\n';
}

void print_info(fink::instruments::european_call &option,
                fink::models::gbm_params &params)
{
    print_row("Spot price", params.s0);
    print_row("Volatility (stddev)", params.sigma);
    print_row("Risk free rate", params.r);
    print_row("Time to maturity (years)", option.expiry);
    print_row("Strike price", option.payoff.strike);
}

std::string format_paths(size_t n)
{
    if (n >= 1'000'000)
        return std::to_string(n / 1'000'000) + "M";
    if (n >= 1'000)
        return std::to_string(n / 1'000) + "K";
    return std::to_string(n);
}

} // namespace fink::examples
