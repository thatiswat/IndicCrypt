#include "indiccrypt/fpe/parameters.hpp"

#include <cassert>
#include <cstddef>
#include <vector>

int main() {
    using namespace indiccrypt::crypto;
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

    const std::vector<IndicSymbol> symbols = {
        a,
        aa
    };

    const FpeAlphabet alphabet(
        Script::Devanagari,
        symbols,
        "indic-devanagari-v1"
    );

    const FpeDomain domain(alphabet);

    const ByteVector keyBytes(
        32,
        static_cast<unsigned char>(0x42)
    );

    const Key key{
        ByteVector(keyBytes)
    };

    const std::vector<std::byte> tweak = {
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03},
        std::byte{0x04}
    };

    const FpeParameters parameters{
        domain,
        key,
        tweak
    };

    assert(
        &parameters.domain() == &domain
    );

    assert(
        &parameters.key() == &key
    );

    assert(
        parameters.tweak().size() == 4
    );

    assert(
        parameters.minimumLength() == 2
    );

    assert(
        parameters.maximumLength() >=
        parameters.minimumLength()
    );

    parameters.validateLength(2);
    parameters.validateLength(10);

    bool shortRejected = false;

    try {
        parameters.validateLength(1);
    } catch (const std::invalid_argument&) {
        shortRejected = true;
    }

    assert(shortRejected);

    bool longRejected = false;

    try {
        parameters.validateLength(
            parameters.maximumLength() + 1
        );
    } catch (const std::invalid_argument&) {
        longRejected = true;
    }

    assert(longRejected);

    return 0;
}