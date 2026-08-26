#include "indiccrypt/fpe/y_number.hpp"

#include <cstddef>

namespace indiccrypt::fpe {

Ff1YNumber::Integer Ff1YNumber::fromBytes(
    std::span<const std::byte> bytes
) {
    Integer value = 0;

    for (const std::byte byte : bytes) {
        value <<= 8U;
        value += std::to_integer<unsigned int>(byte);
    }

    return value;
}

} // namespace indiccrypt::fpe