#include "indiccrypt/crypto/aes_gcm.hpp"

#include <cassert>
#include <stdexcept>
#include <vector>

int main() {
    using namespace indiccrypt::crypto;

    const Key key({
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b,
        0x0c, 0x0d, 0x0e, 0x0f,
        0x10, 0x11, 0x12, 0x13,
        0x14, 0x15, 0x16, 0x17,
        0x18, 0x19, 0x1a, 0x1b,
        0x1c, 0x1d, 0x1e, 0x1f
    });

    const Nonce nonce({
        0x00, 0x01, 0x02, 0x03,
        0x04, 0x05, 0x06, 0x07,
        0x08, 0x09, 0x0a, 0x0b
    });

    const std::vector<Byte> plaintext = {
        0x48, 0x65, 0x6c, 0x6c,
        0x6f, 0x20, 0x49, 0x6e,
        0x64, 0x69, 0x63, 0x43,
        0x72, 0x79, 0x70, 0x74
    };

    const std::vector<Byte> aad = {
        0x01, 0x02, 0x03, 0x04
    };

    const auto encrypted =
        AesGcm::encrypt(
            key,
            nonce,
            plaintext,
            aad
        );

    assert(encrypted.ciphertext.size() == plaintext.size());
    assert(encrypted.tag.size() == AesGcm::TagSize);

    const auto decrypted =
        AesGcm::decrypt(
            key,
            nonce,
            encrypted.ciphertext,
            encrypted.tag,
            aad
        );

    assert(decrypted == plaintext);

    // Tampering with ciphertext must fail authentication.
    auto tampered =
        encrypted.ciphertext.bytes();

    std::vector<Byte> tamperedBytes(
        tampered.begin(),
        tampered.end()
    );

    tamperedBytes[0] ^= 0x01;

    bool authenticationFailed = false;

    try {
        AesGcm::decrypt(
            key,
            nonce,
            Ciphertext(std::move(tamperedBytes)),
            encrypted.tag,
            aad
        );
    } catch (const std::runtime_error&) {
        authenticationFailed = true;
    }

    assert(authenticationFailed);

    // Wrong AAD must fail authentication.
    const std::vector<Byte> wrongAad = {
        0xff, 0xee, 0xdd
    };

    authenticationFailed = false;

    try {
        AesGcm::decrypt(
            key,
            nonce,
            encrypted.ciphertext,
            encrypted.tag,
            wrongAad
        );
    } catch (const std::runtime_error&) {
        authenticationFailed = true;
    }

    assert(authenticationFailed);

    // Invalid key length must be rejected.
    bool invalidKeyRejected = false;

    try {
        const Key invalidKey({0x00, 0x01});

        AesGcm::encrypt(
            invalidKey,
            nonce,
            plaintext
        );
    } catch (const std::invalid_argument&) {
        invalidKeyRejected = true;
    }

    assert(invalidKeyRejected);

    // Invalid nonce length must be rejected.
    bool invalidNonceRejected = false;

    try {
        const Nonce invalidNonce({0x00, 0x01});

        AesGcm::encrypt(
            key,
            invalidNonce,
            plaintext
        );
    } catch (const std::invalid_argument&) {
        invalidNonceRejected = true;
    }

    assert(invalidNonceRejected);

    return 0;
}