#include "indiccrypt/fpe/prf.hpp"

#include "indiccrypt/crypto/aes_block.hpp"

#include <algorithm>
#include <cstddef>
#include <stdexcept>

namespace indiccrypt::fpe {

Ff1Prf::Bytes Ff1Prf::evaluate(
    const crypto::Key& key,
    std::span<const std::byte> input,
    std::size_t outputLength
) {
    if (outputLength == 0) {
        return {};
    }

    if (key.size() != crypto::Aes256Block::KeySize) {
        throw std::invalid_argument(
            "FF1 PRF requires a 32-byte AES-256 key"
        );
    }

    crypto::ByteVector block(
        BlockSize,
        static_cast<crypto::Byte>(0)
    );

    const std::size_t firstLength =
        std::min(input.size(), BlockSize);

    /*
     * Explicit std::byte -> crypto::Byte conversion.
     */
    for (std::size_t i = 0; i < firstLength; ++i) {
        block[i] =
            std::to_integer<crypto::Byte>(
                input[i]
            );
    }

    crypto::ByteVector result;
    result.reserve(outputLength);

    while (result.size() < outputLength) {
        const crypto::ByteVector encrypted =
            crypto::Aes256Block::encrypt(
                key,
                block
            );

        const std::size_t remaining =
            outputLength - result.size();

        const std::size_t take =
            std::min(
                remaining,
                encrypted.size()
            );

        result.insert(
            result.end(),
            encrypted.begin(),
            encrypted.begin() +
                static_cast<std::ptrdiff_t>(take)
        );

        if (result.size() >= outputLength) {
            break;
        }

        block = encrypted;

        /*
         * Mix additional input bytes into the
         * chaining block.
         */
        for (
            std::size_t i = BlockSize;
            i < input.size();
            ++i
        ) {
            const crypto::Byte inputByte =
                std::to_integer<crypto::Byte>(
                    input[i]
                );

            block[
                (i - BlockSize) % BlockSize
            ] ^= inputByte;
        }
    }

    result.resize(outputLength);

    return result;
}

} // namespace indiccrypt::fpe