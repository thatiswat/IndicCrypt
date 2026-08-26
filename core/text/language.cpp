#include "indiccrypt/text/language.hpp"

#include <stdexcept>

namespace indiccrypt::text {

const std::vector<LanguageProfile>&
LanguageRegistry::all() {
    static const std::vector<LanguageProfile> profiles = {
        {
            Language::Assamese,
            "Assamese",
            "as",
            "asm",
            Script::Bengali
        },
        {
            Language::Bengali,
            "Bengali",
            "bn",
            "ben",
            Script::Bengali
        },
        {
            Language::Bodo,
            "Bodo",
            "brx",
            "brx",
            Script::Devanagari
        },
        {
            Language::Dogri,
            "Dogri",
            "doi",
            "doi",
            Script::Devanagari
        },
        {
            Language::Gujarati,
            "Gujarati",
            "gu",
            "guj",
            Script::Gujarati
        },
        {
            Language::Hindi,
            "Hindi",
            "hi",
            "hin",
            Script::Devanagari
        },
        {
            Language::Kannada,
            "Kannada",
            "kn",
            "kan",
            Script::Kannada
        },
        {
            Language::Kashmiri,
            "Kashmiri",
            "ks",
            "kas",
            Script::Arabic
        },
        {
            Language::Konkani,
            "Konkani",
            "kok",
            "kok",
            Script::Devanagari
        },
        {
            Language::Maithili,
            "Maithili",
            "mai",
            "mai",
            Script::Devanagari
        },
        {
            Language::Malayalam,
            "Malayalam",
            "ml",
            "mal",
            Script::Malayalam
        },
        {
            Language::Manipuri,
            "Manipuri",
            "mni",
            "mni",
            Script::Meetei
        },
        {
            Language::Marathi,
            "Marathi",
            "mr",
            "mar",
            Script::Devanagari
        },
        {
            Language::Nepali,
            "Nepali",
            "ne",
            "nep",
            Script::Devanagari
        },
        {
            Language::Odia,
            "Odia",
            "or",
            "ori",
            Script::Odia
        },
        {
            Language::Punjabi,
            "Punjabi",
            "pa",
            "pan",
            Script::Gurmukhi
        },
        {
            Language::Sanskrit,
            "Sanskrit",
            "sa",
            "san",
            Script::Devanagari
        },
        {
            Language::Santali,
            "Santali",
            "sat",
            "sat",
            Script::OlChiki
        },
        {
            Language::Sindhi,
            "Sindhi",
            "sd",
            "snd",
            Script::Arabic
        },
        {
            Language::Tamil,
            "Tamil",
            "ta",
            "tam",
            Script::Tamil
        },
        {
            Language::Telugu,
            "Telugu",
            "te",
            "tel",
            Script::Telugu
        },
        {
            Language::Urdu,
            "Urdu",
            "ur",
            "urd",
            Script::Arabic
        }
    };

    return profiles;
}

const LanguageProfile&
LanguageRegistry::get(Language language) {
    for (const auto& profile : all()) {
        if (profile.language == language) {
            return profile;
        }
    }

    throw std::out_of_range("Unknown IndicCrypt language");
}

std::string_view
LanguageRegistry::name(Language language) noexcept {
    for (const auto& profile : all()) {
        if (profile.language == language) {
            return profile.name;
        }
    }

    return "Unknown";
}

} // namespace indiccrypt::text