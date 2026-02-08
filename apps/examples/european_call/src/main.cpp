#include <iostream>

#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>

using namespace fink::instruments;

int main()
{
    const european_call option{
        .expiry = 1.5,
        .payoff = call_payoff{.strike = 120.0},
    };
    std::cout << option.expiry << '\n';
}
