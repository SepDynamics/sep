# Refactoring Recommendations for `src/`

The current directory structure mixes generated build files with handwritten code and spreads related modules across several folders. To streamline development:

1. **Remove CMake build artifacts** – `src/*/CMakeFiles` and their subdirectories should be excluded from version control. Keeping them makes the tree noisy and obscures the true code layout.
2. **Consolidate application code** – Move `apps/common` into `apps/oanda_trader` or a shared `ui` module if additional GUIs appear. This simplifies the hierarchy.
3. **Unify memory and engine layers** – Many memory helpers under `src/memory` interact closely with the core engine. Consider combining them into `engine/memory` to clarify ownership.
4. **Group quantum algorithms** – Files in `src/quantum` could live under a single `quantum` library with clear submodules for QFH, QBSA, and evolution. This would make dependencies explicit and allow easier reuse by other engines.
5. **Introduce documentation** – Adding READMEs within each source directory (similar to these docs) will help maintain clarity as the project evolves.

By removing generated files and reorganizing the remaining directories by domain (engine, memory, quantum, api, ui), the codebase will become easier to navigate and maintain.
