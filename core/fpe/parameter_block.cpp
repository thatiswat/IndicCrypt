#include "indiccrypt/fpe/parameter_block.hpp"

#include <cstdint>
#include <limits>
#include <stdexcept>

namespace indiccrypt::fpe {

namespace {

void writeUint32BE(
    Ff1ParameterBlock::Bytes& output,
    std::size_t offset,
    std::uint32_t value
) {
    if (offset + 4U > output.size()) {
        throw std::out_of_range(
            "FF1 parameter block write exceeds block size"
        );
    }

    output[offset + 0U] =
        static_cast<std::byte>((value >> 24U) & 0xffU);

    output[offset + 1U] =
        static_cast<std::byte>((value >> 16U) & 0xffU);

    output[offset + 2U] =
        static_cast<std::byte>((value >> 8U) & 0xffU);

    output[offset + 3U] =
        static_cast<std::byte>(value & 0xffU);
}

} // namespace

Ff1ParameterBlock::Bytes Ff1ParameterBlock::build(
    const FpeParameters& parameters,
    std::size_t messageLength
) {
    parameters.validateLength(messageLength);

    const std::size_t radix =
        parameters.domain().radix().value();

    const std::size_t tweakLength =
        parameters.tweak().size();

    if (radix < 2U || radix > 65536U) {
        throw std::invalid_argument(
            "FF1 radix must be in [2, 65536]"
        );
    }

    if (
        messageLength >
        std::numeric_limits<std::uint32_t>::max()
    ) {
        throw std::invalid_argument(
            "FF1 message length exceeds uint32 capacity"
        );
    }

    if (
        tweakLength >
        std::numeric_limits<std::uint32_t>::max()
    ) {
        throw std::invalid_argument(
            "FF1 tweak length exceeds uint32 capacity"
        );
    }

    Bytes block{};

    /*
     * FF1 P:
     *
     * [1]1
     * [2]1
     * [1]1
     * [radix]3
     * [10]1
     * [u mod 256]1
     * [n]4
     * [t]4
     *
     * Total = 16 bytes.
     */

    block[0] = std::byte{0x01};
    block[1] = std::byte{0x02};
    block[2] = std::byte{0x01};

    /*
     * radix is encoded as a 3-byte big-endian integer.
     */
    block[3] =
        static_cast<std::byte>(
            (radix >> 16U) & 0xffU
        );

    block[4] =
        static_cast<std::byte>(
            (radix >> 8U) & 0xffU
        );

    block[5] =
        static_cast<std::byte>(
            radix & 0xffU
        );

    /*
     * Number of Feistel rounds.
     */
    block[6] = std::byte{0x0a};

    /*
     * u = floor(n / 2)
     */
    const std::size_t u =
        messageLength / 2U;

    block[7] =
        static_cast<std::byte>(
            u & 0xffU
        );

    /*
     * n.
     */
    writeUint32BE(
        block,
        8U,
        static_cast<std::uint32_t>(
            messageLength
        )
    );

    /*
     * t = tweak length.
     */
    writeUint32BE(
        block,
        12U,
        static_cast<std::uint32_t>(
            tweakLength
        )
    );

    return block;
}

} // namespace indiccrypt::fpe