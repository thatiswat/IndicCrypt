#include "indiccrypt/fpe/ff1.hpp"

#include <stdexcept>

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

    throw std::logic_error(
        "FF1 encryption implementation is not available yet"
    );
}

std::vector<FpeRadix::Value> Ff1::decrypt(
    std::span<const FpeRadix::Value> ciphertext
) const {
    validate(ciphertext);

    throw std::logic_error(
        "FF1 decryption implementation is not available yet"
    );
}

} // namespace indiccrypt::fpe