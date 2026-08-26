#pragma once

#include "indiccrypt/fpe/parameters.hpp"

#include <cstddef>
#include <span>
#include <vector>

namespace indiccrypt::fpe {

class Ff1Round {
public:
    using Digit = FpeRadix::Value;

    [[nodiscard]]
    static std::vector<Digit> encrypt(
        const FpeParameters& parameters,
        std::span<const Digit> a,
        std::span<const Digit> b,
        std::size_t round
    );

    [[nodiscard]]
    static std::vector<Digit> decrypt(
        const FpeParameters& parameters,
        std::span<const Digit> a,
        std::span<const Digit> b,
        std::size_t round
    );
};

} // namespace indiccrypt::fpe