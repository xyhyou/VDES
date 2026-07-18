# User Collaboration Preferences
> Last updated: 2026-07-10

- **Encoding Standards**: Keep C++ files saved with **UTF-8 with BOM** (CRITICAL to avoid MSVC compilation issues with Chinese comments/strings). Configuration files (like CMakeLists.txt) MUST be saved in plain **UTF-8 (No BOM)**.
- **Language**: English for code comments, Doxygen docs, and console print logs. Chinese or English is acceptable for handoff and communication.
- **Build & Exec**: Always build and test through the VS DevCmd Prompt environment. Run tests synchronously and verify console output for regression check.
