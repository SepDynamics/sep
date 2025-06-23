# Header Dependency Overview

This document summarizes key header relationships within the SEP project after resolving include and type issues.

## Core Relationships

- **`core/core_types.h`** defines fundamental data structures such as `PatternData`. It now initializes members in declaration order to avoid `-Wreorder` warnings. This header includes `compat/math_common.h` for shared math utilities.
- **`pattern/pattern_common.h`** provides shared threshold constants (`MIN_COHERENCE`, `DEMOTION_THRESHOLD`, etc.) used by both CPU and CUDA components.
- **`compat/math_common.h`** pulls in `pattern/pattern_common.h` and exposes device-friendly math helpers. It also mirrors the threshold constants for CUDA kernels.
- **`pattern/pattern_evolution.h`** declares the `PatternEvolution` utility used across modules and tests. Implementations reside in `src/pattern/pattern_evolution.cpp`.
- **`pattern/pattern_processor.h`** and its implementation depend on `pattern_common.h` for threshold values and on `quantum/quantum_processor.h` for quantum calculations.

These headers form the backbone of pattern processing and ensure consistent threshold values between host and device code.
