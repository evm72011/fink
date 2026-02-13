#include <cmath>
#include <cstddef>
#include <cstdint>

#include <cuda_runtime.h>
#include <curand_kernel.h>

#include <fink/backends/gpu/mc_backend_cuda.hpp>

namespace
{

__device__ inline double payoff_call(double st, double k) noexcept
{
    const double x = st - k;
    return x > 0.0 ? x : 0.0;
}

__global__ void european_call_gbm_kernel(std::uint64_t seed,
                                        std::size_t n,
                                        double s0,
                                        double r,
                                        double sigma,
                                        double t,
                                        double strike,
                                        double* out_sum,
                                        double* out_sumsq)
{
    const std::size_t tid = static_cast<std::size_t>(blockIdx.x) * blockDim.x + threadIdx.x;
    const std::size_t stride = static_cast<std::size_t>(gridDim.x) * blockDim.x;

    // grid-stride loop, each thread accumulates local sum/sumsq
    double local_sum = 0.0;
    double local_sumsq = 0.0;

    // curand state (Philox is good default)
    curandStatePhilox4_32_10_t state;

    // Use tid as sequence number: deterministic for fixed grid config.
    curand_init(static_cast<unsigned long long>(seed),
                static_cast<unsigned long long>(tid),
                0ULL,
                &state);

    const double drift = (r - 0.5 * sigma * sigma) * t;
    const double vol = sigma * std::sqrt(t);

    for (std::size_t i = tid; i < n; i += stride)
    {
        // Standard normal
        const double z = curand_normal_double(&state);

        const double st = s0 * std::exp(drift + vol * z);
        const double pf = payoff_call(st, strike);

        local_sum += pf;
        local_sumsq += pf * pf;
    }

    // Reduce within block using shared memory (sum and sumsq)
    extern __shared__ double sdata[];
    double* s_sum = sdata;
    double* s_sumsq = sdata + blockDim.x;

    const unsigned int lane = threadIdx.x;
    s_sum[lane] = local_sum;
    s_sumsq[lane] = local_sumsq;
    __syncthreads();

    for (unsigned int s = blockDim.x / 2; s > 0; s >>= 1)
    {
        if (lane < s)
        {
            s_sum[lane] += s_sum[lane + s];
            s_sumsq[lane] += s_sumsq[lane + s];
        }
        __syncthreads();
    }

    if (lane == 0)
    {
        // Atomic add block results to global accumulators
        atomicAdd(out_sum, s_sum[0]);
        atomicAdd(out_sumsq, s_sumsq[0]);
    }
}

inline void cuda_check(cudaError_t e)
{
    if (e != cudaSuccess)
        std::abort();
}

} // namespace

namespace fink::backends::gpu
{

fink::mc::mc_result mc_backend_cuda::run_european_call_gbm(const fink::mc::mc_config& cfg,
                                                           const european_gbm_call_params& p) const
{
    if (cfg.paths == 0)
        return fink::mc::mc_result{};

    double* d_sum = nullptr;
    double* d_sumsq = nullptr;

    cuda_check(cudaMalloc(&d_sum, sizeof(double)));
    cuda_check(cudaMalloc(&d_sumsq, sizeof(double)));

    cuda_check(cudaMemset(d_sum, 0, sizeof(double)));
    cuda_check(cudaMemset(d_sumsq, 0, sizeof(double)));

    const std::size_t block = block_size_;
    // simple grid size heuristic
    int sm_count = 0;
    cuda_check(cudaDeviceGetAttribute(&sm_count, cudaDevAttrMultiProcessorCount, 0));
    const std::size_t grid = static_cast<std::size_t>(sm_count) * 8;

    const std::size_t shmem_bytes = 2 * block * sizeof(double);

    european_call_gbm_kernel<<<static_cast<unsigned int>(grid),
                              static_cast<unsigned int>(block),
                              shmem_bytes>>>(
        static_cast<std::uint64_t>(cfg.seed),
        cfg.paths,
        p.s0,
        p.r,
        p.sigma,
        p.t,
        p.strike,
        d_sum,
        d_sumsq);

    cuda_check(cudaGetLastError());
    cuda_check(cudaDeviceSynchronize());

    double h_sum = 0.0;
    double h_sumsq = 0.0;

    cuda_check(cudaMemcpy(&h_sum, d_sum, sizeof(double), cudaMemcpyDeviceToHost));
    cuda_check(cudaMemcpy(&h_sumsq, d_sumsq, sizeof(double), cudaMemcpyDeviceToHost));

    cuda_check(cudaFree(d_sum));
    cuda_check(cudaFree(d_sumsq));

    const double n = static_cast<double>(cfg.paths);
    const double mean = h_sum / n;

    // unbiased sample variance from sum/sumsq
    double var = 0.0;
    if (cfg.paths > 1)
    {
        const double ex2 = h_sumsq / n;
        const double ex = mean;
        const double pop_var = ex2 - ex * ex;                 // population variance
        var = pop_var * (n / (n - 1.0));                      // unbiased
        if (var < 0.0) var = 0.0;                             // numeric guard
    }

    const double stderr = (cfg.paths > 0) ? (std::sqrt(var) / std::sqrt(n)) : 0.0;

    fink::mc::mc_result r;
    r.mean = mean;
    r.variance = var;
    r.stderr = stderr;
    r.paths = cfg.paths;
    return r;
}

} // namespace fink::backends::gpu