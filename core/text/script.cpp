#include "indiccrypt/text/script.hpp"

#include <unicode/uscript.h>
#include <unicode/unistr.h>

#include <stdexcept>
#include <utility>

namespace indiccrypt::text {

namespace {

Script mapScript(UScriptCode script) noexcept {
    switch (script) {
        case USCRIPT_DEVANAGARI:
            return Script::Devanagari;

        case USCRIPT_BENGALI:
            return Script::Bengali;

        case USCRIPT_GURMUKHI:
            return Script::Gurmukhi;

        case USCRIPT_GUJARATI:
            return Script::Gujarati;

        case USCRIPT_ORIYA:
            return Script::Odia;

        case USCRIPT_TAMIL:
            return Script::Tamil;

        case USCRIPT_TELUGU:
            return Script::Telugu;

        case USCRIPT_KANNADA:
            return Script::Kannada;

        case USCRIPT_MALAYALAM:
            return Script::Malayalam;

        case USCRIPT_MEITEI_MAYEK:
            return Script::Meetei;

        case USCRIPT_OL_CHIKI:
            return Script::OlChiki;

        case USCRIPT_ARABIC:
            return Script::Arabic;

        case USCRIPT_LATIN:
            return Script::Latin;

        default:
            return Script::Other;
    }
}

} // namespace

Script ScriptEngine::detect(std::string_view utf8) {
    icu::UnicodeString text =
        icu::UnicodeString::fromUTF8(
            icu::StringPiece(
                utf8.data(),
                static_cast<int32_t>(utf8.size())
            )
        );

    if (text.isEmpty()) {
        return Script::Unknown;
    }

    const UChar32 codePoint = text.char32At(0);

    if (codePoint < 0) {
        throw std::runtime_error(
            "Invalid Unicode code point"
        );
    }

    UErrorCode status = U_ZERO_ERROR;

    const UScriptCode script =
        uscript_getScript(codePoint, &status);

    if (U_FAILURE(status)) {
        throw std::runtime_error(
            "ICU script detection failed"
        );
    }

    return mapScript(script);
}

std::vector<ScriptDetection>
ScriptEngine::detectAll(std::string_view utf8) {
    std::vector<ScriptDetection> result;

    icu::UnicodeString text =
        icu::UnicodeString::fromUTF8(
            icu::StringPiece(
                utf8.data(),
                static_cast<int32_t>(utf8.size())
            )
        );

    for (int32_t offset = 0;
         offset < text.length();) {

        const UChar32 codePoint =
            text.char32At(offset);

        if (codePoint < 0) {
            throw std::runtime_error(
                "Invalid Unicode code point"
            );
        }

        UErrorCode status = U_ZERO_ERROR;

        const UScriptCode script =
            uscript_getScript(codePoint, &status);

        if (U_FAILURE(status)) {
            throw std::runtime_error(
                "ICU script detection failed"
            );
        }

        result.push_back({
            mapScript(script),
            std::string{}
        });

        offset += U16_LENGTH(codePoint);
    }

    return result;
}

std::string_view scriptName(
    Script script
) noexcept {
    switch (script) {
        case Script::Devanagari:
            return "Devanagari";

        case Script::Bengali:
            return "Bengali";

        case Script::Gurmukhi:
            return "Gurmukhi";

        case Script::Gujarati:
            return "Gujarati";

        case Script::Odia:
            return "Odia";

        case Script::Tamil:
            return "Tamil";

        case Script::Telugu:
            return "Telugu";

        case Script::Kannada:
            return "Kannada";

        case Script::Malayalam:
            return "Malayalam";

        case Script::Meetei:
            return "Meetei Mayek";

        case Script::OlChiki:
            return "Ol Chiki";

        case Script::Arabic:
            return "Arabic";

        case Script::Latin:
            return "Latin";

        case Script::Other:
            return "Other";

        case Script::Unknown:
            return "Unknown";
    }

    return "Unknown";
}

} // namespace indiccrypt::text