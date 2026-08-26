#include "indiccrypt/fpe/y.hpp"

#include "indiccrypt/crypto/aes_block.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace indiccrypt::fpe {

namespace {

crypto::ByteVector xorBlocks(
    std::span<const crypto::Byte> left,
    std::span<const crypto::Byte> right
) {
    if (
        left.size() != Ff1YGenerator::BlockSize ||
        right.size() != Ff1YGenerator::BlockSize
    ) {
        throw std::invalid_argument(
            "FF1 Y generation requires 16-byte blocks"
        );
    }

    crypto::ByteVector result(
        Ff1YGenerator::BlockSize
    );

    for (std::size_t i = 0;
         i < Ff1YGenerator::BlockSize;
         ++i) {
        result[i] = left[i] ^ right[i];
    }

    return result;
}

crypto::ByteVector counterBlock(
    std::uint32_t counter
) {
    crypto::ByteVector block(
        Ff1YGenerator::BlockSize,
        static_cast<crypto::Byte>(0)
    );

    block[12] =
        static_cast<crypto::Byte>(
            (counter >> 24U) & 0xffU
        );

    block[13] =
        static_cast<crypto::Byte>(
            (counter >> 16U) & 0xffU
        );

    block[14] =
        static_cast<crypto::Byte>(
            (counter >> 8U) & 0xffU
        );

    block[15] =
        static_cast<crypto::Byte>(
            counter & 0xffU
        );

    return block;
}

} // namespace

Ff1YGenerator::Bytes Ff1YGenerator::generate(
    const crypto::Key& key,
    std::span<const crypto::Byte> r,
    std::size_t length
) {
    if (key.size() != crypto::Aes256Block::KeySize) {
        throw std::invalid_argument(
            "FF1 Y generation requires a 32-byte AES-256 key"
        );
    }

    if (r.size() != BlockSize) {
        throw std::invalid_argument(
            "FF1 Y generation requires a 16-byte R value"
        );
    }

    if (length == 0U) {
        return {};
    }

    Bytes output;
    output.reserve(length);

    /*
     * Y starts with R.
     */
    const std::size_t initialTake =
        std::min(length, BlockSize);

    output.insert(
        output.end(),
        r.begin(),
        r.begin() +
            static_cast<std::ptrdiff_t>(initialTake)
    );

    if (output.size() == length) {
        return output;
    }

    /*
     * Continue with:
     *
     * AES_K(
     *     R XOR [j]^16
     * )
     *
     * where j starts at 1.
     */
    std::uint32_t counter = 1U;

    while (output.size() < length) {
        const auto counterValue =
            counterBlock(counter);

        const auto mixed =
            xorBlocks(
                r,
                counterValue
            );

        const auto encrypted =
            crypto::Aes256Block::encrypt(
                key,
                mixed
            );

        const std::size_t remaining =
            length - output.size();

        const std::size_t take =
            std::min(
                remaining,
                encrypted.size()
            );

        output.insert(
            output.end(),
            encrypted.begin(),
            encrypted.begin() +
                static_cast<std::ptrdiff_t>(take)
        );

        ++counter;
    }

    return output;
}

} // namespace indiccrypt::fpe