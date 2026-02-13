#pragma once

#include <string>
#include <iomanip>
#include <iostream>
#include <string_view>

#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>

namespace fink::examples
{
template <typename T>  
void print_row(std::string_view label, T value, int width = 40)
{
    std::cout << std::left << std::setfill('.') << std::setw(width) << label
              << std::setfill(' ') << value << '\n';
};

void print(const fink::models::gbm_params &params);

void print(const fink::instruments::european_call &option);

std::string format_paths(size_t n);
} // namespace fink::examples