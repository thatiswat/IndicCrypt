#include "indiccrypt/fpe/prf.hpp"

#include "indiccrypt/crypto/types.hpp"

#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <vector>

int main() {
    using namespace indiccrypt::crypto;
    using namespace indiccrypt::fpe;

    /*
     * AES-256 test key.
     */
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

    /*
     * FF1 PRF input is P || Q and must be:
     *
     *   - non-empty
     *   - a multiple of 16 bytes
     */
    const std::vector<std::byte> input(
        32,
        std::byte{0x00}
    );

    /*
     * ---------------------------------------------------------
     * Basic PRF evaluation
     * ---------------------------------------------------------
     */
    const auto output =
        Ff1Prf::evaluate(
            key,
            input,
            Ff1Prf::BlockSize
        );

    assert(
        output.size() ==
        Ff1Prf::BlockSize
    );

    /*
     * ---------------------------------------------------------
     * Determinism
     * ---------------------------------------------------------
     */
    const auto repeated =
        Ff1Prf::evaluate(
            key,
            input,
            Ff1Prf::BlockSize
        );

    assert(
        output ==
        repeated
    );

    /*
     * ---------------------------------------------------------
     * Different input must produce
     * a different PRF result.
     * ---------------------------------------------------------
     */
    auto changedInput = input;

    changedInput[0] =
        std::byte{0xff};

    const auto changed =
        Ff1Prf::evaluate(
            key,
            changedInput,
            Ff1Prf::BlockSize
        );

    assert(
        output !=
        changed
    );

    /*
     * ---------------------------------------------------------
     * Multiple FF1 blocks
     * ---------------------------------------------------------
     */
    const std::vector<std::byte> largerInput(
        48,
        std::byte{0x11}
    );

    const auto largerOutput =
        Ff1Prf::evaluate(
            key,
            largerInput,
            Ff1Prf::BlockSize
        );

    assert(
        largerOutput.size() ==
        Ff1Prf::BlockSize
    );

    /*
     * ---------------------------------------------------------
     * Different key must produce
     * a different result.
     * ---------------------------------------------------------
     */
    const ByteVector otherKeyBytes(
        32,
        static_cast<Byte>(0x42)
    );

    const Key otherKey(
        otherKeyBytes
    );

    const auto differentKeyOutput =
        Ff1Prf::evaluate(
            otherKey,
            input,
            Ff1Prf::BlockSize
        );

    assert(
        output !=
        differentKeyOutput
    );

    /*
     * ---------------------------------------------------------
     * Empty input must be rejected.
     * ---------------------------------------------------------
     */
    bool emptyRejected = false;

    try {
        const std::vector<std::byte> emptyInput;

        (void)Ff1Prf::evaluate(
            key,
            emptyInput,
            Ff1Prf::BlockSize
        );
    } catch (const std::invalid_argument&) {
        emptyRejected = true;
    }

    assert(
        emptyRejected
    );

    /*
     * ---------------------------------------------------------
     * Non-block-aligned input must be rejected.
     * ---------------------------------------------------------
     */
    bool invalidLengthRejected = false;

    try {
        const std::vector<std::byte> invalidInput(
            17,
            std::byte{0x00}
        );

        (void)Ff1Prf::evaluate(
            key,
            invalidInput,
            Ff1Prf::BlockSize
        );
    } catch (const std::invalid_argument&) {
        invalidLengthRejected = true;
    }

    assert(
        invalidLengthRejected
    );

    /*
     * ---------------------------------------------------------
     * Invalid AES key size must be rejected.
     *
     * Valid FF1 AES keys:
     *
     *   16 bytes -> AES-128
     *   24 bytes -> AES-192
     *   32 bytes -> AES-256
     *
     * Therefore 15 bytes is intentionally invalid.
     * ---------------------------------------------------------
     */
    bool invalidKeyRejected = false;

    try {
        const Key invalidKey(
            ByteVector(
                15,
                static_cast<Byte>(0x42)
            )
        );

        (void)Ff1Prf::evaluate(
            invalidKey,
            input,
            Ff1Prf::BlockSize
        );
    } catch (const std::invalid_argument&) {
        invalidKeyRejected = true;
    }

    assert(
        invalidKeyRejected
    );

    /*
     * ---------------------------------------------------------
     * Output-length control
     * ---------------------------------------------------------
     */
    const auto shortOutput =
        Ff1Prf::evaluate(
            key,
            input,
            8
        );

    assert(
        shortOutput.size() == 8
    );

    const auto longOutput =
        Ff1Prf::evaluate(
            key,
            input,
            32
        );

    assert(
        longOutput.size() == 32
    );

    /*
     * ---------------------------------------------------------
     * Zero-length output
     * ---------------------------------------------------------
     */
    const auto emptyOutput =
        Ff1Prf::evaluate(
            key,
            input,
            0
        );

    assert(
        emptyOutput.empty()
    );

    return 0;
}