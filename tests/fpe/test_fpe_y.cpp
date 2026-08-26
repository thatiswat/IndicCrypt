#include "indiccrypt/fpe/y.hpp"

#include "indiccrypt/crypto/types.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <vector>

int main() {
    using namespace indiccrypt::crypto;
    using namespace indiccrypt::fpe;

    const ByteVector keyBytes(
        32,
        static_cast<Byte>(0x42)
    );

    const Key key(keyBytes);

    const std::array<Byte, 16> r{
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f
    };

    // Empty output.
    const auto empty =
        Ff1YGenerator::generate(
            key,
            r,
            0
        );

    assert(empty.empty());

    // One AES block.
    const auto oneBlock =
        Ff1YGenerator::generate(
            key,
            r,
            16
        );

    assert(
        oneBlock.size() ==
        Ff1YGenerator::BlockSize
    );

    // Determinism.
    const auto repeated =
        Ff1YGenerator::generate(
            key,
            r,
            16
        );

    assert(oneBlock == repeated);

    // Multiple blocks.
    const auto multiple =
        Ff1YGenerator::generate(
            key,
            r,
            32
        );

    assert(multiple.size() == 32);

    assert(
        multiple !=
        oneBlock
    );

    // Non-block-aligned length.
    const auto partial =
        Ff1YGenerator::generate(
            key,
            r,
            23
        );

    assert(partial.size() == 23);

    // Different R must produce different output.
    auto changedR = r;
    changedR[0] ^= static_cast<Byte>(0xff);

    const auto changed =
        Ff1YGenerator::generate(
            key,
            changedR,
            16
        );

    assert(
        changed !=
        oneBlock
    );

    return 0;
}