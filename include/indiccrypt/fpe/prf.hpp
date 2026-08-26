#pragma once

#include "indiccrypt/crypto/types.hpp"

#include <cstddef>
#include <span>

namespace indiccrypt::fpe {

class Ff1Prf {
public:
    static constexpr std::size_t BlockSize = 16;

    using Bytes = crypto::ByteVector;

    [[nodiscard]]
    static Bytes evaluate(
        const crypto::Key& key,
        std::span<const std::byte> input,
        std::size_t outputLength
    );
};

} // namespace indiccrypt::fpe