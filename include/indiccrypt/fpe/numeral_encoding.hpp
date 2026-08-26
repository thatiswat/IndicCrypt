#pragma once

#include "indiccrypt/fpe/numeral.hpp"

#include <cstddef>
#include <vector>

namespace indiccrypt::fpe {

class Ff1NumeralEncoding {
public:
    [[nodiscard]]
    static std::vector<std::byte> encode(
        const Ff1Numeral& numeral,
        std::size_t byteLength
    );
};

} // namespace indiccrypt::fpe