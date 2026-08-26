# IndicCrypt

**IndicCrypt** is a C++ cryptographic library focused on secure encryption and
format-preserving encryption for Indic-language text.

The project combines a structured Indic text-processing layer with modern
cryptographic primitives and an implementation of the foundations required for
FF1-style format-preserving encryption.

> **Status:** Active development  
> **Current test status:** 25/25 tests passing  
> **Language:** C++23

---

## Vision

Indic languages have linguistic structures that are not well represented by
treating text as arbitrary bytes.

IndicCrypt is being designed around a different model:

```text
Indic Text
    │
    ▼
Unicode / Grapheme Processing
    │
    ▼
Indic Symbols
    │
    ▼
Language / Script Aware Domain
    │
    ▼
Format-Preserving Encryption
    │
    ▼
Encrypted Indic Text
````

The long-term goal is to allow applications to protect sensitive Indic text
while preserving its structural representation.

---

## Current Status

IndicCrypt is being developed incrementally with independently tested layers.

### Completed

* Unicode foundation using ICU4C
* Grapheme processing
* Indic script model
* Indic language model
* `IndicSymbol`
* Text canonicalization
* Cryptographic byte/key types
* Key lifecycle primitives
* KDF
* HKDF
* AES block primitives
* AES-256-GCM
* FPE alphabet abstraction
* FPE codec
* FPE radix
* FPE domain
* FPE parameters
* Arbitrary-precision FPE numerals
* FPE numeral mathematics
* FF1 parameter infrastructure
* FF1 PRF foundation
* FF1 `P` parameter block
* FF1 `Q` block foundation
* Arbitrary-precision numeral encoding
* 25 automated test suites

### Currently in development

The actual FF1 encryption/decryption round construction is still being
implemented.

The current development path is:

```text
P
+
Q
│
▼
PRF
│
▼
Y
│
▼
FF1 Round Function
│
▼
10 Feistel Rounds
│
▼
Encryption / Decryption
│
▼
Known-Answer Test Vectors
```

IndicCrypt should not be considered production-ready cryptographic software
until the complete algorithm has been implemented, independently validated,
and reviewed.

---

# Architecture

```text
IndicCrypt
│
├── core/
│   ├── crypto/
│   │   ├── AES
│   │   ├── AES-GCM
│   │   ├── KDF
│   │   ├── HKDF
│   │   └── key lifecycle
│   │
│   ├── text/
│   │   ├── canonicalizer
│   │   ├── grapheme
│   │   ├── language
│   │   ├── script
│   │   └── symbol
│   │
│   └── fpe/
│       ├── alphabet
│       ├── codec
│       ├── domain
│       ├── radix
│       ├── parameters
│       ├── numeral
│       ├── numeral math
│       ├── PRF
│       ├── parameter block
│       ├── Q block
│       └── numeral encoding
│
├── include/
│   └── indiccrypt/
│       ├── crypto/
│       ├── text/
│       └── fpe/
│
├── tests/
│   ├── crypto/
│   ├── text/
│   └── fpe/
│
└── research/
    ├── crypto-design.md
    ├── search-security.md
    └── threat-model.md
```

---

# Text Processing Layer

IndicCrypt does not treat Indic text simply as a sequence of bytes.

The text layer provides abstractions for:

* Unicode-aware processing
* Grapheme boundaries
* Script information
* Language information
* Canonicalization
* Structured symbols

The primary symbol abstraction is:

```cpp
indiccrypt::text::IndicSymbol
```

Each symbol contains:

```text
Symbol ID
Grapheme
Script
Language
```

This allows the FPE layer to operate on a controlled symbol domain rather than
directly operating on arbitrary UTF-8 byte sequences.

---

# Cryptographic Layer

IndicCrypt uses established cryptographic primitives rather than attempting
to implement fundamental primitives from scratch where an established
cryptographic library is appropriate.

The current cryptographic layer includes:

```text
Key
│
├── KDF
├── HKDF
└── AES
     ├── AES block operations
     └── AES-256-GCM
```

AES-GCM is implemented through OpenSSL's EVP interface.

The cryptographic layer includes validation and negative tests for invalid
parameters and authentication failures.

---

# Format-Preserving Encryption

The FPE subsystem is designed around a domain abstraction.

```text
IndicSymbol
     │
     ▼
FpeAlphabet
     │
     ▼
FpeCodec
     │
     ▼
FpeRadix
     │
     ▼
FpeDomain
     │
     ▼
FpeParameters
     │
     ▼
FF1
```

## FpeAlphabet

Defines the symbols that belong to an encryption domain.

```cpp
FpeAlphabet(
    Script script,
    std::vector<IndicSymbol> symbols,
    std::string version
);
```

The alphabet provides deterministic symbol-to-index mapping.

---

## FpeCodec

Converts between:

```text
IndicSymbol[]
```

and:

```text
FpeRadix::Value[]
```

This separates the representation of text from the mathematical representation
used by FPE.

---

## FpeRadix

Represents the numerical domain used by FPE.

It validates that every digit satisfies:

```text
0 <= digit < radix
```

---

## FpeDomain

Combines:

```text
Alphabet
Codec
Radix
```

into a single encryption domain.

---

## FpeParameters

Carries the parameters required by FF1:

```text
Domain
Key
Tweak
Length constraints
```

---

# Arbitrary-Precision Numerals

FF1 requires arithmetic that can exceed native integer sizes.

IndicCrypt therefore uses:

```text
Boost.Multiprecision::cpp_int
```

through the `Ff1Numeral` abstraction.

Supported operations include:

```cpp
toInteger()
fromInteger()
digits()
radix()
size()
empty()
```

The numeral layer supports values substantially larger than 64-bit integers.

This is important for FPE domains with large radix powers.

---

# FF1 Foundation

The FF1 subsystem currently contains the building blocks required for the
algorithm.

```text
Ff1
│
├── Ff1Numeral
├── Ff1NumeralMath
├── Ff1NumeralEncoding
├── Ff1Prf
├── Ff1ParameterBlock
└── Ff1QBlock
```

## Parameter block

The FF1 `P` block is represented as a fixed 16-byte structure.

Its construction is independently tested.

## Q block

The project also contains the Q-block construction layer, which is being
refined toward the complete FF1 specification.

## PRF

The FF1 PRF layer is independently tested and will be used by the final
round-function implementation.

## Numeral encoding

Arbitrary-precision FF1 numerals can be encoded into fixed-width big-endian
byte sequences.

---

# Testing

IndicCrypt uses CTest with CMake/Ninja.

Current status:

```text
25 / 25 tests passing
100% tests passed
```

The test suite covers the major layers of the project.

Examples include:

```text
indiccrypt_core_tests
indiccrypt_text_tests
indiccrypt_grapheme_tests
indiccrypt_script_tests
indiccrypt_language_tests
indiccrypt_symbol_tests
indiccrypt_canonicalizer_tests

indiccrypt_crypto_types_tests
indiccrypt_kdf_tests
indiccrypt_hkdf_tests
indiccrypt_key_lifecycle_tests
indiccrypt_aes_gcm_tests
indiccrypt_aes_block_tests

indiccrypt_fpe_alphabet_tests
indiccrypt_fpe_codec_tests
indiccrypt_fpe_radix_tests
indiccrypt_fpe_domain_tests
indiccrypt_fpe_parameters_tests
indiccrypt_fpe_numeral_tests
indiccrypt_numeral_math_tests

indiccrypt_ff1_tests
indiccrypt_fpe_prf_tests
indiccrypt_fpe_parameter_block_tests
indiccrypt_fpe_q_block_tests
indiccrypt_fpe_numeral_encoding_tests
```

Run the complete test suite with:

```bash
ctest --test-dir build --output-on-failure
```

Expected result:

```text
100% tests passed
```

---

# Building

## Requirements

IndicCrypt currently requires:

* C++23-compatible compiler
* CMake
* Ninja
* OpenSSL 3
* ICU4C
* Boost

The project is currently being developed and tested on macOS.

---

## Dependencies

### OpenSSL

Used for cryptographic primitives and AES-GCM.

### ICU4C

Used for Unicode-aware text processing.

### Boost.Multiprecision

Used for arbitrary-precision integer arithmetic required by the FPE layer.

---

# Build

Clone the repository:

```bash
git clone https://github.com/thatiswat/IndicCrypt.git
cd IndicCrypt
```

Configure:

```bash
cmake -S . -B build -G Ninja
```

Build:

```bash
cmake --build build
```

Run tests:

```bash
ctest --test-dir build --output-on-failure
```

---

# Development Workflow

IndicCrypt is intentionally being built in small, independently tested
milestones.

The current progression is approximately:

```text
Unicode foundation
        ↓
Indic text model
        ↓
Cryptographic primitives
        ↓
FPE domain
        ↓
Arbitrary precision numerals
        ↓
FF1 infrastructure
        ↓
P block
        ↓
Q block
        ↓
NUM_radix encoding
        ↓
FF1 round function
        ↓
Complete FF1
        ↓
Indic text integration
        ↓
Validation / test vectors
        ↓
Security review
```

Every major component is expected to have its own tests before it becomes part
of the next layer.

---

# Security

Security is a primary design goal.

The repository contains ongoing research and design documentation:

```text
research/crypto-design.md
research/search-security.md
research/threat-model.md
```

These documents describe the current security assumptions, design decisions,
and threat model.

However:

> **IndicCrypt is currently experimental software and should not be used to
> protect production secrets.**

A complete implementation requires:

* Complete FF1 implementation
* Official known-answer test vectors
* Interoperability testing
* Boundary-condition testing
* Negative/security testing
* Independent cryptographic review
* Careful API and key-management review

Passing the project's unit tests does **not** constitute cryptographic
validation.

---

# Design Principles

IndicCrypt follows several principles:

### 1. Structured text

Indic text is represented through language, script, grapheme, and symbol
abstractions.

### 2. Separation of concerns

Text processing, domain encoding, cryptography, and FPE mathematics are kept in
separate layers.

### 3. Established cryptography

Cryptographic primitives should rely on established implementations such as
OpenSSL where appropriate.

### 4. Explicit validation

Invalid keys, nonces, tags, radix digits, lengths, and numerical values should
be rejected explicitly.

### 5. Arbitrary precision

FPE arithmetic should not be artificially constrained by native integer sizes.

### 6. Test-driven development

Each subsystem is introduced together with independent tests.

### 7. No premature production claims

The project will not be considered production-ready merely because the current
test suite passes.

---

# Roadmap

## Phase 1 — Foundation

* [x] Repository structure
* [x] CMake build system
* [x] ICU4C integration
* [x] Unicode foundation
* [x] Indic text model
* [x] Text canonicalization

## Phase 2 — Cryptography

* [x] Crypto types
* [x] Key lifecycle
* [x] KDF
* [x] HKDF
* [x] AES block primitive
* [x] AES-256-GCM
* [ ] Expanded cryptographic validation

## Phase 3 — FPE Domain

* [x] Alphabet
* [x] Codec
* [x] Radix
* [x] Domain
* [x] Parameters
* [x] Numeral representation
* [x] Numeral mathematics

## Phase 4 — FF1

* [x] FF1 API
* [x] FF1 PRF foundation
* [x] FF1 P block
* [x] FF1 Q block foundation
* [x] Arbitrary-precision numeral encoding
* [ ] Exact P/Q integration
* [ ] Complete PRF input construction
* [ ] FF1 round function
* [ ] FF1 encryption
* [ ] FF1 decryption
* [ ] Official test vectors
* [ ] Interoperability testing

## Phase 5 — Indic FPE

* [ ] Indic alphabet registry
* [ ] Language-specific domains
* [ ] Script-specific domains
* [ ] Grapheme-preserving encryption
* [ ] Indic text encrypt/decrypt API

## Phase 6 — Security & Release

* [ ] Extensive negative testing
* [ ] Fuzz testing
* [ ] Differential testing
* [ ] Performance benchmarks
* [ ] Security review
* [ ] API stabilization
* [ ] Documentation
* [ ] Release candidate

---

# Project Status

```text
Current milestone: FF1 foundation

Tests:             25 / 25
Build:             Passing
Text foundation:   Complete
Crypto foundation: Complete
FPE foundation:    Complete
FF1 implementation: In progress
Production ready:  No
```

---

# License

License information will be added as the project approaches its first public
release.

```

This README now accurately reflects the project at **25/25 tests** without pretending that the actual FF1 encryption/decryption engine is complete.
```
