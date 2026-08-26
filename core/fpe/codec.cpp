#include "indiccrypt/fpe/codec.hpp"

#include <utility>

namespace indiccrypt::fpe {

FpeCodec::FpeCodec(
    const FpeAlphabet& alphabet
) noexcept
    : alphabet_(&alphabet) {}

std::vector<FpeCodec::Index> FpeCodec::encode(
    std::span<const text::IndicSymbol> symbols
) const {
    std::vector<Index> result;
    result.reserve(symbols.size());

    for (const auto& symbol : symbols) {
        result.push_back(
            alphabet_->indexOf(symbol)
        );
    }

    return result;
}

std::vector<text::IndicSymbol> FpeCodec::decode(
    std::span<const Index> indices
) const {
    std::vector<text::IndicSymbol> result;
    result.reserve(indices.size());

    for (const auto index : indices) {
        result.push_back(
            alphabet_->symbolAt(index)
        );
    }

    return result;
}

FpeCodec::Index FpeCodec::encodeOne(
    const text::IndicSymbol& symbol
) const {
    return alphabet_->indexOf(symbol);
}

const text::IndicSymbol& FpeCodec::decodeOne(
    Index index
) const {
    return alphabet_->symbolAt(index);
}

} // namespace indiccrypt::fpe