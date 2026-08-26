# INDICCRYPT Search Security

## 1. Objective

INDICCRYPT may provide controlled search over protected personal data
without exposing the plaintext field to the database.

## 2. Search Token

Conceptually:

canonical_search_value
        ↓
HMAC-SHA-256(K_search, value)
        ↓
search token
        ↓
database index

## 3. Key Separation

The search key must be different from:

- Encryption key
- FPE key
- Root key

## 4. Search Model

Initial implementation targets exact-match search.

Example:

plaintext:

भारत

search:

भारत

Both operations independently derive the same keyed search token.

## 5. Leakage

The search index may reveal:

- Whether two protected values are equal
- Frequency of repeated values
- Search activity patterns

Therefore the searchable index is itself sensitive information.

## 6. Prefix Search

Prefix search must not be implemented by naïvely hashing every plaintext
prefix unless its leakage properties are explicitly accepted.

Any prefix-search construction will be treated as a separate research
component.

## 7. Security Requirement

No plaintext search value may be stored alongside its searchable token.

## 8. Future Work

Potential research directions include:

- Oblivious search
- Leakage-reduced indexes
- Encrypted inverted indexes
- Query privacy
- Access-pattern protection