# IndicCrypt Architecture

## 1. Overview

IndicCrypt is structured as a layered C++ cryptographic library for
Unicode-aware and Indic-language format-preserving encryption.

The architecture separates:

1. Unicode and Indic text processing
2. Cryptographic primitives
3. FPE domain representation
4. Arbitrary-precision numerical operations
5. FF1 construction
6. Application-facing encryption/decryption

The intended data flow is:

```text
                    ┌─────────────────────┐
                    │      Input Text     │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │   Text Processing   │
                    │                     │
                    │ ICU4C               │
                    │ Grapheme            │
                    │ Script              │
                    │ Language            │
                    │ Canonicalization    │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │    IndicSymbol[]    │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │    FPE Domain       │
                    │                     │
                    │ Alphabet            │
                    │ Codec               │
                    │ Radix               │
                    │ Domain              │
                    │ Parameters          │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │     FF1 Engine      │
                    │                     │
                    │ Numeral             │
                    │ P / Q               │
                    │ PRF                 │
                    │ Round Function      │
                    │ Feistel Network     │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │   Encrypted Digits  │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │    FPE Codec        │
                    └──────────┬──────────┘
                               │
                               ▼
                    ┌─────────────────────┐
                    │   Output Text       │
                    └─────────────────────┘
````

---

# 2. Repository Structure

```text
IndicCrypt/
│
├── CMakeLists.txt
├── README.md
├── ARCHITECTURE.md
├── LICENSE
├── .gitignore
│
├── research/
│   ├── crypto-design.md
│   ├── search-security.md
│   └── threat-model.md
│
├── include/
│   └── indiccrypt/
│       │
│       ├── crypto/
│       │   ├── types.hpp
│       │   ├── kdf.hpp
│       │   ├── hkdf.hpp
│       │   ├── key_lifecycle.hpp
│       │   ├── aes_block.hpp
│       │   └── aes_gcm.hpp
│       │
│       ├── text/
│       │   ├── canonicalizer.hpp
│       │   ├── grapheme.hpp
│       │   ├── language.hpp
│       │   ├── script.hpp
│       │   └── symbol.hpp
│       │
│       └── fpe/
│           ├── alphabet.hpp
│           ├── codec.hpp
│           ├── domain.hpp
│           ├── radix.hpp
│           ├── parameters.hpp
│           ├── numeral.hpp
│           ├── numeral_math.hpp
│           ├── prf.hpp
│           ├── parameter_block.hpp
│           ├── q_block.hpp
│           ├── numeral_encoding.hpp
│           └── ff1.hpp
│
├── core/
│   │
│   ├── crypto/
│   │   ├── types.cpp
│   │   ├── kdf.cpp
│   │   ├── hkdf.cpp
│   │   ├── key_lifecycle.cpp
│   │   ├── aes_block.cpp
│   │   └── aes_gcm.cpp
│   │
│   ├── text/
│   │   ├── canonicalizer.cpp
│   │   ├── grapheme.cpp
│   │   ├── language.cpp
│   │   ├── script.cpp
│   │   └── symbol.cpp
│   │
│   └── fpe/
│       ├── alphabet.cpp
│       ├── codec.cpp
│       ├── domain.cpp
│       ├── radix.cpp
│       ├── parameters.cpp
│       ├── numeral.cpp
│       ├── numeral_math.cpp
│       ├── prf.cpp
│       ├── parameter_block.cpp
│       ├── q_block.cpp
│       ├── numeral_encoding.cpp
│       └── ff1.cpp
│
└── tests/
    │
    ├── crypto/
    ├── text/
    └── fpe/
```

---

# 3. Layered Architecture

IndicCrypt is intentionally divided into layers.

```text
┌───────────────────────────────────────────┐
│             Application API               │
├───────────────────────────────────────────┤
│             Indic FPE Layer               │
├───────────────────────────────────────────┤
│                FF1 Layer                  │
├───────────────────────────────────────────┤
│             FPE Domain Layer              │
├───────────────────────────────────────────┤
│       Arbitrary Precision Mathematics     │
├───────────────────────────────────────────┤
│           Cryptographic Layer             │
├───────────────────────────────────────────┤
│          Indic Text Processing            │
├───────────────────────────────────────────┤
│       External Cryptographic / Unicode    │
│              Dependencies                 │
└───────────────────────────────────────────┘
```

Each layer should depend only on lower-level abstractions where practical.

---

# 4. Text Processing Layer

Location:

```text
include/indiccrypt/text/
core/text/
```

The text layer provides the representation of Indic text used by the FPE
layer.

## Components

### `IndicSymbol`

Represents a logical symbol in the Indic text domain.

```cpp
class IndicSymbol {
public:
    SymbolId id() const noexcept;

    const std::string& grapheme() const noexcept;

    Script script() const noexcept;

    Language language() const noexcept;
};
```

A symbol contains:

```text
Symbol ID
Grapheme
Script
Language
```

The purpose is to prevent cryptographic code from having to understand the
details of Unicode representation.

---

## `Script`

Represents the script associated with a symbol.

Examples include the Indic scripts represented by the project's script model.

The FPE layer can use script information when selecting an alphabet.

---

## `Language`

Represents the language associated with a symbol.

The current language model includes:

```text
Assamese
Bengali
Bodo
Dogri
Gujarati
Hindi
Kannada
Kashmiri
Konkani
Maithili
Malayalam
Manipuri
Marathi
Nepali
Odia
Punjabi
Sanskrit
Santali
Sindhi
Tamil
Telugu
Urdu
```

---

## `Grapheme`

The grapheme layer handles user-perceived text units using Unicode-aware
processing.

This is important for Indic text because a visible character may consist of
multiple Unicode code points.

The FPE layer should therefore operate on the project's logical symbol
representation rather than blindly splitting UTF-8 bytes.

---

## `Canonicalizer`

Canonicalization establishes a deterministic representation before text enters
the encryption domain.

Conceptually:

```text
Input Unicode
      │
      ▼
Canonicalization
      │
      ▼
Stable logical representation
```

---

# 5. Cryptographic Layer

Location:

```text
include/indiccrypt/crypto/
core/crypto/
```

This layer provides the cryptographic primitives used by higher-level
components.

It currently contains:

```text
Crypto Types
KDF
HKDF
Key Lifecycle
AES Block
AES-256-GCM
```

---

## Key Types

Cryptographic data is represented through dedicated types rather than generic
containers wherever possible.

Examples include:

```text
Key
Nonce
Ciphertext
AuthenticationTag
ByteVector
```

This helps prevent accidental mixing of cryptographic values.

---

# 6. Key Lifecycle

Key lifecycle management is separated from the encryption algorithm.

The architecture distinguishes:

```text
Key creation
     │
     ▼
Key derivation
     │
     ▼
Key usage
     │
     ▼
Key destruction / lifecycle
```

FF1 receives an already-established key through `FpeParameters`.

The FF1 layer should not be responsible for application-level key storage.

---

# 7. AES-GCM

AES-GCM is implemented through OpenSSL's EVP API.

The current interface is conceptually:

```cpp
AesGcmCiphertext encrypt(
    const Key& key,
    const Nonce& nonce,
    std::span<const Byte> plaintext,
    std::span<const Byte> aad
);

ByteVector decrypt(
    const Key& key,
    const Nonce& nonce,
    const Ciphertext& ciphertext,
    const AuthenticationTag& tag,
    std::span<const Byte> aad
);
```

AES-GCM is primarily used as a general authenticated encryption primitive.

It is architecturally separate from the FF1 Feistel construction.

---

# 8. FPE Domain Layer

Location:

```text
include/indiccrypt/fpe/
core/fpe/
```

The FPE domain layer converts structured symbols into mathematical digits.

The architecture is:

```text
IndicSymbol[]
       │
       ▼
FpeAlphabet
       │
       ▼
FpeCodec
       │
       ▼
FpeRadix::Value[]
```

---

# 9. FpeAlphabet

`FpeAlphabet` defines the valid symbols in an encryption domain.

It contains:

```text
Script
Symbol collection
Version
```

It provides:

```cpp
radix()
symbols()
indexOf()
symbolAt()
```

The alphabet must provide deterministic mapping:

```text
Symbol
  │
  ▼
Index
```

and the inverse:

```text
Index
  │
  ▼
Symbol
```

This mapping is fundamental to format-preserving encryption.

---

# 10. FpeCodec

The codec converts between symbols and radix digits.

Conceptually:

```text
IndicSymbol
     │
     ▼
FpeCodec
     │
     ▼
Radix digit
```

and:

```text
Radix digit
     │
     ▼
FpeCodec
     │
     ▼
IndicSymbol
```

This keeps text representation independent from the FF1 mathematical engine.

---

# 11. FpeRadix

`FpeRadix` defines the numerical base of an FPE domain.

```cpp
using Value = std::size_t;
```

It provides:

```cpp
value()
contains()
validate()
```

The fundamental invariant is:

```text
0 <= digit < radix
```

---

# 12. FpeDomain

`FpeDomain` combines the alphabet, codec, and radix.

```text
             FpeDomain
          ┌──────┼──────┐
          │      │      │
          ▼      ▼      ▼
      Alphabet  Codec  Radix
```

It provides:

```cpp
encode(symbols)
decode(digits)
```

This is the bridge between the text representation and the mathematical FPE
representation.

---

# 13. FpeParameters

`FpeParameters` provides the configuration required by FF1.

```text
FpeParameters
├── Domain
├── Key
└── Tweak
```

It also provides length validation.

The FF1 implementation receives these parameters rather than constructing
cryptographic configuration internally.

---

# 14. Arbitrary-Precision Numeral Layer

Location:

```text
core/fpe/numeral.cpp
core/fpe/numeral_math.cpp
```

FF1 requires operations over numbers represented in arbitrary radix domains.

`Ff1Numeral` wraps:

```text
FpeRadix
+
vector<Digit>
```

and supports conversion to and from:

```text
boost::multiprecision::cpp_int
```

---

## Numeral conversion

Forward:

```text
digits
  │
  ▼
NUM_radix
  │
  ▼
cpp_int
```

Reverse:

```text
cpp_int
  │
  ▼
radix conversion
  │
  ▼
digits
```

This provides the mathematical foundation required by FF1.

---

# 15. Numeral Mathematics

`Ff1NumeralMath` provides operations such as:

```cpp
radixPower()
modulus()
addModulo()
```

Conceptually:

```text
radix^n
```

and:

```text
x mod radix^n
```

are handled using arbitrary precision.

This prevents overflow for large FPE domains.

---

# 16. FF1 Layer

Location:

```text
include/indiccrypt/fpe/
core/fpe/
```

The FF1 layer is the core FPE algorithmic layer.

Current architecture:

```text
                    Ff1
                     │
       ┌─────────────┼─────────────┐
       │             │             │
       ▼             ▼             ▼
 Parameters       Numeral         PRF
       │             │             │
       │             │             │
       ▼             ▼             ▼
       P             NUM           PRF
       │             │             │
       └─────────────┼─────────────┘
                     │
                     ▼
                     Q
                     │
                     ▼
                   P || Q
                     │
                     ▼
                     Y
                     │
                     ▼
               FF1 Round Function
```

The complete round integration is still under development.

---

# 17. FF1 Parameter Block

`Ff1ParameterBlock` constructs the fixed-size FF1 `P` structure.

The current implementation produces the 16-byte parameter block required by the
FF1 construction.

Conceptually:

```text
P =
┌────────────────────────────────────┐
│ FF1 fixed parameters               │
├────────────────────────────────────┤
│ Version / method fields            │
│ Radix                              │
│ Number of rounds                   │
│ Message split information          │
│ Message length                    │
│ Tweak length                       │
└────────────────────────────────────┘
```

The implementation is independently tested.

---

# 18. FF1 Q Block

`Ff1QBlock` represents the per-message portion of the FF1 PRF input.

The intended flow is:

```text
Tweak
   │
   ▼
Padding / round information
   │
   ▼
Numeral representation
   │
   ▼
Q
```

The Q implementation is currently a foundation for the final exact FF1
construction and is not yet considered the completed production FF1 round
implementation.

---

# 19. FF1 PRF

The FF1 PRF layer provides the cryptographic pseudorandom function used by the
round construction.

Conceptually:

```text
Key
 │
 ▼
AES-based PRF
 │
 ▼
P || Q
 │
 ▼
PRF output
 │
 ▼
Y
```

The PRF itself is independently tested.

---

# 20. Numeral Encoding

`Ff1NumeralEncoding` converts arbitrary-precision numeral values into
fixed-width big-endian byte sequences.

Conceptually:

```text
Ff1Numeral
     │
     ▼
cpp_int
     │
     ▼
big-endian encoding
     │
     ▼
byte sequence
```

This component is independently tested, including:

* zero
* ordinary values
* large values
* deterministic output
* insufficient output size

---

# 21. Current FF1 Pipeline

The intended complete pipeline is:

```text
Input symbols
      │
      ▼
FpeCodec
      │
      ▼
Radix digits
      │
      ▼
Ff1Numeral
      │
      ▼
        ┌───────────────┐
        │ FF1           │
        │               │
        │ P             │
        │ Q             │
        │ PRF           │
        │ Y             │
        │ Round         │
        │ Feistel       │
        └───────┬───────┘
                │
                ▼
          Cipher digits
                │
                ▼
            FpeCodec
                │
                ▼
        Encrypted symbols
```

---

# 22. Encryption / Decryption Boundary

The intended public abstraction is:

```cpp
Ff1 ff1(parameters);

auto ciphertext =
    ff1.encrypt(plaintext);
```

and:

```cpp
auto plaintext =
    ff1.decrypt(ciphertext);
```

The important invariant is:

```text
decrypt(encrypt(X)) == X
```

However, round-trip correctness alone is insufficient for cryptographic
validation.

The completed implementation must also be checked against independent
known-answer vectors.

---

# 23. Testing Architecture

Each architectural layer has its own test target.

```text
tests/
│
├── text/
│   ├── core
│   ├── grapheme
│   ├── script
│   ├── language
│   ├── symbol
│   └── canonicalizer
│
├── crypto/
│   ├── types
│   ├── KDF
│   ├── HKDF
│   ├── key lifecycle
│   ├── AES block
│   └── AES-GCM
│
└── fpe/
    ├── alphabet
    ├── codec
    ├── radix
    ├── domain
    ├── parameters
    ├── numeral
    ├── numeral math
    ├── FF1
    ├── PRF
    ├── parameter block
    ├── Q block
    └── numeral encoding
```

Current status:

```text
25 / 25 tests passing
```

---

# 24. Dependency Direction

The intended dependency direction is:

```text
Application
    │
    ▼
Indic FPE
    │
    ▼
FF1
    │
    ├──────────────► FPE Domain
    │
    ├──────────────► Numeral Math
    │
    └──────────────► Crypto
                         │
                         ▼
                      OpenSSL


FPE Domain
    │
    ▼
Indic Text
    │
    ▼
ICU4C
```

The cryptographic layer should not depend on the Indic text layer.

The text layer should not depend on FF1.

The FPE domain acts as the bridge between them.

---

# 25. Security Boundaries

Security-sensitive operations are isolated into dedicated components.

```text
                    Application
                         │
                         ▼
                  FPE Parameters
                    │       │
                    │       └──────► Tweak
                    │
                    └──────────────► Key
                                      │
                                      ▼
                                  FF1 / PRF
                                      │
                                      ▼
                                    AES
```

The following should never be mixed together unnecessarily:

```text
Text normalization
Key management
Cryptographic primitives
FPE mathematics
Application storage
```

This reduces the risk of introducing security bugs through cross-layer
coupling.

---

# 26. Error Handling

The library uses explicit exceptions for invalid input and invalid state.

Examples include:

```text
std::invalid_argument
std::out_of_range
std::overflow_error
std::logic_error
std::runtime_error
```

Typical validation includes:

```text
Invalid key size
Invalid nonce size
Invalid authentication tag
Invalid radix digit
Invalid FPE length
Negative numeral
Numeral overflow
Invalid FF1 round
Invalid parameter construction
```

---

# 27. External Dependencies

## ICU4C

Responsible for Unicode-aware processing.

```text
IndicCrypt
    │
    ▼
  ICU4C
```

## OpenSSL 3

Responsible for established cryptographic primitives.

```text
IndicCrypt
    │
    ▼
 OpenSSL EVP
    │
    ▼
 AES
```

## Boost.Multiprecision

Responsible for arbitrary-precision arithmetic.

```text
Ff1Numeral
    │
    ▼
Boost.Multiprecision
    │
    ▼
cpp_int
```

---

# 28. Build System

The project uses:

```text
CMake
   │
   ▼
Ninja
   │
   ▼
C++23
```

External libraries are located and linked through CMake.

The core implementation is compiled into the main IndicCrypt core target and
the individual test executables link against that target.

---

# 29. Current Development State

```text
Unicode foundation             COMPLETE
Indic text model               COMPLETE
Canonicalization               COMPLETE

Crypto types                   COMPLETE
Key lifecycle                  COMPLETE
KDF                            COMPLETE
HKDF                           COMPLETE
AES block                      COMPLETE
AES-GCM                        COMPLETE

FPE alphabet                   COMPLETE
FPE codec                      COMPLETE
FPE radix                      COMPLETE
FPE domain                     COMPLETE
FPE parameters                 COMPLETE

FPE numeral                    COMPLETE
Numeral mathematics            COMPLETE
Numeral encoding               COMPLETE

FF1 interface                  COMPLETE
FF1 PRF foundation             COMPLETE
FF1 P block                    COMPLETE
FF1 Q foundation               COMPLETE

FF1 P/Q integration             IN PROGRESS
FF1 round function              NOT COMPLETE
FF1 encryption                  NOT COMPLETE
FF1 decryption                  NOT COMPLETE
Known-answer validation         NOT COMPLETE
```

---

# 30. Immediate Development Path

The next architectural stages are:

```text
Current
  │
  ▼
Exact P || Q construction
  │
  ▼
PRF integration
  │
  ▼
Y generation
  │
  ▼
Round arithmetic
  │
  ▼
10 FF1 rounds
  │
  ▼
Encryption
  │
  ▼
Decryption
  │
  ▼
Known-answer vectors
  │
  ▼
Indic text integration
```

Only after those stages are complete should the public high-level encryption
API be considered stable.

---

# 31. Architectural Goal

The final architecture should allow IndicCrypt to support:

```text
          Indic Text
              │
              ▼
       Unicode-aware
        normalization
              │
              ▼
       Script / Language
           domain
              │
              ▼
          FPE Codec
              │
              ▼
          FF1 Engine
              │
              ▼
       Format-preserving
          ciphertext
              │
              ▼
       Same text domain
```

The key architectural property is that **Indic text representation and
cryptographic mathematics remain separate**.

This allows the FF1 engine to operate over a generic radix domain while the
Indic-specific layers determine how real-world text maps into that domain.

---

## Current Test Baseline

```text
IndicCrypt Architecture
────────────────────────────────────
Build:                 PASS
Tests:                 25 / 25
Text layer:            PASS
Crypto layer:          PASS
FPE domain:            PASS
Numeral layer:         PASS
FF1 foundation:        PASS
Complete FF1:          IN PROGRESS
Production readiness:  NO
```

````

Save it as:

```bash
touch ARCHITECTURE.md
````

then paste the above into it and commit:

```bash
git add README.md ARCHITECTURE.md
git commit -m "document IndicCrypt architecture"
git push
```