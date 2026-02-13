#pragma once

namespace fink::pricers
{
/**
 * @brief Result of a price calculation
 */
struct pricer_result
{
    /// Estimated option price.
    double price{};

    /// Standard error of the estimator.
    double std_error{};
};
}
