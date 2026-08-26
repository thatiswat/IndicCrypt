#include "indiccrypt/fpe/numeral_encoding.hpp"

#include <boost/multiprecision/cpp_int.hpp>

#include <cstddef>
#include <limits>
#include <stdexcept>
#include <vector>

namespace indiccrypt::fpe {

std::vector<std::byte> Ff1NumeralEncoding::encode(
    const Ff1Numeral& numeral,
    std::size_t byteLength
) {
    if (byteLength == 0U) {
        throw std::invalid_argument(
            "FF1 numeral encoding requires a non-zero byte length"
        );
    }

    const auto value = numeral.toInteger();

    if (value < 0) {
        throw std::invalid_argument(
            "FF1 numeral cannot encode a negative integer"
        );
    }

    const boost::multiprecision::cpp_int maximum =
        (boost::multiprecision::cpp_int(1)
         << (byteLength * 8U)) - 1;

    if (value > maximum) {
        throw std::overflow_error(
            "FF1 numeral does not fit in requested byte length"
        );
    }

    std::vector<std::byte> encoded(
        byteLength,
        std::byte{0}
    );

    boost::multiprecision::cpp_int remaining = value;

    for (std::size_t i = 0; i < byteLength; ++i) {
        const unsigned int byte =
            static_cast<unsigned int>(
                (remaining & 0xff).convert_to<unsigned int>()
            );

        encoded[
            byteLength - 1U - i
        ] = static_cast<std::byte>(byte);

        remaining >>= 8U;
    }

    return encoded;
}

} // namespace indiccrypt::fpe