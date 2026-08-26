#include "indiccrypt/crypto/kdf.hpp"

#include <openssl/evp.h>
#include <openssl/kdf.h>

#include <span>
#include <stdexcept>
#include <vector>

namespace indiccrypt::crypto {

std::string_view
KeyDerivation::purposeLabel(
    KeyPurpose purpose
) noexcept {
    switch (purpose) {
        case KeyPurpose::Encryption:
            return "indiccrypt/encryption/v1";

        case KeyPurpose::Fpe:
            return "indiccrypt/fpe/v1";

        case KeyPurpose::Search:
            return "indiccrypt/search/v1";

        case KeyPurpose::Integrity:
            return "indiccrypt/integrity/v1";
    }

    return "indiccrypt/unknown/v1";
}

Key KeyDerivation::hkdfSha256(
    std::span<const Byte> ikm,
    std::span<const Byte> salt,
    std::span<const Byte> info,
    std::size_t outputLength
) {
    if (ikm.empty()) {
        throw std::invalid_argument(
            "HKDF input key material must not be empty"
        );
    }

    if (outputLength == 0) {
        throw std::invalid_argument(
            "HKDF output length must not be zero"
        );
    }

    EVP_PKEY_CTX* context =
        EVP_PKEY_CTX_new_id(
            EVP_PKEY_HKDF,
            nullptr
        );

    if (context == nullptr) {
        throw std::runtime_error(
            "Failed to create HKDF context"
        );
    }

    std::vector<Byte> output(outputLength);
    std::size_t derivedLength = output.size();

    bool success =
        EVP_PKEY_derive_init(context) > 0;

    success =
        success &&
        EVP_PKEY_CTX_set_hkdf_md(
            context,
            EVP_sha256()
        ) > 0;

    if (!salt.empty()) {
        success =
            success &&
            EVP_PKEY_CTX_set1_hkdf_salt(
                context,
                salt.data(),
                static_cast<int>(salt.size())
            ) > 0;
    }

    success =
        success &&
        EVP_PKEY_CTX_set1_hkdf_key(
            context,
            ikm.data(),
            ikm.size()
        ) > 0;

    if (!info.empty()) {
        success =
            success &&
            EVP_PKEY_CTX_add1_hkdf_info(
                context,
                info.data(),
                info.size()
            ) > 0;
    }

    success =
        success &&
        EVP_PKEY_derive(
            context,
            output.data(),
            &derivedLength
        ) > 0;

    EVP_PKEY_CTX_free(context);

    if (!success || derivedLength != outputLength) {
        throw std::runtime_error(
            "HKDF-SHA-256 derivation failed"
        );
    }

    return Key(std::move(output));
}

Key KeyDerivation::derive(
    const Key& rootKey,
    KeyPurpose purpose,
    std::size_t outputLength
) {
    if (rootKey.size() == 0) {
        throw std::invalid_argument(
            "Root key must not be empty"
        );
    }

    if (outputLength == 0) {
        throw std::invalid_argument(
            "Output length must not be zero"
        );
    }

    EVP_PKEY_CTX* context =
        EVP_PKEY_CTX_new_id(
            EVP_PKEY_HKDF,
            nullptr
        );

    if (context == nullptr) {
        throw std::runtime_error(
            "Failed to create HKDF context"
        );
    }

    const std::string_view label =
        purposeLabel(purpose);

    std::vector<Byte> output(outputLength);

    bool success = true;

    success =
        success &&
        EVP_PKEY_derive_init(context) > 0;

    success =
        success &&
        EVP_PKEY_CTX_set_hkdf_md(
            context,
            EVP_sha256()
        ) > 0;

    success =
        success &&
        EVP_PKEY_CTX_set1_hkdf_salt(
            context,
            reinterpret_cast<const unsigned char*>(
                label.data()
            ),
            static_cast<int>(label.size())
        ) > 0;

    success =
        success &&
        EVP_PKEY_CTX_set1_hkdf_key(
            context,
            rootKey.bytes().data(),
            rootKey.bytes().size()
        ) > 0;

    success =
        success &&
        EVP_PKEY_CTX_add1_hkdf_info(
            context,
            reinterpret_cast<const unsigned char*>(
                label.data()
            ),
            static_cast<int>(label.size())
        ) > 0;

    std::size_t derivedLength = output.size();

    success =
        success &&
        EVP_PKEY_derive(
            context,
            output.data(),
            &derivedLength
        ) > 0;

    EVP_PKEY_CTX_free(context);

    if (!success || derivedLength != outputLength) {
        throw std::runtime_error(
            "HKDF derivation failed"
        );
    }

    return Key(std::move(output));
}

} // namespace indiccrypt::crypto