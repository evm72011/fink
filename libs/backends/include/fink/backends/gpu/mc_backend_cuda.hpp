#pragma once

#include <cstddef>
#include <cstdint>

#include <fink/mc/config.hpp>
#include <fink/mc/result.hpp>

namespace fink::backends::gpu
{

struct european_gbm_call_params
{
    double s0{};
    double r{};
    double sigma{};
    double t{};
    double strike{};
};

class mc_backend_cuda
{
public:
    explicit mc_backend_cuda(std::size_t block_size = 256) noexcept
        : block_size_(block_size)
    {
    }

    [[nodiscard]] fink::mc::mc_result price_european_call_gbm(const fink::mc::mc_config& cfg,
                                                              const european_gbm_call_params& p) const;

private:
    std::size_t block_size_{256};
};

} // namespace fink::backends::gpu