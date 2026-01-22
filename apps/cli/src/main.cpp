#include "iostream"
#include <fink/instruments/aliases.hpp>
#include <fink/math/discount.hpp>

int main()
{
    std::cout << std::boolalpha;

    using namespace fink::instruments;

    const european_call c{
        .expiry = 1.5,
        .payoff = call_payoff{.strike = 100.0},
    };

    std::cout << "fink-cli:\n";
    std::cout << (c.style == exercise_style::european) << '\n';
    std::cout << c.payoff(110.0) << '\n';

    std::cout << 100.0 * fink::math::discount_continuous(.05, 1) << '\n';
}
