#pragma once

#include "indiccrypt/text/script.hpp"

#include <cstdint>
#include <string_view>
#include <vector>

namespace indiccrypt::text {

enum class Language : std::uint8_t {
    Assamese,
    Bengali,
    Bodo,
    Dogri,
    Gujarati,
    Hindi,
    Kannada,
    Kashmiri,
    Konkani,
    Maithili,
    Malayalam,
    Manipuri,
    Marathi,
    Nepali,
    Odia,
    Punjabi,
    Sanskrit,
    Santali,
    Sindhi,
    Tamil,
    Telugu,
    Urdu
};

struct LanguageProfile {
    Language language;
    std::string_view name;
    std::string_view iso639_1;
    std::string_view iso639_3;
    Script primaryScript;
};

class LanguageRegistry {
public:
    [[nodiscard]]
    static const LanguageProfile& get(Language language);

    [[nodiscard]]
    static const std::vector<LanguageProfile>& all();

    [[nodiscard]]
    static std::string_view name(Language language) noexcept;
};

} // namespace indiccrypt::text