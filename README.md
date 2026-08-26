# INDICCRYPT

> A C++23 multilingual security framework for protecting Indic-language
> personal data through Unicode-aware processing, authenticated encryption,
> format-preserving encryption research, and controlled searchable indexes.

INDICCRYPT is an experimental security infrastructure project focused on
protecting personal data represented in Indic-language text while maintaining
a deterministic, Unicode-safe processing pipeline.

The system is designed as a layered cryptographic architecture rather than
a custom encryption algorithm. Established cryptographic primitives are
combined with an Indic-aware representation layer to investigate how
multilingual encrypted data can integrate with existing software and
database systems.

---

## Project Status

**Current phase: Cryptographic Core + Indic Text Foundation**

The Unicode and Indic text-processing pipeline is implemented.

The cryptographic foundation is implemented through:

- Strongly typed cryptographic primitives
- HKDF-SHA-256
- Domain-separated key derivation
- RFC 5869 HKDF verification
- Secure key lifecycle handling
- AES-256-GCM authenticated encryption
- Associated Authenticated Data (AAD)
- Authentication failure detection
- Tamper detection
- Key and nonce validation

Current automated verification:

```text
12 / 12 tests passing
````

The Format-Preserving Encryption layer is currently in the domain-design
phase.

---

# 1. Vision

INDICCRYPT investigates a security architecture where encrypted personal
data can remain represented through well-defined textual domains associated
with Indic scripts.

The intended architecture is:

```text
                    APPLICATION
                         │
                         ▼
                   UTF-8 INPUT
                         │
                         ▼
                ┌─────────────────┐
                │     ICU4C       │
                │ Unicode Engine  │
                └────────┬────────┘
                         │
                         ▼
                   IndicText
                         │
                         ▼
                 Grapheme Engine
                         │
                         ▼
                   Script Engine
                         │
                         ▼
                Language Registry
                         │
                         ▼
                  IndicSymbol[]
                         │
                         ▼
                  Canonicalizer
                         │
                         ▼
              CRYPTOGRAPHIC LAYER
                         │
            ┌────────────┼────────────┐
            ▼            ▼            ▼
        AES-GCM         FF1         Search
            │            │            │
            ▼            ▼            ▼
      Authenticated   FPE Domain   HMAC Index
        Ciphertext
```

---

# 2. Problem

Modern applications process personal information across multiple Indian
languages and scripts.

Traditional encryption systems operate on byte sequences and generally
produce binary ciphertext. Applications that expect textual data may need
additional encoding, storage, indexing, or integration layers.

Multilingual systems introduce additional challenges:

* Unicode normalization
* Grapheme boundaries
* Script identification
* Language metadata
* Database collation
* Text indexing
* Searchability
* Application validation
* Interoperability between services

INDICCRYPT investigates whether these concerns can be addressed through a
dedicated multilingual text-processing and cryptographic architecture.

---

# 3. Design Goals

## Unicode Safety

Process Indic text using standards-aware Unicode infrastructure instead of
treating text as an arbitrary sequence of bytes.

## Deterministic Canonicalization

Equivalent Unicode representations should reach the cryptographic boundary
through a deterministic canonical representation.

## Script Awareness

The system explicitly distinguishes language metadata from Unicode script
metadata.

```text
Language ≠ Script
```

## Multilingual Support

Provide first-class metadata for the 22 languages listed in the Eighth
Schedule of the Constitution of India.

## Cryptographic Separation

Text processing, key derivation, encryption, FPE, authentication, and
searchable indexes remain separate architectural components.

## Low-Latency Native Core

The security engine is implemented in C++23 with a focus on predictable
performance and efficient native execution.

## Database Integration

Investigate ciphertext representations that can minimize application and
database integration changes.

---

# 4. Architecture

```text
                         INDICCRYPT
                             │
             ┌───────────────┴───────────────┐
             │                               │
             ▼                               ▼
        TEXT ENGINE                    CRYPTO ENGINE
             │                               │
             ▼                               ▼
          ICU4C                          Key Types
             │                               │
             ▼                               ▼
        IndicText                         HKDF
             │                               │
             ▼                               ▼
       GraphemeEngine                  Key Domains
             │                               │
             ▼                               ▼
        ScriptEngine                     K_ENC
             │                               │
             ▼                               ▼
     LanguageRegistry                 AES-256-GCM
             │                               │
             ▼                               ▼
       IndicSymbol[]                   Ciphertext
             │
             ▼
       Canonicalizer
             │
             └───────────────┐
                             ▼
                    Future FPE Layer
                             │
                             ▼
                         FF1 Domain
```

---

# 5. Unicode Processing

## 5.1 ICU4C

INDICCRYPT uses ICU4C for Unicode-aware processing.

The current pipeline uses ICU for:

* Unicode normalization
* Grapheme segmentation
* Script identification
* Unicode property handling

---

## 5.2 NFC Normalization

Input text is normalized into Unicode Normalization Form C.

```text
Raw UTF-8
    │
    ▼
ICU4C
    │
    ▼
NFC
    │
    ▼
Canonical Unicode representation
```

Normalization is intentionally performed before the cryptographic boundary.

The cryptographic layer does not silently normalize text.

---

# 6. IndicText

`IndicText` provides the core Unicode representation.

Responsibilities include:

* UTF-8 input handling
* Unicode decoding
* NFC normalization
* Unicode-safe representation
* Conversion back to UTF-8

Example:

```cpp
auto text =
    IndicText::fromUtf8("नमस्ते भारत");
```

The text layer is independent from cryptographic operations.

---

# 7. Grapheme Engine

Indic scripts cannot safely be processed by treating every Unicode code
point as an independent user-visible character.

INDICCRYPT therefore provides grapheme-level segmentation.

```text
UTF-8
  │
  ▼
Unicode
  │
  ▼
Grapheme boundaries
  │
  ▼
GraphemeCluster[]
```

The implementation uses ICU character-break functionality.

The system explicitly accounts for ICU UTF-16 offsets when converting
segmentation results back into UTF-8 byte ranges.

---

# 8. Script Engine

The Script Engine uses ICU Unicode Script properties.

Examples:

```text
नमस्ते    → Devanagari
தமிழ்     → Tamil
ಕನ್ನಡ     → Kannada
తెలుగు    → Telugu
বাংলা     → Bengali
ગુજરાતી   → Gujarati
ਪੰਜਾਬੀ    → Gurmukhi
മലയാളം   → Malayalam
```

The architecture does not equate a language with a script.

For example, multiple languages can share a script while the same language
may have multiple writing-system considerations.

---

# 9. 22-Language Registry

INDICCRYPT provides a strongly typed registry for the 22 Scheduled
Languages:

```text
1.  Assamese
2.  Bengali
3.  Bodo
4.  Dogri
5.  Gujarati
6.  Hindi
7.  Kannada
8.  Kashmiri
9.  Konkani
10. Maithili
11. Malayalam
12. Manipuri
13. Marathi
14. Nepali
15. Odia
16. Punjabi
17. Sanskrit
18. Santali
19. Sindhi
20. Tamil
21. Telugu
22. Urdu
```

Language profiles contain metadata such as:

* Internal language identifier
* Language name
* ISO 639 metadata where applicable
* Primary script

The registry is metadata-driven and is separate from automatic language
detection.

---

# 10. IndicSymbol

`IndicSymbol` forms the bridge between linguistic processing and future
cryptographic domains.

Conceptually:

```text
IndicSymbol
├── Symbol ID
├── Grapheme
├── Script
└── Language
```

Example:

```cpp
IndicSymbol {
    id,
    "न",
    Script::Devanagari,
    Language::Hindi
}
```

The current symbol identifier is an internal deterministic identifier.

It is not intended to be a cryptographic hash or security primitive.

---

# 11. Canonicalizer

The Canonicalizer connects the text-processing pipeline.

```text
UTF-8
  │
  ▼
IndicText
  │
  ▼
GraphemeEngine
  │
  ▼
ScriptEngine
  │
  ▼
LanguageRegistry
  │
  ▼
IndicSymbol[]
```

The purpose is to establish a deterministic representation before
cryptographic processing.

This creates an explicit security boundary between:

```text
Text Semantics
      │
      ▼
Canonical Representation
      │
      ▼
Cryptography
```

---

# 12. Cryptographic Architecture

The cryptographic layer is built around established primitives.

Current components:

```text
Root Key
    │
    ▼
HKDF-SHA-256
    │
    ├───────────────┬───────────────┬───────────────┐
    ▼               ▼               ▼               ▼
 Encryption        FPE            Search         Integrity
    │               │               │
    ▼               ▼               ▼
 K_ENC           K_FPE          K_SEARCH
    │               │               │
    ▼               ▼               ▼
 AES-256-GCM       FF1         HMAC-SHA-256
```

The root key is not directly reused for individual cryptographic
operations.

---

# 13. Strongly Typed Crypto Primitives

INDICCRYPT avoids passing generic byte buffers throughout the security
boundary.

The current type system includes:

```text
Key
Nonce
Tweak
Ciphertext
AuthenticationTag
SearchToken
```

This allows cryptographic APIs to communicate intent through their types.

For example:

```cpp
AesGcm::encrypt(
    key,
    nonce,
    plaintext,
    aad
);
```

rather than exposing an API based entirely on untyped byte arrays.

---

# 14. Key Lifecycle

`Key` has explicit ownership semantics.

Current design:

```text
Key
├── owned byte storage
├── move construction
├── move assignment
├── copy disabled
└── destruction-time zeroization
```

Copying key objects is intentionally disabled:

```cpp
Key copy = key; // prohibited
```

Moving is supported:

```cpp
Key derived = KeyDerivation::derive(...);
```

The implementation performs explicit zeroization of owned key storage when
the object is destroyed or overwritten.

This is a baseline memory-hygiene mechanism and is not considered a complete
defense against every memory-disclosure mechanism.

---

# 15. HKDF-SHA-256

INDICCRYPT uses HKDF-SHA-256 for key derivation.

The conceptual construction is:

```text
K_ROOT
   │
   ▼
HKDF-SHA-256
   │
   ├── indiccrypt/encryption/v1
   ├── indiccrypt/fpe/v1
   ├── indiccrypt/search/v1
   └── indiccrypt/integrity/v1
```

Domain-separated labels prevent different cryptographic functions from
silently sharing the same derived key.

The labels are versioned:

```text
indiccrypt/encryption/v1
indiccrypt/fpe/v1
indiccrypt/search/v1
indiccrypt/integrity/v1
```

Future protocol revisions can introduce a new version rather than silently
changing the semantics of an existing key domain.

---

# 16. HKDF Verification

The implementation is tested against an RFC 5869 HKDF-SHA-256 known-answer
test vector.

The project also tests:

* Deterministic derivation
* Output length
* Domain separation
* Different purpose labels
* Same-input reproducibility

Current status:

```text
HKDF implementation              ✓
Domain separation                ✓
RFC 5869 verification            ✓
```

---

# 17. AES-256-GCM

INDICCRYPT now contains an authenticated encryption layer based on the
OpenSSL EVP API.

Construction:

```text
              AES-256-GCM
             /           \
            /             \
       Plaintext           AAD
            │               │
            └───────┬───────┘
                    ▼
               Encryption
                    │
             ┌──────┴──────┐
             ▼             ▼
        Ciphertext      AuthTag
```

Current parameters:

```text
AES key       32 bytes
GCM nonce     12 bytes
Auth tag      16 bytes
```

The implementation does not implement AES itself.

It uses OpenSSL's established EVP cryptographic interface.

---

# 18. AES-GCM Security Properties Tested

The AES-GCM test suite currently verifies:

### Encryption

Plaintext can be transformed into ciphertext.

### Decryption

Valid ciphertext, nonce, key, and authentication tag recover the original
plaintext.

### Authentication

Authentication tags are generated and verified.

### AAD

Associated Authenticated Data can be incorporated into the authentication
boundary without being encrypted.

### Tamper Detection

Modification of ciphertext causes authentication failure.

### AAD Integrity

Modification of AAD causes authentication failure.

### Key Validation

Incorrect key sizes are rejected.

### Nonce Validation

Incorrect nonce sizes are rejected.

Current status:

```text
AES-256-GCM encryption       ✓
AES-256-GCM decryption       ✓
Authentication               ✓
AAD                          ✓
Tamper detection             ✓
Key validation               ✓
Nonce validation             ✓
```

---

# 19. Nonce Model

The current AES-GCM API requires a 12-byte nonce.

The API deliberately does not generate a nonce internally.

Nonce lifecycle and uniqueness are application/protocol responsibilities
and will be addressed explicitly in the higher-level encryption envelope.

This is important because nonce reuse with GCM can catastrophically weaken
security.

The final INDICCRYPT envelope must therefore define:

* Nonce generation
* Nonce storage
* Nonce uniqueness
* Serialization
* Versioning
* Key association

---

# 20. Format-Preserving Encryption

The next major cryptographic layer is Format-Preserving Encryption.

The project investigates NIST SP 800-38G FF1 as the FPE primitive.

The intended architecture is:

```text
IndicSymbol[]
      │
      ▼
FPE Alphabet
      │
      ▼
Radix Domain
      │
      ▼
FF1
      │
      ▼
Encrypted Symbol Sequence
      │
      ▼
Indic Representation
```

The project does not assume that a Unicode block automatically constitutes
a valid cryptographic alphabet.

A valid FPE domain must define:

* Alphabet
* Symbol ordering
* Valid symbol set
* Radix
* Supported lengths
* Encoding/decoding
* Key
* Tweak
* Version

The FPE layer is currently **not implemented**.

---

# 21. Searchable Encryption

INDICCRYPT investigates controlled exact-match search over protected data.

The conceptual design is:

```text
Canonical Search Value
        │
        ▼
HMAC-SHA-256(K_SEARCH, value)
        │
        ▼
    Search Token
        │
        ▼
   Database Index
```

The search key is separated from:

```text
K_ENC
K_FPE
K_ROOT
```

Searchable indexes are treated as sensitive information.

Potential leakage includes:

* Equality
* Frequency
* Search activity
* Access patterns

INDICCRYPT therefore does not claim zero leakage from searchable indexes.

Prefix search is considered a separate research problem.

---

# 22. Security Model

The current threat model considers an attacker who may obtain:

* Database contents
* Ciphertext
* Search indexes
* Application logs
* Public metadata

while not possessing the relevant cryptographic keys.

Primary security goals:

```text
Confidentiality
Integrity
Key separation
Unicode-safe canonicalization
Controlled search leakage
```

Initial out-of-scope threats include:

* Compromised endpoints holding plaintext
* Malicious processes with access to keys
* Operating-system compromise
* Advanced side-channel attacks
* Compromise of the root key-management infrastructure

---

# 23. Verification Strategy

INDICCRYPT uses layered verification.

## Current

* C++ unit tests
* ICU Unicode tests
* Grapheme tests
* Script tests
* Language registry tests
* IndicSymbol tests
* Canonicalization tests
* Crypto type tests
* HKDF tests
* RFC 5869 verification
* Key lifecycle tests
* AES-GCM tests

## Planned

* Property-based testing
* libFuzzer
* CBMC
* TLA+ protocol modeling
* CodeQL
* Differential testing
* Cryptographic test vectors
* Performance benchmarks
* Database integration testing

---

# 24. Current Test Suite

Current automated test count:

```text
12 / 12 passing
```

Test architecture:

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
```

Run:

```bash
ctest --test-dir build --output-on-failure
```

Expected:

```text
100% tests passed
```

---

# 25. Technology Stack

## Systems

* C++23
* Clang / LLVM
* CMake
* Ninja

## Unicode

* ICU4C

## Cryptography

* OpenSSL 3
* AES-256-GCM
* HKDF-SHA-256
* HMAC-SHA-256
* Planned FF1

## Verification

* CTest
* Planned libFuzzer
* Planned CBMC
* Planned TLA+
* Planned CodeQL

## Integration — Planned

* C ABI
* pybind11
* Python
* FastAPI
* PostgreSQL
* PostgreSQL extensions / UDFs
* Next.js developer interface

---

# 26. Project Structure

```text
indiccrypt/
│
├── core/
│   ├── indiccrypt.cpp
│   │
│   ├── crypto/
│   │   ├── types.cpp
│   │   ├── kdf.cpp
│   │   └── aes_gcm.cpp
│   │
│   └── text/
│       ├── indic_text.cpp
│       ├── grapheme.cpp
│       ├── script.cpp
│       ├── language.cpp
│       ├── symbol.cpp
│       └── canonicalizer.cpp
│
├── include/
│   └── indiccrypt/
│       ├── core.hpp
│       │
│       ├── crypto/
│       │   ├── types.hpp
│       │   ├── kdf.hpp
│       │   └── aes_gcm.hpp
│       │
│       └── text/
│           ├── indic_text.hpp
│           ├── grapheme.hpp
│           ├── script.hpp
│           ├── language.hpp
│           ├── symbol.hpp
│           └── canonicalizer.hpp
│
├── tests/
│   ├── test_core.cpp
│   │
│   ├── crypto/
│   │   ├── test_crypto_types.cpp
│   │   ├── test_kdf.cpp
│   │   ├── test_hkdf_rfc5869.cpp
│   │   ├── test_key_lifecycle.cpp
│   │   └── test_aes_gcm.cpp
│   │
│   └── text/
│       ├── test_indic_text.cpp
│       ├── test_grapheme.cpp
│       ├── test_script.cpp
│       ├── test_language.cpp
│       ├── test_symbol.cpp
│       └── test_canonicalizer.cpp
│
├── benchmarks/
├── docs/
├── research/
│   ├── threat-model.md
│   ├── crypto-design.md
│   └── search-security.md
│
├── tools/
│
├── CMakeLists.txt
├── README.md
└── .gitignore
```

---

# 27. Build

Requirements:

```text
C++23 compiler
CMake >= 3.25
Ninja
ICU4C
OpenSSL 3
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

# 28. Roadmap

## Phase 1 — Unicode Foundation

* [x] C++23 project
* [x] CMake/Ninja build
* [x] ICU4C integration
* [x] NFC normalization
* [x] Unicode processing
* [x] Grapheme segmentation

## Phase 2 — Indic Representation

* [x] Script detection
* [x] 22-language registry
* [x] IndicSymbol
* [x] Canonicalizer

## Phase 3 — Cryptographic Foundation

* [x] Strongly typed crypto primitives
* [x] Key ownership model
* [x] Key move semantics
* [x] Key zeroization
* [x] HKDF-SHA-256
* [x] Domain separation
* [x] RFC 5869 verification
* [x] AES-256-GCM
* [x] AAD support
* [x] Authentication failure detection

## Phase 4 — Indic FPE

* [ ] FPE alphabet abstraction
* [ ] Indic symbol domains
* [ ] Radix validation
* [ ] FPE domain encoding
* [ ] FF1 integration
* [ ] FF1 known-answer tests
* [ ] Indic ciphertext representation
* [ ] Tweak construction
* [ ] FPE security analysis

## Phase 5 — Search Security

* [ ] Search key derivation
* [ ] HMAC-SHA-256 search tokens
* [ ] Exact-match searchable index
* [ ] Leakage measurements
* [ ] Search security tests

## Phase 6 — Encryption Envelope

* [ ] Versioned ciphertext format
* [ ] Nonce generation
* [ ] Nonce lifecycle
* [ ] Key identifier
* [ ] Authentication metadata
* [ ] Serialization/deserialization
* [ ] Backward-compatible versions

## Phase 7 — Database Integration

* [ ] PostgreSQL adapter
* [ ] PostgreSQL extension/UDF research
* [ ] Unicode/collation testing
* [ ] Encrypted field experiments
* [ ] Search index experiments
* [ ] Schema compatibility benchmarks

## Phase 8 — Verification

* [ ] Property-based testing
* [ ] libFuzzer
* [ ] CBMC
* [ ] TLA+ models
* [ ] CodeQL
* [ ] Differential testing
* [ ] Performance benchmarks

## Phase 9 — Developer Platform

* [ ] C ABI
* [ ] pybind11 bindings
* [ ] FastAPI service
* [ ] PostgreSQL integration
* [ ] Developer API
* [ ] Next.js developer console

---

# 29. Current Milestone

INDICCRYPT has completed its initial Unicode and authenticated-encryption
foundation.

```text
C++23 Core                    ✓
CMake / Ninja                 ✓
ICU4C                         ✓
NFC Normalization             ✓
Grapheme Processing           ✓
Script Detection              ✓
22-Language Registry          ✓
IndicSymbol                   ✓
Canonicalization              ✓

Crypto Types                  ✓
Key Lifecycle                 ✓
HKDF-SHA-256                 ✓
Domain Separation             ✓
RFC 5869 Verification         ✓
AES-256-GCM                   ✓
AAD                           ✓
Authentication                ✓
Tamper Detection              ✓

Automated Tests               12 / 12 ✓
```

The next major milestone is the **Indic Format-Preserving Encryption
Domain**.

The objective is to define a mathematically valid symbol/radix domain that
can interface with an established FPE construction such as FF1.

---

# 30. Security Disclaimer

INDICCRYPT is currently a security engineering and cryptographic research
project.

Although it uses established cryptographic primitives, the complete
INDICCRYPT construction has not undergone an independent security audit.

The project should not be used to protect production personal data until:

* The complete cryptographic construction is finalized
* The FPE layer is implemented and reviewed
* Nonce/key lifecycle handling is finalized
* Search leakage is formally evaluated
* Database integration is tested
* Automated verification is expanded
* Independent security review is completed

No claim of production-grade security or regulatory compliance is made by
the current implementation.

INDICCRYPT is designed to investigate and demonstrate secure systems
engineering principles using established cryptographic primitives.

````

### After replacing it

Run:

```bash
git add README.md
git commit -m "update README with cryptographic architecture"
git status
````

You should finish with:

```text
nothing to commit, working tree clean
```

Then the next real engineering milestone is **FPE Domain Design**, not more README work.
