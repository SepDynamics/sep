### **Project Dossier: SEP Engine**

**Document ID:** SEP-PLAN-20250709-B
**Revision:** 2.0
**Subject:** Build Failure Analysis & Workbench Integration Plan

**1.0 Executive Summary**

The SEP Engine project has encountered a critical build failure within the `sep_api` module, blocking all further integration and testing. Analysis of the build log indicates a severe type-system conflict stemming from the project's CUDA compatibility shims (`crow_isolation.h`, `shim.h`) being incorrectly applied during standard C++ compilation. This is a **Severity 1 issue** that must be resolved before any other work proceeds.

The static analysis report highlights several lower-priority code quality issues (dead stores, potential null dereferences in third-party code) and one medium-priority concurrency risk (`BlockInCriticalSection` in the audio module). While these should be addressed, they are secondary to the primary build failure.

This document provides a revised, prioritized action plan. The immediate focus is to unblock the build by resolving the header and type conflicts. Subsequent phases will focus on hardening the build system and then proceeding with the original plan to implement the `sep_workbench` application.

---

### **2.0 The Mechanic's Diagnosis ("What's Wrong with the Engine")**

Alright, we put the engine on the stand, tried to turn it over, and it seized up hard. The build failed.

The problem is in the wiring harness for the API (`libsep_api`). It's a mess of crossed wires.

We built a set of special, low-voltage wiring adapters (`crow_isolation.h`, `shim.h`) so we could plug our high-performance ECU (`libsep_quantum`) into the supercharger's diagnostic port (the CUDA compiler, `nvcc`). That was smart for testing the supercharger on its own.

The problem is, we're now trying to use those same 5-volt adapters to wire up the main 12-volt battery and dashboard (`std::string`, standard C++). The connectors don't match. The compiler is throwing sparks, telling us `crow_string` is not a `std::string`, and that essential components like `crow::status` are missing entirely. It's a major electrical short.

**The Fix:** We have to rip out the bad wiring. We'll use the proper, heavy-gauge 12-volt harness for the main engine build and only use the special 5-volt adapters for the supercharger diagnostics. We need to go through the `api` module, snip out every reference to the isolation shims, and wire it up to the *real* parts. No more adapters where they don't belong.

---

### **3.0 QA Inspector's Corrective Action Plan**

**Finding:** Critical build failure in target `sep_api` due to improper inclusion of compatibility shims, resulting in multiple `error: ‘...’ has not been declared` and type mismatch errors.
**Root Cause:** The preprocessor logic (`#ifdef CROW_DISABLE_RTTI`) intended to select between real and stubbed headers is failing or being misconfigured, causing the CUDA-specific `crow_isolation.h` to be used in a standard C++ compilation context. This creates a direct conflict between the shim's types (e.g., `crow::crow_string`) and the standard library's types (`std::string`).
**Corrective Action:** A new Phase 0 is required to un-block the build. All other development is on hold until Phase 0 is complete and validated.

---

## **Revised To-Do List: Path to Workbench Executable**

### **Phase 0: Un-block the Build (Critical Path)**

**Objective:** Get a clean, successful build of all static libraries.

1.  [ ] **Fix Crow Header Conflicts in `libsep_api`:**
    *   **Task:** This is the primary build-breaker. The `crow_isolation.h` shim is for CUDA (`nvcc`) compilation only, where standard library headers can cause issues. Your regular C++ compiler needs the real Crow headers.
    *   **Action:**
        1.  In `src/api/auth_middleware.h`, `src/api/rate_limit_middleware.h`, and any other API headers, **remove the `#ifdef CROW_DISABLE_RTTI` logic.**
        2.  These files should *only* include the real Crow headers (e.g., `#include "crow/http_request.h"`).
        3.  Update your `src/api/CMakeLists.txt` to ensure it only includes the real Crow headers from `third_party/crow/include`. It should **not** see the isolation shims. The shims are for `libsep_compat` and CUDA kernels.
    *   **QA Rationale:** This enforces proper header scoping and resolves the immediate type conflicts causing the build to fail.

2.  [ ] **Fix Type Mismatches (`crow_string` vs `std::string`):**
    *   **Task:** The build log shows you're trying to construct `std::string` from `crow::crow_string` (the shim type). This won't work.
    *   **Action:**
        1.  In files like `src/api/crow_adapter.cpp` and `crow_request_adapter.cpp`, replace all usage of `crow::crow_string` with `std::string`.
        2.  The `crow_isolation.h` shim should be modified so its `request` and `response` structs use `std::string` directly, or you need to perform explicit conversions (`std::string(the_crow_string.c_str())`). The former is cleaner.
    *   **QA Rationale:** Unifies the string type used across the API layer, eliminating conversion errors.

3.  [ ] **Fix ASIO and Socket Adapter Errors:**
    *   **Task:** The errors in `socket_adaptors.h` and `asio_isolation.h` point to namespace and type conflicts with the Boost.Asio library.
    *   **Action:**
        1.  In `src/crow/asio_isolation.h`, the `asio_stub` namespace alias is a good idea but needs to be used consistently. Ensure all Boost.Asio types are used through one consistent namespace (`asio` or `asio_stub`).
        2.  Fix the error in `SocketAdaptor::close()` by handling the `asio::error_code` correctly, not by trying to assign it to an integer `error_t`.
    *   **QA Rationale:** Corrects linker and type errors related to the networking backend.

4.  **Re-run Build:**
    *   **Task:** Run `make -j$(nproc)` from a clean build directory.
    *   **Exit Criteria:** The build for **all** static libraries (`.a` files) completes with zero errors.

### **Phase 1: Stabilization & Refactoring (The Shop Cleanup)**

**(Begins after Phase 0 is complete)**

1.  [ ] **Refactor `memory` and `quantum` Modules:** This remains a high-priority architectural fix.
2.  [ ] **Consolidate Header Files & Enforce `-Werror`:** With the build unblocked, immediately enforce code hygiene. A clean, warning-free build is the new baseline.
3.  [ ] **Review and Triage Static Analysis Report:**
    *   Address the `deadcode.DeadStores` and `unix.BlockInCriticalSection` warnings in your own code (`pattern_processor.cpp` and `pipewire_capture.cpp`).
    *   Log the `core.NullDereference` and `core.CallAndMessage` findings in third-party headers as items to monitor, but do not block progress on them.

### **Phase 2: Build the `sep_workbench` Executable**

**Objective:** Assemble the components into the final visual demo application.

1.  [ ] **Create the Workbench CMake Target:**
    *   **Task:** In `src/CMakeLists.txt`, add a new `add_executable(sep_workbench ...)` target.
    *   **Sources:** Add `main.cpp`, `config.cpp`, and all files from the `src/demos/` directory.
    *   **Mechanic's Take:** You're now building the car's chassis and dropping the engine into it.

2.  [ ] **Link All Engine and System Dependencies:**
    *   **Task:** In the `sep_workbench` `CMakeLists.txt`, add a `target_link_libraries` command.
    *   **Action:** Link against **all** of your static libraries: `sep_api`, `sep_quantum`, `sep_memory`, `sep_core`, `sep_compat`, `sep_blender`, `sep_audio`, etc. Also link against required system libraries (GLFW, GLEW, OpenGL).
    *   **QA Rationale:** This integrates all previously validated subsystems into a single application binary.

3.  [ ] **Implement the Workbench `main.cpp`:**
    *   **Task:** Follow the `SEP_WORKBENCH_CHECKLIST.md`. The `main.cpp` will be the application's entry point.
    *   **Action:**
        1.  Initialize GLFW and create a `Window`.
        2.  Initialize the SEP Engine (via your wrapper or directly).
        3.  Initialize the `DemoManager`.
        4.  Register all the demo classes (e.g., `manager.registerDemo("genesis", ...)`).
        5.  Set up the main application loop (`while (!window.shouldClose())`).
        6.  Inside the loop, call `demoManager.update()` and `demoManager.render()`.
    *   **QA Rationale:** Establishes the primary control flow for the user-facing application.

4.  [ ] **Build and Run the Workbench:**
    *   **Task:** Compile the `sep_workbench` executable.
    *   **Exit Criteria:** The application launches, displays a window, and runs the default "Genesis Pattern" demo without crashing.
