#include "indiccrypt/text/indic_text.hpp"

#include <unicode/normalizer2.h>
#include <unicode/unistr.h>

#include <stdexcept>

namespace indiccrypt::text {

namespace {

std::string normalizeNfc(std::string_view input) {
    UErrorCode status = U_ZERO_ERROR;

    const icu::Normalizer2* normalizer =
        icu::Normalizer2::getNFCInstance(status);

    if (U_FAILURE(status) || normalizer == nullptr) {
        throw std::runtime_error("Failed to initialize ICU NFC normalizer");
    }

    icu::UnicodeString source =
        icu::UnicodeString::fromUTF8(
            icu::StringPiece(input.data(),
                             static_cast<int32_t>(input.size()))
        );

    icu::UnicodeString normalized;

    normalizer->normalize(source, normalized, status);

    if (U_FAILURE(status)) {
        throw std::runtime_error("ICU NFC normalization failed");
    }

    std::string result;
    normalized.toUTF8String(result);

    return result;
}

std::vector<CodePoint> decodeCodePoints(
    const std::string& utf8
) {
    icu::UnicodeString unicode =
        icu::UnicodeString::fromUTF8(
            icu::StringPiece(
                utf8.data(),
                static_cast<int32_t>(utf8.size())
            )
        );

    std::vector<CodePoint> result;

    for (int32_t offset = 0;
         offset < unicode.length();) {

        UChar32 codePoint = unicode.char32At(offset);

        if (codePoint < 0) {
            throw std::runtime_error(
                "Invalid Unicode code point"
            );
        }

        result.push_back(
            static_cast<CodePoint>(codePoint)
        );

        offset += U16_LENGTH(codePoint);
    }

    return result;
}

} // namespace

IndicText IndicText::fromUtf8(
    std::string_view input
) {
    std::string normalized =
        normalizeNfc(input);

    auto codePoints =
        decodeCodePoints(normalized);

    return IndicText(
        std::move(normalized),
        std::move(codePoints)
    );
}

IndicText::IndicText(
    std::string normalizedUtf8,
    std::vector<CodePoint> codePoints
)
    : normalizedUtf8_(std::move(normalizedUtf8)),
      codePoints_(std::move(codePoints)) {}

const std::string& IndicText::utf8() const noexcept {
    return normalizedUtf8_;
}

const std::vector<CodePoint>&
IndicText::codePoints() const noexcept {
    return codePoints_;
}

} // namespace indiccrypt::text