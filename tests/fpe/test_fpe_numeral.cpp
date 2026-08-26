#include "indiccrypt/fpe/numeral.hpp"

#include <algorithm>
#include <cassert>
#include <stdexcept>
#include <vector>

int main() {
    using namespace indiccrypt::fpe;

    const FpeRadix radix(10);

    const Ff1Numeral numeral(
        radix,
        {1, 2, 3, 4, 5}
    );

    assert(numeral.toInteger() == 12345);

    // Integer -> radix digits must reproduce the original numeral.
    const Ff1Numeral recovered =
        Ff1Numeral::fromInteger(
            radix,
            numeral.toInteger()
        );

    assert(
        recovered.digits().size() ==
        numeral.digits().size()
    );

    assert(
        std::equal(
            recovered.digits().begin(),
            recovered.digits().end(),
            numeral.digits().begin(),
            numeral.digits().end()
        )
    );

    // Zero.
    const Ff1Numeral zero =
        Ff1Numeral::fromInteger(
            radix,
            Ff1Numeral::Integer{0}
        );

    assert(zero.digits().size() == 1);
    assert(zero.digits()[0] == 0);

    // Large arbitrary-precision value.
    Ff1Numeral::Integer huge = 1;

    for (int i = 0; i < 500; ++i) {
        huge *= 10;
        huge += 7;
    }

    const Ff1Numeral hugeNumeral =
        Ff1Numeral::fromInteger(
            radix,
            huge
        );

    assert(
        hugeNumeral.toInteger() == huge
    );

    // Invalid digit must be rejected.
    bool rejected = false;

    try {
        const Ff1Numeral invalid(
            radix,
            {1, 2, 10}
        );
    } catch (const std::out_of_range&) {
        rejected = true;
    }

    assert(rejected);

    // Negative integer must be rejected.
    bool negativeRejected = false;

    try {
        const auto invalid =
            Ff1Numeral::fromInteger(
                radix,
                Ff1Numeral::Integer{-1}
            );
    } catch (const std::invalid_argument&) {
        negativeRejected = true;
    }

    assert(negativeRejected);

    return 0;
}