# Source Memory Overview

This document illustrates how the `memory` module is organized inside `src/` and how it interacts with the `quantum` and `core` components.

## Tiered Memory System

The engine implements a three‑tier memory hierarchy:

| Tier | Purpose | Backing |
| --- | --- | --- |
| **Short‑Term Memory (STM)** | Fast storage for new or transient patterns. | Host or unified memory. |
| **Medium‑Term Memory (MTM)** | Holds patterns with moderate coherence and stability. | Host, device, or unified memory. |
| **Long‑Term Memory (LTM)** | Persistent storage for highly coherent patterns. Can be backed by Redis. | Device/unified memory + optional Redis. |

Each tier is implemented by `memory::MemoryTier` (see `src/memory/memory_tier.cpp`). The tiers allocate blocks from a dedicated memory pool and track fragmentation, utilization, and pattern metadata.

## MemoryTierManager

`memory::MemoryTierManager` (in `src/memory/memory_tier_manager.cpp`) is a singleton that orchestrates all tiers. Its responsibilities include:

- Allocating and freeing `MemoryBlock` objects in a specific tier.
- Promoting/demoting blocks when coherence, stability, or generation counts cross thresholds.
- Maintaining a lookup table so blocks can be found by pointer.
- Exposing metrics for utilization and fragmentation.
- Managing pattern relationships via a `dag::DagGraph` from the `core` module.
- Optionally persisting LTM patterns through `persistence::RedisManager`.

```
+-------------------------+         +---------------------+
|  quantum algorithms     | <-----> | memory::MemoryTier  |
|  (pattern evolution)    |         | - STM / MTM / LTM   |
+-----------+-------------+         +----------+----------+
            |                                 ^
            | produce PatternData             |
            v                                 |
   +--------+---------+                uses metrics,
   | MemoryTierManager|  --(DAG, logs)-->  core utilities
   +------------------+
```

The quantum module determines the initial tier for a pattern (see `src/quantum/pattern_processor.cpp`). Once stored, the manager monitors coherence and stability to promote from STM → MTM → LTM or demote in the opposite direction. All promotion/demotion decisions rely on configuration values stored in `MemoryTierManager::Config`.

The core module supplies:

- `dag::DagGraph` for tracking relationships between patterns.
- Logging via `logging::Manager`.
- Allocation metrics and error handling utilities.

## File Locations

- `src/memory/memory_tier.cpp` – Implementation of the tier class (allocation, defragmentation, pattern storage).
- `src/memory/memory_tier_manager.cpp` – Singleton manager coordinating tiers and pattern promotion.
- `src/memory/redis_manager.cpp` – Optional persistence layer for LTM.

These files form the backbone of the memory subsystem and are closely tied to the quantum algorithms that generate pattern data and the core services that collect metrics and maintain the DAG.
