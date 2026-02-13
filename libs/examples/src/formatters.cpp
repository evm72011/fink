#include <fink/examples/formatters.hpp>

namespace fink::examples
{
void print(const fink::models::gbm_params &params)
{
    std::cout << "Geometric Brownian motion parameters\n";
    print_row("Spot price", params.s0);
    print_row("Volatility (stddev)", params.sigma);
    print_row("Risk free rate", params.r);
    std::cout << '\n';
}

void print(const fink::instruments::european_call &option)
{
    std::cout << "European call option\n";
    print_row("Time to maturity (years)", option.expiry);
    print_row("Strike price", option.payoff.strike);
    std::cout << '\n';
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