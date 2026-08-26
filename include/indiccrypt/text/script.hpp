#pragma once

#include <cstdint>
#include <string>
#include <string_view>
#include <vector>

namespace indiccrypt::text {

enum class Script : std::uint16_t {
    Unknown = 0,

    Devanagari,
    Bengali,
    Gurmukhi,
    Gujarati,
    Odia,
    Tamil,
    Telugu,
    Kannada,
    Malayalam,
    Meetei,
    OlChiki,
    Arabic,
    Latin,
    Other
};

struct ScriptDetection {
    Script script;
    std::string text;
};

class ScriptEngine {
public:
    [[nodiscard]]
    static Script detect(std::string_view utf8);

    [[nodiscard]]
    static std::vector<ScriptDetection>
    detectAll(std::string_view utf8);
};

[[nodiscard]]
std::string_view scriptName(Script script) noexcept;

} // namespace indiccrypt::text