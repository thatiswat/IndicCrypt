#include "indiccrypt/text/script.hpp"

#include <cassert>

int main() {
    using indiccrypt::text::Script;
    using indiccrypt::text::ScriptEngine;

    assert(
        ScriptEngine::detect("नमस्ते")
        == Script::Devanagari
    );

    assert(
        ScriptEngine::detect("বাংলা")
        == Script::Bengali
    );

    assert(
        ScriptEngine::detect("தமிழ்")
        == Script::Tamil
    );

    assert(
        ScriptEngine::detect("తెలుగు")
        == Script::Telugu
    );

    assert(
        ScriptEngine::detect("ಕನ್ನಡ")
        == Script::Kannada
    );

    assert(
        ScriptEngine::detect("മലയാളം")
        == Script::Malayalam
    );

    assert(
        ScriptEngine::detect("ગુજરાતી")
        == Script::Gujarati
    );

    assert(
        ScriptEngine::detect("ਪੰਜਾਬੀ")
        == Script::Gurmukhi
    );

    return 0;
}