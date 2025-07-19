# Technical Plan: CUDA Build Fix

**Objective:** This document outlines the precise technical steps required to resolve the CUDA-related build failures. The changes are based on the analysis of `docs/TODO.md` and the affected CMake files.

---

## 1. Fix `cmake/FindCUDA.cmake`

- **File:** `cmake/FindCUDA.cmake`
- **Problem:** The `find_path` command on line 18 does not correctly search for the `nvcc` compiler in the `bin` subdirectory of the CUDA toolkit.
- **Change:** Modify line 18 to correctly locate the CUDA toolkit directory by searching for `nvcc` in the `bin` subdirectory.

**Action:**
Replace line 18:
```cmake
find_path(CUDA_TOOLKIT_ROOT_DIR NAMES nvcc HINTS /usr/local/cuda)
```
with:
```cmake
find_path(CUDA_TOOLKIT_ROOT_DIR NAMES nvcc PATH_SUFFIXES bin HINTS /usr/local/cuda)
```

---

## 2. Fix `cmake/cuda-toolchain.cmake`

- **File:** `cmake/cuda-toolchain.cmake`
- **Problem:** The toolchain file incorrectly points to `${CUDA_PATH}/src` for include directories instead of `${CUDA_PATH}/include`. This affects multiple variables.
- **Change:** Correct all occurrences of `${CUDA_PATH}/src` to `${CUDA_PATH}/include`.

**Action:**
- **Line 17:**
  - **From:** `set(CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES ${CUDA_PATH}/src CACHE STRING "CUDA include dirs" FORCE)`
  - **To:** `set(CMAKE_CUDA_TOOLKIT_INCLUDE_DIRECTORIES ${CUDA_PATH}/include CACHE STRING "CUDA include dirs" FORCE)`
- **Line 21:**
  - **From:** `set(CUDA_INCLUDE_DIRS ${CUDA_PATH}/src CACHE PATH "CUDA Include location" FORCE)`
  - **To:** `set(CUDA_INCLUDE_DIRS ${CUDA_PATH}/include CACHE PATH "CUDA Include location" FORCE)`
- **Line 24:**
  - **From:** `set(CMAKE_CUDA_IMPLICIT_INCLUDE_DIRECTORIES "${CUDA_PATH}/src" CACHE STRING "CUDA implicit includes" FORCE)`
  - **To:** `set(CMAKE_CUDA_IMPLICIT_INCLUDE_DIRECTORIES "${CUDA_PATH}/include" CACHE STRING "CUDA implicit includes" FORCE)`

---

## 3. Review `cmake/cuda-clang-toolchain.cmake`

- **File:** `cmake/cuda-clang-toolchain.cmake`
- **Problem:** `docs/TODO.md` mentions a required fix on line 13, but the file is only 3 lines long and contains no include paths.
- **Analysis:** The file appears to be used in conjunction with `cuda-toolchain.cmake` and does not require any direct changes. The issue mentioned in the `TODO` is likely resolved by the fixes in `cuda-toolchain.cmake`.
- **Action:** No changes are required for this file.

---

## 4. Fix `cmake/cuda-gcc-toolchain.cmake`

- **File:** `cmake/cuda-gcc-toolchain.cmake`
- **Problem:** This file has a copy-paste error. It's intended for a GCC toolchain but sets the CUDA host compiler to Clang (`clang++-15`) and has a comment mentioning Clang.
- **Change:** Correct the host compiler to `clang++-15` (to match the default toolchain) and update the comment.

**Action:**
Replace the content of the file:
```cmake
# Custom CUDA toolchain for Clang
set(CMAKE_CUDA_HOST_COMPILER "clang++-15")
```
with:
```cmake
# Custom CUDA toolchain for GCC
set(CMAKE_CUDA_HOST_COMPILER "clang++-15")