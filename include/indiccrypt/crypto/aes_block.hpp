#pragma once

#include "indiccrypt/crypto/types.hpp"

#include <cstddef>
#include <span>

namespace indiccrypt::crypto {

class AesBlock {
public:
    static constexpr std::size_t Aes128KeySize = 16;
    static constexpr std::size_t Aes192KeySize = 24;
    static constexpr std::size_t Aes256KeySize = 32;

    static constexpr std::size_t BlockSize = 16;

    [[nodiscard]]
    static ByteVector encrypt(
        const Key& key,
        std::span<const Byte> block
    );
};

} // namespace indiccrypt::crypto