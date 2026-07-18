# Design Decisions & Rationale
> Last updated: 2026-07-18

## 1. Split of Legacy Unified MarineMeteorologyEW Warnings (Mew Types 1-6)
- **Problem**: Originally, the database stored all warnings (Tropical Cyclone, Gale, Large Wave, etc.) in a unified table, linking them via a shared row ID. This caused deletion anomalies: deleting a single warning entry ended up cascade-deleting multiple entries because they shared a common parent row ID.
- **Solution**: We created 6 separate, type-safe structures (`MewTropicalCyclone`, `MewGale`, `MewLargeWave`, `MewSeaFog`, `MewStormSurge`, and `MewSeaIce`) and backed them with 6 distinct database tables. This preserves clean list item behavior and single-row deletions.
- **Spatial Indexing**: Added SQLite R-Tree virtual table `MewTropicalCycloneBBox` specifically for `MewTropicalCyclone` coordinates to allow bounding box picking/queries without complex math in SQL.

## 2. Realignment of AIS AtoN Dynamics (FI = 34) Parser
- **Problem**: The parser previously assumed the presence of the standard VDES ASM header fields (`MRN` and `Fragment`) at bits 16-34. However, the standard protocol (GB specification Table 87) states that for FI=34, the payload has no MRN/Fragment fields in the header; instead, the elements start directly after `AtoN Attribute` (3 bits) and `Status` (4 bits). Additionally, each element block was missing the 2-bit `Fragment Description` field.
- **Solution**:
  - Deleted `MRN` and `fragment` from `ASM_DAC_412_FI_34` struct and set `dynamics.MRN = 0` and `dynamics.fragment = 0` during DTO mapping in VDES manager.
  - Shifted start bit of elements to `23` (instead of `42`).
  - Added `fragmentDescription` to `ASM_DAC_412_FI_34::Element` and aligned element lengths ($L=115$, $110$, $166$ bits) and offsets with the protocol standard.

## 3. Bridge (FI = 41) Restructuring to Support Multiple Spans
- **Problem**: The original implementation saved each span (通航孔) in a separate row in the database, resulting in a single physical bridge being retrieved as multiple separate bridges. This broke logical page-based queries and mapping integrity.
- **Solution**:
  - Restructured C++ `Bridge` to represent a single bridge structure, keeping general attributes (`MRN`, `flowVelocity`, `flowDirection`, etc.) at the root and storing spans inside a `std::vector<Span> spans` member.
  - Redesigned the database table: using `MRN` as the unique `PRIMARY KEY`, and serializing all spans as a binary BLOB in the `Spans` column via the internal `BridgeSpanStruct` helper.
  - Updated the parser callback to group spans before calling `SaveBridge`, enabling correct bridge-level pagination, query consistency, and automatic `REPLACE INTO` overwrites.

## 4. Channel Boundary (FI = 43/44) Merging by MRN Primary Key
- **Problem**: Originally left and right channel boundaries were saved as independent rows in the database. However, in real-world scenarios, a channel is defined by its MRN and can contain a left boundary, a right boundary, or both. Representing them independently broke the concept of a single channel.
- **Solution**:
  - Restructured `ChannelBoundary` DTO to contain `leftCoordinates` and `rightCoordinates` instead of `edgeType` and `coordinates`.
  - Changed database table schema to use `MRN` as `PRIMARY KEY` and added separate `LeftCoordinates` and `RightCoordinates` BLOB columns.
  - On parsing FI 43/44 in `HandleASMMessage`, the system queries existing double-sided coordinates for the MRN, overwrites the newly received side, and writes the combined record using `REPLACE INTO`.

## 5. Net Sounder (FI = 45) 2-bit Fragment Description & FI = 8 Text Updates
- **Problem**: New specification introduces a 2-bit fragment description (`fragmentDesc`) for each node in a Net Sounder. When `fragmentDesc == 1`, it signals that supplementary descriptive text is required. Descriptive text is received via a separate Supplementary Description Fragment (DAC 413, FI 8) matching `(MRN, mainDAC, mainFI)`.
- **Solution**:
  - Added `fragmentDesc` to `ASM_DAC_412_FI_45::NetInfo`, `NetSounder::NetInfo`, and internal database BLOB struct `NetInfoStruct`.
  - Aligned element bit layouts in `ParseASMDAC412FI45` and `SendNetSounder` to read/write the 2-bit `fragmentDesc` immediately after the 20-bit node MRN, increasing the step size of subsequent nodes from 49 bits to 51 bits.
  - Implemented supplementary description updates in `HandleASMMessage` for FI 8: if `mainDAC == 412` and `mainFI == 45`, it queries all NetSounder records from the database, matches node MRNs, updates the record's description text, and writes back.

## 6. Overload Resolution Trap in Deletion APIs & Singular Deletion Implementations
- **Problem**: The plural delete interfaces `DeleteChannelBoundaries` and `DeleteChannelCenterlines` had a signature `(index = 0, number = -1)`. If a caller passed a single integer ID (like `302`), it was matched to the first overload (setting `index = 302`), resulting in a delete offset that deleted nothing and returned success, leaving the records untouched.
- **Solution**: We implemented dedicated singular APIs `DeleteChannelCenterline(dataID)` and `DeleteChannelBoundary(dataID)` which delete explicitly by primary keys (`ID` and `MRN`), resolving the compiler overload resolution trap. We also added `DeleteAtoNDynamic(dataID)` and `DeleteAISAtoNDynamic(dataID)` to preemptively resolve the same matching trap for AtoN dynamics. All singular delete methods now trigger event notifications.

## 7. Channel Centerline (FI 42) & Channel Boundary (FI 43 / 44) Description updating support
- **Problem**: Similar to NetSounders, the specification also requires text description support for Channel Centerlines and Channel Boundaries. A supplementary text message (DAC 413, FI 8) targeting `mainDAC = 412`, `mainFI = 42/43/44` updates their descriptions.
- **Solution**:
  - Added `std::string description;` to `ChannelCenterline` and `ChannelBoundary` structs.
  - Added `Description TEXT` column to their database schemas.
  - Updated load/save logic to map this column.
  - Added FI 8 handlers for `mainFI == 42` and `mainFI == 43 || mainFI == 44` to update descriptions in the database and notify UI observers.
  - Modified parser logic to query and preserve any existing descriptions prior to record deletion/replacement during initialization.

## 8. ABB SMS-like Sentence Assembling and ASM Mixed Chinese Decoding separation
- **Problem**: 
  - Standard VDES messages require support for receiving multisentence NMEA `!AIABB` (FI=3 and FI=4) SMS-like packets. We need to group incoming fragments, reconstruct the binary payloads, convert decoded GBK texts to UTF-8, and store them with their Source MMSI into a local `MessageInbox` table.
  - Additionally, for 13-bit Chinese encoding, the legacy `FI == 2` decoder restricted input lengths based on the 952-bit AIS Msg 8 limit, which blockaded longer VDES ASM `FI == 4` messages.
- **Solution**:
  - Activated the `ENABLE_PARSE_ABB` flag, parsed incoming `ABB` lines, and mapped assembled payloads to the `ASMManager` decoder.
  - Implemented automatic GBK-to-UTF-8 transcoding and saved messages (content, Source MMSI, timestamps) to the sqlite database `MessageInbox`.
  - Separated `FI == 4` from `FI == 2` in `AISBitsManager.cpp`, bypassing the 952-bit filter map check to support arbitrarily long ASM bitstreams.
  - Added global `/utf-8` compiler flags to MSVC CMake settings to address compiler syntax token-offset issues on source files containing Chinese string literals.


