#include "indiccrypt/fpe/radix.hpp"

#include <stdexcept>

namespace indiccrypt::fpe {

FpeRadix::FpeRadix(Value radix)
    : radix_(radix) {

    if (radix_ < 2) {
        throw std::invalid_argument(
            "FPE radix must be at least 2"
        );
    }
}

FpeRadix::Value FpeRadix::value() const noexcept {
    return radix_;
}

bool FpeRadix::contains(Value digit) const noexcept {
    return digit < radix_;
}

void FpeRadix::validate(
    std::span<const Value> digits
) const {
    for (const auto digit : digits) {
        if (!contains(digit)) {
            throw std::out_of_range(
                "FPE digit is outside radix domain"
            );
        }
    }
}

} // namespace indiccrypt::fpe