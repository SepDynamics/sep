# SEP Engine

The SEP Engine is a high-performance C++ framework for quantum-inspired pattern analysis and evolution. It provides a modular, tiered architecture for processing complex data streams, managing their lifecycle in memory, and exposing insights through a flexible API.

## Key Features

*   **Quantum-Inspired Algorithms**: Implements QBSA (Quantum Bit-State Analysis) and QFH (Quantum Fluctuation Hashing) for sophisticated pattern coherence and stability analysis.
*   **Tiered Memory System**: Automatically manages pattern data across Short-Term (STM), Medium-Term (MTM), and Long-Term (LTM) memory tiers based on coherence and stability metrics.
*   **High-Performance Core**: Built on C++ with a CUDA-accelerated backend for processing large data batches efficiently.
*   **HTTP API & Bridging**: A modern HTTP API built with Crow, alongside a stable C-style bridge for integration with other languages like JavaScript/Node.js.
*   **Specialized Integrations**: Optional modules provide direct integration with Blender (for 3D visualization) and PipeWire (for real-time audio analysis).

## Architecture Overview

The engine is designed with clear, decoupled modules that follow unidirectional dependencies. A foundational `core` library provides common utilities, while specialized modules for `quantum` algorithms, `memory` management, and `context` processing build upon it. The `api` layer exposes the engine's functionality to the outside world.

For a complete breakdown of the architecture, components, and dependency graph, please see the [**ARCHITECTURE.md**](./docs/ARCHITECTURE.md) document.

### Module Breakdown

| Module      | Description                                                                                             |
| :---------- | :------------------------------------------------------------------------------------------------------ |
| **`core`**  | Foundational utilities: engine loop, configuration, error handling, metrics, and the DAG graph.         |
| **`quantum`** | Implements the core quantum-inspired algorithms (QBSA, QFH) and pattern evolution logic.              |
| **`memory`**| Manages the STM/MTM/LTM tiered memory system and optional Redis persistence layer.                      |
| **`context`** | High-level logic for processing context objects, extracting embeddings, and managing relationships.      |
| **`api`**     | Exposes the engine via an HTTP server (Crow), C-language bridge, and supporting clients.               |
| **`blender`** | Optional module for integrating with Blender for 3D visualization of patterns.                        |
| **`audio`**   | Optional module for real-time audio capture and processing via PipeWire.                              |
| **`compat`**  | CUDA backend implementation, providing GPU acceleration and compatibility shims.                      |

## Building the Engine

The project uses CMake for building.

**1. Install Dependencies:**
Ensure you have a modern C++ compiler (GCC 12+), CMake (3.22+), and the required libraries (Boost, TBB, cURL). If GPU support is needed, the CUDA toolkit (12.x) is required.

**2. Configure and Build:**

```bash
mkdir build
cd build
cmake ..
make -j$(nproc)
```

This will produce the main `sep_engine` executable and the associated static libraries for each module.

## Running the Engine

The primary output is a server executable. Run it from the build directory:

```bash
./sep_engine
```

Configuration can be provided via a JSON file, environment variables, or command-line arguments. See `src/core/manager.cpp` for details.

## Testing

The consolidated test suite is located in the `/tests` directory. To build and run the tests:

```bash
# From the build directory
cmake .. -DSEP_BUILD_TESTS=ON
make -j$(nproc)
ctest
```