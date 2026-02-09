#include <iostream>
#include <iomanip>

#include <fink/examples/format.hpp>

void print_row(std::string_view label, double value)
{
    constexpr int w = 50;
    std::cout << std::left << std::setfill('.') << std::setw(w) << label
              << std::setfill(' ') << value << '\n';
}

std::string format_paths(size_t n)
{
    if (n >= 1'000'000)
        return std::to_string(n / 1'000'000) + "M";
    if (n >= 1'000)
        return std::to_string(n / 1'000) + "K";
    return std::to_string(n);
}
