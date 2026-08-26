#pragma once

#include <cstdint>

namespace indiccrypt {

class CryptoCore {
public:
    explicit CryptoCore(std::uint64_t seed);

    [[nodiscard]]
    std::uint64_t state() const noexcept;

private:
    std::uint64_t state_;
};

} // namespace indiccrypt