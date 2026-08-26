#include "indiccrypt/fpe/ff1.hpp"

#include "indiccrypt/crypto/types.hpp"
#include "indiccrypt/fpe/alphabet.hpp"
#include "indiccrypt/fpe/domain.hpp"
#include "indiccrypt/fpe/parameters.hpp"
#include "indiccrypt/text/symbol.hpp"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <vector>

int main() {
    using namespace indiccrypt::crypto;
    using namespace indiccrypt::fpe;
    using namespace indiccrypt::text;

    /*
     * FF1 reference interoperability vector.
     *
     * Key:
     * 2B7E151628AED2A6ABF7158809CF4F3C
     *
     * Radix:
     * 10
     *
     * Tweak:
     * empty
     *
     * Plaintext:
     * 0123456789
     *
     * Expected ciphertext:
     * 2433477484
     */

    const std::vector<IndicSymbol> symbols{
        IndicSymbol(0, "0", Script::Latin, Language::Sanskrit),
        IndicSymbol(1, "1", Script::Latin, Language::Sanskrit),
        IndicSymbol(2, "2", Script::Latin, Language::Sanskrit),
        IndicSymbol(3, "3", Script::Latin, Language::Sanskrit),
        IndicSymbol(4, "4", Script::Latin, Language::Sanskrit),
        IndicSymbol(5, "5", Script::Latin, Language::Sanskrit),
        IndicSymbol(6, "6", Script::Latin, Language::Sanskrit),
        IndicSymbol(7, "7", Script::Latin, Language::Sanskrit),
        IndicSymbol(8, "8", Script::Latin, Language::Sanskrit),
        IndicSymbol(9, "9", Script::Latin, Language::Sanskrit)
    };

    const FpeAlphabet alphabet(
        Script::Latin,
        symbols,
        "ff1-reference-decimal-v1"
    );

    const FpeDomain domain(alphabet);

    /*
     * NIST FF1 AES-128 test key.
     */
    const ByteVector keyBytes{
        0x2b, 0x7e, 0x15, 0x16,
        0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88,
        0x09, 0xcf, 0x4f, 0x3c
    };

    const Key key(keyBytes);

    /*
     * Empty tweak.
     */
    const std::vector<std::byte> tweak{};

    const FpeParameters parameters{
        domain,
        key,
        tweak
    };

    const Ff1 ff1(parameters);

    /*
     * Plaintext:
     *
     *     0123456789
     */
    const std::vector<FpeRadix::Value> plaintext{
        0, 1, 2, 3, 4,
        5, 6, 7, 8, 9
    };

    /*
     * NIST/reference expected ciphertext:
     *
     *     2433477484
     */
    const std::vector<FpeRadix::Value> expectedCiphertext{
        2, 4, 3, 3, 4,
        7, 7, 4, 8, 4
    };

    /*
     * ---------------------------------------------------------
     * Encrypt
     * ---------------------------------------------------------
     */
    const auto ciphertext =
        ff1.encrypt(plaintext);

    /*
     * Print the actual result BEFORE asserting.
     *
     * This lets us identify where the implementation
     * diverges from the reference vector.
     */
    std::cout << "Actual ciphertext: ";

    for (const auto digit : ciphertext) {
        std::cout << digit;
    }

    std::cout << '\n';

    std::cout << "Expected ciphertext: ";

    for (const auto digit : expectedCiphertext) {
        std::cout << digit;
    }

    std::cout << '\n';

    /*
     * ---------------------------------------------------------
     * Basic format preservation
     * ---------------------------------------------------------
     */
    assert(
        ciphertext.size() ==
        plaintext.size()
    );

    for (const auto digit : ciphertext) {
        assert(
            digit <
            domain.radix().value()
        );
    }

    /*
     * ---------------------------------------------------------
     * Reference interoperability
     * ---------------------------------------------------------
     */
    assert(
        ciphertext ==
        expectedCiphertext
    );

    /*
     * ---------------------------------------------------------
     * Decryption
     * ---------------------------------------------------------
     */
    const auto decrypted =
        ff1.decrypt(ciphertext);

    assert(
        decrypted ==
        plaintext
    );

    /*
     * ---------------------------------------------------------
     * Explicit round-trip check
     * ---------------------------------------------------------
     */
    const auto roundTripCiphertext =
        ff1.encrypt(plaintext);

    const auto roundTripPlaintext =
        ff1.decrypt(roundTripCiphertext);

    assert(
        roundTripPlaintext ==
        plaintext
    );

    return 0;
}