# Context Module Overview

This report surveys the current implementation in `src/context` and how it maps to the public API.

## Missing Public Headers

The context logic resides only in source files. The README notes:

```
## From branch: codex/create-readme-for-context-processing-2025-06-22

This directory contains the runtime logic for processing `Context` objects. It currently ships only as source, with no public headers under `include/`.
```

`processor.cpp` implements `ProcessorImpl`, `PriorityManager`, `RelationshipManager`, and `ResourcePredictorImpl`. The factory method `createResourcePredictor` has no declaration in `include/`.

## Context Headers Under `include/quantum`

Several headers in `include/quantum` belong to context processing. The table in the README lists them with implementations in `src/context`:

```
| `pattern_processor.h` | Abstract processor working on quantum pattern states. | `src/quantum/pattern_processor.cpp` |
| `priority.h` | Priority tiers and manager for context relevance. | Implemented within `src/context/processor.cpp` |
| `processor.h` | Main quantum `Processor` API manipulating `Pattern` objects. | `src/quantum/processor.cpp` |
| `qbsa.h` | Quantum Bit-State Analysis algorithms. | `src/quantum/qbsa.cpp` |
| `qbsa_qfh.h` | Factory for QFH-based `QBSAProcessor`. | `src/quantum/qbsa_qfh.cpp` |
| `qfh.h` | Quantum Fluctuation Hashing algorithm and streaming processor. | `src/quantum/qfh.cpp` |
| `quantum_pattern_processor.h` | Pattern processor built on top of a `QuantumProcessor`. | `src/quantum/quantum_pattern_processor.cpp` |
| `quantum_processor.h` | Compatibility header mapping the old `QuantumProcessor` name to `Processor`. | `src/quantum/quantum_processor.cpp` |
| `quantum_processor_qfh.h` | QFH-enhanced processor interface. | `src/quantum/quantum_processor_qfh.cpp`, `src/quantum/quantum_processor_qfh_common.cpp` |
| `relationship.h` | Relationship manager for contexts. | Implemented within `src/context/processor.cpp` |
| `resource_predictor.h` | Abstract interface for resource prediction. | Implemented within `src/context/processor.cpp` |
| `types.h` | Fundamental data structures and config used across the module. | `src/quantum/types_serialization.cpp` |
```

These headers would be clearer under a dedicated `include/context` directory.

## Candidate Files for Removal

Several helper scripts and tests are not referenced by any build files and could be removed:

- `tests/util/unused_include_scanner.py`
- `tests/core/performance_test_suite.cpp`
- `tests/brace_check.py`

Removing them would reduce clutter.
