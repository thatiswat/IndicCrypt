#include "indiccrypt/crypto/kdf.hpp"

#include <cassert>
#include <cstdint>
#include <span>
#include <vector>

namespace {

using Byte = indiccrypt::crypto::Byte;

std::vector<Byte> hex(std::initializer_list<unsigned int> values) {
    std::vector<Byte> result;
    result.reserve(values.size());

    for (const auto value : values) {
        result.push_back(static_cast<Byte>(value));
    }

    return result;
}

bool equal(
    std::span<const Byte> lhs,
    std::span<const Byte> rhs
) {
    if (lhs.size() != rhs.size()) {
        return false;
    }

    for (std::size_t i = 0; i < lhs.size(); ++i) {
        if (lhs[i] != rhs[i]) {
            return false;
        }
    }

    return true;
}

} // namespace

int main() {
    using namespace indiccrypt::crypto;

    /*
     * RFC 5869 / HKDF-SHA-256 test case:
     *
     * IKM  = 0x0b repeated 22 times
     * salt = 0x00..0c
     * info = 0xf0..f9
     *
     * Our public KDF API currently models a root-key + purpose
     * construction, so this test validates deterministic output and
     * domain separation through that API.
     */

    const Key rootKey({
        0x0b, 0x0b, 0x0b, 0x0b,
        0x0b, 0x0b, 0x0b, 0x0b,
        0x0b, 0x0b, 0x0b, 0x0b,
        0x0b, 0x0b, 0x0b, 0x0b,
        0x0b, 0x0b, 0x0b, 0x0b,
        0x0b, 0x0b
    });

    const Key first =
        KeyDerivation::derive(
            rootKey,
            KeyPurpose::Encryption,
            42
        );

    const Key second =
        KeyDerivation::derive(
            rootKey,
            KeyPurpose::Encryption,
            42
        );

    assert(first.size() == 42);
    assert(equal(first.bytes(), second.bytes()));

    const Key differentPurpose =
        KeyDerivation::derive(
            rootKey,
            KeyPurpose::Search,
            42
        );

    assert(
        !equal(
            first.bytes(),
            differentPurpose.bytes()
        )
    );

    return 0;
}