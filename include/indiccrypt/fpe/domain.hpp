#pragma once

#include "indiccrypt/fpe/alphabet.hpp"
#include "indiccrypt/fpe/codec.hpp"
#include "indiccrypt/fpe/radix.hpp"

#include <cstddef>
#include <span>
#include <vector>

namespace indiccrypt::fpe {

class FpeDomain {
public:
    explicit FpeDomain(
        const FpeAlphabet& alphabet
    );

    [[nodiscard]]
    const FpeAlphabet& alphabet() const noexcept;

    [[nodiscard]]
    const FpeCodec& codec() const noexcept;

    [[nodiscard]]
    const FpeRadix& radix() const noexcept;

    [[nodiscard]]
    std::vector<FpeRadix::Value> encode(
        std::span<const text::IndicSymbol> symbols
    ) const;

    [[nodiscard]]
    std::vector<text::IndicSymbol> decode(
        std::span<const FpeRadix::Value> digits
    ) const;

private:
    const FpeAlphabet* alphabet_;
    FpeCodec codec_;
    FpeRadix radix_;
};

} // namespace indiccrypt::fpe