#include "indiccrypt/fpe/ff1.hpp"

#include <cassert>
#include <cstddef>
#include <stdexcept>
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
        std::byte{0x02}
    };

    const FpeParameters parameters{
        domain,
        key,
        tweak
    };

    const Ff1 ff1(parameters);

    const std::vector<FpeRadix::Value> valid = {
        0,
        1
    };

    bool encryptRejected = false;

    try {
        ff1.encrypt(valid);
    } catch (const std::logic_error&) {
        encryptRejected = true;
    }

    assert(encryptRejected);

    bool decryptRejected = false;

    try {
        ff1.decrypt(valid);
    } catch (const std::logic_error&) {
        decryptRejected = true;
    }

    assert(decryptRejected);

    // Invalid digit must be rejected before FF1 execution.
    const std::vector<FpeRadix::Value> invalid = {
        0,
        99
    };

    bool invalidRejected = false;

    try {
        ff1.encrypt(invalid);
    } catch (const std::out_of_range&) {
        invalidRejected = true;
    }

    assert(invalidRejected);

    return 0;
}