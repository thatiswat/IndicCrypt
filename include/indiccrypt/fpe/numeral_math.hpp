#pragma once

#include "indiccrypt/fpe/numeral.hpp"

namespace indiccrypt::fpe {

class Ff1NumeralMath {
public:
    using Integer = Ff1Numeral::Integer;

    [[nodiscard]]
    static Integer radixPower(
        const FpeRadix& radix,
        std::size_t exponent
    );

    [[nodiscard]]
    static Integer modulus(
        const FpeRadix& radix,
        std::size_t length
    );

    [[nodiscard]]
    static Ff1Numeral addModulo(
        const Ff1Numeral& value,
        const Integer& amount
    );
};

} // namespace indiccrypt::fpe