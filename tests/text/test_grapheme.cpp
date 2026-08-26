#include "indiccrypt/text/grapheme.hpp"

#include <cassert>
#include <string>

int main() {
    using indiccrypt::text::GraphemeEngine;

    {
        const auto clusters =
            GraphemeEngine::segment("नमस्ते");

        assert(!clusters.empty());
    }

    {
        const auto clusters =
            GraphemeEngine::segment("தமிழ்");

        assert(!clusters.empty());
    }

    {
        const auto clusters =
            GraphemeEngine::segment("ಕನ್ನಡ");

        assert(!clusters.empty());
    }

    {
        const auto clusters =
            GraphemeEngine::segment("తెలుగు");

        assert(!clusters.empty());
    }

    {
        const auto clusters =
            GraphemeEngine::segment("বাংলা");

        assert(!clusters.empty());
    }

    return 0;
}