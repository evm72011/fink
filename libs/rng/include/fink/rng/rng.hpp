#pragma once
#include <concepts>

namespace fink::rng
{

template <class R>
concept uniform_rng = requires(R &r) {
    { r.uniform() } noexcept -> std::convertible_to<double>;
};

template <class R>
concept normal_rng_like = requires(R &r) {
    { r() } noexcept -> std::convertible_to<double>;
};

} // namespace fink::rng
