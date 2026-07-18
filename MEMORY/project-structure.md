# Project Structure & Release Guidelines
> Last updated: 2026-07-10

## Directory Layout
```
d:\VDES\
├── Headers\            ← Internal C++ header files (e.g., VDES.h, VDESDTD.h, ASMDTD.h)
├── Sources\            ← C++ implementation source files (e.g., VDES.cpp, ASMManager.cpp)
├── Publish\            ← Public/external headers released to API consumers
│   └── include\        ← Copies of VDES.h and VDESDTD.h
├── VDESDemo\           ← Demo console app for validation and testing
└── MEMORY\             ← Handoff persistent memory files
```

## Release/Export Sync Rule
- The header files under `Headers/` (specifically `VDES.h` and `VDESDTD.h`) are copied to `Publish/include/` to form the public SDK headers.
- **Rule**: Whenever you modify internal declarations in `Headers/VDES.h` or `Headers/VDESDTD.h`, you must also copy the modifications to their counterparts in `Publish/include/VDES.h` and `Publish/include/VDESDTD.h`.
