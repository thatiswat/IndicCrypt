#include "indiccrypt/text/indic_text.hpp"

#include <cassert>
#include <string>

int main() {
    using indiccrypt::text::IndicText;

    const auto text =
        IndicText::fromUtf8("नमस्ते भारत");

    assert(!text.utf8().empty());
    assert(!text.codePoints().empty());

    const auto tamil =
        IndicText::fromUtf8("தமிழ்");

    assert(!tamil.codePoints().empty());

    const auto kannada =
        IndicText::fromUtf8("ಕನ್ನಡ");

    assert(!kannada.codePoints().empty());

    return 0;
}