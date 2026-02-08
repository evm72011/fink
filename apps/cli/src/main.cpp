#include <iostream>
#include <random>

#include <fink/instruments/aliases.hpp>
#include <fink/math/discount.hpp>

#include <fink/models/gbm.hpp>

#include <fink/pricers/black_scholes.hpp>
#include <fink/pricers/european_mc.hpp>
#include <fink/rng/normal_rng.hpp>
#include <fink/rng/pcg32.hpp>

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

    using namespace fink::models;
    gbm_params params{.s0{100.0}, .r{0.05}, .sigma{0.1}};

    std::random_device rd;
    fink::rng::pcg32 r(rd());
    fink::rng::normal_rng normal_rng(r);
    auto z = normal_rng();

    auto price_s = gbm_terminal_price(params, 1.0, z);
    std::cout << z << " : " << price_s << '\n';

    auto price_o1 =
        fink::pricers::bs_european_call(100.0, 100.0, 0.05, 0.2, 1.5);
    std::cout << price_o1 << '\n';

    fink::pricers::mc_config conf;
    auto price_o2 = fink::pricers::price_european_mc(c, params, conf);
    std::cout << price_o2.price << '\n';
}
