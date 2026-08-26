#include "indiccrypt/fpe/codec.hpp"

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

    const std::vector<IndicSymbol> alphabetSymbols = {
        a,
        aa,
        i
    };

    const FpeAlphabet alphabet(
        Script::Devanagari,
        alphabetSymbols,
        "indic-devanagari-v1"
    );

    const FpeCodec codec(alphabet);

    // Single-symbol encoding.
    assert(codec.encodeOne(a) == 0);
    assert(codec.encodeOne(aa) == 1);
    assert(codec.encodeOne(i) == 2);

    // Single-symbol decoding.
    assert(codec.decodeOne(0) == a);
    assert(codec.decodeOne(1) == aa);
    assert(codec.decodeOne(2) == i);

    // Sequence encoding.
    const std::vector<IndicSymbol> input = {
        i,
        a,
        aa,
        i
    };

    const auto encoded = codec.encode(input);

    assert(encoded.size() == 4);
    assert(encoded[0] == 2);
    assert(encoded[1] == 0);
    assert(encoded[2] == 1);
    assert(encoded[3] == 2);

    // Sequence decoding.
    const auto decoded = codec.decode(encoded);

    assert(decoded == input);

    // Fundamental round-trip invariant.
    const auto roundTrip =
        codec.decode(
            codec.encode(input)
        );

    assert(roundTrip == input);

    // Empty sequence is valid.
    const std::vector<IndicSymbol> emptySymbols;

    const auto emptyEncoded =
        codec.encode(emptySymbols);

    assert(emptyEncoded.empty());

    const auto emptyDecoded =
        codec.decode(emptyEncoded);

    assert(emptyDecoded.empty());

    return 0;
}