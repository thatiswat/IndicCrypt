#pragma once

#include "indiccrypt/fpe/parameters.hpp"

#include <span>
#include <vector>

namespace indiccrypt::fpe {

class Ff1 {
public:
    explicit Ff1(
        const FpeParameters& parameters
    ) noexcept;

    [[nodiscard]]
    std::vector<FpeRadix::Value> encrypt(
        std::span<const FpeRadix::Value> plaintext
    ) const;

    [[nodiscard]]
    std::vector<FpeRadix::Value> decrypt(
        std::span<const FpeRadix::Value> ciphertext
    ) const;

private:
    const FpeParameters* parameters_;

    void validate(
        std::span<const FpeRadix::Value> input
    ) const;
};

} // namespace indiccrypt::fpe