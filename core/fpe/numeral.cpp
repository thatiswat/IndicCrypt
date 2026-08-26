#include "indiccrypt/fpe/numeral.hpp"

#include <stdexcept>
#include <utility>

namespace indiccrypt::fpe {

Ff1Numeral::Ff1Numeral(
    FpeRadix radix,
    std::vector<Digit> digits
)
    : radix_(std::move(radix)),
      digits_(std::move(digits)) {

    radix_.validate(digits_);
}

const FpeRadix& Ff1Numeral::radix() const noexcept {
    return radix_;
}

std::span<const Ff1Numeral::Digit>
Ff1Numeral::digits() const noexcept {
    return digits_;
}

std::size_t Ff1Numeral::size() const noexcept {
    return digits_.size();
}

bool Ff1Numeral::empty() const noexcept {
    return digits_.empty();
}

Ff1Numeral::Integer
Ff1Numeral::toInteger() const {
    Integer value = 0;

    const Integer radixValue =
        static_cast<unsigned int>(radix_.value());

    for (const Digit digit : digits_) {
        value *= radixValue;
        value += digit;
    }

    return value;
}

Ff1Numeral
Ff1Numeral::fromInteger(
    FpeRadix radix,
    const Integer& value
) {
    if (value < 0) {
        throw std::invalid_argument(
            "FF1 numeral cannot represent a negative integer"
        );
    }

    if (value == 0) {
        return Ff1Numeral(
            std::move(radix),
            {0}
        );
    }

    const Integer radixValue =
        static_cast<unsigned int>(radix.value());

    std::vector<Digit> reversed;

    Integer remaining = value;

    while (remaining > 0) {
        const Integer remainder =
            remaining % radixValue;

        reversed.push_back(
            remainder.convert_to<Digit>()
        );

        remaining /= radixValue;
    }

    std::vector<Digit> digits(
        reversed.rbegin(),
        reversed.rend()
    );

    return Ff1Numeral(
        std::move(radix),
        std::move(digits)
    );
}

} // namespace indiccrypt::fpe