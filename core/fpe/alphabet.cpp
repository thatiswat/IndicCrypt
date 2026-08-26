#include "indiccrypt/fpe/alphabet.hpp"

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace indiccrypt::fpe {

FpeAlphabet::FpeAlphabet(
    text::Script script,
    std::vector<text::IndicSymbol> symbols,
    std::string version
)
    : script_(script),
      symbols_(std::move(symbols)),
      version_(std::move(version)) {

    if (symbols_.empty()) {
        throw std::invalid_argument(
            "FPE alphabet cannot be empty"
        );
    }

    if (version_.empty()) {
        throw std::invalid_argument(
            "FPE alphabet version cannot be empty"
        );
    }

    for (std::size_t i = 0; i < symbols_.size(); ++i) {
        for (std::size_t j = i + 1; j < symbols_.size(); ++j) {
            if (symbols_[i] == symbols_[j]) {
                throw std::invalid_argument(
                    "FPE alphabet contains duplicate symbols"
                );
            }
        }
    }
}

text::Script FpeAlphabet::script() const noexcept {
    return script_;
}

std::string_view FpeAlphabet::version() const noexcept {
    return version_;
}

std::size_t FpeAlphabet::radix() const noexcept {
    return symbols_.size();
}

std::span<const text::IndicSymbol>
FpeAlphabet::symbols() const noexcept {
    return symbols_;
}

FpeAlphabet::Index FpeAlphabet::indexOf(
    const text::IndicSymbol& symbol
) const {
    const auto it = std::find(
        symbols_.begin(),
        symbols_.end(),
        symbol
    );

    if (it == symbols_.end()) {
        throw std::out_of_range(
            "Symbol is not present in FPE alphabet"
        );
    }

    return static_cast<Index>(
        std::distance(symbols_.begin(), it)
    );
}

const text::IndicSymbol& FpeAlphabet::symbolAt(
    Index index
) const {
    if (index >= symbols_.size()) {
        throw std::out_of_range(
            "FPE alphabet index is out of range"
        );
    }

    return symbols_[index];
}

} // namespace indiccrypt::fpe