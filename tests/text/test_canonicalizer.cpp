#include "indiccrypt/text/canonicalizer.hpp"

#include <cassert>

int main() {
    using indiccrypt::text::Canonicalizer;
    using indiccrypt::text::Language;
    using indiccrypt::text::Script;

    const auto hindi =
        Canonicalizer::canonicalize(
            "नमस्ते",
            Language::Hindi
        );

    assert(!hindi.empty());

    for (const auto& symbol : hindi) {
        assert(symbol.language() == Language::Hindi);
        assert(symbol.script() == Script::Devanagari);
        assert(!symbol.grapheme().empty());
        assert(symbol.id() != 0);
    }

    const auto tamil =
        Canonicalizer::canonicalize(
            "தமிழ்",
            Language::Tamil
        );

    assert(!tamil.empty());

    for (const auto& symbol : tamil) {
        assert(symbol.language() == Language::Tamil);
        assert(symbol.script() == Script::Tamil);
        assert(!symbol.grapheme().empty());
        assert(symbol.id() != 0);
    }

    return 0;
}