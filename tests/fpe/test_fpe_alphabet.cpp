#include "indiccrypt/fpe/alphabet.hpp"

#include <cassert>
#include <stdexcept>
#include <vector>

int main() {
    using namespace indiccrypt::fpe;
    using namespace indiccrypt::text;

    const std::vector<IndicSymbol> symbols = {
        IndicSymbol(
            1,
            "अ",
            Script::Devanagari,
            Language::Hindi
        ),
        IndicSymbol(
            2,
            "आ",
            Script::Devanagari,
            Language::Hindi
        ),
        IndicSymbol(
            3,
            "इ",
            Script::Devanagari,
            Language::Hindi
        )
    };

    FpeAlphabet alphabet(
        Script::Devanagari,
        symbols,
        "indic-devanagari-v1"
    );

    assert(
        alphabet.script() == Script::Devanagari
    );

    assert(
        alphabet.version() == "indic-devanagari-v1"
    );

    assert(alphabet.radix() == 3);

    assert(
        alphabet.symbols().size() == 3
    );

    assert(
        alphabet.indexOf(symbols[0]) == 0
    );

    assert(
        alphabet.indexOf(symbols[1]) == 1
    );

    assert(
        alphabet.indexOf(symbols[2]) == 2
    );

    assert(
        alphabet.symbolAt(0) == symbols[0]
    );

    assert(
        alphabet.symbolAt(1) == symbols[1]
    );

    assert(
        alphabet.symbolAt(2) == symbols[2]
    );

    // Round-trip invariant.
    for (std::size_t i = 0; i < alphabet.radix(); ++i) {
        assert(
            alphabet.indexOf(
                alphabet.symbolAt(i)
            ) == i
        );
    }

    // Empty alphabet must fail.
    bool emptyRejected = false;

    try {
        FpeAlphabet invalid(
            Script::Devanagari,
            {},
            "indic-devanagari-v1"
        );
    } catch (const std::invalid_argument&) {
        emptyRejected = true;
    }

    assert(emptyRejected);

    // Empty version must fail.
    bool versionRejected = false;

    try {
        FpeAlphabet invalid(
            Script::Devanagari,
            symbols,
            ""
        );
    } catch (const std::invalid_argument&) {
        versionRejected = true;
    }

    assert(versionRejected);

    // Duplicate symbol must fail.
    bool duplicateRejected = false;

    try {
        std::vector<IndicSymbol> duplicateSymbols = {
            symbols[0],
            symbols[0]
        };

        FpeAlphabet invalid(
            Script::Devanagari,
            duplicateSymbols,
            "indic-devanagari-v1"
        );
    } catch (const std::invalid_argument&) {
        duplicateRejected = true;
    }

    assert(duplicateRejected);

    // Unknown symbol must fail.
    bool unknownRejected = false;

    try {
        const IndicSymbol unknown(
            999,
            "क",
            Script::Devanagari,
            Language::Hindi
        );

        alphabet.indexOf(unknown);
    } catch (const std::out_of_range&) {
        unknownRejected = true;
    }

    assert(unknownRejected);

    // Invalid index must fail.
    bool indexRejected = false;

    try {
        alphabet.symbolAt(999);
    } catch (const std::out_of_range&) {
        indexRejected = true;
    }

    assert(indexRejected);

    return 0;
}