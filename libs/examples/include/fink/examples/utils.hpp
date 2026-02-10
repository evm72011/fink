#include <string_view>

#include <fink/models/gbm.hpp>
#include <fink/instruments/aliases.hpp>

namespace fink::examples
{
void print_row(std::string_view label, double value);
std::string format_paths(size_t n);
fink::models::gbm_params default_gbm_params();
fink::instruments::european_call default_european_call();
void print_info(fink::instruments::european_call &option, 
                fink::models::gbm_params &params);
}
