# IndicCrypt

IndicCrypt is a C++23 cryptographic library focused on **format-preserving encryption (FPE) for Indic-language text**.

The project combines:

- Unicode-aware Indic text processing
- Script and language-aware symbol modeling
- Structured encryption domains
- Modern cryptographic primitives
- Arbitrary-precision arithmetic
- FF1 format-preserving encryption

> **Status:** Active development
>
> **Current test status:** 29/29 tests passing
>
> **Language:** C++23

---

## Vision

Indic languages have linguistic structures that are not well represented by treating
text as arbitrary bytes.

IndicCrypt is designed around a structured representation:

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
FPE Radix Representation
    │
    ▼
FF1 Format-Preserving Encryption
    │
    ▼
Encrypted Symbol Domain
    │
    ▼
Encrypted Indic Text
````

The long-term goal is to allow applications to protect sensitive Indic text while
preserving its structural representation.

---

# Current Status

IndicCrypt is being developed incrementally through independently tested layers.

## Completed

### Text foundation

* [x] Unicode foundation using ICU4C
* [x] Grapheme processing
* [x] Indic script model
* [x] Indic language model
* [x] `IndicSymbol`
* [x] Text canonicalization

### Cryptographic foundation

* [x] Cryptographic byte/key types
* [x] Key lifecycle primitives
* [x] KDF
* [x] HKDF
* [x] AES block primitives
* [x] AES-256-GCM

### FPE foundation

* [x] FPE alphabet abstraction
* [x] FPE codec
* [x] FPE radix
* [x] FPE domain
* [x] FPE parameters
* [x] Arbitrary-precision FPE numerals
* [x] FPE numeral mathematics
* [x] Numeral encoding

### FF1 implementation

* [x] FF1 parameter block (`P`)
* [x] FF1 `Q` block
* [x] FF1 PRF
* [x] FF1 Y generation
* [x] FF1 Y number representation
* [x] FF1 round value arithmetic
* [x] FF1 round function
* [x] 10-round FF1 Feistel construction
* [x] FF1 encryption
* [x] FF1 decryption
* [x] Even-length message handling
* [x] Odd-length message handling
* [x] Message-length preservation
* [x] Radix-domain preservation

---

# FF1 Pipeline

The current FF1 implementation is structured as:

```text
Input
  │
  ▼
FpeParameters
  │
  ├── Domain
  ├── Key
  └── Tweak
  │
  ▼
Initial A / B split
  │
  ▼
┌──────────────────────────────┐
│       FF1 Round 0            │
│                              │
│ P + Q                        │
│   ↓                          │
│ PRF                          │
│   ↓                          │
│ Y                            │
│   ↓                          │
│ NUM(A) ± Y mod radix^m       │
└──────────────────────────────┘
  │
  ▼
Round 1
  │
  ▼
...
  │
  ▼
Round 9
  │
  ▼
Ciphertext
```

Decryption performs the same construction in reverse order using subtraction.

---

# Format Preservation

FF1 operates on radix values instead of producing arbitrary binary ciphertext.

Conceptually:

```text
Plaintext:

[0, 1, 0, 1, 0]

        │
        ▼
       FF1
        │
        ▼

Ciphertext:

[1, 0, 1, 1, 0]
```

The ciphertext remains inside the configured radix domain and preserves the
original message length.

This property is important for structured data and text domains.

---

# Odd-Length Messages

The implementation explicitly handles odd-length inputs.

Examples currently tested include:

```text
2 digits
3 digits
4 digits
5 digits
6 digits
7 digits
8 digits
9 digits
10 digits
```

For odd lengths, the FF1 halves alternate in size during the Feistel rounds.

For example, with three digits:

```text
Round 0:

A = 1 digit
B = 2 digits

        ↓

C = 1 digit

        ↓

A = previous B
B = C


Round 1:

A = 2 digits
B = 1 digit

        ↓

C = 2 digits
```

The implementation preserves the total message length throughout the process.

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
│       ├── numeral encoding
│       ├── PRF
│       ├── parameter block
│       ├── Q block
│       ├── Y
│       ├── Y number
│       ├── round value
│       ├── round
│       └── FF1
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

A symbol contains:

```text
Symbol ID
Grapheme
Script
Language
```

This allows the FPE layer to operate on a controlled symbol domain instead of
directly operating on arbitrary UTF-8 byte sequences.

---

# FPE Domain

The FPE subsystem separates text representation from mathematical encryption.

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
Ff1
```

## FpeAlphabet

Defines the symbols belonging to an encryption domain.

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

This separates text representation from the numerical representation used by FF1.

---

## FpeRadix

Represents the numerical domain used by FPE.

Each digit satisfies:

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

This allows the FPE implementation to operate on radix powers substantially larger
than 64-bit integer limits.

---

# FF1 Components

The FF1 implementation is split into small components:

```text
Ff1
 │
 ├── Ff1Numeral
 ├── Ff1NumeralMath
 ├── Ff1NumeralEncoding
 ├── Ff1Prf
 ├── Ff1ParameterBlock
 ├── Ff1QBlock
 ├── Ff1YGenerator
 ├── Ff1YNumber
 ├── Ff1RoundValue
 └── Ff1Round
```

Each component has its own tests where appropriate.

---

# Testing

IndicCrypt uses CTest with CMake/Ninja.

Current result:

```text
29/29 tests passed

100% tests passed

Total Test time ≈ 13 seconds
```

Run the complete suite:

```bash
ctest --test-dir build --output-on-failure
```

Run the FF1 integration test:

```bash
ctest --test-dir build -R indiccrypt_ff1_tests -V
```

The FF1 test suite currently verifies:

* Encryption executes successfully
* Decryption executes successfully
* Encryption is deterministic
* Decryption recovers the original plaintext
* Ciphertext remains inside the radix
* Message length is preserved
* Invalid digits are rejected
* Even-length messages
* Odd-length messages
* Multiple message lengths

---

# Build

## Requirements

IndicCrypt currently requires:

* C++23-compatible compiler
* CMake
* Ninja
* OpenSSL 3
* ICU4C
* Boost

The project is currently developed and tested on macOS.

## Configure

```bash
cmake -S . -B build -G Ninja
```

## Build

```bash
cmake --build build
```

## Test

```bash
ctest --test-dir build --output-on-failure
```

---

# Dependencies

### OpenSSL

Used for cryptographic primitives and AES-GCM.

### ICU4C

Used for Unicode-aware text processing.

### Boost.Multiprecision

Used for arbitrary-precision arithmetic required by the FPE implementation.

---

# Development Workflow

IndicCrypt is intentionally being built in independently tested milestones.

Current progression:

```text
Unicode foundation
        ↓
Indic text model
        ↓
Cryptographic primitives
        ↓
FPE domain
        ↓
Arbitrary-precision numerals
        ↓
FF1 infrastructure
        ↓
P block
        ↓
Q block
        ↓
PRF
        ↓
Y generation
        ↓
FF1 round arithmetic
        ↓
Complete FF1
        ↓
Indic text integration
        ↓
Known-answer vectors
        ↓
Differential testing
        ↓
Security review
```

The major difference from earlier stages is that **the FF1 encryption/decryption
engine is now implemented and passing its integration tests**.

---

# Security

Security is a primary design goal.

The repository contains ongoing security and design documentation:

```text
research/crypto-design.md
research/search-security.md
research/threat-model.md
```

However:

> **IndicCrypt is experimental software and should not yet be used to protect
> production secrets.**

Passing 29/29 unit and integration tests does not constitute cryptographic
validation.

Before a production release, the project still requires:

* Official FF1 known-answer test vectors
* Differential/interoperability testing
* Extensive boundary testing
* Fuzz testing
* Security review
* Cryptographic review
* API review
* Key-management review
* Performance benchmarking

---

# Design Principles

## 1. Structured text

Indic text is represented through language, script, grapheme, and symbol abstractions.

## 2. Separation of concerns

Text processing, domain encoding, cryptography, and FPE mathematics remain separate
layers.

## 3. Established cryptography

Established cryptographic primitives such as OpenSSL are preferred where appropriate.

## 4. Explicit validation

Invalid keys, lengths, radix digits, and numerical values are rejected explicitly.

## 5. Arbitrary precision

FPE arithmetic should not be artificially constrained by native integer sizes.

## 6. Test-driven development

Major subsystems are introduced with independent tests.

## 7. No premature production claims

Passing tests is a milestone, not proof that a cryptographic implementation is ready
for production.

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
* [x] Numeral encoding

## Phase 4 — FF1

* [x] FF1 API
* [x] FF1 PRF
* [x] FF1 P block
* [x] FF1 Q block
* [x] Y generation
* [x] Y number representation
* [x] Round value arithmetic
* [x] FF1 round function
* [x] FF1 encryption
* [x] FF1 decryption
* [x] Even-length handling
* [x] Odd-length handling
* [ ] Official FF1 known-answer vectors
* [ ] Differential/interoperability testing

## Phase 5 — Indic FPE

* [ ] Indic alphabet registry
* [ ] Language-specific domains
* [ ] Script-specific domains
* [ ] Grapheme-preserving encryption
* [ ] Indic text encryption API
* [ ] Indic text decryption API
* [ ] End-to-end Indic text tests

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
Current milestone:  FF1 implementation

Tests:              29 / 29
Build:              Passing
Text foundation:    Complete
Crypto foundation:  Complete
FPE foundation:     Complete
FF1 rounds:         Complete
FF1 encryption:     Complete
FF1 decryption:     Complete

Known-answer tests: Pending
Indic text API:     Pending
Security review:    Pending
Production ready:   No
```

---

# License

License information will be added as the project approaches its first public release.

```

This version matches your **actual current milestone**: the FF1 engine is no longer merely “in development”; you have reached **29/29 passing tests**. Your uploaded README's old 25/25 status and “FF1 implementation still being implemented” section are now stale. :contentReference[oaicite:1]{index=1}
```
