#include "indiccrypt/fpe/q_block.hpp"

#include "indiccrypt/fpe/numeral.hpp"
#include "indiccrypt/fpe/numeral_encoding.hpp"
#include "indiccrypt/fpe/numeral_math.hpp"

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace indiccrypt::fpe {

namespace {

std::size_t requiredByteLength(
    const FpeRadix& radix,
    std::size_t length
) {
    if (length == 0U) {
        return 0U;
    }

    const Ff1Numeral::Integer maximum =
        Ff1NumeralMath::radixPower(
            radix,
            length
        ) - 1;

    if (maximum == 0) {
        return 1U;
    }

    std::size_t bits = 0U;
    Ff1Numeral::Integer value = maximum;

    while (value > 0) {
        value >>= 1;
        ++bits;
    }

    return (bits + 7U) / 8U;
}

} // namespace

std::vector<std::byte> Ff1QBlock::build(
    const FpeParameters& parameters,
    std::size_t messageLength,
    std::size_t round,
    std::span<const FpeRadix::Value> numeral
) {
    parameters.validateLength(messageLength);

    if (round >= 10U) {
        throw std::invalid_argument(
            "FF1 round must be in [0, 9]"
        );
    }

    const std::size_t u =
        messageLength / 2U;

    const std::size_t v =
        messageLength - u;

    /*
     * In FF1 the current B half alternates for
     * odd-length messages.
     *
     * Round 0 -> B has v digits
     * Round 1 -> B has u digits
     * Round 2 -> B has v digits
     * ...
     *
     * For even n, u == v, so this naturally
     * remains unchanged.
     */
    const std::size_t expectedBLength =
        (round % 2U == 0U)
            ? v
            : u;

    if (numeral.size() != expectedBLength) {
        throw std::invalid_argument(
            "FF1 Q block requires the current B numeral length"
        );
    }

    parameters.domain().radix().validate(numeral);

    const std::size_t b =
        requiredByteLength(
            parameters.domain().radix(),
            numeral.size()
        );

    /*
     * Q =
     *
     * T
     * || [0]^(-t-b-1 mod 16)
     * || [i]^1
     * || NUM_radix(B)^b
     */
    std::vector<std::byte> q;

    const auto tweak =
        parameters.tweak();

    q.insert(
        q.end(),
        tweak.begin(),
        tweak.end()
    );

    const std::size_t zeroPadding =
        (
            16U -
            (
                (tweak.size() + b + 1U) %
                16U
            )
        ) % 16U;

    q.insert(
        q.end(),
        zeroPadding,
        std::byte{0}
    );

    q.push_back(
        static_cast<std::byte>(round)
    );

    const Ff1Numeral value(
        parameters.domain().radix(),
        std::vector<FpeRadix::Value>(
            numeral.begin(),
            numeral.end()
        )
    );

    const auto encoded =
        Ff1NumeralEncoding::encode(
            value,
            b
        );

    q.insert(
        q.end(),
        encoded.begin(),
        encoded.end()
    );

    return q;
}

} // namespace indiccrypt::fpe