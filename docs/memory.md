# Memory Module Audit

This document records the current state of the memory subsystem after reviewing
files under `src/memory` and `include/memory`.

The memory system consists of three tiers managed by
`MemoryTierManager`:

- **STM** – short‑term memory for freshly generated patterns.
- **MTM** – medium‑term memory for patterns that show improving coherence.
- **LTM** – long‑term memory that persists stable patterns to disk using an
  optional Redis backend.

Patterns migrate between these tiers as their coherence and generation counts
change. The tiers interact with other modules in the following ways:

- The **quantum** module allocates blocks from STM when new patterns are
  created and signals promotions based on coherence metrics.
- The **context** module uses MTM and LTM lookups when resolving relationships.
- The **blender** and **audio** modules query LTM for visualization and sample
  playback.
- `MemoryTierManager` promotes or demotes patterns as coherence metrics cross
  configured thresholds. Generations are tracked so unstable patterns remain in
  STM until maturity.
## Unused artifacts

- `cmake_install.cmake` files checked in under `src/*` were build artifacts and
  have been removed. One obsolete copy in `src/memory` was deleted.
- Unused RAII helpers (`StreamDestroyer` and `EventDestroyer`) in
  `compat/cuda_helpers.h` were dropped.

## Header to implementation map

- **`include/memory/memory_tier.hpp`** → `src/memory/memory_tier.cpp`
- **`include/memory/memory_tier_manager.hpp`** →
  `src/memory/memory_tier_manager.cpp`
- **`include/memory/manager.h`** → `src/memory/manager.cpp`
- **`include/memory/unified_memory.h`** → template instantiations in
  `src/memory/memory.cu`
   (the full template is defined in the header)
- **`include/memory/logger.hpp`** → header only (used by
  `memory_tier.cpp` and `memory_tier_manager.cpp`)
- **`include/memory/spdlog_isolation.h`** and **`include/memory/types.h`** →
  header only
- **`src/memory/redis_manager.cpp`** implements persistence helpers declared in
  `include/memory/types.h`

## Redundant headers

`memory_tier.hpp` now contains the tier enums and `MemoryBlock` structure.
The previous `memory_tier.h` header was removed to simplify the API surface.

`manager.h` mainly defines logging middleware. It could be relocated alongside
other logging utilities or combined with its source file.

