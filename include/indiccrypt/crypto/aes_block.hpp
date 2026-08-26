#pragma once

#include "indiccrypt/crypto/types.hpp"

#include <cstddef>
#include <span>

namespace indiccrypt::crypto {

class Aes256Block {
public:
    static constexpr std::size_t KeySize = 32;
    static constexpr std::size_t BlockSize = 16;

    [[nodiscard]]
    static ByteVector encrypt(
        const Key& key,
        std::span<const Byte> block
    );
};

} // namespace indiccrypt::crypto