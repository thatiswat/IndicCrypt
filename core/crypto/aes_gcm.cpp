#include "indiccrypt/crypto/aes_gcm.hpp"

#include <openssl/evp.h>

#include <memory>
#include <stdexcept>
#include <vector>

namespace indiccrypt::crypto {

namespace {

using EvpContextPtr =
    std::unique_ptr<EVP_CIPHER_CTX, decltype(&EVP_CIPHER_CTX_free)>;

EvpContextPtr makeContext() {
    EVP_CIPHER_CTX* raw = EVP_CIPHER_CTX_new();

    if (raw == nullptr) {
        throw std::runtime_error(
            "Failed to allocate AES-GCM context"
        );
    }

    return {
        raw,
        &EVP_CIPHER_CTX_free
    };
}

void validateKeyAndNonce(
    const Key& key,
    const Nonce& nonce
) {
    if (key.size() != AesGcm::KeySize) {
        throw std::invalid_argument(
            "AES-256-GCM requires a 32-byte key"
        );
    }

    if (nonce.size() != AesGcm::NonceSize) {
        throw std::invalid_argument(
            "AES-GCM requires a 12-byte nonce"
        );
    }
}

} // namespace

AesGcmCiphertext AesGcm::encrypt(
    const Key& key,
    const Nonce& nonce,
    std::span<const Byte> plaintext,
    std::span<const Byte> aad
) {
    validateKeyAndNonce(key, nonce);

    auto context = makeContext();

    if (EVP_EncryptInit_ex(
            context.get(),
            EVP_aes_256_gcm(),
            nullptr,
            nullptr,
            nullptr
        ) != 1) {
        throw std::runtime_error(
            "AES-GCM initialization failed"
        );
    }

    if (EVP_CIPHER_CTX_ctrl(
            context.get(),
            EVP_CTRL_GCM_SET_IVLEN,
            static_cast<int>(nonce.size()),
            nullptr
        ) != 1) {
        throw std::runtime_error(
            "Failed to configure GCM nonce length"
        );
    }

    if (EVP_EncryptInit_ex(
            context.get(),
            nullptr,
            nullptr,
            key.bytes().data(),
            nonce.bytes().data()
        ) != 1) {
        throw std::runtime_error(
            "AES-GCM key/nonce initialization failed"
        );
    }

    int outputLength = 0;

    if (!aad.empty()) {
        if (EVP_EncryptUpdate(
                context.get(),
                nullptr,
                &outputLength,
                aad.data(),
                static_cast<int>(aad.size())
            ) != 1) {
            throw std::runtime_error(
                "AES-GCM AAD processing failed"
            );
        }
    }

    ByteVector ciphertext(plaintext.size());
    int ciphertextLength = 0;

    if (!plaintext.empty()) {
        if (EVP_EncryptUpdate(
                context.get(),
                ciphertext.data(),
                &ciphertextLength,
                plaintext.data(),
                static_cast<int>(plaintext.size())
            ) != 1) {
            throw std::runtime_error(
                "AES-GCM encryption failed"
            );
        }
    }

    int finalLength = 0;

    if (EVP_EncryptFinal_ex(
            context.get(),
            ciphertext.data() + ciphertextLength,
            &finalLength
        ) != 1) {
        throw std::runtime_error(
            "AES-GCM finalization failed"
        );
    }

    ciphertext.resize(
        static_cast<std::size_t>(
            ciphertextLength + finalLength
        )
    );

    ByteVector tag(AesGcm::TagSize);

    if (EVP_CIPHER_CTX_ctrl(
            context.get(),
            EVP_CTRL_GCM_GET_TAG,
            static_cast<int>(tag.size()),
            tag.data()
        ) != 1) {
        throw std::runtime_error(
            "Failed to obtain AES-GCM authentication tag"
        );
    }

    return {
        Ciphertext(std::move(ciphertext)),
        AuthenticationTag(std::move(tag))
    };
}

ByteVector AesGcm::decrypt(
    const Key& key,
    const Nonce& nonce,
    const Ciphertext& ciphertext,
    const AuthenticationTag& tag,
    std::span<const Byte> aad
) {
    validateKeyAndNonce(key, nonce);

    if (tag.size() != AesGcm::TagSize) {
        throw std::invalid_argument(
            "AES-GCM requires a 16-byte authentication tag"
        );
    }

    auto context = makeContext();

    if (EVP_DecryptInit_ex(
            context.get(),
            EVP_aes_256_gcm(),
            nullptr,
            nullptr,
            nullptr
        ) != 1) {
        throw std::runtime_error(
            "AES-GCM initialization failed"
        );
    }

    if (EVP_CIPHER_CTX_ctrl(
            context.get(),
            EVP_CTRL_GCM_SET_IVLEN,
            static_cast<int>(nonce.size()),
            nullptr
        ) != 1) {
        throw std::runtime_error(
            "Failed to configure GCM nonce length"
        );
    }

    if (EVP_DecryptInit_ex(
            context.get(),
            nullptr,
            nullptr,
            key.bytes().data(),
            nonce.bytes().data()
        ) != 1) {
        throw std::runtime_error(
            "AES-GCM key/nonce initialization failed"
        );
    }

    int outputLength = 0;

    if (!aad.empty()) {
        if (EVP_DecryptUpdate(
                context.get(),
                nullptr,
                &outputLength,
                aad.data(),
                static_cast<int>(aad.size())
            ) != 1) {
            throw std::runtime_error(
                "AES-GCM AAD processing failed"
            );
        }
    }

    ByteVector plaintext(ciphertext.size());
    int plaintextLength = 0;

    if (!ciphertext.bytes().empty()) {
        if (EVP_DecryptUpdate(
                context.get(),
                plaintext.data(),
                &plaintextLength,
                ciphertext.bytes().data(),
                static_cast<int>(ciphertext.size())
            ) != 1) {
            throw std::runtime_error(
                "AES-GCM decryption failed"
            );
        }
    }

    if (EVP_CIPHER_CTX_ctrl(
            context.get(),
            EVP_CTRL_GCM_SET_TAG,
            static_cast<int>(tag.size()),
            const_cast<Byte*>(
                tag.bytes().data()
            )
        ) != 1) {
        throw std::runtime_error(
            "Failed to configure authentication tag"
        );
    }

    int finalLength = 0;

    if (EVP_DecryptFinal_ex(
            context.get(),
            plaintext.data() + plaintextLength,
            &finalLength
        ) != 1) {
        throw std::runtime_error(
            "AES-GCM authentication failed"
        );
    }

    plaintext.resize(
        static_cast<std::size_t>(
            plaintextLength + finalLength
        )
    );

    return plaintext;
}

} // namespace indiccrypt::crypto