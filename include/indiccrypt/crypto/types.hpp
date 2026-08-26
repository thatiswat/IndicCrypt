#pragma once

#include <cstddef>
#include <cstdint>
#include <span>
#include <vector>

namespace indiccrypt::crypto {

using Byte = std::uint8_t;
using ByteVector = std::vector<Byte>;

class Key {
public:
    explicit Key(ByteVector bytes);

    ~Key() noexcept;

    Key(const Key&) = delete;
    Key& operator=(const Key&) = delete;

    Key(Key&& other) noexcept;
    Key& operator=(Key&& other) noexcept;

    [[nodiscard]]
    std::span<const Byte> bytes() const noexcept;

    [[nodiscard]]
    std::size_t size() const noexcept;

private:
    ByteVector bytes_;
};

class Nonce {
public:
    explicit Nonce(ByteVector bytes);

    [[nodiscard]]
    std::span<const Byte> bytes() const noexcept;

    [[nodiscard]]
    std::size_t size() const noexcept;

private:
    ByteVector bytes_;
};

class Tweak {
public:
    explicit Tweak(ByteVector bytes);

    [[nodiscard]]
    std::span<const Byte> bytes() const noexcept;

    [[nodiscard]]
    std::size_t size() const noexcept;

private:
    ByteVector bytes_;
};

class Ciphertext {
public:
    explicit Ciphertext(ByteVector bytes);

    [[nodiscard]]
    std::span<const Byte> bytes() const noexcept;

    [[nodiscard]]
    std::size_t size() const noexcept;

private:
    ByteVector bytes_;
};

class AuthenticationTag {
public:
    explicit AuthenticationTag(ByteVector bytes);

    [[nodiscard]]
    std::span<const Byte> bytes() const noexcept;

    [[nodiscard]]
    std::size_t size() const noexcept;

private:
    ByteVector bytes_;
};

class SearchToken {
public:
    explicit SearchToken(ByteVector bytes);

    [[nodiscard]]
    std::span<const Byte> bytes() const noexcept;

    [[nodiscard]]
    std::size_t size() const noexcept;

private:
    ByteVector bytes_;
};

} // namespace indiccrypt::crypto