#include "indiccrypt/fpe/parameter_block.hpp"

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

    const auto block =
        Ff1ParameterBlock::build(
            parameters,
            8
        );

    assert(
        block.size() ==
        Ff1ParameterBlock::Size
    );

    // ------------------------------------------------------------
    // FF1 P header
    // ------------------------------------------------------------

    // Version.
    assert(
        block[0] ==
        std::byte{0x01}
    );

    // Method / radix encoding marker.
    assert(
        block[1] ==
        std::byte{0x02}
    );

    // Number of rounds field marker.
    assert(
        block[2] ==
        std::byte{0x01}
    );

    // ------------------------------------------------------------
    // Radix = 10
    //
    // Three-byte big-endian representation:
    //
    // 00 00 0A
    // ------------------------------------------------------------

    assert(
        block[3] ==
        std::byte{0x00}
    );

    assert(
        block[4] ==
        std::byte{0x00}
    );

    assert(
        block[5] ==
        std::byte{0x0A}
    );

    // ------------------------------------------------------------
    // Number of rounds = 10
    // ------------------------------------------------------------

    assert(
        block[6] ==
        std::byte{0x0A}
    );

    // ------------------------------------------------------------
    // u = floor(n / 2)
    //
    // n = 8
    // u = 4
    // ------------------------------------------------------------

    assert(
        block[7] ==
        std::byte{0x04}
    );

    // ------------------------------------------------------------
    // n = 8
    //
    // 00 00 00 08
    // ------------------------------------------------------------

    assert(
        block[8] ==
        std::byte{0x00}
    );

    assert(
        block[9] ==
        std::byte{0x00}
    );

    assert(
        block[10] ==
        std::byte{0x00}
    );

    assert(
        block[11] ==
        std::byte{0x08}
    );

    // ------------------------------------------------------------
    // t = 4
    //
    // 00 00 00 04
    // ------------------------------------------------------------

    assert(
        block[12] ==
        std::byte{0x00}
    );

    assert(
        block[13] ==
        std::byte{0x00}
    );

    assert(
        block[14] ==
        std::byte{0x00}
    );

    assert(
        block[15] ==
        std::byte{0x04}
    );

    // ------------------------------------------------------------
    // Determinism
    // ------------------------------------------------------------

    const auto repeated =
        Ff1ParameterBlock::build(
            parameters,
            8
        );

    assert(block == repeated);

    return 0;
}