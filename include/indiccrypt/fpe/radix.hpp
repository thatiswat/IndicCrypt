#pragma once

#include <cstddef>
#include <span>

namespace indiccrypt::fpe {

class FpeRadix {
public:
    using Value = std::size_t;

    explicit FpeRadix(Value radix);

    [[nodiscard]]
    Value value() const noexcept;

    [[nodiscard]]
    bool contains(Value digit) const noexcept;

    void validate(
        std::span<const Value> digits
    ) const;

private:
    Value radix_;
};

} // namespace indiccrypt::fpe