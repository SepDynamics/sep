### **Checklist: From Zero to a Running "Memory Garden" Demo**

**Phase 1: Establish a Clean, Reliable Build Environment (The Foundation)**

This phase is about eliminating all the variables from your scattered build attempts. We need ONE canonical build directory and ONE command to build it.

*   **[ ] Step 1: Nuke a**ll** existing build directories.**
    *   This is non-negotiable. It eliminates stale cache, old configurations, and confusion.
    *   Delete `sep_build`, `buildajn`, `cmakemake`, the `build` in your home directory, *all of them*.
    *   **Command:**
        ```bash
        rm -rf /sep/sep_build
        rm -rf /sep/buildajn 
        rm -rf /sep/cmakemake
        # ...and any others you know of.
        ```

*   **[ ] Step 2: Create ONE new, clean build directory.**
    *   We will call it `build` inside the project root `/sep`. This is the standard convention.
    *   **Command:**
        ```bash
        cd /sep
        mkdir build
        cd build
        ```

*   **[ ] Step 3: Fix the Core Compilation Errors.**
    *   The `RateLimitConfig` error is the first thing that will stop you. It's a blocker for compiling `libsep_api.a`, which `sep_workbench` might depend on. We fix this now so it doesn't bite us later.
    *   **Action:**
        1.  Open `/sep/src/api/rate_limit_middleware.cpp`.
        2.  Find the constructor: `RateLimitMiddleware::RateLimitMiddleware()`.
        3.  Change `set_config(sep::config::RateLimitConfig{});` to `set_config(sep::api::RateLimitConfig{});`.
        4.  Find the `set_config` method signature.
        5.  Change `void RateLimitMiddleware::set_config(const sep::config::RateLimitConfig& config)` to `void RateLimitMiddleware::set_config(const sep::api::RateLimitConfig& config)`.
        6.  Open `/sep/include/api/rate_limit_middleware.h`.
        7.  Change `void set_config(const sep::config::RateLimitConfig& config);` to `void set_config(const sep::api::RateLimitConfig& config);`.
        8.  Change `sep::config::RateLimitConfig config_;` to `sep::api::RateLimitConfig config_;`.

*   **[ ] Step 4: Fix the header include paths.**
    *   Your error logs show consistent `pp_file_not_found` for headers like `core/common.h`. This is because the include paths are not correct for your directory structure.
    *   **Action:**
        1.  Open `/sep/src/memory/memory_tier_manager.hpp`
        2.  Change `#include "core/common.h"` to `#include "sep/core/common.h"`.
        3.  Do a project-wide search for similar errors (`core/`, `memory/`, `quantum/`) and add the `sep/` prefix. **BE SYSTEMATIC.** Your `include` directory is structured as `include/sep/MODULE/header.h`, so all cross-module includes should follow that pattern.

**Phase 2: Build the Workbench Executable**

Now that the core libraries should compile, we focus on building the specific target you want.

*   **[ ] Step 5: Configure the build with CMake.**
    *   From your `/sep/build` directory, run the CMake command. We'll enable the workbench and its dependencies.
    *   **Command:**
        ```bash
        cd /sep/build
        cmake -DCMAKE_BUILD_TYPE=Debug \
              -DCMAKE_CXX_COMPILER=g++-14 \
              -DBUILD_TESTING=OFF \
              -DSEP_WITH_AUDIO=ON \
              -DSEP_WITH_CYCLES=ON \
              -DSEP_BUILD_WORKBENCH=ON \
              ..
        ```
        *Self-Correction:* The `-DSEP_BUILD_WORKBENCH=ON` flag might not exist. If CMake complains, open `/sep/CMakeLists.txt` and see how the `examples/workbench` directory is added. It's likely added unconditionally if it exists. If so, just run the command without that flag.

*   **[ ] Step 6: Build the `sep_workbench` target.**
    *   Focus only on this target. Don't build everything. This saves time and reduces the noise from unrelated errors.
    *   **Command:**
        ```bash
        cd /sep/build
        make sep_workbench
        ```

*   **[ ] Step 7: Debug any remaining compile errors.**
    *   If `make sep_workbench` fails, **STOP**.
    *   Read the **FIRST** error message carefully. It's the most important one.
    *   The error will likely be another missing include or a type/namespace mismatch. Apply the logic from Phase 1.
    *   Repeat Step 6 until `sep_workbench` links successfully.

**Phase 3: Run and Validate the Memory Garden Demo**

This is the payoff.

*   **[ ] Step 8: Prepare the runtime environment.**
    *   The workbench looks for `config.json`. Make sure a valid one exists.
    *   **Action:** Copy `/sep/examples/workbench/config.json` into your build directory:
        ```bash
        cp ../examples/workbench/config.json .
        ```

*   **[ ] Step 9: Run the workbench.**
    *   The executable will be in `/sep/build/examples/workbench/`.
    *   **Command:**
        ```bash
        cd /sep/build
        ./examples/workbench/sep_workbench
        ```

*   **[ ] Step 10: Switch to the Memory Garden Demo.**
    *   The application starts in the "Genesis" demo. You need to implement a key press to switch demos.
    *   **Action:** Open `/sep/examples/workbench/main.cpp`. In `mainLoop`, add a basic demo switcher to the keyboard handling section:
        ```cpp
        // Inside mainLoop(), in the keyboard handling block
        if (g_renderer->hasKeyEvent()) {
            unsigned char key = g_renderer->getLastKey();
            if (key == '1') demo_manager.switchToDemo("genesis");
            if (key == '2') demo_manager.switchToDemo("audio");
            if (key == '3') demo_manager.switchToDemo("memory"); // THIS IS THE ONE YOU WANT
            // ... add others if you like ...
            else {
                demo_manager.handleKeyboard(key);
            }
        }
        ```
    *   Re-run `make sep_workbench` and then run the executable again. Press `3`.

*   **[ ] Step 11: Observe the visual.**
    *   You should now see the Memory Garden. It might be simple—just a few points. That doesn't matter.
    *   **This is your win.** You have a visual representation of your system running.

---
