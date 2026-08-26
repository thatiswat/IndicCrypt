#pragma once

#include "indiccrypt/crypto/types.hpp"
#include "indiccrypt/fpe/domain.hpp"

#include <vector>
#include <cstddef>
#include <span>

namespace indiccrypt::fpe {

class FpeParameters {
public:
    using Length = std::size_t;

    FpeParameters(
        const FpeDomain& domain,
        const crypto::Key& key,
        std::span<const std::byte> tweak
    );

    [[nodiscard]]
    const FpeDomain& domain() const noexcept;

    [[nodiscard]]
    const crypto::Key& key() const noexcept;

    [[nodiscard]]
    std::span<const std::byte> tweak() const noexcept;

    [[nodiscard]]
    Length minimumLength() const noexcept;

    [[nodiscard]]
    Length maximumLength() const noexcept;

    void validateLength(Length length) const;

private:
    const FpeDomain* domain_;
    const crypto::Key* key_;
    std::vector<std::byte> tweak_;
};

} // namespace indiccrypt::fpe