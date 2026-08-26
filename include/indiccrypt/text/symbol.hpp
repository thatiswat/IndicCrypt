#pragma once

#include "indiccrypt/text/language.hpp"

#include <cstdint>
#include <string>

namespace indiccrypt::text {

using SymbolId = std::uint64_t;

class IndicSymbol {
public:
    IndicSymbol(
        SymbolId id,
        std::string grapheme,
        Script script,
        Language language
    );

    [[nodiscard]]
    SymbolId id() const noexcept;

    [[nodiscard]]
    const std::string& grapheme() const noexcept;

    [[nodiscard]]
    Script script() const noexcept;

    [[nodiscard]]
    Language language() const noexcept;

    [[nodiscard]]
    bool operator==(const IndicSymbol& other) const noexcept;

private:
    SymbolId id_;
    std::string grapheme_;
    Script script_;
    Language language_;
};

} // namespace indiccrypt::text