#pragma once

#include "indiccrypt/fpe/numeral.hpp"

#include <cstddef>
#include <span>

namespace indiccrypt::fpe {

class Ff1YNumber {
public:
    using Integer = Ff1Numeral::Integer;

    [[nodiscard]]
    static Integer fromBytes(
        std::span<const std::byte> bytes
    );
};

} // namespace indiccrypt::fpe