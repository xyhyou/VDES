# Project Memory Index
> Last updated: 2026-07-18

## Files
- [project-status.md](./project-status.md) — Extra operations details and status.
- [project-structure.md](./project-structure.md) — Directory structure, naming conventions, and release/export mechanism.
- [design-decisions.md](./design-decisions.md) — Rationale behind database/protocol designs (Mew, AtoN, Bridge, Channel Boundary, Net Sounder, Deletion Overloads, and ABB parsing).
- [user-preferences.md](./user-preferences.md) — User collaboration preferences.
- [chinese_encoding_limitations.md](../chinese_encoding_limitations.md) — Detailed analysis of 13-bit and 14-bit Chinese encoding limitations regarding punctuation marks.

## Quick Summary
- VDES library rewritten to support restructured early warning database schemas (Mew Types 1-6), corrected AIS AtoN Dynamics (FI=34) standards, and Bridge (FI=41) multi-span structure.
- Refactored Channel Boundary (FI=43/44) to store left/right sides as BLOBs under a single MRN primary key record.
- Added 2-bit Fragment Description support to Net Sounders (FI=45) and implemented automatic description updates via Supplementary Description Fragments (FI=8) for NetSounders, Channel Centerlines, and Channel Boundaries.
- Fixed C++ compiler overload resolution bug in deletion APIs by declaring and implementing explicit singular delete methods (`DeleteChannelCenterline`, `DeleteChannelBoundary`, `DeleteAtoNDynamic`, and `DeleteAISAtoNDynamic`).
- Added support for multisentence NMEA `!AIABB` (FI=3 and FI=4) SMS-like message parsing, GBK-to-UTF-8 transcoding, and database serialization with Source MMSI.
- Separated `FI == 4` (ASM) 13-bit Chinese decoding from `FI == 2` (AIS Msg 8) to remove the 952-bit limitation.
- Resolved MSVC UTF-8 compiler token-offset warning C4819 and associated syntax errors by adding `/utf-8` compiler options globally in CMake configuration.
