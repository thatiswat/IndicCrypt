cmake_minimum_required(VERSION 3.25)

project(
    indiccrypt
    VERSION 0.1.0
    DESCRIPTION "Experimental multilingual cryptographic research system"
    LANGUAGES CXX
)

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
# Core library
# ---------------------------------------------------------------------------

add_library(indiccrypt_core
    core/indiccrypt.cpp
)

target_include_directories(indiccrypt_core
    PUBLIC
        ${PROJECT_SOURCE_DIR}/include
        ${ICU_INCLUDE_DIR}
)

target_link_libraries(indiccrypt_core
    PUBLIC
        ${ICU_UC_LIBRARY}
)

target_compile_features(indiccrypt_core
    PUBLIC cxx_std_23
)

if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(indiccrypt_core PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wconversion
        -Wshadow
    )
endif()

# ---------------------------------------------------------------------------
# Tests
# ---------------------------------------------------------------------------

enable_testing()

add_executable(indiccrypt_tests
    tests/test_core.cpp
)

target_link_libraries(indiccrypt_tests
    PRIVATE indiccrypt_core
)

add_test(
    NAME indiccrypt_tests
    COMMAND indiccrypt_tests
)