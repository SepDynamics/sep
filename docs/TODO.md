# TODO.md - SEP Financial Tool Development Priorities

## Immediate Priorities (Fix Today)
- Stabilize OANDA connection to prevent crashes (debug connectToOanda, add error handling, manage streaming thread).
- Implement static UI layout with fixed panels for usability (use ImGui::SetNextWindowPos/Size).
- Integrate real-time OANDA data into SEP engine (format data, pass to engine, display output metrics).
- Map SEP outputs to financial metrics (volatility to coherence, volume to stability, order book to entropy).

## Code Implementation TODOs
From src/apps/oanda_trader/oanda_trader_app.cpp:
- Implement order placement logic.
- Display actual positions from OANDA.
- Display order history from OANDA.

From src/memory/memory_tier_manager.cpp:
- Implement cleanupExpiredData().
- Implement pruneDataByPriority().
- Implement pruneWeakRelationships().
- Implement calculateRelationshipScores().
- Implement calculateRelationshipCoherence().
- Implement cleanupExpiredPatterns().
- Implement prunePatternsByPriority().
- Implement registerGenericData() with proper cloning.
- Implement loadDataFromPersistence().
- Implement storeDataToPersistence().
- Implement processMemoryBlocks().

## Future Tasks
- Implement backtesting framework (event-driven, performance metrics).
- Develop pattern-based trading strategies.
- Add risk management (position sizing, stop-loss, portfolio risk).
- Expand data sources (Yahoo Finance, Alpha Vantage).
- Optimize performance (GPU acceleration, memory pruning).
- Implement API endpoints and monitoring dashboard.

## Notes
- Focus on dynamic, quick shifts to resolve issues.
- Test each component before integration.
- Document implementations.
- Reuse existing SEP engine; prioritize data pipeline.

---

## **Comprehensive Project Analysis & Resolution Report: SEP Engine**

### **1. Executive Summary**

This report provides a holistic analysis of the SEP Engine project. The project is a sophisticated C++ application designed for high-performance data analysis, with a strong focus on quantitative finance, pattern recognition, and real-time trading capabilities. It leverages CUDA for GPU acceleration, a tiered memory management system, and a suite of custom "quantum-inspired" algorithms for deep pattern analysis. A graphical user interface (GUI) built with ImGui serves as a workbench for visualization and interaction.

**Architectural Overview:**
The project is well-structured into distinct components:
*   **API (`/src/api`):** A web service backend using the Crow framework, exposing the engine's capabilities via a REST API.
*   **Engine (`/src/engine`):** The core CUDA/CPU processing backend, handling data parsing, logging, and Directed Acyclic Graph (DAG) computations.
*   **Quantum (`/src/quantum`):** The proprietary algorithms (QBSA, QFH) for pattern evolution, coherence, and stability analysis.
*   **Memory (`/src/memory`):** A sophisticated tiered memory manager (STM, MTM, LTM) with Redis integration for persistence.
*   **Connectors (`/src/connectors`):** Modules for integrating with external data sources, specifically the OANDA forex trading platform.
*   **Workbench (`/src/workbench`):** A comprehensive GUI for demonstrating engine features, visualizing data, and providing a trading dashboard.

**Static Analysis Findings:**
A static analysis scan has identified **679 issues** of varying severity. A significant portion of these issues are located within third-party dependencies (`yaml-cpp`, `imgui`), which is common. However, several critical and high-severity issues have been found that require immediate attention.

**Key Findings:**
1.  **Critical Build Errors:** The analysis found 5 critical errors in `yaml-cpp` that will prevent compilation. These are due to missing standard headers.
2.  **High-Severity Risks:** There are 72 high-severity issues, primarily related to incorrect floating-point-to-integer rounding, suspicious `sizeof` usage on pointers, and potential null pointer dereferences. These bugs can lead to incorrect calculations, crashes, and unpredictable behavior.
3.  **Medium-Severity Issues:** 476 medium-severity issues were found, including the use of reserved identifiers, undefined behavior from manipulating non-trivial C++ objects with C-style memory functions (`memcpy`/`memset`), and unchecked return values from standard library functions.
4.  **Code Hygiene Issues:** Numerous low-severity and style issues were identified, most notably circular header dependencies within the core engine and quantum modules, which can increase compilation times and cause subtle build errors.

**Primary Recommendation:**
A prioritized, step-by-step resolution plan is provided below. The immediate focus should be on resolving the **Critical** build errors. Following that, address the **High** severity issues within the third-party code, as they represent significant risks to correctness and stability. Finally, address the **Medium** and **Low** severity issues within the proprietary codebase (`engine`, `quantum`, `api`) to improve code quality, maintainability, and prevent future bugs.

---

### **2. Detailed Static Analysis Findings & Resolution Strategy**

The following section details the most critical categories of issues found and provides clear instructions on how to resolve them.

#### **Priority 1: Fix Build-Breaking Errors (Critical)**

These errors prevent the project from compiling successfully and must be addressed first.

*   **Issue:** Use of undeclared identifiers `uint16_t` and `uint32_t`.
*   **File:** `/sep/build/_deps/yaml-cpp-src/src/emitterutils.cpp`
*   **Severity:** CRITICAL
*   **Cause:** The file uses fixed-width integer types without including the necessary header.
*   **Instruction:**
    1.  Open the file `/sep/build/_deps/yaml-cpp-src/src/emitterutils.cpp`.
    2.  Add the following line at the top of the file with the other includes:
        ```cpp
        #include <cstdint>
        ```
    This will provide the definitions for `uint16_t` and `uint32_t`, resolving the compilation errors.

#### **Priority 2: Address High-Severity Risks**

These issues introduce a high risk of runtime crashes, incorrect calculations, or undefined behavior.

##### **A. Incorrect Floating-Point Rounding (`bugprone-incorrect-roundings`)**

*   **Issue:** Casting a floating-point expression like `(float + 0.5f)` to an integer is a common but unreliable method for rounding. It can produce incorrect results due to floating-point representation.
*   **Files:** `imgui.cpp`, `imgui_internal.h`, `imgui_widgets.cpp`, `imgui_demo.cpp`
*   **Instruction:**
    1.  Include the `<cmath>` header where necessary.
    2.  Replace instances of `(int)(expression + 0.5f)` or the `IM_ROUND` macro with `lround(expression)`. `lround()` is the standard and correct way to round a floating-point number to the nearest integer.

    **Example Fix in `/sep/third_party/imgui/imgui.cpp:13511`:**
    ```cpp
    // Before
    const float scroll_speed = IM_ROUND(window->FontRefSize * 100 * io.DeltaTime);

    // After (assuming <cmath> is included)
    #include <cmath> // Add if not present
    const float scroll_speed = lround(window->FontRefSize * 100 * io.DeltaTime);
    ```

##### **B. Suspicious `sizeof` Usage on Pointers (`bugprone-sizeof-expression`)**

*   **Issue:** Using `sizeof(pointer_variable)` returns the size of the pointer itself (e.g., 8 bytes on a 64-bit system), not the size of the memory block it points to. This leads to incorrect memory copies.
*   **Files:** `imgui.h`, `imstb_rectpack.h`
*   **Instruction:**
    1.  Locate the incorrect `memcpy`, `memmove`, or memory allocation calls.
    2.  Replace `sizeof(T)` where `T` is a pointer type with `sizeof(*T)` (size of one element) or `count * sizeof(*T)` (size of the buffer).

    **Example Fix in `/sep/third_party/imgui/imgui.h:2208`:**
    ```cpp
    // Before (incorrectly uses sizeof(v), where v is a reference to T)
    inline void push_back(const T& v) { ... memcpy(&Data[Size], &v, sizeof(v)); ... }

    // After (correctly uses sizeof(T))
    inline void push_back(const T& v) { ... memcpy(&Data[Size], &v, sizeof(T)); ... }
    ```

##### **C. Null Pointer Dereferences (`core.NullDereference`)**

*   **Issue:** The static analyzer has identified code paths where a null pointer can be dereferenced, leading to a guaranteed crash.
*   **Files:** `imgui.cpp`, `imstb_textedit.h`
*   **Instruction:**
    1.  Review the logic path identified by the analyzer for each report.
    2.  Add a null check before the pointer is dereferenced.

    **Example Fix in `/sep/third_party/imgui/imgui.cpp:7158`:**
    ```cpp
    // Before (window->RootWindowForNav can be null)
    while (window->RootWindowForNav->ChildFlags & ImGuiChildFlags_NavFlattened)
        window = window->RootWindowForNav;

    // After
    while (window->RootWindowForNav && (window->RootWindowForNav->ChildFlags & ImGuiChildFlags_NavFlattened))
        window = window->RootWindowForNav;
    ```

#### **Priority 3: Remediate Medium-Severity Issues in Project Code**

These issues affect code quality, portability, and can hide subtle bugs.

##### **A. Circular Header Dependencies (`misc-header-include-cycle`)**

*   **Issue:** Several core headers include each other, creating a circular dependency (e.g., `engine/config.h` -> `quantum/types.h` -> `engine/types.h` -> `engine/config.h`). This slows down compilation and can lead to errors.
*   **Files:** `engine/config.h`, `engine/types.h`, `quantum/types.h`
*   **Instruction:**
    1.  Break the cycle using **forward declarations**.
    2.  In the header file, only include headers that are strictly necessary for the definitions in that header. Move other includes to the corresponding `.cpp` file.

    **Example Fix:**
    1.  **In `/sep/src/engine/types.h`:**
        ```cpp
        // BEFORE
        #include "config.h" // Causes cycle
        // ...

        // AFTER
        namespace sep::config { struct SystemConfig; } // Forward declaration
        // ... (remove #include "config.h")
        ```
    2.  **In `/sep/src/quantum/types.h`:**
        ```cpp
        // BEFORE
        #include "engine/types.h" // Causes cycle
        // ...

        // AFTER
        namespace sep { struct PinState; } // Forward declaration
        // ... (remove #include "engine/types.h")
        ```
    3.  Add the full `#include` directives to the `.cpp` files that need the complete type definitions.

##### **B. Undefined Behavior with `memcpy`/`memset` on C++ Objects (`bugprone-undefined-memory-manipulation`)**

*   **Issue:** Using C-style memory functions on non-TriviallyCopyable C++ objects (i.e., objects with constructors, destructors, or virtual functions) is undefined behavior. This is widespread in the ImGui codebase.
*   **Files:** `imgui.cpp`, `imgui_draw.cpp`, `imgui_widgets.cpp`
*   **Instruction:**
    *   **Ideal Fix (Major Refactor):** Replace `memset` in constructors with proper member initialization. Replace `memcpy` with copy constructors or assignment operators. This would be a significant change to the ImGui library.
    *   **Pragmatic Approach:** Since this is in a third-party library, the risk may be acceptable if the library is known to function correctly on target platforms. The recommended action is to **suppress these warnings specifically for the `third_party/imgui` directory** in your build system configuration to reduce noise, while being aware of the potential for non-portability.

##### **C. Unchecked Function Return Values (`cert-err33-c`)**

*   **Issue:** The return values of functions like `snprintf`, `fprintf`, and `signal` are ignored. This can hide errors like failed writes or unsuccessful signal handler registration.
*   **Files:** `imgui_impl_opengl3.cpp`, `imgui_demo.cpp`, `workbench_main.cpp`
*   **Instruction:**
    1.  For functions where the return value is genuinely not needed and the risk is low (like `printf` for logging or `signal` in simple cases), explicitly cast the result to `(void)` to signal your intent to the compiler and static analyzer.
    2.  For functions where failure is critical (e.g., `snprintf` where buffer overflow is a risk), check the return value against the buffer size.

    **Example Fix in `/sep/src/workbench/core/workbench_main.cpp`:**
    ```cpp
    // Before
    std::signal(SIGINT, signalHandler);
    std::signal(SIGTERM, signalHandler);

    // After
    (void)std::signal(SIGINT, signalHandler);
    (void)std::signal(SIGTERM, signalHandler);
    ```

#### **Priority 4: Low-Severity and Style Issues**

These should be fixed to improve code clarity and maintainability.

*   **Issue:** Using `&vec[0]` instead of `vec.data()` to get a pointer to a container's data.
*   **Files:** `yaml-cpp/include/yaml-cpp/binary.h`, `yaml-cpp/src/binary.cpp`
*   **Instruction:** This is a simple stylistic fix. Replace all occurrences of `&some_vector[0]` with `some_vector.data()`. The latter is safer as it correctly handles empty vectors.

*   **Issue:** `switch` on a non-enum value without a `default` case.
*   **Files:** `yaml-cpp/src/binary.cpp`, `imgui_widgets.cpp`, etc.
*   **Instruction:** Add a `default:` case to all `switch` statements that are missing one. Even if you believe all cases are covered, this prevents unexpected behavior if the input value changes in the future.
    ```cpp
    switch (value) {
        case 1: /*...*/ break;
        case 2: /*...*/ break;
        default:
            // Unhandled case, either assert or do nothing.
            break;
    }
    ```

---

### **3. Conclusion**

The SEP Engine is a substantial and complex project with a clear, modular architecture. The static analysis has revealed numerous areas for improvement, primarily concentrated in third-party dependencies. By following the prioritized plan above, you can systematically eliminate critical bugs, improve code safety and stability, and enhance the overall quality of the codebase. It is recommended to configure the build system to treat warnings as errors for your proprietary code while potentially lowering the warning level for third-party directories to focus on the most critical issues first.