# Quantum Module Review

This document records the review of the `quantum` module. The goal was to
identify outdated kernels or helpers, verify that header files only declare
interfaces, and note areas for consolidation.

## Outdated Kernels or Helpers

No CUDA kernels or obsolete helper functions were found in `src/quantum`.
All implementations rely on host side algorithms. Generated build artifacts such
as `cmake_install.cmake` were removed from every module directory.

## Header and Implementation Separation

Several headers contained inline method definitions. These have been moved to the
corresponding `.cpp` files so that headers only declare interfaces:

- `qbsa.h` – `getOptions()` now implemented in `qbsa.cpp`.
- `qfh.h` – `QFHEvent::operator==` moved to `qfh.cpp`.
- `quantum_processor_qfh.h` – `lastQFHResult()` implemented in
  `quantum_processor_qfh_common.cpp`.
- `quantum_pattern_processor.h` – helper methods and constructor definitions
  relocated to `quantum_pattern_processor.cpp`.

## Experimental Code

The files `qbsa_qfh.cpp` and `quantum_processor_qfh_common.cpp` remain
experimental integrations of QFH analysis with the core processor. Once these
interfaces stabilize they could be merged with `quantum_processor.cpp` to reduce
duplication. The existing DAG utilities mentioned in the `README` may also be
consolidated with context management to avoid multiple lineage trackers.

## DAG Utility Relocation

`dag_graph.h` and its implementation were moved out of the `quantum` module. The
header now lives in `include/core` and is built from `src/core/dag_graph.cpp`.
This centralizes the DAG structures for use by memory and context code.

## Integration with Pattern Processing

After cleanup the quantum processors expose minimal headers and integrate
through `QuantumPatternProcessor`. This class relies on `QuantumProcessor` and
the optional QFH-enhanced helpers to evolve `PatternData` records. The pattern
module converts its internal state to `QuantumState` structures and delegates
coherence and stability calculations to the quantum layer. Results drive memory
tiering and mutation logic in `PatternProcessor`.

