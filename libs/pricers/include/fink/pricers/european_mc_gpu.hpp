#pragma once

#include <cmath>

#include <fink/backends/gpu/mc_backend_cuda.hpp>
#include <fink/instruments/concepts.hpp>
#include <fink/mc/config.hpp>
#include <fink/models/gbm.hpp>

namespace fink::pricers
{
template <fink::instruments::european_option_like Option>
double price_european_mc_cuda(
    const Option &opt,
    const fink::models::gbm_params &model,
    const fink::mc::mc_config &cfg,
    const fink::backends::gpu::mc_backend_cuda &backend)
{
    fink::backends::gpu::european_gbm_call_params p{
        .s0 = model.s0,
        .r = model.r,
        .sigma = model.sigma,
        .t = opt.expiry,
        .strike = opt.payoff.strike,
    };

    const auto r = backend.run_european_call_gbm(cfg, p);
    const double df = std::exp(-model.r * opt.expiry);
    return df * r.mean;
}
} // namespace fink::pricers