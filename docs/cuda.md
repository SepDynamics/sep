# CUDA Codebase Notes

This document summarizes the contents of `src/compat` and `include/compat`.

## Directory Overview

`src/compat/README.md` describes the CUDA layer as consisting of kernels, wrapper classes and a small C API. The wrappers live under `core/` while kernels like `pattern_kernels.cu` and `quantum_kernels.cu` implement the compute work. Utility files such as `event.cu` and `raii.cpp` provide RAII helpers.

The README also warns that `core/core.cu` duplicates earlier functionality and is not referenced by the build. Generated files like `cmake_install.cmake` should be removed from version control.

`include/compat/README.md` lists the public headers that mirror the implementation directory. Files such as `cuda_wrapper.h`, `core.h`, `raii.h`, `stream.h` and `event.h` map to the sources in `src/compat`.

## Build Artifacts and Unused Files

- `src/compat/cmake_install.cmake` is a generated install script and can be deleted.
- `src/compat/core/core.cu` is legacy code and not used by the build.

## Header to Implementation Mapping

- `cuda_wrapper.h` → `core/cuda_wrapper.cu`
- `core.h` → `core/core.cpp`
- `stream.h` → `core/stream.cpp` (helper struct defined inline in `stream_impl.h`)
- `event.h` → `event.cu`
- `raii.h` → `raii.cpp`
- `kernels.h` / `kernels.cuh` → `pattern_kernels.cu` and `quantum_kernels.cu`
- Memory helpers declared in `memory.h` are implemented in `raii.cpp`
- `cuda_api.cu` exposes a C interface but lacks a dedicated header.

## Duplicate or Legacy Kernels

Only one copy of `pattern_kernels.cu` and `quantum_kernels.cu` exists. The file `core/core.cu` duplicates earlier functionality and can be safely removed.

