#pragma once

#include "indiccrypt/fpe/alphabet.hpp"

#include <cstddef>
#include <span>
#include <vector>

namespace indiccrypt::fpe {

class FpeCodec {
public:
    using Index = FpeAlphabet::Index;

    explicit FpeCodec(
        const FpeAlphabet& alphabet
    ) noexcept;

    [[nodiscard]]
    std::vector<Index> encode(
        std::span<const text::IndicSymbol> symbols
    ) const;

    [[nodiscard]]
    std::vector<text::IndicSymbol> decode(
        std::span<const Index> indices
    ) const;

    [[nodiscard]]
    Index encodeOne(
        const text::IndicSymbol& symbol
    ) const;

    [[nodiscard]]
    const text::IndicSymbol& decodeOne(
        Index index
    ) const;

private:
    const FpeAlphabet* alphabet_;
};

} // namespace indiccrypt::fpe