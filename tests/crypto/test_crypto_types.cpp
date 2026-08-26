#include "indiccrypt/crypto/types.hpp"

#include <cassert>

int main() {
    using namespace indiccrypt::crypto;

    const ByteVector keyBytes = {
        0x00, 0x11, 0x22, 0x33,
        0x44, 0x55, 0x66, 0x77,
        0x88, 0x99, 0xaa, 0xbb,
        0xcc, 0xdd, 0xee, 0xff
    };

    const Key key(keyBytes);

    assert(key.size() == 16);
    assert(key.bytes()[0] == 0x00);
    assert(key.bytes()[15] == 0xff);

    const Nonce nonce({
        0x01, 0x02, 0x03, 0x04,
        0x05, 0x06, 0x07, 0x08,
        0x09, 0x0a, 0x0b, 0x0c
    });

    assert(nonce.size() == 12);

    const Tweak tweak({
        0xaa, 0xbb, 0xcc
    });

    assert(tweak.size() == 3);

    const Ciphertext ciphertext({
        0xde, 0xad, 0xbe, 0xef
    });

    assert(ciphertext.size() == 4);

    const AuthenticationTag tag({
        0x10, 0x20, 0x30, 0x40
    });

    assert(tag.size() == 4);

    const SearchToken token({
        0x55, 0x66, 0x77, 0x88
    });

    assert(token.size() == 4);

    return 0;
}