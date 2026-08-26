#include "indiccrypt/fpe/parameters.hpp"

#include <stdexcept>
#include <utility>

namespace indiccrypt::fpe {

namespace {

constexpr std::size_t kMinimumLength = 2;
constexpr std::size_t kMaximumTweakLength = 64;

} // namespace

FpeParameters::FpeParameters(
    const FpeDomain& domain,
    const crypto::Key& key,
    std::span<const std::byte> tweak
)
    : domain_(&domain),
      key_(&key),
      tweak_(tweak.begin(), tweak.end()) {

    if (domain_->radix().value() < 2) {
        throw std::invalid_argument(
            "FF1 radix must be at least 2"
        );
    }

    if (tweak_.size() > kMaximumTweakLength) {
        throw std::invalid_argument(
            "FF1 tweak exceeds maximum supported length"
        );
    }
}

const FpeDomain& FpeParameters::domain() const noexcept {
    return *domain_;
}

const crypto::Key& FpeParameters::key() const noexcept {
    return *key_;
}

std::span<const std::byte>
FpeParameters::tweak() const noexcept {
    return tweak_;
}

FpeParameters::Length
FpeParameters::minimumLength() const noexcept {
    return kMinimumLength;
}

FpeParameters::Length
FpeParameters::maximumLength() const noexcept {
    return 4096;
}

void FpeParameters::validateLength(
    Length length
) const {
    if (length < minimumLength()) {
        throw std::invalid_argument(
            "FF1 input is shorter than minimum length"
        );
    }

    if (length > maximumLength()) {
        throw std::invalid_argument(
            "FF1 input exceeds maximum supported length"
        );
    }
}

} // namespace indiccrypt::fpe