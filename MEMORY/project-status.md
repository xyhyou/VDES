# Project Status & Environment Notes
> Last updated: 2026-07-10

## Build and Testing Details
- **Toolchain**: CMake build system with Visual Studio 18 (2022) generator.
- **Architecture**: 32-bit (x86-debug configuration).
- **AddressSanitizer (ASan)**: Enabled on the library and Demo.
- **Execution Rule**: The test executable `VDESDemo.exe` MUST be executed inside the Visual Studio Developer Command Prompt env:
  ```cmd
  cmd.exe /c """D:\Program Files\Microsoft Visual Studio\18\Community\Common7\Tools\VsDevCmd.bat"" -arch=x86 && out\build\x86-debug\VDESDemo\VDESDemo.exe"
  ```
  Failing to run it inside the developer command prompt environment will cause immediate silent termination (exit code 1) due to missing `clang_rt.asan_dynamic-i386.dll` in runtime PATH.

## SQLite Database
- Database uses **SQLite3** via `SQLiteCpp` library wrapper.
- Extensively uses the **R-Tree** extension (e.g., `MewTropicalCycloneBBox`) for fast coordinate-based spatial queries.
