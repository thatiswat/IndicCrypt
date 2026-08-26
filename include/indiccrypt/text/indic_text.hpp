#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace indiccrypt::text {

using CodePoint = std::uint32_t;

class IndicText {
public:
    static IndicText fromUtf8(std::string_view input);

    [[nodiscard]]
    const std::string& utf8() const noexcept;

    [[nodiscard]]
    const std::vector<CodePoint>& codePoints() const noexcept;

private:
    IndicText(
        std::string normalizedUtf8,
        std::vector<CodePoint> codePoints
    );

    std::string normalizedUtf8_;
    std::vector<CodePoint> codePoints_;
};

} // namespace indiccrypt::text