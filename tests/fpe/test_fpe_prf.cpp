#include "indiccrypt/fpe/prf.hpp"

#include "indiccrypt/crypto/types.hpp"

#include <cassert>
#include <cstddef>
#include <vector>

int main() {
    using namespace indiccrypt::crypto;
    using namespace indiccrypt::fpe;

    const ByteVector keyBytes{
        0x60, 0x3d, 0xeb, 0x10,
        0x15, 0xca, 0x71, 0xbe,
        0x2b, 0x73, 0xae, 0xf0,
        0x85, 0x7d, 0x77, 0x81,
        0x1f, 0x35, 0x2c, 0x07,
        0x3b, 0x61, 0x08, 0xd7,
        0x2d, 0x98, 0x10, 0xa3,
        0x09, 0x14, 0xdf, 0xf4
    };

    const Key key(keyBytes);

    const std::vector<std::byte> input{
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03},
        std::byte{0x04},
        std::byte{0x05}
    };

    const auto output =
        Ff1Prf::evaluate(
            key,
            input,
            16
        );

    assert(output.size() == 16);

    // Determinism.
    const auto repeated =
        Ff1Prf::evaluate(
            key,
            input,
            16
        );

    assert(output == repeated);

    // Different output lengths.
    const auto shortOutput =
        Ff1Prf::evaluate(
            key,
            input,
            8
        );

    assert(shortOutput.size() == 8);

    const auto longOutput =
        Ff1Prf::evaluate(
            key,
            input,
            32
        );

    assert(longOutput.size() == 32);

    // Different input should produce a different result.
    auto changedInput = input;
    changedInput[0] = std::byte{0xff};

    const auto changed =
        Ff1Prf::evaluate(
            key,
            changedInput,
            16
        );

    assert(output != changed);

    // Zero-length output.
    const auto empty =
        Ff1Prf::evaluate(
            key,
            input,
            0
        );

    assert(empty.empty());

    return 0;
}