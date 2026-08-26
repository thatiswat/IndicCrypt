cmake_minimum_required(VERSION 3.25)

project(
    indiccrypt
    VERSION 0.1.0
    DESCRIPTION "Experimental multilingual cryptographic research system"
    LANGUAGES CXX
)

# ---------------------------------------------------------------------------
# C++ Configuration
# ---------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# ---------------------------------------------------------------------------
# ICU4C
# ---------------------------------------------------------------------------

set(ICU_ROOT "/usr/local/opt/icu4c@78")

find_path(
    ICU_INCLUDE_DIR
    NAMES unicode/unistr.h
    PATHS "${ICU_ROOT}/include"
    NO_DEFAULT_PATH
)

find_library(
    ICU_UC_LIBRARY
    NAMES icuuc
    PATHS "${ICU_ROOT}/lib"
    NO_DEFAULT_PATH
)

if(NOT ICU_INCLUDE_DIR OR NOT ICU_UC_LIBRARY)
    message(FATAL_ERROR "ICU4C 78 was not found")
endif()

message(STATUS "ICU include: ${ICU_INCLUDE_DIR}")
message(STATUS "ICU library: ${ICU_UC_LIBRARY}")

# ---------------------------------------------------------------------------
# OpenSSL
# ---------------------------------------------------------------------------

set(OPENSSL_ROOT "/usr/local/opt/openssl@3")

find_path(
    OPENSSL_INCLUDE_DIR
    NAMES openssl/evp.h
    PATHS "${OPENSSL_ROOT}/include"
    NO_DEFAULT_PATH
)

find_library(
    OPENSSL_CRYPTO_LIBRARY
    NAMES crypto
    PATHS "${OPENSSL_ROOT}/lib"
    NO_DEFAULT_PATH
)

if(NOT OPENSSL_INCLUDE_DIR OR NOT OPENSSL_CRYPTO_LIBRARY)
    message(FATAL_ERROR "OpenSSL 3 was not found")
endif()

message(STATUS "OpenSSL include: ${OPENSSL_INCLUDE_DIR}")
message(STATUS "OpenSSL crypto: ${OPENSSL_CRYPTO_LIBRARY}")

# ---------------------------------------------------------------------------
# IndicCrypt Core Library
# ---------------------------------------------------------------------------

add_library(indiccrypt_core
    core/indiccrypt.cpp

    core/text/indic_text.cpp
    core/text/grapheme.cpp
    core/text/script.cpp
    core/text/language.cpp
    core/text/symbol.cpp
    core/text/canonicalizer.cpp

    core/crypto/types.cpp
    core/crypto/kdf.cpp
    core/crypto/aes_gcm.cpp
    core/crypto/aes_block.cpp

    core/fpe/alphabet.cpp
    core/fpe/codec.cpp
    core/fpe/radix.cpp
    core/fpe/domain.cpp
    core/fpe/parameters.cpp
    core/fpe/ff1.cpp
    core/fpe/numeral.cpp
    core/fpe/numeral_math.cpp
    core/fpe/prf.cpp
    core/fpe/parameter_block.cpp
    core/fpe/q_block.cpp
    core/fpe/numeral_encoding.cpp
    core/fpe/y.cpp
    core/fpe/y_number.cpp
    core/fpe/round_value.cpp
    core/fpe/round.cpp
)

target_include_directories(indiccrypt_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include

    PRIVATE
        ${ICU_INCLUDE_DIR}
        ${OPENSSL_INCLUDE_DIR}
)

target_link_libraries(indiccrypt_core
    PRIVATE
        ${ICU_UC_LIBRARY}
        ${OPENSSL_CRYPTO_LIBRARY}
)

target_compile_features(indiccrypt_core
    PUBLIC
        cxx_std_23
)

# ---------------------------------------------------------------------------
# Compiler Warnings
# ---------------------------------------------------------------------------

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(indiccrypt_core
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wshadow
    )
endif()

# ---------------------------------------------------------------------------
# Testing
# ---------------------------------------------------------------------------

enable_testing()

# ---------------------------------------------------------------------------
# Core Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_tests
    tests/test_core.cpp
)

target_link_libraries(indiccrypt_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_core_tests
    COMMAND indiccrypt_tests
)

# ---------------------------------------------------------------------------
# IndicText Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_text_tests
    tests/text/test_indic_text.cpp
)

target_link_libraries(indiccrypt_text_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_text_tests
    COMMAND indiccrypt_text_tests
)

# ---------------------------------------------------------------------------
# Grapheme Engine Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_grapheme_tests
    tests/text/test_grapheme.cpp
)

target_link_libraries(indiccrypt_grapheme_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_grapheme_tests
    COMMAND indiccrypt_grapheme_tests
)

# ---------------------------------------------------------------------------
# Script Engine Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_script_tests
    tests/text/test_script.cpp
)

target_link_libraries(indiccrypt_script_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_script_tests
    COMMAND indiccrypt_script_tests
)

# ---------------------------------------------------------------------------
# Language Registry Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_language_tests
    tests/text/test_language.cpp
)

target_link_libraries(indiccrypt_language_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_language_tests
    COMMAND indiccrypt_language_tests
)

# ---------------------------------------------------------------------------
# IndicSymbol Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_symbol_tests
    tests/text/test_symbol.cpp
)

target_link_libraries(indiccrypt_symbol_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_symbol_tests
    COMMAND indiccrypt_symbol_tests
)

# ---------------------------------------------------------------------------
# Canonicalizer Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_canonicalizer_tests
    tests/text/test_canonicalizer.cpp
)

target_link_libraries(indiccrypt_canonicalizer_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_canonicalizer_tests
    COMMAND indiccrypt_canonicalizer_tests
)

# ---------------------------------------------------------------------------
# Cryptographic Type Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_crypto_types_tests
    tests/crypto/test_crypto_types.cpp
)

target_link_libraries(indiccrypt_crypto_types_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_crypto_types_tests
    COMMAND indiccrypt_crypto_types_tests
)

# ---------------------------------------------------------------------------
# Key Derivation Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_kdf_tests
    tests/crypto/test_kdf.cpp
)

target_link_libraries(indiccrypt_kdf_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_kdf_tests
    COMMAND indiccrypt_kdf_tests
)

# ---------------------------------------------------------------------------
# HKDF Verification Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_hkdf_tests
    tests/crypto/test_hkdf_rfc5869.cpp
)

target_link_libraries(indiccrypt_hkdf_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_hkdf_tests
    COMMAND indiccrypt_hkdf_tests
)

# ---------------------------------------------------------------------------
# Key Lifecycle Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_key_lifecycle_tests
    tests/crypto/test_key_lifecycle.cpp
)

target_link_libraries(indiccrypt_key_lifecycle_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_key_lifecycle_tests
    COMMAND indiccrypt_key_lifecycle_tests
)

# ---------------------------------------------------------------------------
# AES-GCM Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_aes_gcm_tests
    tests/crypto/test_aes_gcm.cpp
)

target_link_libraries(indiccrypt_aes_gcm_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_aes_gcm_tests
    COMMAND indiccrypt_aes_gcm_tests
)

# ---------------------------------------------------------------------------
# AES Block Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_aes_block_tests
    tests/crypto/test_aes_block.cpp
)

target_link_libraries(indiccrypt_aes_block_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_aes_block_tests
    COMMAND indiccrypt_aes_block_tests
)

# ---------------------------------------------------------------------------
# FPE Alphabet Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_alphabet_tests
    tests/fpe/test_fpe_alphabet.cpp
)

target_link_libraries(indiccrypt_fpe_alphabet_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_alphabet_tests
    COMMAND indiccrypt_fpe_alphabet_tests
)

# ---------------------------------------------------------------------------
# FPE Codec Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_codec_tests
    tests/fpe/test_fpe_codec.cpp
)

target_link_libraries(indiccrypt_fpe_codec_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_codec_tests
    COMMAND indiccrypt_fpe_codec_tests
)

# ---------------------------------------------------------------------------
# FPE Radix Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_radix_tests
    tests/fpe/test_fpe_radix.cpp
)

target_link_libraries(indiccrypt_fpe_radix_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_radix_tests
    COMMAND indiccrypt_fpe_radix_tests
)

# ---------------------------------------------------------------------------
# FPE Domain Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_domain_tests
    tests/fpe/test_fpe_domain.cpp
)

target_link_libraries(indiccrypt_fpe_domain_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_domain_tests
    COMMAND indiccrypt_fpe_domain_tests
)

# ---------------------------------------------------------------------------
# FPE Parameters Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_parameters_tests
    tests/fpe/test_fpe_parameters.cpp
)

target_link_libraries(indiccrypt_fpe_parameters_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_parameters_tests
    COMMAND indiccrypt_fpe_parameters_tests
)

# ---------------------------------------------------------------------------
# FF1 Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_ff1_tests
    tests/fpe/test_ff1.cpp
)

target_link_libraries(indiccrypt_ff1_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_ff1_tests
    COMMAND indiccrypt_ff1_tests
)

# ---------------------------------------------------------------------------
# FPE Numeral Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_numeral_tests
    tests/fpe/test_fpe_numeral.cpp
)

target_link_libraries(indiccrypt_fpe_numeral_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_numeral_tests
    COMMAND indiccrypt_fpe_numeral_tests
)

# ---------------------------------------------------------------------------
# Numeral Math Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_numeral_math_tests
    tests/fpe/test_numeral_math.cpp
)

target_link_libraries(indiccrypt_numeral_math_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_numeral_math_tests
    COMMAND indiccrypt_numeral_math_tests
)

# ---------------------------------------------------------------------------
# FPE PRF Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_prf_tests
    tests/fpe/test_fpe_prf.cpp
)

target_link_libraries(indiccrypt_fpe_prf_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_prf_tests
    COMMAND indiccrypt_fpe_prf_tests
)

# ---------------------------------------------------------------------------
# FPE Parameter Block Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_parameter_block_tests
    tests/fpe/test_fpe_parameter_block.cpp
)

target_link_libraries(indiccrypt_fpe_parameter_block_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_parameter_block_tests
    COMMAND indiccrypt_fpe_parameter_block_tests
)

# ---------------------------------------------------------------------------
# FPE Q-Block Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_q_block_tests
    tests/fpe/test_fpe_q_block.cpp
)

target_link_libraries(indiccrypt_fpe_q_block_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_q_block_tests
    COMMAND indiccrypt_fpe_q_block_tests
)

# ---------------------------------------------------------------------------
# FPE Numeral Encoding Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_numeral_encoding_tests
    tests/fpe/test_fpe_numeral_encoding.cpp
)

target_link_libraries(indiccrypt_fpe_numeral_encoding_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_numeral_encoding_tests
    COMMAND indiccrypt_fpe_numeral_encoding_tests
)

# ---------------------------------------------------------------------------
# FPE Y Generator Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_y_tests
    tests/fpe/test_fpe_y.cpp
)

target_link_libraries(indiccrypt_fpe_y_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_y_tests
    COMMAND indiccrypt_fpe_y_tests
)

# ---------------------------------------------------------------------------
# FPE Y Number Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_y_number_tests
    tests/fpe/test_fpe_y_number.cpp
)

target_link_libraries(indiccrypt_fpe_y_number_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_y_number_tests
    COMMAND indiccrypt_fpe_y_number_tests
)

# ---------------------------------------------------------------------------
# FPE Round Value Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_round_value_tests
    tests/fpe/test_fpe_round_value.cpp
)

target_link_libraries(indiccrypt_fpe_round_value_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_round_value_tests
    COMMAND indiccrypt_fpe_round_value_tests
)

# ---------------------------------------------------------------------------
# FPE Round Tests
# ---------------------------------------------------------------------------

add_executable(indiccrypt_fpe_round_tests
    tests/fpe/test_fpe_round.cpp
)

target_link_libraries(indiccrypt_fpe_round_tests
    PRIVATE
        indiccrypt_core
)

add_test(
    NAME indiccrypt_fpe_round_tests
    COMMAND indiccrypt_fpe_round_tests
)