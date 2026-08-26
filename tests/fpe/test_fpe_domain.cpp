#include "indiccrypt/fpe/domain.hpp"

#include <cassert>
#include <vector>

int main() {
    using namespace indiccrypt::fpe;
    using namespace indiccrypt::text;

    const IndicSymbol a(
        1,
        "अ",
        Script::Devanagari,
        Language::Hindi
    );

    const IndicSymbol aa(
        2,
        "आ",
        Script::Devanagari,
        Language::Hindi
    );

    const IndicSymbol i(
        3,
        "इ",
        Script::Devanagari,
        Language::Hindi
    );

    const std::vector<IndicSymbol> symbols = {
        a,
        aa,
        i
    };

    const FpeAlphabet alphabet(
        Script::Devanagari,
        symbols,
        "indic-devanagari-v1"
    );

    const FpeDomain domain(alphabet);

    assert(
        &domain.alphabet() == &alphabet
    );

    assert(
        domain.radix().value() == 3
    );

    const std::vector<IndicSymbol> plaintext = {
        i,
        a,
        aa,
        i
    };

    const auto encoded =
        domain.encode(plaintext);

    assert(encoded.size() == 4);

    assert(encoded[0] == 2);
    assert(encoded[1] == 0);
    assert(encoded[2] == 1);
    assert(encoded[3] == 2);

    const auto decoded =
        domain.decode(encoded);

    assert(decoded == plaintext);

    // Fundamental domain invariant.
    assert(
        domain.decode(
            domain.encode(plaintext)
        ) == plaintext
    );

    // Invalid radix digit must be rejected.
    const std::vector<FpeRadix::Value> invalid = {
        0,
        1,
        99
    };

    bool rejected = false;

    try {
        domain.decode(invalid);
    } catch (const std::out_of_range&) {
        rejected = true;
    }

    assert(rejected);

    // Empty sequence is valid.
    const std::vector<IndicSymbol> empty;

    const auto emptyEncoded =
        domain.encode(empty);

    assert(emptyEncoded.empty());

    const auto emptyDecoded =
        domain.decode(emptyEncoded);

    assert(emptyDecoded.empty());

    return 0;
}