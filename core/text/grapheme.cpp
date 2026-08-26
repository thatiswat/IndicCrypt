#include "indiccrypt/text/grapheme.hpp"

#include <unicode/brkiter.h>
#include <unicode/unistr.h>

#include <stdexcept>
#include <utility>

namespace indiccrypt::text {

std::vector<GraphemeCluster>
GraphemeEngine::segment(std::string_view utf8) {
    UErrorCode status = U_ZERO_ERROR;

    icu::UnicodeString text =
        icu::UnicodeString::fromUTF8(
            icu::StringPiece(
                utf8.data(),
                static_cast<int32_t>(utf8.size())
            )
        );

    std::unique_ptr<icu::BreakIterator> iterator(
        icu::BreakIterator::createCharacterInstance(
            icu::Locale::getDefault(),
            status
        )
    );

    if (U_FAILURE(status) || iterator == nullptr) {
        throw std::runtime_error(
            "Failed to initialize ICU grapheme iterator"
        );
    }

    iterator->setText(text);

    std::vector<GraphemeCluster> clusters;

    int32_t start = iterator->first();

    while (start != icu::BreakIterator::DONE) {
        const int32_t end = iterator->next();

        if (end == icu::BreakIterator::DONE) {
            break;
        }

        icu::UnicodeString cluster =
            text.tempSubStringBetween(start, end);

        std::string clusterUtf8;
        cluster.toUTF8String(clusterUtf8);

        clusters.push_back({
            static_cast<std::size_t>(start),
            static_cast<std::size_t>(end - start),
            std::move(clusterUtf8)
        });

        start = end;
    }

    return clusters;
}

} // namespace indiccrypt::text