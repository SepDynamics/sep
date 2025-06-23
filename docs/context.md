# Context Processing and Memory Tier Integration

The context module links user-provided `Context` objects to the rest of the engine. `processor.cpp` implements a `Processor` that validates input, generates or extracts embeddings and determines which memory tier should store the result. A `RelationshipManager` tracks similarity scores while a `PriorityManager` keeps access priorities.

Public headers for the module now live under `include/context`.  `processor.h`,
`priority.h` and `relationship.h` were moved from `include/quantum` to make the
ownership clearer.  Only `resource_predictor.h` remains in `include/quantum`
until that component is refactored.  These headers expose the APIs used by
`processor.cpp` and `relationship.cpp`.

During processing the `Processor` calculates a stability score for each embedding. Scores below `0.7` are placed in the **STM** tier, values between `0.7` and `0.89` go to **MTM**, and scores above `0.9` are promoted to **LTM**. The chosen tier is allocated through `memory::MemoryTierManager` and the resulting embedding is forwarded to the pattern subsystem via `pattern::PatternProcessor`.

Downstream modules include these headers from the new `context` path.  The
processor forwards embeddings to `pattern::PatternProcessor` and allocates
memory via `memory::MemoryTierManager`, forming a bridge between subsystems.

Redundant build artifacts such as `cmake_install.cmake` have been removed from the repository.
