#include "indiccrypt/fpe/round_value.hpp"

#include <cassert>

int main() {
    using namespace indiccrypt::fpe;

    const FpeRadix radix(10);

    // 10^0 = 1
    assert(
        Ff1RoundValue::modulus(radix, 0) == 1
    );

    // 10^1 = 10
    assert(
        Ff1RoundValue::modulus(radix, 1) == 10
    );

    // 10^5 = 100000
    assert(
        Ff1RoundValue::modulus(radix, 5) == 100000
    );

    // Reduction.
    assert(
        Ff1RoundValue::reduce(
            Ff1RoundValue::Integer{12345},
            radix,
            3
        ) == 345
    );

    // Addition modulo 10^3.
    assert(
        Ff1RoundValue::add(
            Ff1RoundValue::Integer{900},
            Ff1RoundValue::Integer{200},
            radix,
            3
        ) == 100
    );

    // Subtraction modulo 10^3.
    assert(
        Ff1RoundValue::subtract(
            Ff1RoundValue::Integer{100},
            Ff1RoundValue::Integer{200},
            radix,
            3
        ) == 900
    );

    // Large arbitrary-precision arithmetic.
    const Ff1RoundValue::Integer huge =
        Ff1RoundValue::Integer{
            999999999999999999LL
        };

    const auto result =
        Ff1RoundValue::add(
            huge,
            Ff1RoundValue::Integer{2},
            radix,
            18
        );

    assert(
        result ==
        Ff1RoundValue::Integer{1}
    );

    // Determinism.
    assert(
        result ==
        Ff1RoundValue::add(
            huge,
            Ff1RoundValue::Integer{2},
            radix,
            18
        )
    );

    return 0;
}