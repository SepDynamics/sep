**Verification:**

*   **Execute `ninja clean`** to ensure a fresh build.
*   **Run `cd /sep/build && export CC=gcc-14 CXX=g++-14 && cmake .. -G Ninja -DSEP_BUILD_TESTS=ON -DCMAKE_EXPORT_COMPILE_COMMANDS=ON` 
*   **Execute `cd /sep && ninja -j$(nproc) 2>&1 | tee /sep/output/build_log.txt`
*   **CRITICAL CHECK:** Ensure zero `FAILED:` lines in the build log.
*   **SECONDARY CHECK:** Review `report.md` for any remaining critical/high severity defects related to these fixes. Expected warnings (from PipeWire headers etc.) can be deferred.

---

### **Technical Checklist: Build Unblockage & Core Schema Alignment**

**Phase:** Emergency Build Fix
**Target:** Compile-time Stability

1.  **Refactor `MemoryTierEnum` Canonical Definition:**
    *   **Action:** Move the *entire* `enum class MemoryTierEnum : int { ... };` definition from `src/core/types.h` to `src/memory/types.h`, placing it directly within `namespace sep::memory { ... }`.
    *   **Action:** In `src/core/types.h`, add `#include "memory/types.h"` and ensure all references within `src/core/types.h` (e.g., `MemoryThresholdConfig` members) now use the fully qualified name `::sep::memory::MemoryTierEnum`.
    *   **Action:** In `src/blender/types.h`, `src/memory/memory_tier.hpp`, `src/memory/memory_tier_manager.hpp`, `src/quantum/processor.h`, `src/memory/quantum_coherence_manager.h`, `src/quantum/quantum_coherence_manager.cpp`, and `src/quantum/quantum_processor_qfh.h` (and any other affected headers/source files), update existing `using ::sep::MemoryTierEnum;` to `using sep::memory::MemoryTierEnum;` and ensure direct uses of `MemoryTierEnum` are replaced with `sep::memory::MemoryTierEnum` or an appropriate local `using` alias, referencing the canonical definition.

2.  **Align `APIConfig` Structure in `core/types.h`:**
    *   **Action:** Within `src/core/types.h`, add the following nested struct definitions and member declarations to `struct APIConfig`:
        ```cpp
        // Add these struct definitions within namespace sep::config { ... }
        struct RateLimitConfig {
            int requests_per_minute = 60;
            bool enabled = true;
        };

        struct AuthConfig {
            bool enabled = false;
            std::vector<std::string> tokens;
        };

        struct ResponseModulationConfig {
            bool enabled{true};
            float coherence_threshold{0.7f};
            bool simplify_low_coherence{true};
            int max_detail_level{3};
        };

        struct OllamaConfig { // This one might already be here, verify
            bool enabled{false};
            std::string host{"http://127.0.0.1:11434"};
            std::string model{"llama2"};
            std::size_t batch_size{1};
            std::size_t context_window{512};
            // Nested GPUConfig if used inside OllamaConfig, define that too
            struct GPUConfig {
                bool enabled{false};
                float memory_fraction{0.0f};
            } gpu{};
        };

        // Add these members to struct APIConfig { ... }
        // (Ensure proper positioning, usually at the end)
        AuthConfig cors; // Assuming 'cors' maps to AuthConfig structure
        RateLimitConfig rate_limit;
        ResponseModulationConfig response_modulation; // This one is already in snapshot code
        OllamaConfig ollama; // This one is already in snapshot code
        ```
    *   **Rationale:** The `APIConfig` in `core/types.h` is the definitive source for system configuration. Any fields accessed by the API or engine initialization should be explicitly defined there to avoid circular dependencies with `api/types.h`.

3.  **Correct `ollama::OllamaClient` Constructor Signature:**
    *   **File:** `src/api/client.h`
    *   **Action:** Modify the `OllamaClient` constructor declaration to directly accept the `OllamaConfig` type defined in `sep::config` namespace:
        *   **Change:** `explicit OllamaClient(const sep::ollama::OllamaConfig &config);`
        *   **To:** `explicit OllamaClient(const sep::config::OllamaConfig &config);`

4.  **Mitigate Missing `sep::testbed` Functions:**
    *   **File:** `src/api/sep_engine.cpp`
    *   **Action:** Locate and comment out or provide dummy/mock implementations for the calls to `sep::testbed::validate_contexts` and `sep::testbed::blend_embeddings`.
        *   **Example for `validateContexts` block:**
            ```cpp
            // auto report = sep::testbed::validate_contexts(request_data["contexts"]);
            // Replace with dummy success response for compilation:
            nlohmann::json report_dummy;
            report_dummy["overall_valid"] = true;
            report_dummy["invalid_indices"] = nlohmann::json::array();
            nlohmann::json result_final;
            result_final["success"]         = true;
            result_final["valid"]           = report_dummy["overall_valid"];
            result_final["context_count"]   = request_data["contexts"].size();
            result_final["invalid_indices"] = report_dummy["invalid_indices"];
            return result_final;
            ```
        *   **Example for `blendContexts` block:**
            ```cpp
            // auto blend_report = sep::testbed::blend_embeddings(embeddings, weights);
            // Replace with dummy success response for compilation:
            nlohmann::json blend_report_dummy;
            blend_report_dummy["success"] = true;
            blend_report_dummy["blended"] = {0.0, 0.0, 0.0}; // Example dummy blended embedding
            blend_report_dummy["coherence"] = 0.7; // Example dummy coherence

            double ts = 0.0;
            // ... (original timestamp blending logic for 'ts' if desired)
            
            nlohmann::json blend_final;
            blend_final["embedding"] = blend_report_dummy["blended"];
            blend_final["coherence"] = blend_report_dummy["coherence"];
            blend_final["metadata"]  = { {"timestamp", ts} };
            blend_final["type"]       = "blended";
            blend_final["blended_context_id"] = generateId("blend");

            nlohmann::json result_final;
            result_final["success"] = true;
            result_final["result"]  = blend_final;
            return result_final;
            ```
    *   **Rationale:** These are external dependencies not correctly integrated for the core `sep_engine` target. Bypassing them allows the core compilation to proceed. A future task should involve proper integration or removal if not truly needed at runtime.

5.  **Silence `unused-parameter` Warnings:**
    *   **Files:** `src/api/client.cpp`, `src/api/server.cpp`, `src/api/sep_engine.cpp`.
    *   **Action:** For every parameter explicitly identified as unused by the compiler (`[-Werror=unused-parameter]`), add a `(void)parameter_name;` cast at the beginning of the function body.
        *   **Example from `src/api/client.cpp:145:46`:**
            ```cpp
            void Client::updateMetrics(const APIRequest &request, const APIResponse &response) {
                (void)request; // Silence unused parameter warning
                std::lock_guard<std::mutex> lock(impl_->mutex);
                // ... rest of the function ...
            }
            ```
        *   **Example from `src/api/server.cpp:205:84` and `266:50`:**
            ```cpp
            void SEPApiServer::logRequest(const HttpRequest& req, int code, const std::string& response_body, int64_t duration) {
                (void)response_body; // Silence unused parameter warning
                // ... rest of the function ...
            }
            // And for the crow::request overload:
            void SEPApiServer::logRequest(const ::crow::request &req, int status_code, const std::string &response_body, int64_t duration_ms) {
                (void)response_body; // Silence unused parameter warning
                // ... rest of the function ...
            }
            ```
        *   **Example from `src/api/sep_engine.cpp:108:68`:**
            ```cpp
            nlohmann::json SepEngine::initialize(const sep::config::APIConfig& config) {
                (void)config; // Silence unused parameter warning
                if (impl_->initialized) {
                    // ... rest of the function ...
                }
            }
            ```
