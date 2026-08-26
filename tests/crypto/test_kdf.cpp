#include "indiccrypt/crypto/kdf.hpp"

#include <algorithm>
#include <cassert>
#include <span>

namespace {

bool different(
    std::span<const indiccrypt::crypto::Byte> a,
    std::span<const indiccrypt::crypto::Byte> b
) {
    if (a.size() != b.size()) {
        return true;
    }

    return !std::equal(a.begin(), a.end(), b.begin());
}

} // namespace

int main() {
    using namespace indiccrypt::crypto;

    const Key rootKey({
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b,
        0x1c, 0x1d, 0x1e, 0x1f
    });

    const Key encryptionKey =
        KeyDerivation::derive(
            rootKey,
            KeyPurpose::Encryption,
            32
        );

    const Key fpeKey =
        KeyDerivation::derive(
            rootKey,
            KeyPurpose::Fpe,
            32
        );

    const Key searchKey =
        KeyDerivation::derive(
            rootKey,
            KeyPurpose::Search,
            32
        );

    assert(encryptionKey.size() == 32);
    assert(fpeKey.size() == 32);
    assert(searchKey.size() == 32);

    assert(different(
        encryptionKey.bytes(),
        fpeKey.bytes()
    ));

    assert(different(
        encryptionKey.bytes(),
        searchKey.bytes()
    ));

    assert(different(
        fpeKey.bytes(),
        searchKey.bytes()
    ));

    const Key encryptionKeyAgain =
        KeyDerivation::derive(
            rootKey,
            KeyPurpose::Encryption,
            32
        );

    assert(
        encryptionKey.bytes().size() ==
        encryptionKeyAgain.bytes().size()
    );

    assert(std::equal(
        encryptionKey.bytes().begin(),
        encryptionKey.bytes().end(),
        encryptionKeyAgain.bytes().begin()
    ));

    return 0;
}