#pragma once

#include "indiccrypt/fpe/parameters.hpp"

#include <array>
#include <cstddef>

namespace indiccrypt::fpe {

class Ff1ParameterBlock {
public:
    static constexpr std::size_t Size = 16;

    using Bytes = std::array<std::byte, Size>;

    [[nodiscard]]
    static Bytes build(
        const FpeParameters& parameters,
        std::size_t messageLength
    );
};

} // namespace indiccrypt::fpe