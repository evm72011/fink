#pragma once

#include <cstdint>

namespace fink::rng
{

class pcg32
{
public:
    using result_type = std::uint32_t;

    explicit pcg32(std::uint64_t seed = 0x853c49e6748fea9bULL) noexcept
        : state_(seed)
    {
        advance();
    }

    void advance() noexcept
    {
        static_cast<void>(next());
    }

    [[nodiscard]] result_type next() noexcept
    {
        const std::uint64_t old = state_;
        state_ = old * 6364136223846793005ULL + inc_;

        auto xorshifted =
            static_cast<std::uint32_t>(((old >> 18u) ^ old) >> 27u);
        auto rot = static_cast<std::uint32_t>(old >> 59u);
        return (xorshifted >> rot) | (xorshifted << ((-rot) & 31));
    }

    [[nodiscard]] double uniform() noexcept
    {
        return (static_cast<double>(next()) + 0.5) / 4294967296.0;
    }

private:
    std::uint64_t state_;
    std::uint64_t inc_{0xda3e39cb94b95bdbULL};
};

} // namespace fink::rng
