#pragma once

#include "indiccrypt/text/script.hpp"
#include "indiccrypt/text/symbol.hpp"

#include <cstddef>
#include <span>
#include <string>
#include <vector>

namespace indiccrypt::fpe {

class FpeAlphabet {
public:
    using Index = std::size_t;

    FpeAlphabet(
        text::Script script,
        std::vector<text::IndicSymbol> symbols,
        std::string version
    );

    [[nodiscard]]
    text::Script script() const noexcept;

    [[nodiscard]]
    std::string_view version() const noexcept;

    [[nodiscard]]
    std::size_t radix() const noexcept;

    [[nodiscard]]
    std::span<const text::IndicSymbol> symbols() const noexcept;

    [[nodiscard]]
    Index indexOf(
        const text::IndicSymbol& symbol
    ) const;

    [[nodiscard]]
    const text::IndicSymbol& symbolAt(
        Index index
    ) const;

private:
    text::Script script_;
    std::vector<text::IndicSymbol> symbols_;
    std::string version_;
};

} // namespace indiccrypt::fpe