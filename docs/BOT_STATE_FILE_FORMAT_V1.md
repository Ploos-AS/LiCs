# Persistent Bot State File Format v1

## Header
The file begins with the ASCII line `BSTATE1\n`.

## Records
Each record is encoded as four length-prefixed fields:
`<scope-length>:<scope><key-length>:<key><value-length>:<value>\n`

Lengths are byte lengths, not character counts.

## Rules
- UTF-8 text is accepted; embedded NUL is rejected.
- Empty scope/key/value are rejected for stored records.
- Maximum field length: 65535 bytes.
- Duplicate scope+key records are invalid.
- Unknown/truncated records make the complete file invalid.
- A valid file represents one complete state snapshot.
- Writes use a temporary file in the same directory, flush/sync, then atomic rename.
- The persistent file is never modified in place.
- File permissions are created as owner-only where supported.
- M2.4.1 does not define encryption or multi-process locking.

## Recovery
On parse or checksum failure, the backend must reject the file rather than partially loading it.

## Compatibility
The header is versioned. Future incompatible formats use a new header; readers must not silently reinterpret unknown versions.
