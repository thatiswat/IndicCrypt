#include "indiccrypt/fpe/round_value.hpp"

#include <stdexcept>

namespace indiccrypt::fpe {

Ff1RoundValue::Integer Ff1RoundValue::modulus(
    const FpeRadix& radix,
    std::size_t length
) {
    if (radix.value() < 2U) {
        throw std::invalid_argument(
            "FF1 radix must be at least 2"
        );
    }

    Integer result = 1;

    const Integer radixValue =
        static_cast<unsigned int>(
            radix.value()
        );

    for (std::size_t i = 0; i < length; ++i) {
        result *= radixValue;
    }

    return result;
}

Ff1RoundValue::Integer Ff1RoundValue::reduce(
    const Integer& value,
    const FpeRadix& radix,
    std::size_t length
) {
    const Integer m =
        modulus(radix, length);

    if (m == 0) {
        throw std::logic_error(
            "FF1 round modulus cannot be zero"
        );
    }

    Integer result = value % m;

    if (result < 0) {
        result += m;
    }

    return result;
}

Ff1RoundValue::Integer Ff1RoundValue::add(
    const Integer& value,
    const Integer& amount,
    const FpeRadix& radix,
    std::size_t length
) {
    return reduce(
        value + amount,
        radix,
        length
    );
}

Ff1RoundValue::Integer Ff1RoundValue::subtract(
    const Integer& value,
    const Integer& amount,
    const FpeRadix& radix,
    std::size_t length
) {
    return reduce(
        value - amount,
        radix,
        length
    );
}

} // namespace indiccrypt::fpe