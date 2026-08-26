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
    EVP_CIPHER_CTX* raw =
        EVP_CIPHER_CTX_new();

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

const EVP_CIPHER* selectCipher(
    std::size_t keySize
) {
    switch (keySize) {
        case AesBlock::Aes128KeySize:
            return EVP_aes_128_ecb();

        case AesBlock::Aes192KeySize:
            return EVP_aes_192_ecb();

        case AesBlock::Aes256KeySize:
            return EVP_aes_256_ecb();

        default:
            throw std::invalid_argument(
                "AES requires a 16, 24, or 32-byte key"
            );
    }
}

} // namespace

ByteVector AesBlock::encrypt(
    const Key& key,
    std::span<const Byte> block
) {
    if (
        key.size() != AesBlock::Aes128KeySize &&
        key.size() != AesBlock::Aes192KeySize &&
        key.size() != AesBlock::Aes256KeySize
    ) {
        throw std::invalid_argument(
            "AES requires a 16, 24, or 32-byte key"
        );
    }

    if (block.size() != AesBlock::BlockSize) {
        throw std::invalid_argument(
            "AES block encryption requires exactly 16 bytes"
        );
    }

    auto context = makeContext();

    const EVP_CIPHER* cipher =
        selectCipher(key.size());

    if (EVP_EncryptInit_ex(
            context.get(),
            cipher,
            nullptr,
            key.bytes().data(),
            nullptr
        ) != 1) {
        throw std::runtime_error(
            "AES block initialization failed"
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

    ByteVector output(
        AesBlock::BlockSize
    );

    int outputLength = 0;

    if (EVP_EncryptUpdate(
            context.get(),
            output.data(),
            &outputLength,
            block.data(),
            static_cast<int>(block.size())
        ) != 1) {
        throw std::runtime_error(
            "AES block encryption failed"
        );
    }

    int finalLength = 0;

    if (EVP_EncryptFinal_ex(
            context.get(),
            output.data() + outputLength,
            &finalLength
        ) != 1) {
        throw std::runtime_error(
            "AES block finalization failed"
        );
    }

    output.resize(
        static_cast<std::size_t>(
            outputLength + finalLength
        )
    );

    if (output.size() != AesBlock::BlockSize) {
        throw std::runtime_error(
            "AES produced an invalid block size"
        );
    }

    return output;
}

} // namespace indiccrypt::crypto