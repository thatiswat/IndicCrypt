#pragma once

#include "indiccrypt/crypto/types.hpp"

#include <span>

namespace indiccrypt::crypto {

struct AesGcmCiphertext {
    Ciphertext ciphertext;
    AuthenticationTag tag;
};

class AesGcm {
public:
    static constexpr std::size_t KeySize = 32;
    static constexpr std::size_t NonceSize = 12;
    static constexpr std::size_t TagSize = 16;

    [[nodiscard]]
    static AesGcmCiphertext encrypt(
        const Key& key,
        const Nonce& nonce,
        std::span<const Byte> plaintext,
        std::span<const Byte> aad = {}
    );

    [[nodiscard]]
    static ByteVector decrypt(
        const Key& key,
        const Nonce& nonce,
        const Ciphertext& ciphertext,
        const AuthenticationTag& tag,
        std::span<const Byte> aad = {}
    );
};

} // namespace indiccrypt::crypto