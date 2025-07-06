# Quantum Processing Header Flow

This chart outlines how data moves through the quantum-processing headers in the SEP engine. Patterns originate either from in-memory data structures or via the engine's HTTP API. They then pass through several stages of analysis before results are stored back in memory or returned through the API.

```mermaid
flowchart TD
    Mem["Memory / API"]
    processor_h["processor.h"]
    qbsa_h["qbsa.h"]
    qfh_h["qfh.h"]
    qbsa_qfh_h["qbsa_qfh.h"]
    qp_qfh_h["quantum_processor_qfh.h"]
    qp_h["quantum_processor.h"]
    cycles_h["cycles.h"]
    evolution_h["evolution.h"]
    pattern_h["pattern.h"]
    pattern_evolution_h["pattern_evolution.h"]
    pattern_evolution_bridge_h["pattern_evolution_bridge.h"]
    relationship_h["relationship.h"]
    resource_predictor_h["resource_predictor.h"]
    qmo_h["quantum_manifold_optimizer.h"]
    data_h["data.hpp"]
    gpu_context_h["gpu_context.h"]
    priority_h["priority.h"]
    types_h["types.h"]

    Mem --> processor_h
    data_h --> pattern_h
    types_h --> pattern_h
    gpu_context_h --> processor_h
    priority_h --> processor_h
    pattern_h --> processor_h
    evolution_h --> pattern_evolution_h --> processor_h
    pattern_evolution_bridge_h --> processor_h
    processor_h --> qbsa_h
    qbsa_h --> qbsa_qfh_h
    qfh_h --> qbsa_qfh_h
    qbsa_qfh_h --> qp_qfh_h
    cycles_h --> qp_qfh_h
    qp_qfh_h --> qp_h
    relationship_h --> qp_h
    resource_predictor_h --> qmo_h --> qp_h
    processor_h --> qp_h
    qp_h --> Mem
```

**Header Roles**

- `cycles.h` – Implements `QuantumRenderer` for iterative scene evolution.
- `data.hpp` – Core structures describing quantum states and pattern metrics.
- `evolution.h` – Utility helpers to drive pattern evolution cycles.
- `gpu_context.h` – Abstraction layer for GPU resources used by kernels.
- `pattern.h` – Basic pattern containers shared across modules.
- `pattern_evolution.h` – High‑level helpers for evolving patterns.
- `pattern_evolution_bridge.h` – Connects evolution helpers with the API layer.
- `priority.h` – Scheduling weights for processing patterns.
- `processor.h` – Base `PatternProcessor` orchestrating evolution.
- `qbsa.h` – Quantum Binary State Analysis algorithms.
- `qbsa_qfh.h` – Extension combining QBSA with QFH results.
- `qfh.h` – Quantum Fourier Hierarchy transform routines.
- `quantum_manifold_optimizer.h` – Optimizes quantum states across memory tiers.
- `quantum_processor.h` – Public interface for quantum pattern processing.
- `quantum_processor_qfh.h` – Processor variant leveraging QFH.
- `relationship.h` – Manages relationships and similarity calculations between patterns.
- `resource_predictor.h` – Estimates resource needs for context batches.
- `types.h` – Shared type definitions for quantum structures.

Data fed through these headers ultimately flows into `quantum_processor.h` where final results are produced and returned to memory or the API.
