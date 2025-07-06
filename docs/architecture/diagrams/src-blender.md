# Blender Integration Source Layout

This document outlines the key files involved in the SEP Blender integration and shows how pattern data flows through the system.

## Directory Overview

```
src/blender/
├── api.cpp                 # C API for registering objects and updating patterns
├── blender_integration.cpp # Entry point for linking SEP with Blender runtime
├── mesh_handler.cpp        # Creates, updates and deforms Blender meshes
├── gpu_context.cpp         # Manages compute shader and GPU buffers
├── pattern_visualization_pipeline.cpp # Converts pattern data into mesh updates
├── cycles_renderer.cpp     # Pattern-driven rendering using Cycles (when SEP_HAS_CYCLES=1)
├── compression.cpp         # Pattern data compression utilities
└── compression_utils.cpp   # Helper functions for compression
```

## Data Flow

1. **Pattern Receipt**
   - `api.cpp` exposes C functions such as `sep_register_mesh` and `sep_update_mesh`.
   - Blender calls these to send mesh handles and pattern metrics into the engine.

2. **Processing and Storage**
   - `blender_integration.cpp` creates a `PatternBridge` instance which stores object handles and pattern state.
   - `gpu_context.cpp` ensures GPU resources are ready for compute shaders.
   - `mesh_handler.cpp` converts SEP pattern data into Blender mesh structures.

3. **Visualization Pipeline**
   - `pattern_visualization_pipeline.cpp` orchestrates projection from N‑dimensional pattern coordinates to 3D space and updates meshes via `MeshHandler`.
   - Optional overlays such as coherence history are set via GPU uniform layers.

4. **Rendering Pipeline (when SEP_HAS_CYCLES=1)**
   - `cycles_renderer.cpp` provides pattern-driven rendering capabilities:
     - Converts patterns to Cycles scenes
     - Maps pattern properties (coherence, stability, entropy) to visual elements
     - Supports real-time scene updates based on pattern evolution
   - Currently uses stub implementation when Cycles is not available

5. **Handoff to Other Modules**
   - After mesh updates or rendering, control can return to higher‑level modules (e.g., the Python addon in `blender_addon`) or custom visualization code.
   - Metrics and pattern identifiers are passed back through the API to track engine state.

This pathway allows patterns produced by the SEP engine to be visualized inside Blender while keeping the integration modular.
