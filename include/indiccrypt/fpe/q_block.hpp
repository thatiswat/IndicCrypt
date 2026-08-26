#pragma once

#include "indiccrypt/fpe/parameters.hpp"

#include <cstddef>
#include <span>
#include <vector>

namespace indiccrypt::fpe {

class Ff1QBlock {
public:
    [[nodiscard]]
    static std::vector<std::byte> build(
        const FpeParameters& parameters,
        std::size_t messageLength,
        std::size_t round,
        std::span<const FpeRadix::Value> numeral
    );
};

} // namespace indiccrypt::fpe