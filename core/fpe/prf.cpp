#include "indiccrypt/fpe/prf.hpp"

#include "indiccrypt/crypto/aes_block.hpp"

#include <algorithm>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

namespace indiccrypt::fpe {

namespace {

crypto::ByteVector xorBlock(
    std::span<const std::byte> left,
    std::span<const crypto::Byte> right
) {
    if (
        left.size() != Ff1Prf::BlockSize ||
        right.size() != Ff1Prf::BlockSize
    ) {
        throw std::invalid_argument(
            "FF1 PRF XOR requires 16-byte blocks"
        );
    }

    crypto::ByteVector result(
        Ff1Prf::BlockSize,
        static_cast<crypto::Byte>(0)
    );

    for (
        std::size_t i = 0;
        i < Ff1Prf::BlockSize;
        ++i
    ) {
        result[i] =
            std::to_integer<crypto::Byte>(left[i])
            ^ right[i];
    }

    return result;
}

crypto::ByteVector xorCryptoBlocks(
    std::span<const crypto::Byte> left,
    std::span<const crypto::Byte> right
) {
    if (
        left.size() != Ff1Prf::BlockSize ||
        right.size() != Ff1Prf::BlockSize
    ) {
        throw std::invalid_argument(
            "FF1 PRF XOR requires 16-byte blocks"
        );
    }

    crypto::ByteVector result(
        Ff1Prf::BlockSize,
        static_cast<crypto::Byte>(0)
    );

    for (
        std::size_t i = 0;
        i < Ff1Prf::BlockSize;
        ++i
    ) {
        result[i] =
            left[i] ^ right[i];
    }

    return result;
}

} // namespace

Ff1Prf::Bytes Ff1Prf::evaluate(
    const crypto::Key& key,
    std::span<const std::byte> input,
    std::size_t outputLength
) {
    if (outputLength == 0U) {
        return {};
    }

    if (
        key.size() !=
        crypto::Aes256Block::KeySize
    ) {
        throw std::invalid_argument(
            "FF1 PRF requires a 32-byte AES-256 key"
        );
    }

    if (
        input.empty() ||
        input.size() % BlockSize != 0U
    ) {
        throw std::invalid_argument(
            "FF1 PRF input must be a non-empty "
            "multiple of 16 bytes"
        );
    }

    /*
     * R = 0^128
     *
     * R = AES_K(R XOR X)
     *
     * for every 16-byte input block X.
     */
    crypto::ByteVector chaining(
        BlockSize,
        static_cast<crypto::Byte>(0)
    );

    for (
        std::size_t offset = 0;
        offset < input.size();
        offset += BlockSize
    ) {
        const auto block =
            std::span<const std::byte>(
                input.data() + offset,
                BlockSize
            );

        const auto mixed =
            xorBlock(
                block,
                chaining
            );

        chaining =
            crypto::Aes256Block::encrypt(
                key,
                mixed
            );
    }

    /*
     * The first output block is R.
     */
    Bytes output;

    output.reserve(outputLength);

    const std::size_t firstTake =
        std::min(
            outputLength,
            chaining.size()
        );

    output.insert(
        output.end(),
        chaining.begin(),
        chaining.begin() +
            static_cast<std::ptrdiff_t>(firstTake)
    );

    /*
     * Extend output if requested.
     */
    std::uint32_t counter = 1U;

    while (output.size() < outputLength) {
        crypto::ByteVector counterBlock(
            BlockSize,
            static_cast<crypto::Byte>(0)
        );

        counterBlock[12] =
            static_cast<crypto::Byte>(
                (counter >> 24U) & 0xffU
            );

        counterBlock[13] =
            static_cast<crypto::Byte>(
                (counter >> 16U) & 0xffU
            );

        counterBlock[14] =
            static_cast<crypto::Byte>(
                (counter >> 8U) & 0xffU
            );

        counterBlock[15] =
            static_cast<crypto::Byte>(
                counter & 0xffU
            );

        const auto mixed =
            xorCryptoBlocks(
                chaining,
                counterBlock
            );

        const auto encrypted =
            crypto::Aes256Block::encrypt(
                key,
                mixed
            );

        const std::size_t remaining =
            outputLength - output.size();

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