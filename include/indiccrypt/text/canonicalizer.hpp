#pragma once

#include "indiccrypt/text/symbol.hpp"

#include <string_view>
#include <vector>

namespace indiccrypt::text {

class Canonicalizer {
public:
    [[nodiscard]]
    static std::vector<IndicSymbol> canonicalize(
        std::string_view utf8,
        Language language
    );
};

} // namespace indiccrypt::text