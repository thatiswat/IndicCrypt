#include "indiccrypt/fpe/round.hpp"

#include "indiccrypt/crypto/types.hpp"
#include "indiccrypt/fpe/alphabet.hpp"
#include "indiccrypt/fpe/domain.hpp"
#include "indiccrypt/fpe/parameters.hpp"

#include "indiccrypt/text/symbol.hpp"

#include <array>
#include <cassert>
#include <cstddef>
#include <vector>

int main() {
    using namespace indiccrypt::crypto;
    using namespace indiccrypt::fpe;
    using namespace indiccrypt::text;

    const std::vector<IndicSymbol> symbols{
        IndicSymbol(1, "A", Script::Latin, Language::Sanskrit),
        IndicSymbol(2, "B", Script::Latin, Language::Sanskrit),
        IndicSymbol(3, "C", Script::Latin, Language::Sanskrit),
        IndicSymbol(4, "D", Script::Latin, Language::Sanskrit),
        IndicSymbol(5, "E", Script::Latin, Language::Sanskrit),
        IndicSymbol(6, "F", Script::Latin, Language::Sanskrit),
        IndicSymbol(7, "G", Script::Latin, Language::Sanskrit),
        IndicSymbol(8, "H", Script::Latin, Language::Sanskrit),
        IndicSymbol(9, "I", Script::Latin, Language::Sanskrit),
        IndicSymbol(10, "J", Script::Latin, Language::Sanskrit)
    };

    const FpeAlphabet alphabet(
        Script::Latin,
        symbols,
        "test-v1"
    );

    const FpeDomain domain(alphabet);

    const ByteVector keyBytes(
        32,
        static_cast<Byte>(0x42)
    );

    const Key key(keyBytes);

    const std::array<std::byte, 4> tweak{
        std::byte{0x01},
        std::byte{0x02},
        std::byte{0x03},
        std::byte{0x04}
    };

    const FpeParameters parameters(
        domain,
        key,
        tweak
    );

    const std::vector<FpeRadix::Value> a{
        1, 2, 3, 4
    };

    const std::vector<FpeRadix::Value> b{
        5, 6, 7, 8
    };

    const auto encrypted =
        Ff1Round::encrypt(
            parameters,
            a,
            b,
            0
        );

    assert(encrypted.size() == b.size());

    const auto encryptedAgain =
        Ff1Round::encrypt(
            parameters,
            a,
            b,
            0
        );

    assert(encrypted == encryptedAgain);

    for (const auto digit : encrypted) {
        assert(digit < 10);
    }

    const auto decrypted =
        Ff1Round::decrypt(
            parameters,
            encrypted,
            b,
            0
        );

    assert(decrypted.size() == encrypted.size());

    return 0;
}