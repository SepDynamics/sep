# Blender Module Notes

This file summarizes how the SEP Engine hooks into Blender.  It focuses on three areas:
shader handling, the GPU context helpers, and how build artefacts were cleaned up.

## Shader Handling

Shaders live under `assets/shaders` and are compiled during the build.  The
`pattern_visualization_pipeline.cpp` file reloads the compute shader
`pattern_process.comp` whenever assets change.  The resulting SPIR-V binary is
passed to Blender's GPU API through the `GPUContext` wrapper.  No other shader
sources are present.

## GPU Context Code

`include/blender/gpu_context.h` defines a lightweight `GPUContext` class and a
`GpuBufferPtr` RAII helper.  Only `init`, `deleteBuffer`, `mapBuffer` and
`unmapBuffer` are provided, with stub implementations that return success or
`nullptr`.  `src/blender/gpu_context.cpp` merely wires the `GPUBufferDeleter`
operator to call `deleteBuffer`.  More advanced operations are expected to be
implemented once the real Blender runtime is available.

## Cleanup Approach

Old CMake output files such as `cmake_install.cmake` were removed from the
repository.  The engine now relies solely on `CMakeLists.txt` for build
information.  Headers in `include/blender` avoid heavy inline implementations;
only small helpers like `sep_result_to_string` in `types.h` and the `contains`
method in `base_types.h` remain inline.

