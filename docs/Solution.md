**Core Problem:**

The compiler (`clang++`) is not "seeing" the definitions for standard CUDA types (like `cudaError_t`, `cudaStream_t`, `cudaEvent_t`) and constants/functions (like `cudaSuccess`, `cudaMallocManaged`) at the point where your code (or your project's compatibility headers) are trying to use them. This happens despite `SEP_CUDA_AVAILABLE=1` being defined and `-isystem /usr/local/cuda-12.9/include` being in the compile flags.

The most likely culprits are:
1.  **Incorrect Include Order**: System CUDA headers (like `cuda_runtime.h`) are not being included *before* your project's headers that depend on CUDA types.
2.  **Conflicting Definitions/Shadowing**: Your project (specifically within `/sep/include/compat/` or `/sep/include/`) might have its own files named similarly to standard CUDA headers (e.g., `cuda_runtime.h`, `cuda_common.h`) or files that *redefine* CUDA types/constants. This can lead to the compiler picking up your project's incomplete or conflicting definitions instead of the official ones, or causing redefinition errors. The `report.md` strongly suggests this with redefinition errors for `cudaSuccess` and `cudaMemcpyKind` enums.
3.  **Mismanagement of Conditional Compilation**: The `SEP_CUDA_AVAILABLE` macro might not be consistently gating all necessary inclusions or definitions, or the `__CUDACC__` macro (used by NVIDIA's `nvcc` compiler) might be inappropriately influencing header inclusion logic when compiling with `clang++`.

**High-Level Strategy for Resolution:**

The goal is to establish a "single source of truth" for CUDA definitions when `SEP_CUDA_AVAILABLE=1` (which should be the system's CUDA headers) and a clear, non-conflicting stubbing mechanism when `SEP_CUDA_AVAILABLE=0`.

Here's a conceptual document to guide your refactoring:

---

## Refactoring Strategy for CUDA Integration Issues

**Objective:** Ensure consistent and correct visibility of CUDA type definitions and API declarations across the `sep` project, resolving compilation errors related to "unknown type" and "undeclared identifier" for CUDA symbols.

**Guiding Principles:**

1.  **System CUDA Headers as Authority (when `SEP_CUDA_AVAILABLE=1`):** The CUDA toolkit's headers (e.g., `<cuda_runtime.h>`, `<driver_types.h>`) are the definitive source for all CUDA types, enums, and function declarations. Project code should not redefine these.
2.  **Centralized Inclusion Point:** Manage the inclusion of system CUDA headers through a single, project-controlled header file. This ensures they are included correctly and consistently.
3.  **Clear Stubbing Mechanism (when `SEP_CUDA_AVAILABLE=0`):** If the project needs to compile without CUDA, stubs for essential CUDA types and functions should be minimal, clearly demarcated, and must not conflict with actual CUDA definitions.
4.  **Compatibility Layer (`sep/include/compat/`) Purpose:** These headers should *wrap* or *extend* CUDA functionality or provide *true stubs*, not redefine fundamental CUDA elements. They must rely on the centralized inclusion point for CUDA definitions.
5.  **Minimize Header Pollution:** Avoid including large CUDA headers (like `<cuda_runtime.h>`) in widely used project headers unless absolutely necessary. Prefer forward declarations where possible and include the full headers in `.cpp` or `.cu` files.

**Key Components and Variables to Manage:**

1.  **`SEP_CUDA_AVAILABLE` Macro:**
    *   **Management**: This is the primary switch. It's correctly defined in your build system.
    *   **Mindfulness**: All CUDA-dependent code sections, custom type definitions (if any that *wrap* CUDA types), and inclusions of CUDA-specific project headers should be guarded by `#if SEP_CUDA_AVAILABLE`.

2.  **System CUDA Include Path (`/usr/local/cuda-12.9/include`):**
    *   **Management**: Correctly specified via `-isystem`.
    *   **Mindfulness**: Ensure no project include paths (`-I/sep/include`) contain files that inadvertently shadow system CUDA headers (e.g., a project file named `cuda_runtime.h` in a high-priority include directory).

3.  **CUDA Types (`cudaError_t`, `cudaStream_t`, `cudaEvent_t`, `cudaDeviceProp`, etc.):**
    *   **Management**: These are defined in system CUDA headers (primarily `<driver_types.h>` which is pulled in by `<cuda_runtime.h>`).
    *   **Mindfulness**: Your project code should never redefine these. If your `compat` layer needs to refer to them, it must ensure the system headers have been included first.

4.  **CUDA Constants/Enums (`cudaSuccess`, `cudaMemcpyHostToDevice`, etc.):**
    *   **Management**: Also defined in system CUDA headers.
    *   **Mindfulness**: Your project code (e.g., `cuda_impl.h` as per the report) should not redefine these. Stub versions for `SEP_CUDA_AVAILABLE=0` must use distinct names or be carefully guarded to avoid redefinition when CUDA is enabled.

5.  **CUDA API Functions (`cudaMallocManaged`, `cudaFree`, `cudaStreamCreateWithFlags`, etc.):**
    *   **Management**: Declarations are in system CUDA headers.
    *   **Mindfulness**: Your `compat` layer might wrap these or provide stub implementations. Stubs must be strictly guarded by `#if !SEP_CUDA_AVAILABLE`.

6.  **Project's Compatibility Headers (in `sep/include/compat/`):**
    *   `cuda_common.h`, `cuda_helpers.h`, `cuda_impl.h`, `raii.h`, `core.h` (related to CUDA), `kernels.cuh`:
        *   **Management**: These are your abstraction/utility layers.
        *   **Mindfulness**:
            *   They *must* include your new "Central CUDA Include Header" (see below) at their very beginning if they use any CUDA types or call CUDA APIs.
            *   They must *not* redefine fundamental CUDA types/constants. Their role is to build *upon* the system CUDA definitions.
            *   If they provide function stubs for when `SEP_CUDA_AVAILABLE=0`, these stubs must be within `#if !SEP_CUDA_AVAILABLE ... #endif` blocks.
    *   `compat/cuda_runtime.h` (Project's version):
        *   **Management**: This file is highly problematic due to name collision.
        *   **Mindfulness**: It should be renamed (e.g., `sep_cuda_compat_utils.h`) or its content integrated elsewhere. It should not attempt to be a replacement for the system `<cuda_runtime.h>`.

7.  **`SEP_HD` Macro (for `__host__ __device__`):**
    *   **Management**: This controls compilation for host, device, or both.
    *   **Mindfulness**: Its definition should be centralized. When compiling `.cpp` files with `clang++` (even if CUDA-aware), it might need to be empty for functions not intended for device code. `nvcc` handles this naturally for `.cu` files. A robust definition should consider `defined(__CUDACC__)`.

**Proposed Refactoring Steps (High-Level):**

1.  **Step 1: Create a Central CUDA Include Header.**
    *   **Action**: Create a new header file, e.g., `/sep/include/compat/sep_cuda_foundations.h`.
    *   **Content**:
        ```c++
        #ifndef SEP_CUDA_FOUNDATIONS_H
        #define SEP_CUDA_FOUNDATIONS_H

        // This macro is set by your CMake build system.
        // #define SEP_CUDA_AVAILABLE 1 (Example, should come from build)

        #if SEP_CUDA_AVAILABLE
            // --- CUDA IS ENABLED ---
            // Include core system CUDA headers. This ensures all standard CUDA
            // types, enums, and function declarations are available.
            #include <cuda_runtime.h> // Main CUDA runtime API
            #include <driver_types.h> // For cudaError_t, cudaEvent_t, etc. (often included by cuda_runtime.h)

            // Define SEP_HD for CUDA-aware compilation contexts
            #if defined(__CUDACC__) // Compiling with NVCC
                #define SEP_HD __host__ __device__
            #else // Compiling with a C++ compiler (like clang++ aware of CUDA toolkit)
                  // For functions declared in .h files included by both .cu and .cpp,
                  // and intended for both host and device.
                #define SEP_HD __host__ __device__ // Clang supports this with CUDA integration
            #endif
            #define SEP_H __host__
            #define SEP_D __device__

        #else // --- CUDA IS DISABLED ---
            // Provide minimal, non-conflicting stubs for essential CUDA types
            // to allow the codebase to compile.
            typedef int cudaError_t;
            #define cudaSuccess 0
            // Add other common error codes if your stubs check for them
            #define cudaErrorMemoryAllocation 2
            #define cudaErrorInvalidValue 1

            // Opaque pointer types for streams and events
            typedef void* cudaStream_t;
            typedef void* cudaEvent_t;

            // Minimal stub for cudaDeviceProp if it appears in shared headers
            typedef struct { int major; int minor; char name[256]; } cudaDeviceProp;

            // Stub for cudaMemcpyKind (use distinct names to avoid conflict if real headers are somehow included)
            enum sep_cudaMemcpyKind_stub {
                sep_cudaMemcpyHostToHost = 0,
                sep_cudaMemcpyHostToDevice = 1,
                sep_cudaMemcpyDeviceToHost = 2,
                sep_cudaMemcpyDeviceToDevice = 3,
                sep_cudaMemcpyDefault = 4
            };
            // Map standard names to stub names IF AND ONLY IF CUDA IS NOT AVAILABLE
            #define cudaMemcpyHostToDevice sep_cudaMemcpyHostToDevice
            #define cudaMemcpyDeviceToHost sep_cudaMemcpyDeviceToHost
            #define cudaMemcpyDefault sep_cudaMemcpyDefault
            // ... other constants as needed by your compat layer's API surface.

            #define SEP_HD
            #define SEP_H
            #define SEP_D

        #endif // SEP_CUDA_AVAILABLE

        #endif // SEP_CUDA_FOUNDATIONS_H
        ```

2.  **Step 2: Audit and Refactor Project's `compat` Headers.**
    *   **Action**: For each header in `/sep/include/compat/` (and any other custom CUDA-related headers):
        1.  **Include the Central Header**: Add `#include "compat/sep_cuda_foundations.h"` (adjust path) as the *very first* substantial include.
        2.  **Remove Redundant Definitions**: Delete any lines that redefine `cudaError_t`, `cudaSuccess`, `cudaStream_t`, `SEP_HD`, `cudaMemcpyKind` enum values, etc. These will now come from `sep_cuda_foundations.h`.
        3.  **Guard API Stubs**: If a `compat` header provides *stub implementations* of CUDA API functions (e.g., a fake `cudaMallocManaged`), ensure these are strictly within `#if !SEP_CUDA_AVAILABLE ... #endif` blocks.
        4.  **