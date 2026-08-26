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