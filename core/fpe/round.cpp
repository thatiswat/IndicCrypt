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

std::size_t requiredByteLength(
    const FpeRadix& radix,
    std::size_t length
) {
    if (length == 0U) {
        return 0U;
    }

    /*
     * We need:
     *
     *     b = ceil(ceil(length * log2(radix)) / 8)
     *
     * The maximum radix number with `length`
     * digits is radix^length - 1.
     *
     * Its bit length is exactly:
     *
     *     ceil(length * log2(radix))
     *
     * for the FF1 calculation.
     */
    const auto maximum =
        Ff1NumeralMath::radixPower(
            radix,
            length
        ) - 1;

    if (maximum == 0) {
        return 1U;
    }

    std::size_t bits = 0U;

    auto value = maximum;

    while (value > 0) {
        value >>= 1;
        ++bits;
    }

    return (bits + 7U) / 8U;
}

std::size_t calculateYLength(
    const FpeRadix& radix,
    std::size_t m
) {
    /*
     * FF1:
     *
     *     b = ceil(ceil(m * log2(radix)) / 8)
     *
     *     d = 4 * ceil(b / 4) + 4
     */
    const std::size_t b =
        requiredByteLength(
            radix,
            m
        );

    const std::size_t bBlocks =
        (b + 3U) / 4U;

    return
        (4U * bBlocks) +
        4U;
}

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
     * FF1 round:
     *
     *     m = |A|
     *
     *     C = (NUM(A) +/- Y) mod radix^m
     *
     * The result therefore has exactly |A|
     * digits.
     */
    const std::size_t m =
        a.size();

    /*
     * ---------------------------------------------------------
     * Q
     * ---------------------------------------------------------
     *
     * Q contains:
     *
     *     T
     *     || zero padding
     *     || round
     *     || NUM(B)
     */
    const auto q =
        Ff1QBlock::build(
            parameters,
            n,
            round,
            b
        );

    /*
     * ---------------------------------------------------------
     * P
     * ---------------------------------------------------------
     */
    const auto p =
        Ff1ParameterBlock::build(
            parameters,
            n
        );

    /*
     * ---------------------------------------------------------
     * P || Q
     * ---------------------------------------------------------
     */
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
     * ---------------------------------------------------------
     * R = PRF_K(P || Q)
     * ---------------------------------------------------------
     *
     * R is always exactly one AES block.
     */
    const auto r =
        Ff1Prf::evaluate(
            parameters.key(),
            prfInput,
            Ff1Prf::BlockSize
        );

    /*
     * ---------------------------------------------------------
     * Generate S/Y
     * ---------------------------------------------------------
     *
     * FF1 requires:
     *
     *     b = ceil(ceil(m * log2(radix)) / 8)
     *
     *     d = 4 * ceil(b / 4) + 4
     *
     * For the NIST decimal vector:
     *
     *     m = 5
     *     radix = 10
     *     b = 3
     *     d = 8
     *
     * Therefore we MUST NOT always request
     * a full 16-byte Y value.
     */
    const std::size_t yLength =
        calculateYLength(
            parameters.domain().radix(),
            m
        );

    const auto yBytes =
        Ff1YGenerator::generate(
            parameters.key(),
            r,
            yLength
        );

    /*
     * Convert crypto::ByteVector to std::byte.
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

    /*
     * Y = NUM(S)
     */
    const auto yValue =
        Ff1YNumber::fromBytes(
            yAsBytes
        );

    /*
     * ---------------------------------------------------------
     * NUM(A)
     * ---------------------------------------------------------
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
     * ---------------------------------------------------------
     * radix^m
     * ---------------------------------------------------------
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

        if (result < 0) {
            result += modulus;
        }
    }

    /*
     * ---------------------------------------------------------
     * Convert result back to radix digits.
     * ---------------------------------------------------------
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