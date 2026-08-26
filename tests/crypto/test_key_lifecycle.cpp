#include "indiccrypt/crypto/types.hpp"

#include <cassert>
#include <utility>

int main() {
    using namespace indiccrypt::crypto;

    Key original({
        0x10, 0x20, 0x30, 0x40,
        0x50, 0x60, 0x70, 0x80
    });

    assert(original.size() == 8);

    Key moved(std::move(original));

    assert(moved.size() == 8);

    const auto data = moved.bytes();

    assert(data[0] == 0x10);
    assert(data[7] == 0x80);

    Key destination({
        0xaa, 0xbb, 0xcc
    });

    destination = std::move(moved);

    assert(destination.size() == 8);

    const auto destinationData =
        destination.bytes();

    assert(destinationData[0] == 0x10);
    assert(destinationData[7] == 0x80);

    return 0;
}