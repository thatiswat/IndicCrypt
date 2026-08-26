#include "indiccrypt/text/language.hpp"

#include <cassert>

int main() {
    using indiccrypt::text::Language;
    using indiccrypt::text::LanguageRegistry;
    using indiccrypt::text::Script;

    const auto& hindi =
        LanguageRegistry::get(Language::Hindi);

    assert(hindi.name == "Hindi");
    assert(hindi.iso639_1 == "hi");
    assert(hindi.iso639_3 == "hin");
    assert(hindi.primaryScript == Script::Devanagari);

    const auto& tamil =
        LanguageRegistry::get(Language::Tamil);

    assert(tamil.primaryScript == Script::Tamil);

    const auto& kannada =
        LanguageRegistry::get(Language::Kannada);

    assert(kannada.primaryScript == Script::Kannada);

    const auto& urdu =
        LanguageRegistry::get(Language::Urdu);

    assert(urdu.primaryScript == Script::Arabic);

    assert(LanguageRegistry::all().size() == 22);

    return 0;
}