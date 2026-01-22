#include <cmath>
#include <fink/math/stats.hpp>

namespace fink::math
{

double mean(std::span<const double> x) noexcept
{
    if (x.empty())
    {
        return 0.0;
    }

    double sum = 0.0;
    for (auto v : x)
    {
        sum += v;
    }
    return sum / static_cast<double>(x.size());
}

double mean(const double *data, std::size_t n) noexcept
{
    if (data == nullptr || n == 0)
    {
        return 0.0;
    }
    return mean(std::span<const double>{data, n});
}

double variance(std::span<const double> x) noexcept
{
    if (x.size() < 2)
    {
        return 0.0;
    }

    const auto mu = mean(x);

    double sum = 0.0;
    for (auto v : x)
    {
        const double d = v - mu;
        sum = std::fma(d, d, sum);
    }
    return sum / static_cast<double>(x.size() - 1);
}

double variance(const double *data, std::size_t n) noexcept
{
    if (data == nullptr || n == 0)
    {
        return 0.0;
    }
    return variance(std::span<const double>{data, n});
}

double stddev(std::span<const double> x) noexcept
{
    return std::sqrt(variance(x));
}

double stddev(const double *data, std::size_t n) noexcept
{
    return stddev(std::span<const double>(data, n));
}
} // namespace fink::math
