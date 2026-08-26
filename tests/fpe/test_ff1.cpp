#include "indiccrypt/fpe/ff1.hpp"

#include "indiccrypt/crypto/types.hpp"
#include "indiccrypt/fpe/alphabet.hpp"
#include "indiccrypt/fpe/domain.hpp"
#include "indiccrypt/fpe/parameters.hpp"
#include "indiccrypt/text/symbol.hpp"

#include <cassert>
#include <cstddef>
#include <vector>

int main() {
    using namespace indiccrypt::crypto;
    using namespace indiccrypt::fpe;
    using namespace indiccrypt::text;

    /*
     * Test alphabet.
     *
     * A two-symbol radix is intentionally used here so that
     * the test exercises the FF1 arithmetic over a small domain.
     */
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

    /*
     * AES-256 test key.
     */
    const ByteVector keyBytes(
        32,
        static_cast<Byte>(0x42)
    );

    const Key key(keyBytes);

    /*
     * Deterministic tweak.
     */
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

    /*
     * ---------------------------------------------------------
     * Basic encryption
     * ---------------------------------------------------------
     */

    const std::vector<FpeRadix::Value> plaintext = {
        0,
        1
    };

    const auto ciphertext =
        ff1.encrypt(plaintext);

    assert(
        ciphertext.size() ==
        plaintext.size()
    );

    /*
     * Every ciphertext digit must remain inside the domain.
     */
    for (const auto digit : ciphertext) {
        assert(
            digit <
            domain.radix().value()
        );
    }

    /*
     * ---------------------------------------------------------
     * Determinism
     * ---------------------------------------------------------
     */

    const auto repeated =
        ff1.encrypt(plaintext);

    assert(
        ciphertext ==
        repeated
    );

    /*
     * ---------------------------------------------------------
     * Basic decryption
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
     * Variable-length round-trip tests
     *
     * Both even and odd message lengths are tested.
     *
     * 2, 3, 4, 5, 6, 7, 8, 9, 10 digits
     * ---------------------------------------------------------
     */

    const std::vector<std::vector<FpeRadix::Value>> testCases = {
        {0, 1},
        {1, 0, 1},
        {0, 1, 0, 1},
        {1, 0, 1, 0, 1},
        {0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1},
        {0, 1, 0, 1, 0, 1, 0, 1},
        {1, 0, 1, 0, 1, 0, 1, 0, 1},
        {0, 1, 0, 1, 0, 1, 0, 1, 0, 1}
    };

    for (const auto& currentPlaintext : testCases) {
        /*
         * Encrypt.
         */
        const auto currentCiphertext =
            ff1.encrypt(currentPlaintext);

        /*
         * Encryption preserves message length.
         */
        assert(
            currentCiphertext.size() ==
            currentPlaintext.size()
        );

        /*
         * Ciphertext remains inside the radix.
         */
        for (const auto digit : currentCiphertext) {
            assert(
                digit <
                domain.radix().value()
            );
        }

        /*
         * Decrypt.
         */
        const auto currentDecrypted =
            ff1.decrypt(currentCiphertext);

        /*
         * Round-trip must recover the exact input.
         */
        assert(
            currentDecrypted ==
            currentPlaintext
        );

        /*
         * Encryption must be deterministic.
         */
        const auto repeatedCiphertext =
            ff1.encrypt(currentPlaintext);

        assert(
            repeatedCiphertext ==
            currentCiphertext
        );
    }

    /*
     * ---------------------------------------------------------
     * Invalid digit
     * ---------------------------------------------------------
     */

    const std::vector<FpeRadix::Value> invalid = {
        0,
        99
    };

    bool invalidRejected = false;

    try {
        (void)ff1.encrypt(invalid);
    } catch (const std::out_of_range&) {
        invalidRejected = true;
    }

    assert(invalidRejected);

    return 0;
}