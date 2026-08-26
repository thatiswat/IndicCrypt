#include "indiccrypt/fpe/q_block.hpp"

#include "indiccrypt/fpe/numeral.hpp"

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace indiccrypt::fpe {

namespace {

void appendBigEndian(
    std::vector<std::byte>& output,
    std::size_t value,
    std::size_t width
) {
    for (std::size_t i = 0; i < width; ++i) {
        const std::size_t shift =
            (width - 1U - i) * 8U;

        output.push_back(
            static_cast<std::byte>(
                (value >> shift) & 0xffU
            )
        );
    }
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

    const std::size_t b =
        (v + 7U) / 8U;

    const std::size_t modulus =
        1U << 3U;

    (void)modulus;

    std::vector<std::byte> q;

    const auto tweak =
        parameters.tweak();

    /*
     * Q =
     *
     * tweak
     * || 0-padding
     * || round
     * || numeral
     *
     * The exact numeral encoding is delegated to the
     * FF1 numeral layer.
     */

    q.insert(
        q.end(),
        tweak.begin(),
        tweak.end()
    );

    const std::size_t pad =
        (16U - (q.size() % 16U)) % 16U;

    q.insert(
        q.end(),
        pad,
        std::byte{0}
    );

    q.push_back(
        static_cast<std::byte>(round)
    );

    appendBigEndian(
        q,
        numeral.size(),
        4U
    );

    for (const auto digit : numeral) {
        if (
            digit >=
            parameters.domain().radix().value()
        ) {
            throw std::out_of_range(
                "FF1 numeral contains invalid digit"
            );
        }

        q.push_back(
            static_cast<std::byte>(digit)
        );
    }

    (void)u;
    (void)v;
    (void)b;

    return q;
}

} // namespace indiccrypt::fpe