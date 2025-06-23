# Memory Module Audit

This document records the current state of the memory subsystem after reviewing
files under `src/memory` and `include/memory`.

## Unused artifacts

- `src/memory/cmake_install.cmake` was checked in by accident and does not
  participate in the build. It has been removed.

## Header to implementation map

- **`include/memory/memory_tier.hpp`** / **`include/memory/memory_tier.h`** →
  `src/memory/memory_tier.cpp`
- **`include/memory/memory_tier_manager.hpp`** →
  `src/memory/memory_tier_manager.cpp`
- **`include/memory/manager.h`** → `src/memory/manager.cpp`
- **`include/memory/unified_memory.h`** → template instantiations in
  `src/memory/memory.cu`
- **`include/memory/logger.hpp`** → header only (used by
  `memory_tier.cpp` and `memory_tier_manager.cpp`)
- **`include/memory/spdlog_isolation.h`** and **`include/memory/types.h`** →
  header only
- **`src/memory/redis_manager.cpp`** implements persistence helpers declared in
  `include/memory/types.h`

## Redundant headers

`memory_tier.h` and `memory_tier.hpp` both describe the `MemoryTier` structures.
The code currently includes both in implementations. These headers could be
merged to simplify the API surface.

`manager.h` mainly defines logging middleware. It could be relocated alongside
other logging utilities or combined with its source file.

