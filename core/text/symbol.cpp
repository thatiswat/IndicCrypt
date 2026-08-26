#include "indiccrypt/text/symbol.hpp"

#include <utility>

namespace indiccrypt::text {

IndicSymbol::IndicSymbol(
    SymbolId id,
    std::string grapheme,
    Script script,
    Language language
)
    : id_(id),
      grapheme_(std::move(grapheme)),
      script_(script),
      language_(language) {}

SymbolId IndicSymbol::id() const noexcept {
    return id_;
}

const std::string& IndicSymbol::grapheme() const noexcept {
    return grapheme_;
}

Script IndicSymbol::script() const noexcept {
    return script_;
}

Language IndicSymbol::language() const noexcept {
    return language_;
}

bool IndicSymbol::operator==(
    const IndicSymbol& other
) const noexcept {
    return id_ == other.id_
        && grapheme_ == other.grapheme_
        && script_ == other.script_
        && language_ == other.language_;
}

} // namespace indiccrypt::text