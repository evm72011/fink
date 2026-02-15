#pragma once

#include <cmath>

#include <fink/backends/gpu/mc_backend_cuda.hpp>
#include <fink/instruments/concepts.hpp>
#include <fink/mc/config.hpp>
#include <fink/models/gbm.hpp>
#include <fink/math/discount.hpp>
#include <fink/pricers/pricer_result.hpp>

namespace fink::pricers
{
template <fink::instruments::european_option_like Option>
[[nodiscard]] pricer_result price_european_mc_cuda(
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

    auto mc_result = backend.price_european_call_gbm(cfg, p);
    const double df = fink::math::discount_continuous(model.r, opt.expiry);
    return pricer_result {
        .price = df * mc_result.mean,
        .std_err = df * mc_result.std_err,
    };
}
} // namespace fink::pricers

// NOTE: Temporary CUDA-specific pricer path.
// TODO: Integrate with fink::mc::run() so the pricer passes (cfg, instrument/model)
// and the backend is selected in the MC layer.
// TODO: Replace european_gbm_call_params with a generic instrument/model view.