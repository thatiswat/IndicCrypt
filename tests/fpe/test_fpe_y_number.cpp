#include "indiccrypt/fpe/y_number.hpp"

#include <array>
#include <cassert>
#include <cstddef>

int main() {
    using namespace indiccrypt::fpe;

    // Empty Y -> 0.
    const std::array<std::byte, 0> empty{};

    assert(
        Ff1YNumber::fromBytes(empty) == 0
    );

    // Single byte.
    const std::array<std::byte, 1> one{
        std::byte{0x7f}
    };

    assert(
        Ff1YNumber::fromBytes(one) == 127
    );

    // Big-endian interpretation:
    //
    // 01 02 03 04
    // = 0x01020304
    // = 16909060
    const std::array<std::byte, 4> four{
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03},
        std::byte{0x04}
    };

    assert(
        Ff1YNumber::fromBytes(four) ==
        Ff1YNumber::Integer{16909060}
    );

    // Leading zero does not change the numeric value.
    const std::array<std::byte, 5> leadingZero{
        std::byte{0x00},
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03},
        std::byte{0x04}
    };

    assert(
        Ff1YNumber::fromBytes(leadingZero) ==
        Ff1YNumber::Integer{16909060}
    );

    // Arbitrary-precision value.
    const std::array<std::byte, 16> large{
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff},
        std::byte{0xff}
    };

    const auto value =
        Ff1YNumber::fromBytes(large);

    assert(value > 0);

    // Determinism.
    assert(
        value ==
        Ff1YNumber::fromBytes(large)
    );

    // A 16-byte value must exceed native 64-bit range.
    assert(
        value >
        Ff1YNumber::Integer{
            0xffffffffffffffffULL
        }
    );

    return 0;
}