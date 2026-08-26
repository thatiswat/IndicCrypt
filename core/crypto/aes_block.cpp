#include "indiccrypt/crypto/aes_block.hpp"

#include <openssl/evp.h>

#include <memory>
#include <stdexcept>

namespace indiccrypt::crypto {

namespace {

using EvpContextPtr =
    std::unique_ptr<
        EVP_CIPHER_CTX,
        decltype(&EVP_CIPHER_CTX_free)
    >;

EvpContextPtr makeContext() {
    EVP_CIPHER_CTX* raw = EVP_CIPHER_CTX_new();

    if (raw == nullptr) {
        throw std::runtime_error(
            "Failed to allocate AES context"
        );
    }

    return {
        raw,
        &EVP_CIPHER_CTX_free
    };
}

} // namespace

ByteVector Aes256Block::encrypt(
    const Key& key,
    std::span<const Byte> block
) {
    if (key.size() != KeySize) {
        throw std::invalid_argument(
            "AES-256 requires a 32-byte key"
        );
    }

    if (block.size() != BlockSize) {
        throw std::invalid_argument(
            "AES block encryption requires exactly 16 bytes"
        );
    }

    auto context = makeContext();

    if (EVP_EncryptInit_ex(
            context.get(),
            EVP_aes_256_ecb(),
            nullptr,
            key.bytes().data(),
            nullptr
        ) != 1) {
        throw std::runtime_error(
            "AES-256 block initialization failed"
        );
    }

    if (EVP_CIPHER_CTX_set_padding(
            context.get(),
            0
        ) != 1) {
        throw std::runtime_error(
            "Failed to disable AES padding"
        );
    }

    ByteVector output(BlockSize);

    int outputLength = 0;

    if (EVP_EncryptUpdate(
            context.get(),
            output.data(),
            &outputLength,
            block.data(),
            static_cast<int>(block.size())
        ) != 1) {
        throw std::runtime_error(
            "AES-256 block encryption failed"
        );
    }

    int finalLength = 0;

    if (EVP_EncryptFinal_ex(
            context.get(),
            output.data() + outputLength,
            &finalLength
        ) != 1) {
        throw std::runtime_error(
            "AES-256 block finalization failed"
        );
    }

    output.resize(
        static_cast<std::size_t>(
            outputLength + finalLength
        )
    );

    if (output.size() != BlockSize) {
        throw std::runtime_error(
            "AES-256 produced an invalid block size"
        );
    }

    return output;
}

} // namespace indiccrypt::crypto