#pragma once

#include <string_view>

#include <fink/instruments/aliases.hpp>
#include <fink/models/gbm.hpp>

namespace fink::examples
{
void print_row(std::string_view label, double value);

void print(fink::models::gbm_params &params);

void print(fink::instruments::european_call &option);

std::string format_paths(size_t n);
}