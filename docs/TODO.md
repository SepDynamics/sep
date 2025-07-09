### Guiding Philosophy:
**STOP fighting the symptoms (test failures). START fixing the diseases (architecture, includes, build logic).**

---

## The To-Do List: Your Path Out of This Hell

### **Phase 1: Fix the Goddamn Architecture (Stop the Bleeding)**

Your biggest problem is a circular dependency between `quantum` and `memory`, which the architecture diagram even calls out as a symbol conflict. This is non-negotiable. You have to kill it. Low-level modules should NOT know about high-level concepts.

1.  **Break the `memory` -> `quantum` Dependency:**
    *   **Goal:** `libsep_memory.a` should know NOTHING about "coherence," "stability," or `PatternData`. It's a glorified `malloc`. It manages generic blocks of bytes.
    *   **Action:**
        *   Go into `src/memory/memory_tier_manager.cpp` and `memory_tier.hpp`.
        *   **DELETE** every `#include` that points to a `quantum` header.
        *   Find the function `MemoryTierManager::updateBlockMetrics`. It currently takes `coherence`, `stability`, etc. This is the source of your circular dependency.
        *   **CHANGE** it to something generic. Instead of quantum-specific terms, use a generic "score" or "weight".
            ```cpp
            // In memory_tier_manager.hpp
            // OLD:
            // MemoryBlock* updateBlockMetrics(MemoryBlock* block, float coherence, float stability, ...);
            
            // NEW:
            MemoryBlock* updateBlockProperties(MemoryBlock* block, float promotion_score, float priority_score);
            ```
        *   The logic that *calculates* coherence and stability and *decides* whether to promote a block (e.g., `if (coherence > 0.7f)`) belongs in a **higher-level manager**, probably inside your `core` `engine.cpp` or the `api` `sep_engine.cpp`. That higher-level class can depend on both `quantum` and `memory` and orchestrate the two.
        *   **Result:** `libsep_memory.a` now has zero knowledge of `libsep_quantum.a`. The dependency is one-way: `quantum` -> `memory`.

### **Phase 2: Fix the Fucking Type Definitions (Single Source of Truth)**

Your static analysis report is screaming about `unknown type name 'MemoryTierEnum'` and duplicated members in `QuantumState`. This is classic header chaos.

1.  **Consolidate Core Types:**
    *   **Goal:** Create one header file that is the absolute source of truth for all fundamental, shared types.
    *   **Action:**
        *   In your `core` library, use `include/core/types.h`. This is its new home.
        *   **MOVE** the definitions of these critical types into `core/types.h`:
            *   `enum class SEPResult`
            *   `enum class MemoryTierEnum`
            *   `struct QuantumState`
            *   `struct PatternData`
            *   `struct PatternConfig`
        *   **FIX** the `QuantumState` struct definition while you're there. The static analyzer says you have duplicate members (`evolution_rate`, etc.). Clean it up. Make one canonical definition.
        *   **HUNT AND DESTROY:** Go through every other header file in your entire project (`api`, `memory`, `quantum`, etc.). Delete their local, duplicated definitions of these types and replace them with a single `#include "core/types.h"`.

### **Phase 3: Fix the Fucking Build (Tame CMake and Headers)**

The build log shows you're dying on `crow` header issues. Your `crow_isolation.h` shim is a brittle hack that has clearly broken. Let's do this the right way.

1.  **Isolate External Dependencies:**
    *   **Goal:** Only the `api` module should know about `crow`. It should never "leak" into other parts of the build.
    *   **Action:**
        *   Open `src/api/CMakeLists.txt`.
        *   Find `target_include_directories(sep_api ...)`
        *   Make sure the `crow` include path is listed under the `PRIVATE` section, NOT `PUBLIC`. This prevents it from propagating to targets that link `sep_api`.
        *   **DELETE** `crow_isolation.h`. It's a landmine. We're not doing that anymore.
        *   Ensure that only `*.cpp` files inside `src/api/` include `crow.h` or `crow/app.h`. If any `*.h` file in `include/api/` has a Crow include, you're doing it wrong. Use forward declarations and PIMPL idiom if you have to.

### **Phase 4: Fix the Fucking Tests (Finally, the Symptoms)**

Your tests are failing because allocations/promotions are returning `nullptr`. This means the destination tier is full or can't find a free block.

1.  **Debug Memory Allocation Failure:**
    *   **Goal:** Figure out why `allocate()` is failing in the tests.
    *   **Action:**
        *   **Add Logging:** Go into `src/memory/memory_tier.cpp`. In `allocate()`, add `printf` or `spdlog` statements.
            ```cpp
            MemoryBlock* MemoryTier::allocate(std::size_t size) {
                printf("TIER %d: Attempting to allocate %zu bytes.\n", (int)config_.type, size);
                // ... existing logic ...
                if (!block) {
                    printf("TIER %d: No free block found for size %zu. Fragmentation: %f\n", 
                           (int)config_.type, size, calculateFragmentation());
                    defragment();
                    block = findFreeBlock(size);
                    if (!block) {
                        printf("TIER %d: Allocation FAILED even after defrag.\n", (int)config_.type);
                        return nullptr;
                    }
                }
                printf("TIER %d: Allocation SUCCEEDED.\n", (int)config_.type);
                // ... rest of the function ...
            }
            ```
        *   **Run the tests again.** Watch the logs. My bet is the test tiers (1KB, 2KB) are filling up instantly, and your defragment/resize logic isn't getting triggered or isn't working as expected. Your tests need to account for this or your `promoteToTier` logic needs to be more robust (e.g., resize the destination tier if allocation fails).

2.  **Fix Floating-Point Bullshit:**
    *   **Goal:** Make the utilization tests pass.
    *   **Action:** Never use `EXPECT_EQ` for floats.
        *   In `memory_tier_manager_test.cpp`, change `EXPECT_EQ(mgr.getTierUtilization(...), 0.0f)` to `EXPECT_NEAR(mgr.getTierUtilization(...), 0.0f, 1e-6)`. This allows for tiny floating-point inaccuracies.
        *   Better yet, in `MemoryTier::deallocate`, when you merge blocks and recalculate `used_space_`, if `used_space_` is very small (e.g., less than a few bytes), just clamp it to `0`. This ensures a fully deallocated tier has exactly `0.0f` utilization.

### **Phase 5: Clean Up Your Shit**

You have hundreds of warnings. They are noise that hides real problems.

1.  **Enable Strict Compiler Flags:**
    *   **Goal:** Stop ignoring warnings.
    *   **Action:** In your root `CMakeLists.txt`, add:
        ```cmake
        if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
            add_compile_options(-Wall -Wextra -Werror)
        endif()
        ```
    *   This will force you to fix all the `unused parameter` and `reorder-ctor` warnings. It's tedious, but it will make your code infinitely cleaner and more stable. Use `(void)param;` or `[[maybe_unused]]` for unused parameters.
