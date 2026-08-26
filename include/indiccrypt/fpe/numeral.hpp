#pragma once

#include "indiccrypt/fpe/radix.hpp"

#include <boost/multiprecision/cpp_int.hpp>

#include <cstddef>
#include <span>
#include <vector>

namespace indiccrypt::fpe {

class Ff1Numeral {
public:
    using Digit = FpeRadix::Value;
    using Integer = boost::multiprecision::cpp_int;

    Ff1Numeral(
        FpeRadix radix,
        std::vector<Digit> digits
    );

    [[nodiscard]]
    const FpeRadix& radix() const noexcept;

    [[nodiscard]]
    std::span<const Digit> digits() const noexcept;

    [[nodiscard]]
    std::size_t size() const noexcept;

    [[nodiscard]]
    bool empty() const noexcept;

    [[nodiscard]]
    Integer toInteger() const;

    [[nodiscard]]
    static Ff1Numeral fromInteger(
        FpeRadix radix,
        const Integer& value
    );

private:
    FpeRadix radix_;
    std::vector<Digit> digits_;
};

} // namespace indiccrypt::fpe