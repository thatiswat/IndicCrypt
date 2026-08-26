# INDICCRYPT Threat Model

## 1. System Objective

INDICCRYPT protects personal data represented in Indic-language text while
attempting to preserve a database-compatible textual representation.

The system must provide:

- Confidentiality
- Integrity
- Key separation
- Resistance to unauthorized plaintext recovery
- Controlled searchable indexing
- Deterministic canonicalization
- Unicode-safe processing

## 2. Protected Asset

The primary protected asset is plaintext personal data represented as
Unicode text.

Examples include:

- Names
- Addresses
- Descriptions
- Regional-language identifiers
- Free-form personal information

## 3. Adversary

We consider an adversary who may obtain:

- Database contents
- Ciphertext fields
- Search indexes
- Application logs
- Public system metadata

The adversary does not possess the cryptographic master key.

## 4. Security Goals

### Confidentiality

Ciphertext must not allow practical recovery of plaintext without the
appropriate cryptographic key.

### Integrity

Unauthorized modification of protected data must be detectable.

### Key Isolation

Keys used for encryption and searchable indexes must be cryptographically
separated.

### Unicode Safety

Canonicalization must prevent equivalent Unicode representations from
producing inconsistent cryptographic inputs.

### Search Security

Search functionality must explicitly document the information that can
leak through searchable indexes.

## 5. Out of Scope

The initial system does not attempt to protect against:

- A compromised endpoint that already has plaintext
- A malicious application process holding encryption keys
- Operating-system-level compromise
- Side-channel attacks against the initial research implementation
- Traffic analysis outside the database/search layer
- Compromise of the root key-management system

## 6. Security Assumptions

The system assumes:

1. Cryptographic keys are generated using a cryptographically secure RNG.
2. Keys are stored outside ordinary application data.
3. Authentication of ciphertext is mandatory.
4. Nonces are never reused where the selected construction prohibits reuse.
5. Unicode input is canonicalized before cryptographic processing.

## 7. Leakage Model

Searchable encryption may reveal information such as:

- Equality of search tokens
- Frequency of repeated values
- Search access patterns

These leakages must be explicitly measured and documented.

INDICCRYPT does not claim zero leakage from searchable indexes.