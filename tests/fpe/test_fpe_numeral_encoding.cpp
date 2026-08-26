#include "indiccrypt/fpe/numeral_encoding.hpp"

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <vector>

int main() {
    using namespace indiccrypt::fpe;

    const FpeRadix radix(10);

    // 12345 -> 0x3039
    const Ff1Numeral numeral(
        radix,
        {1, 2, 3, 4, 5}
    );

    const auto encoded =
        Ff1NumeralEncoding::encode(
            numeral,
            4
        );

    assert(encoded.size() == 4U);

    assert(encoded[0] == std::byte{0x00});
    assert(encoded[1] == std::byte{0x00});
    assert(encoded[2] == std::byte{0x30});
    assert(encoded[3] == std::byte{0x39});

    // Zero.
    const Ff1Numeral zero =
        Ff1Numeral::fromInteger(
            radix,
            Ff1Numeral::Integer{0}
        );

    const auto zeroEncoded =
        Ff1NumeralEncoding::encode(
            zero,
            4
        );

    const std::vector<std::byte> expectedZero{
        std::byte{0x00},
        std::byte{0x00},
        std::byte{0x00},
        std::byte{0x00}
    };

    assert(zeroEncoded == expectedZero);

    // Large arbitrary-precision value.
    Ff1Numeral::Integer huge = 1;

    for (int i = 0; i < 100; ++i) {
        huge *= 10;
        huge += 7;
    }

    const Ff1Numeral hugeNumeral =
        Ff1Numeral::fromInteger(
            radix,
            huge
        );

    const auto hugeEncoded =
        Ff1NumeralEncoding::encode(
            hugeNumeral,
            64
        );

    assert(hugeEncoded.size() == 64U);

    // Determinism.
    assert(
        hugeEncoded ==
        Ff1NumeralEncoding::encode(
            hugeNumeral,
            64
        )
    );

    // Insufficient output space.
    bool overflowRejected = false;

    try {
        (void)Ff1NumeralEncoding::encode(
            numeral,
            1
        );
    } catch (const std::overflow_error&) {
        overflowRejected = true;
    }

    assert(overflowRejected);

    // Zero-length encoding is invalid.
    bool zeroLengthRejected = false;

    try {
        (void)Ff1NumeralEncoding::encode(
            numeral,
            0
        );
    } catch (const std::invalid_argument&) {
        zeroLengthRejected = true;
    }

    assert(zeroLengthRejected);

    return 0;
}