#include "indiccrypt/fpe/round.hpp"

#include "indiccrypt/fpe/numeral.hpp"
#include "indiccrypt/fpe/numeral_math.hpp"
#include "indiccrypt/fpe/parameter_block.hpp"
#include "indiccrypt/fpe/q_block.hpp"
#include "indiccrypt/fpe/prf.hpp"
#include "indiccrypt/fpe/y.hpp"
#include "indiccrypt/fpe/y_number.hpp"

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace indiccrypt::fpe {

namespace {

void validateRoundInputs(
    const FpeParameters& parameters,
    std::span<const Ff1Round::Digit> a,
    std::span<const Ff1Round::Digit> b,
    std::size_t round
) {
    const std::size_t n =
        a.size() + b.size();

    parameters.validateLength(n);

    if (round >= 10U) {
        throw std::invalid_argument(
            "FF1 round must be in [0, 9]"
        );
    }

    if (a.empty() || b.empty()) {
        throw std::invalid_argument(
            "FF1 round requires non-empty A and B"
        );
    }

    parameters.domain().radix().validate(a);
    parameters.domain().radix().validate(b);
}

std::vector<Ff1Round::Digit> computeRound(
    const FpeParameters& parameters,
    std::span<const Ff1Round::Digit> a,
    std::span<const Ff1Round::Digit> b,
    std::size_t round,
    bool encryption
) {
    validateRoundInputs(
        parameters,
        a,
        b,
        round
    );

    const std::size_t n =
        a.size() + b.size();

    /*
     * FF1 Feistel round:
     *
     *     C = (NUM(A) +/- Y) mod radix^m
     *     A' = B
     *     B' = C
     *
     * The output C always has the same length
     * as the current A.
     *
     * This is especially important for odd n.
     *
     * Example n = 3:
     *
     *     A = 1, B = 2
     *     C has length 1
     *
     *     next:
     *     A = 2, B = 1
     *     C has length 2
     *
     * Therefore m = |A| for every round.
     */
    const std::size_t m =
        a.size();

    /*
     * Q is constructed from the current B.
     */
    const auto q =
        Ff1QBlock::build(
            parameters,
            n,
            round,
            b
        );

    /*
     * P || Q becomes the PRF input.
     */
    const auto p =
        Ff1ParameterBlock::build(
            parameters,
            n
        );

    std::vector<std::byte> prfInput;

    prfInput.reserve(
        p.size() + q.size()
    );

    prfInput.insert(
        prfInput.end(),
        p.begin(),
        p.end()
    );

    prfInput.insert(
        prfInput.end(),
        q.begin(),
        q.end()
    );

    /*
     * R = PRF_K(P || Q)
     *
     * Ff1Prf returns one 16-byte AES block.
     */
    const auto r =
        Ff1Prf::evaluate(
            parameters.key(),
            prfInput,
            Ff1Prf::BlockSize
        );

    /*
     * Generate Y from R.
     *
     * Y is represented as a 16-byte
     * arbitrary-precision integer.
     */
    constexpr std::size_t yLength =
        Ff1YGenerator::BlockSize;

    const auto yBytes =
        Ff1YGenerator::generate(
            parameters.key(),
            r,
            yLength
        );

    /*
     * Ff1YGenerator uses crypto::ByteVector
     * (uint8_t), while Ff1YNumber expects
     * std::byte.
     */
    std::vector<std::byte> yAsBytes;

    yAsBytes.reserve(
        yBytes.size()
    );

    for (const auto byte : yBytes) {
        yAsBytes.push_back(
            static_cast<std::byte>(byte)
        );
    }

    const auto yValue =
        Ff1YNumber::fromBytes(
            yAsBytes
        );

    /*
     * Convert A to its radix integer value.
     */
    const Ff1Numeral aNumeral(
        parameters.domain().radix(),
        std::vector<Ff1Round::Digit>(
            a.begin(),
            a.end()
        )
    );

    const auto aValue =
        aNumeral.toInteger();

    /*
     * C is calculated modulo radix^m.
     */
    const auto modulus =
        Ff1NumeralMath::radixPower(
            parameters.domain().radix(),
            m
        );

    Ff1Numeral::Integer result;

    if (encryption) {
        /*
         * C = (NUM(A) + Y) mod radix^m
         */
        result =
            (aValue + yValue) %
            modulus;
    } else {
        /*
         * A = (NUM(C) - Y) mod radix^m
         */
        result =
            (aValue - yValue) %
            modulus;

        /*
         * Normalize negative modulo.
         */
        if (result < 0) {
            result += modulus;
        }
    }

    /*
     * Convert the result back to radix digits.
     */
    const auto resultNumeral =
        Ff1Numeral::fromInteger(
            parameters.domain().radix(),
            result
        );

    std::vector<Ff1Round::Digit> resultDigits(
        resultNumeral.digits().begin(),
        resultNumeral.digits().end()
    );

    /*
     * FF1 requires exactly m digits.
     */
    if (resultDigits.size() > m) {
        throw std::logic_error(
            "FF1 round result exceeds target length"
        );
    }

    if (resultDigits.size() < m) {
        std::vector<Ff1Round::Digit> padded(
            m - resultDigits.size(),
            0
        );

        padded.insert(
            padded.end(),
            resultDigits.begin(),
            resultDigits.end()
        );

        resultDigits =
            std::move(padded);
    }

    return resultDigits;
}

} // namespace

std::vector<Ff1Round::Digit> Ff1Round::encrypt(
    const FpeParameters& parameters,
    std::span<const Digit> a,
    std::span<const Digit> b,
    std::size_t round
) {
    return computeRound(
        parameters,
        a,
        b,
        round,
        true
    );
}

std::vector<Ff1Round::Digit> Ff1Round::decrypt(
    const FpeParameters& parameters,
    std::span<const Digit> a,
    std::span<const Digit> b,
    std::size_t round
) {
    return computeRound(
        parameters,
        a,
        b,
        round,
        false
    );
}

} // namespace indiccrypt::fpe