#include "indiccrypt/fpe/domain.hpp"

#include <utility>

namespace indiccrypt::fpe {

FpeDomain::FpeDomain(
    const FpeAlphabet& alphabet
)
    : alphabet_(&alphabet),
      codec_(alphabet),
      radix_(alphabet.radix()) {}

const FpeAlphabet& FpeDomain::alphabet() const noexcept {
    return *alphabet_;
}

const FpeCodec& FpeDomain::codec() const noexcept {
    return codec_;
}

const FpeRadix& FpeDomain::radix() const noexcept {
    return radix_;
}

std::vector<FpeRadix::Value> FpeDomain::encode(
    std::span<const text::IndicSymbol> symbols
) const {
    const auto encoded = codec_.encode(symbols);

    radix_.validate(encoded);

    return encoded;
}

std::vector<text::IndicSymbol> FpeDomain::decode(
    std::span<const FpeRadix::Value> digits
) const {
    radix_.validate(digits);

    return codec_.decode(digits);
}

} // namespace indiccrypt::fpe