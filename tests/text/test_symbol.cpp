#include "indiccrypt/text/symbol.hpp"

#include <cassert>

int main() {
    using indiccrypt::text::IndicSymbol;
    using indiccrypt::text::Language;
    using indiccrypt::text::Script;

    const IndicSymbol symbol(
        0x0000000000000042ULL,
        "न",
        Script::Devanagari,
        Language::Hindi
    );

    assert(symbol.id() == 0x42ULL);
    assert(symbol.grapheme() == "न");
    assert(symbol.script() == Script::Devanagari);
    assert(symbol.language() == Language::Hindi);

    return 0;
}