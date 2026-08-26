#include "indiccrypt/core.hpp"

#include <cassert>

int main() {
    indiccrypt::CryptoCore core(42);

    assert(core.state() == 42);

    return 0;
}