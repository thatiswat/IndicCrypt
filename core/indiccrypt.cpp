#include "indiccrypt/core.hpp"

namespace indiccrypt {

CryptoCore::CryptoCore(std::uint64_t seed)
    : state_(seed) {}

std::uint64_t CryptoCore::state() const noexcept {
    return state_;
}

} // namespace indiccrypt