#include "indiccrypt/fpe/numeral_math.hpp"

#include <cassert>

int main() {
    using namespace indiccrypt::fpe;

    const FpeRadix radix(10);

    // 10^0 = 1
    assert(
        Ff1NumeralMath::radixPower(radix, 0) == 1
    );

    // 10^5 = 100000
    assert(
        Ff1NumeralMath::radixPower(radix, 5) == 100000
    );

    assert(
        Ff1NumeralMath::modulus(radix, 5) == 100000
    );

    const Ff1Numeral value(
        radix,
        {1, 2, 3}
    );

    // 123 + 10 = 133
    const auto result =
        Ff1NumeralMath::addModulo(
            value,
            10
        );

    assert(result.toInteger() == 133);

    // 123 + 900 = 1023 mod 1000 = 23
    const auto wrapped =
        Ff1NumeralMath::addModulo(
            value,
            900
        );

    assert(wrapped.toInteger() == 23);

    // Negative modular addition.
    // 123 - 200 = -77 mod 1000 = 923
    const auto negative =
        Ff1NumeralMath::addModulo(
            value,
            -200
        );

    assert(negative.toInteger() == 923);

    // Large arbitrary-precision radix power.
    const auto huge =
        Ff1NumeralMath::radixPower(
            radix,
            500
        );

    assert(huge > 0);

    return 0;
}