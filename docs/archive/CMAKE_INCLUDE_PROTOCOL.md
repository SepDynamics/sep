# CMake Include Protocol

This document outlines the `CMakeLists.txt` files in the project, the files they pertain to, and the correct include protocol for each.

## Top-Level `CMakeLists.txt`

*   **Location:** `/sep/CMakeLists.txt`
*   **Purpose:** Main project file. Configures the entire project, sets global properties, and includes subdirectories.
*   **Key Actions:**
    *   Sets up CUDA compilation.
    *   Finds top-level packages like `Threads`, `TBB`, `OpenGL`, etc.
    *   Adds the `src`, `examples`, and `src/apps` subdirectories.
    *   Defines the `sep_workbench` executable.
*   **Include Protocol:** This file is the root. It does **not** add any global include directories. Include paths are managed by the individual component `CMakeLists.txt` files.

## `/sep/src/CMakeLists.txt`

*   **Location:** `/sep/src/CMakeLists.txt`
*   **Purpose:** A hub file that brings in all the core library components of the SEP engine.
*   **Key Actions:**
    *   `add_subdirectory` for `glad`, `engine`, `memory`, `gpu`, `quantum`, `workbench`, and `connectors`.
*   **Include Protocol:** Does not define any targets or include directories itself. It delegates everything to the subdirectories.

---

## Core Libraries

### `/sep/src/engine/CMakeLists.txt`

*   **Target:** `sep_engine` (static library)
*   **Include Protocol:**
    *   **Public:** Exposes its own directory (`src/engine`) and the root `src` directory.
    *   **Usage:** Any target linking against `sep_engine` can include its headers via `engine/header.h` and any other `src` header via `path/to/header.h`.
    *   **Example:** `#include "engine/manager.h"` or `#include "quantum/qbsa.h"`.

### `/sep/src/memory/CMakeLists.txt`

*   **Target:** `sep_memory` (static library)
*   **Include Protocol:**
    *   **Public:** Exposes the root `src` directory and the `sep_engine`'s include directories.
    *   **Usage:** Can include headers from `engine`, `quantum`, and its own directory.
    *   **Example:** `#include "memory/memory_tier.h"` or `#include "engine/logging.h"`.

### `/sep/src/quantum/CMakeLists.txt`

*   **Target:** `sep_quantum` (static library)
*   **Include Protocol:**
    *   **Public:** Exposes the root `src` directory and the `sep_engine`'s include directories.
    *   **Usage:** Can include headers from `engine`, `memory`, and its own directory.
    *   **Example:** `#include "quantum/qbsa.h"` or `#include "memory/memory_tier_manager.h"`.

### `/sep/src/connectors/CMakeLists.txt`

*   **Target:** `sep_connectors` (static library)
*   **Include Protocol:**
    *   **Public:** Exposes its own directory (`src/connectors`) and the `libcurl` include directories.
    *   **Usage:** Any target linking `sep_connectors` can include its headers via `oanda_connector.h`.
    *   **CRITICAL:** It does **not** expose `src`. Any file within this library that needs to include another core header must use a `src`-relative path, e.g. `#include "quantum/types.h"`.

### `/sep/src/gpu/CMakeLists.txt`

*   **Target:** `sep_gpu_cuda` (static library), `sep_gpu_interface` (interface library)
*   **Include Protocol:**
    *   **Interface:** `sep_gpu_interface` exposes its own directory (`src/gpu`).
    *   **Usage:** Any target linking `sep_gpu_cuda` or `sep_gpu_interface` can include its headers via `gpu/cuda_api.h`.

### `/sep/src/api/CMakeLists.txt`

*   **Target:** `sep_api` (static library)
*   **Include Protocol:**
    *   **Public:** Exposes the root `src` directory and `src/engine`.
    *   **Usage:** Can include any header from the core libraries.

---

## Applications & Workbench

### `/sep/src/workbench/CMakeLists.txt`

*   **Target:** `sep_workbench_lib` (static library)
*   **Include Protocol:**
    *   **Public:** Exposes the root `src` directory, its own directory (`src/workbench`), and the `imgui` backend directory.
    *   **Usage:**
        *   Files inside `workbench` can include other workbench files directly (e.g., `metrics_dashboard.cpp` can use `#include "metrics_dashboard.h"`).
        *   Files inside `workbench` can include any other core library header using a `src`-relative path (e.g., `#include "connectors/oanda_connector.h"`).
    *   **The Error Source:** The recent build failures occurred because `metrics_dashboard.h` was trying to include `oanda_connector.h` directly. Since `sep_connectors` does not expose the root `src` directory to its consumers, and `sep_workbench_lib` links against it, the include path was not resolved. The fix is to ensure that any include from `workbench` to `connectors` is `src`-relative.

### `/sep/src/apps/CMakeLists.txt`

*   **Target:** `oanda_trader` (executable)
*   **Include Protocol:**
    *   **Private:** Includes the root `src` directory and `imgui` directories.
    *   **Usage:** Files in `oanda_trader` can include any core library header using a `src`-relative path.

### `/sep/examples/CMakeLists.txt`

*   **Targets:** `pattern_metric_example`, `financial_demo`, `simple_data_sandbox`, `sep_standalone`, `test_oanda_connector` (executables)
*   **Include Protocol:**
    *   **Private:** Most targets include the root `src` directory.
    *   **Usage:** Files in `examples` can include any core library header using a `src`-relative path.

---

## Other

### `/sep/src/crow/CMakeLists.txt`

*   **Purpose:** Provides fixed headers for the Crow web framework. It uses `include_directories` which is generally discouraged but is used here to ensure its headers are prioritized.

### `/sep/src/tests/CMakeLists.txt`

*   **Purpose:** Configures and builds all unit and integration tests.
*   **Include Protocol:** Includes the root `src` directory for linking against the libraries under test.