#pragma once

#include "indiccrypt/crypto/types.hpp"

#include <cstddef>
#include <span>
#include <string_view>

namespace indiccrypt::crypto {

enum class KeyPurpose {
    Encryption,
    Fpe,
    Search,
    Integrity
};

class KeyDerivation {
public:
    [[nodiscard]]
    static Key derive(
        const Key& rootKey,
        KeyPurpose purpose,
        std::size_t outputLength
    );

    [[nodiscard]]
    static Key hkdfSha256(
        std::span<const Byte> ikm,
        std::span<const Byte> salt,
        std::span<const Byte> info,
        std::size_t outputLength
    );

private:
    [[nodiscard]]
    static std::string_view purposeLabel(
        KeyPurpose purpose
    ) noexcept;
};

} // namespace indiccrypt::crypto