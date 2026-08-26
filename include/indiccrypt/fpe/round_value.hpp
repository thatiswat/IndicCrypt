#pragma once

#include "indiccrypt/fpe/numeral.hpp"

#include <cstddef>

namespace indiccrypt::fpe {

class Ff1RoundValue {
public:
    using Integer = Ff1Numeral::Integer;

    [[nodiscard]]
    static Integer modulus(
        const FpeRadix& radix,
        std::size_t length
    );

    [[nodiscard]]
    static Integer reduce(
        const Integer& value,
        const FpeRadix& radix,
        std::size_t length
    );

    [[nodiscard]]
    static Integer add(
        const Integer& value,
        const Integer& amount,
        const FpeRadix& radix,
        std::size_t length
    );

    [[nodiscard]]
    static Integer subtract(
        const Integer& value,
        const Integer& amount,
        const FpeRadix& radix,
        std::size_t length
    );
};

} // namespace indiccrypt::fpe