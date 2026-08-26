#include "indiccrypt/crypto/types.hpp"

#include <algorithm>
#include <utility>

namespace indiccrypt::crypto {

Key::Key(ByteVector bytes)
    : bytes_(std::move(bytes)) {}

Key::~Key() noexcept {
    std::fill(
        bytes_.begin(),
        bytes_.end(),
        static_cast<Byte>(0)
    );
}

Key::Key(Key&& other) noexcept
    : bytes_(std::move(other.bytes_)) {}

Key& Key::operator=(Key&& other) noexcept {
    if (this != &other) {
        std::fill(
            bytes_.begin(),
            bytes_.end(),
            static_cast<Byte>(0)
        );

        bytes_ = std::move(other.bytes_);
    }

    return *this;
}

std::span<const Byte> Key::bytes() const noexcept {
    return bytes_;
}

std::size_t Key::size() const noexcept {
    return bytes_.size();
}

Nonce::Nonce(ByteVector bytes)
    : bytes_(std::move(bytes)) {}

std::span<const Byte> Nonce::bytes() const noexcept {
    return bytes_;
}

std::size_t Nonce::size() const noexcept {
    return bytes_.size();
}

Tweak::Tweak(ByteVector bytes)
    : bytes_(std::move(bytes)) {}

std::span<const Byte> Tweak::bytes() const noexcept {
    return bytes_;
}

std::size_t Tweak::size() const noexcept {
    return bytes_.size();
}

Ciphertext::Ciphertext(ByteVector bytes)
    : bytes_(std::move(bytes)) {}

std::span<const Byte> Ciphertext::bytes() const noexcept {
    return bytes_;
}

std::size_t Ciphertext::size() const noexcept {
    return bytes_.size();
}

AuthenticationTag::AuthenticationTag(ByteVector bytes)
    : bytes_(std::move(bytes)) {}

std::span<const Byte> AuthenticationTag::bytes() const noexcept {
    return bytes_;
}

std::size_t AuthenticationTag::size() const noexcept {
    return bytes_.size();
}

SearchToken::SearchToken(ByteVector bytes)
    : bytes_(std::move(bytes)) {}

std::span<const Byte> SearchToken::bytes() const noexcept {
    return bytes_;
}

std::size_t SearchToken::size() const noexcept {
    return bytes_.size();
}

} // namespace indiccrypt::crypto