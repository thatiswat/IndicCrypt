# INDICCRYPT Cryptographic Design

## 1. Design Principle

INDICCRYPT separates text processing from cryptographic processing.

The pipeline is:

UTF-8
  ↓
Unicode normalization
  ↓
Grapheme segmentation
  ↓
Script identification
  ↓
Language profile
  ↓
Canonical representation
  ↓
Cryptographic transformation
  ↓
Ciphertext representation

## 2. Key Hierarchy

A root key must not be directly reused for every operation.

Conceptually:

K_root
   |
   +-- K_enc
   |
   +-- K_search
   |
   +-- K_fpe
   |
   +-- K_integrity

The final implementation will use a standardized key-derivation mechanism
with explicit domain separation.

## 3. Encryption

Authenticated encryption is required for binary/envelope data.

Candidate construction:

AES-256-GCM

Alternative:

ChaCha20-Poly1305

The implementation will benchmark both where supported.

## 4. Format-Preserving Encryption

INDICCRYPT will investigate NIST SP 800-38G FF1 for domains constructed
from validated Indic symbol alphabets.

The FPE domain must be defined mathematically before implementation.

A Unicode block is NOT automatically a valid cryptographic alphabet.

The domain must define:

- Alphabet
- Symbol ordering
- Valid symbol set
- Minimum domain size
- Maximum supported length
- Tweak construction
- Key derivation
- Encoding/decoding rules

## 5. Indic Ciphertext

The desired representation is:

Plaintext Indic symbols
        ↓
Canonical symbol sequence
        ↓
FPE domain encoding
        ↓
FF1
        ↓
Ciphertext symbol sequence
        ↓
Valid textual representation

The system must guarantee reversible encoding and decoding.

## 6. Authentication

FPE ciphertext must not be assumed to provide authentication.

Integrity protection therefore requires an explicit authenticated envelope
or authentication mechanism.

The final construction must prevent undetected ciphertext modification.

## 7. Tweak

The FPE tweak must not contain secret material that is transmitted as
plaintext unless its exposure is explicitly permitted by the construction.

The design will define:

- Field context
- Language/script context
- Version
- Optional record context

The security implications of deterministic versus randomized tweaks
must be evaluated.

## 8. Determinism

INDICCRYPT must distinguish between:

- Deterministic searchable representations
- Randomized confidential encryption

The two must not accidentally share the same construction.

## 9. Security Boundary

The canonicalizer produces input for the cryptographic engine.

The cryptographic engine must not perform Unicode normalization implicitly.

This prevents hidden transformations from changing cryptographic semantics.