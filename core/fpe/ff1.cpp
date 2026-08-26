#include "indiccrypt/fpe/ff1.hpp"

#include "indiccrypt/fpe/round.hpp"

#include <cstddef>
#include <stdexcept>
#include <vector>

namespace indiccrypt::fpe {

Ff1::Ff1(
    const FpeParameters& parameters
) noexcept
    : parameters_(&parameters) {}

void Ff1::validate(
    std::span<const FpeRadix::Value> input
) const {
    parameters_->validateLength(input.size());
    parameters_->domain().radix().validate(input);
}

std::vector<FpeRadix::Value> Ff1::encrypt(
    std::span<const FpeRadix::Value> plaintext
) const {
    validate(plaintext);

    const std::size_t n =
        plaintext.size();

    const std::size_t u =
        n / 2U;

    std::vector<FpeRadix::Value> a(
        plaintext.begin(),
        plaintext.begin() +
            static_cast<std::ptrdiff_t>(u)
    );

    std::vector<FpeRadix::Value> b(
        plaintext.begin() +
            static_cast<std::ptrdiff_t>(u),
        plaintext.end()
    );

    /*
     * FF1 encryption:
     *
     *     A = first half
     *     B = second half
     *
     * For each round:
     *
     *     C = round(A, B)
     *     A = B
     *     B = C
     *
     * The round implementation is responsible for
     * returning exactly the required target-half
     * length.
     */
    for (std::size_t round = 0; round < 10U; ++round) {
        const auto c =
            Ff1Round::encrypt(
                *parameters_,
                a,
                b,
                round
            );

        /*
         * The Feistel transition must preserve
         * the complete message length.
         */
        if (c.empty()) {
            throw std::logic_error(
                "FF1 round produced an empty half"
            );
        }

        a = b;
        b = c;

        if (a.size() + b.size() != n) {
            throw std::logic_error(
                "FF1 round changed message length"
            );
        }
    }

    /*
     * Final ciphertext = A || B.
     */
    std::vector<FpeRadix::Value> ciphertext;

    ciphertext.reserve(n);

    ciphertext.insert(
        ciphertext.end(),
        a.begin(),
        a.end()
    );

    ciphertext.insert(
        ciphertext.end(),
        b.begin(),
        b.end()
    );

    if (ciphertext.size() != n) {
        throw std::logic_error(
            "FF1 encryption changed message length"
        );
    }

    return ciphertext;
}

std::vector<FpeRadix::Value> Ff1::decrypt(
    std::span<const FpeRadix::Value> ciphertext
) const {
    validate(ciphertext);

    const std::size_t n =
        ciphertext.size();

    const std::size_t u =
        n / 2U;

    /*
     * Recover the final encrypted halves.
     */
    std::vector<FpeRadix::Value> a(
        ciphertext.begin(),
        ciphertext.begin() +
            static_cast<std::ptrdiff_t>(u)
    );

    std::vector<FpeRadix::Value> b(
        ciphertext.begin() +
            static_cast<std::ptrdiff_t>(u),
        ciphertext.end()
    );

    /*
     * Reverse the Feistel network.
     *
     * Encryption:
     *
     *     A' = B
     *     B' = F(A, B)
     *
     * Therefore:
     *
     *     B = A'
     *     A = F^-1(B', A')
     */
    for (std::size_t round = 10U; round-- > 0U;) {
        const auto previousA =
            Ff1Round::decrypt(
                *parameters_,
                b,
                a,
                round
            );

        if (previousA.empty()) {
            throw std::logic_error(
                "FF1 decryption round produced an empty half"
            );
        }

        b = a;
        a = previousA;

        if (a.size() + b.size() != n) {
            throw std::logic_error(
                "FF1 decryption changed message length"
            );
        }
    }

    /*
     * Final plaintext = A || B.
     */
    std::vector<FpeRadix::Value> plaintext;

    plaintext.reserve(n);

    plaintext.insert(
        plaintext.end(),
        a.begin(),
        a.end()
    );

    plaintext.insert(
        plaintext.end(),
        b.begin(),
        b.end()
    );

    if (plaintext.size() != n) {
        throw std::logic_error(
            "FF1 decryption changed message length"
        );
    }

    return plaintext;
}

} // namespace indiccrypt::fpe