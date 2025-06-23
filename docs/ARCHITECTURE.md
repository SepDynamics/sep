# SEP Engine Architecture

This document outlines the final architecture and component model for the SEP Engine. The goal is to create a clean, maintainable, and logically consistent structure that simplifies development, testing, and future expansion.

## 1. Guiding Principles

The architecture is guided by these principles:

1.  **Clear Component Boundaries**: Each module has a distinct responsibility and a well-defined public interface located in `/include/sep`.
2.  **Unidirectional Dependencies**: High-level modules (like `api`) can depend on low-level modules (like `core`), but not the other way around. This prevents circular dependencies.
3.  **Standard Project Layout**: The project follows a standard layout (`src`, `include`, `tests`, `third_party`, `assets`) for better tooling integration and developer onboarding.
4.  **Consolidation of Core Logic**: Cross-cutting concerns like logging, metrics, error handling, configuration, and the DAG are unified into a single, foundational `core` library.
5.  **Isolate External Dependencies**: Third-party libraries (Crow, nlohmann) are kept separate from the engine's source code.

## 2. Final Directory Structure

```
sep-engine/
├── assets/
│   ├── shaders/              # GLSL/SPIR-V shaders (e.g., pattern_process.comp)
│   └── test_data/            # JSON scenes and other test assets
├── docs/
│   └── ARCHITECTURE.md       # This document
├── include/
│   └── sep/
│       ├── api/
│       ├── audio/
│       ├── blender/
│       ├── compat/           # CUDA implementation and shims
│       ├── context/          # Public context processor headers
│       ├── core/             # Consolidated core headers (config, metrics, dag, etc.)
│       ├── memory/
│       └── quantum/
├── src/
│   ├── api/
│   ├── audio/
│   ├── blender/
│   ├── context/
│   ├── core/
│   ├── memory/
│   └── quantum/
├── tests/                    # Consolidated test suite, mirroring /src
│   ├── api/
│   ├── blender/
│   └── ...
└── third_party/
    ├── crow/
    └── nlohmann/
```

## 3. Component Breakdown

Each module is built as a self-contained static library.

### `core` - The Foundation
*   **Purpose**: Provides the foundational utilities, data structures, and managers required by all other engine modules. It has no dependencies on other SEP modules.
*   **Key Files**:
    *   `engine.cpp`: The main `Engine` class orchestrating the subsystems.
    *   `manager.cpp`: Implements the `ConfigManager` singleton.
    *   `metrics_collector.cpp`, `prometheus_exporter.cpp`, `tracing.cpp`: The unified metrics and tracing system.
    *   `error_handler.cpp`: The singleton for reporting and retrying errors.
    *   `dag_graph.cpp`: The directed acyclic graph implementation for tracking pattern lineage.
*   **Rationale**: The previous `config`, `metrics`, and `dag` modules were small, foundational, and widely used. Merging them into `core` creates a single, robust foundational library (`libsep_core.a`), which simplifies the dependency graph significantly.

### `compat` - CUDA Backend & Shims
*   **Purpose**: Provides the complete CUDA backend for GPU acceleration, along with compatibility shims required for building with different compilers and environments.
*   **Key Files**:
    *   `core/core.cu`: The `CudaCore` singleton managing the CUDA device and kernel launches.
    *   `core/cuda_wrapper.cu`: Low-level wrappers around the CUDA Runtime API.
    *   `quantum_kernels.cu`, `pattern_kernels.cu`: The CUDA kernel implementations.
    *   `raii.cpp`: RAII wrappers for safe management of CUDA resources (streams, events, device memory).
*   **Rationale**: This module was previously named `cuda`. Renaming it to `compat` better reflects its dual role of providing both the CUDA implementation and the necessary shims to abstract it from the rest of the engine.

### `quantum` - The Algorithms
*   **Purpose**: Contains the "secret sauce"—the quantum-inspired algorithms for analyzing and evolving patterns. This module focuses purely on the algorithms themselves.
*   **Key Files**: `qbsa.cpp`, `qfh.cpp`, `evolution.cpp`, `processor.cpp`.
*   **Dependencies**: `core`, `compat`.

### `memory` - Tiered Storage System
*   **Purpose**: Manages the three-tiered memory hierarchy (STM, MTM, LTM) and handles optional pattern persistence via Redis.
*   **Key Files**: `memory_tier_manager.cpp`, `memory_tier.cpp`, `redis_manager.cpp`.
*   **Dependencies**: `core`, `quantum` (for pattern types).

### `context` - High-Level Logic
*   **Purpose**: Acts as the "business logic" layer. It uses the `quantum` algorithms and `memory` tiers to process high-level `Context` objects, extract embeddings, and manage relationships.
*   **Key Files**: `processor.cpp`, `relationship.cpp`.
*   **Dependencies**: `core`, `quantum`, `memory`.
*   **Rationale**: This logic is a clear, high-level abstraction. Giving it a dedicated module with a public `include/sep/context/` header makes its role in the system explicit and moves its public headers out of the `quantum` module.

### `api` - The Public Interface
*   **Purpose**: Exposes the engine's functionality to the outside world via an HTTP server and a C-style bridge.
*   **Key Files**: `server.cpp`, `sep_engine.cpp` (facade), `bridge_c.cpp`, `crow_adapter.cpp`, `crow_error.cpp`.
*   **Dependencies**: `core`, `context`.
*   **Rationale**: The `connection_manager.cpp` implementation was removed as it was unused in production code, leaving only the header for tests. `crow_error.cpp` was moved here from `src/crow` to consolidate all API-related server code.

### `blender` & `audio` - Specialized Integrations
*   **Purpose**: These are optional, platform-specific integrations that can be enabled or disabled at build time.
*   **Key Files**:
    *   `blender`: `api.cpp`, `blender_integration.cpp`, `mesh_handler.cpp`.
    *   `audio`: `pipewire_capture.cpp`, `pipeline.cpp`.
*   **Dependencies**: `core`, `context`.
*   **Rationale**: Keeping these integrations as separate modules prevents their specific dependencies (e.g., PipeWire) from polluting the core engine build.
