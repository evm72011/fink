#pragma once

#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>

namespace fink::examples
{

inline fink::models::gbm_params default_gbm_params()
{
    fink::models::gbm_params result{
        .s0 = 100.0,
        .r = 0.05,
        .sigma = 0.1,
    };
    return result;
}

inline fink::instruments::european_call default_european_call()
{
    using namespace fink::instruments;
    european_call result{
        .expiry = 1.5,
        .payoff = call_payoff{.strike = 120.0},
    };
    return result;
}


} // namespace fink::examples