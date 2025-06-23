# Quantum Module Overview

This guide summarizes the quantum-inspired algorithms and processor classes
found in the repository. The code lives under `include/quantum` and
`src/quantum`.

## QBSA – Quantum Bit-State Analysis

QBSA compares observed bit indices with an expected sequence. It counts
corrections, computes a ratio and flags a collapse when the ratio exceeds a
threshold. `QBSAProcessor` exposes `analyze()` and `detectCollapse()` methods
for this workflow.

## QFH – Quantum Fluctuation Hashing

QFH converts consecutive bits into transition events such as `FLIP` and
`RUPTURE`. Aggregated counts yield ratios that indicate instability and may
also trigger collapse detection. The streaming `QFHProcessor` generates the
events, while `QFHBasedProcessor` wraps the logic for batch analysis.

## Processor Classes

- **Processor** – core class that evolves quantum states and calculates
  coherence and stability metrics.
- **QuantumProcessor** – compatibility wrapper that forwards to `Processor`.
- **QuantumProcessorQFH** – processor variant enriched with QFH metrics.
- **PatternQuantumProcessor** – high level interface used by the pattern
  subsystem.

All public headers remain declaration-only. Implementations reside in
`src/quantum`, keeping interfaces clean and build times short.

