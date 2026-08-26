#include "indiccrypt/fpe/radix.hpp"

#include <cassert>
#include <stdexcept>
#include <vector>

int main() {
    using namespace indiccrypt::fpe;

    FpeRadix radix(10);

    assert(radix.value() == 10);

    assert(radix.contains(0));
    assert(radix.contains(5));
    assert(radix.contains(9));

    assert(!radix.contains(10));
    assert(!radix.contains(100));

    const std::vector<FpeRadix::Value> valid = {
        0, 1, 2, 5, 8, 9
    };

    radix.validate(valid);

    const std::vector<FpeRadix::Value> invalid = {
        0, 1, 10
    };

    bool rejected = false;

    try {
        radix.validate(invalid);
    } catch (const std::out_of_range&) {
        rejected = true;
    }

    assert(rejected);

    // Radix 0 is invalid.
    bool zeroRejected = false;

    try {
        FpeRadix invalidRadix(0);
    } catch (const std::invalid_argument&) {
        zeroRejected = true;
    }

    assert(zeroRejected);

    // Radix 1 is invalid.
    bool oneRejected = false;

    try {
        FpeRadix invalidRadix(1);
    } catch (const std::invalid_argument&) {
        oneRejected = true;
    }

    assert(oneRejected);

    // Empty sequence is valid.
    const std::vector<FpeRadix::Value> empty;

    radix.validate(empty);

    return 0;
}