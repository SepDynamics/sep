# SEP Engine Build Guide

This repository contains the experimental SEP Engine codebase. To ensure we can iterate on the refactor work, start by installing the development dependencies and attempting a build.

## Dependency Installation

Run the provided script from the repository root:

```bash
bash scripts/install_dependencies.sh
```

This installs system packages via `apt` (compilers, CMake, libraries) and pulls third‑party sources like the header-only Crow framework. GoogleTest is built and installed automatically.

## Building

The project is built with CMake. A typical build can be invoked with:

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

At the current stage the build does **not** succeed. Missing CUDA headers and incomplete implementations in the quantum and memory modules trigger compilation failures. Fixing these issues is part of the refactoring effort.

## Directory Overview

- `src/` – C++ sources grouped by module
- `include/` – matching headers
- `third_party/` – external dependencies
- `scripts/` – helper scripts, including `install_dependencies.sh`
- `tests/` – testbed for individual modules

## Bring-Up Strategy

1. **Verify dependencies** – ensure the install script runs without errors. If CUDA support is required, set `INSTALL_CUDA=1` before running the script.
2. **Correct include paths** – headers currently reference the absolute `/sep` prefix. The root `CMakeLists.txt` has been updated to use `${CMAKE_SOURCE_DIR}` so local builds resolve headers correctly.
3. **CUDA shims** – earlier revisions used a symlink at `include/cuda` to reach
   the compatibility headers. This link has been removed. Update any legacy
   includes to reference the `compat/` directory directly.
4. **Resolve compile errors** – key modules like `quantum` and `memory` have missing declarations and inconsistent types. Incrementally stub or implement these functions, compiling after each change.
5. **Continuous testing** – build the unit tests under `tests/` once the main libraries compile. Use the testbed to validate functionality before modifying the main modules.

Tracking progress with a small task list will keep the bring‑up manageable.

