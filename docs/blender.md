# Blender Integration Overview

This document summarizes the Blender-facing portion of the SEP engine.  It lists the primary source files under `src/blender` and the public headers in `include/blender` along with notes on how they relate.  The final section highlights unused pieces that could be removed or consolidated.

## Source to Header Mapping

| Header | Implementation |
|-------|---------------|
| `api.h` | `src/blender/api.cpp` |
| `bridge.h` | not implemented directly; API and tests include this but the class is defined in `pattern_bridge.h` and implemented in `blender_integration.cpp` |
| `pattern_bridge.h` | `src/blender/blender_integration.cpp` |
| `compression.h` | `src/blender/compression.cpp`, `src/blender/compression_utils.cpp` |
| `gpu_context.h` | `src/blender/gpu_context.cpp` (minimal implementations) |
| `mesh_handler.h` | `src/blender/mesh_handler.cpp` |
| `pattern_visualization_pipeline.h` | `src/blender/pattern_visualization_pipeline.cpp` |
| `trace_viewer/trace_viewer.h` | `src/blender/trace_viewer/trace_viewer.cpp` |

Other headers such as `config.h`, `base_types.h`, `pattern_common.h`, `types.h`, and the compatibility headers provide shared structures or helper types and do not have dedicated source files.

## Unused or Redundant Pieces

- **`pattern_bridge.h` vs `bridge.h`** – Both headers declare `BlenderBridge`.  The engine uses `pattern_bridge.h` in `blender_integration.cpp`, while the C API and tests include `bridge.h`.  Maintaining two variants leads to duplicate declarations.  Consolidating these into a single header would remove redundancy.
- **`mock_types.h`** – Only referenced in tests (`tests/blender/api_test.cpp`).  It can remain for unit tests, but it is not needed for production builds.
- **Shaders** – The compute shader `pattern_process.comp` and its compiled `pattern_process.spv` (under `assets/shaders/`) are actively used.  No additional sample shaders were found.

## Minimal Implementations and Refactoring Targets

Several `.cpp` files provide only skeletal implementations:

- `gpu_context.cpp` merely defines the `GPUBufferDeleter` operator because most GPU context methods are inline stubs in `gpu_context.h`.  Tests reference richer functionality that is not implemented.  Extending `GPUContext` or refactoring tests to match the stubbed API would avoid confusion.
- `trace_viewer.cpp` is a tiny utility printing pattern evolution data.  It has no complex logic and could be integrated into tools or removed if unused.

`pattern_bridge.h`/`bridge.h` duplication is the main structural issue.  Aligning the implementation (`blender_integration.cpp`) with a single public header would simplify maintenance and clarify which interface is stable.
