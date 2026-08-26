#pragma once

#include "indiccrypt/crypto/types.hpp"

#include <cstddef>
#include <span>

namespace indiccrypt::fpe {

class Ff1YGenerator {
public:
    static constexpr std::size_t BlockSize = 16;

    using Bytes = crypto::ByteVector;

    [[nodiscard]]
    static Bytes generate(
        const crypto::Key& key,
        std::span<const crypto::Byte> r,
        std::size_t length
    );
};

} // namespace indiccrypt::fpe