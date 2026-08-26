#include "indiccrypt/fpe/numeral_math.hpp"

#include <stdexcept>

namespace indiccrypt::fpe {

Ff1NumeralMath::Integer
Ff1NumeralMath::radixPower(
    const FpeRadix& radix,
    std::size_t exponent
) {
    Integer result = 1;

    const Integer base =
        static_cast<unsigned int>(radix.value());

    for (std::size_t i = 0; i < exponent; ++i) {
        result *= base;
    }

    return result;
}

Ff1NumeralMath::Integer
Ff1NumeralMath::modulus(
    const FpeRadix& radix,
    std::size_t length
) {
    return radixPower(radix, length);
}

Ff1Numeral
Ff1NumeralMath::addModulo(
    const Ff1Numeral& value,
    const Integer& amount
) {
    const Integer modulusValue =
        modulus(
            value.radix(),
            value.size()
        );

    if (modulusValue == 0) {
        throw std::logic_error(
            "FF1 modulus cannot be zero"
        );
    }

    Integer result =
        (value.toInteger() + amount) %
        modulusValue;

    if (result < 0) {
        result += modulusValue;
    }

    return Ff1Numeral::fromInteger(
        value.radix(),
        result
    );
}

} // namespace indiccrypt::fpe