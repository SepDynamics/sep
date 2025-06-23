# Context Processing and Memory Tier Integration

The context module links user-provided `Context` objects to the rest of the engine. `processor.cpp` implements a `Processor` that validates input, generates or extracts embeddings and determines which memory tier should store the result. A `RelationshipManager` tracks similarity scores while a `PriorityManager` keeps access priorities.

All public declarations currently live under `include/quantum` because no `include/context` directory exists yet. The headers `priority.h`, `relationship.h`, `processor.h` and `resource_predictor.h` describe the APIs used by `processor.cpp` and `relationship.cpp`.

During processing the `Processor` calculates a stability score for each embedding. Scores below `0.7` are placed in the **STM** tier, values between `0.7` and `0.89` go to **MTM**, and scores above `0.9` are promoted to **LTM**. The chosen tier is allocated through `memory::MemoryTierManager` and the resulting embedding is forwarded to the pattern subsystem via `pattern::PatternProcessor`.

The absence of a dedicated `include/context` folder means downstream code must include these headers from the `quantum` path. Consolidating them under `include/context` would clarify ownership but has not been done yet.

Redundant build artifacts such as `cmake_install.cmake` have been removed from the repository.
