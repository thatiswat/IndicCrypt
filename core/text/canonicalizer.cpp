#include "indiccrypt/text/canonicalizer.hpp"

#include "indiccrypt/text/grapheme.hpp"
#include "indiccrypt/text/script.hpp"

#include <unicode/unistr.h>

#include <stdexcept>
#include <utility>

namespace indiccrypt::text {

namespace {

SymbolId makeSymbolId(
    std::string_view grapheme,
    Script script,
    Language language
) noexcept {
    // Temporary deterministic identifier.
    // Cryptographic identity MUST NOT depend on this hash.
    std::uint64_t hash = 14695981039346656037ULL;

    for (const unsigned char byte : grapheme) {
        hash ^= byte;
        hash *= 1099511628211ULL;
    }

    hash ^= static_cast<std::uint64_t>(script);
    hash *= 1099511628211ULL;

    hash ^= static_cast<std::uint64_t>(language);
    hash *= 1099511628211ULL;

    return hash;
}

} // namespace

std::vector<IndicSymbol>
Canonicalizer::canonicalize(
    std::string_view utf8,
    Language language
) {
    const auto clusters =
        GraphemeEngine::segment(utf8);

    std::vector<IndicSymbol> result;
    result.reserve(clusters.size());

    const auto& profile =
        LanguageRegistry::get(language);

    for (const auto& cluster : clusters) {
        const Script script =
            ScriptEngine::detect(cluster.utf8);

        result.emplace_back(
            makeSymbolId(
                cluster.utf8,
                script,
                language
            ),
            cluster.utf8,
            script,
            profile.language
        );
    }

    return result;
}

} // namespace indiccrypt::text