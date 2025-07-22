# SEP Engine Financial Pipeline

## Overview
The SEP Engine is a C++ framework using quantum-inspired algorithms (QBSA/QFH) for pattern analysis in data streams, now focused on building a financial processing pipeline for market analysis and trading signals. Core features include pattern metrics (coherence, stability, entropy), multi-tier memory management, and GPU acceleration. The immediate goal is to stabilize OANDA integration, UI, and data pipeline for real-time analysis.

## Current State
- **Core Engine**: Functional with pattern analysis and metrics.
- **OANDA Integration**: Streams real-time prices from practice server but crashes on connect; basic UI displays bid/ask.
- **UI**: Floating windows need static layout for usability.
- **Limitations**: No SEP engine data feed yet; no backtesting or risk management.

## Key Components
- **Quantum Algorithms**: QBSA for bit-level analysis, QFH for state transitions (NULL_STATE, FLIP, RUPTURE).
- **Pattern Metrics**: Coherence (self-similarity), Stability (persistence), Entropy (complexity).
- **Memory Tiers**: STM (short-term, coherence >0.2), MTM (medium, >0.5, 5+ generations), LTM (long, >0.8, 100+ generations).
- **Financial Mapping**: Volatility to coherence, volume to stability, order book to entropy.
- **Modulation Strategy**: Feedback loop to adjust parameters (evolution_rate, energy_level) for target metrics.

## Setup
1. **Prerequisites**: OANDA demo account, API key, CMake, g++, libglfw3-dev.
2. **Credentials**: In keys.txt: export OANDA_API_KEY="your-key"; export OANDA_ACCOUNT_ID="your-id".
3. **Build**: ./build.sh
4. **Run**: ./run_workbench.sh (loads keys, launches with OANDA).
5. **Debug**: SEP_DEBUG=1 for output.

## Architecture
```
[Market Data (OANDA)] → [Data Ingestion] → [SEP Engine (QBSA/QFH, Metrics)] → [Analysis Layer] → [Signals & UI]
                          ↓
                     [Memory Tiers] → [Persistence]
```

- **CMake Structure**: See CMAKE_INCLUDE_PROTOCOL.md for include protocols.
- **Memory Implementation**: See memory_tier_manager_implementation_plan.md for TODOs.
- **Modulation**: See ENGINE_MODULATION_STRATEGY.md for control loop.

## Quick Reference
- **Commands**: ./sep process data.json --gpu; ./sep financial data.json --backtest.
- **API Endpoints (Planned)**: /analyze, /metrics, /signals.
- **Indicators**: SMA/EMA, RSI, MACD; Quantum: MCI, PSR.
- **Performance**: <10ms latency, 1M+ candles/sec.
- **Troubleshooting**: Check keys for crashes; enable GPU for speed.

This README provides essential context for immediate development. Expand as needed; refer to TODO.md for priorities.

Of course. I can provide detailed technical guidance for implementing the specified TODOs. The following plan outlines the necessary logic, external API interactions, and code structure for each task, referencing the provided documents.

-----

## `src/apps/oanda_trader/oanda_trader_app.cpp` Implementation

The tasks in this file involve interacting with the OANDA API to enable trading and display account information. This will require adding new methods to the `OandaConnector` class to handle the specific API endpoints for orders and positions.

### 1\. Implement Order Placement Logic

  * **Objective:** Enable the "Place Order" button in the trade panel to send a market order to OANDA.

  * **Guidance:**

    1.  **`OandaConnector` Enhancement:** Add a new method to `oanda_connector.h` and `oanda_connector.cpp` called `placeOrder`. This method will make a `POST` request to the OANDA orders endpoint.
          * **Endpoint:** `POST /v3/accounts/{accountID}/orders`
          * **Body:** The request requires a JSON body specifying the order details. For a simple market order, this would look like:
            ```json
            {
              "order": {
                "units": "1000",
                "instrument": "EUR_USD",
                "timeInForce": "FOK",
                "type": "MARKET",
                "positionFill": "DEFAULT"
              }
            }
            ```
            The `units` value should be positive for a buy and negative for a sell.
    2.  **`OandaTraderApp` Integration:** In `oanda_trader_app.cpp`, modify the `renderTradePanel` function. The `// TODO: Implement order placement` comment should be replaced with a call to the new `oanda_connector_->placeOrder` method, passing the instrument and units from the UI.

  * **Code Snippet (`OandaConnector::placeOrder`):**

    ```cpp
    #include <nlohmann/json.hpp>

    nlohmann::json OandaConnector::placeOrder(const std::string& instrument, float units) {
        std::string endpoint = "/v3/accounts/" + account_id_ + "/orders";
        
        nlohmann::json order_body;
        order_body["order"]["instrument"] = instrument;
        order_body["order"]["units"] = std::to_string(units);
        order_body["order"]["type"] = "MARKET";
        order_body["order"]["timeInForce"] = "FOK"; // Fill or Kill
        order_body["order"]["positionFill"] = "DEFAULT";

        CurlResponse response = makeRequest(endpoint, "POST", order_body.dump());
        
        if (response.response_code == 201) { // 201 Created is success for this endpoint
            return nlohmann::json::parse(response.data);
        }
        
        last_error_ = "Failed to place order: " + response.data;
        return nlohmann::json{ {"error", last_error_} };
    }
    ```

### 2\. Display Actual Positions from OANDA

  * **Objective:** Fetch and display the user's open positions in the "Open Positions" panel.

  * **Guidance:**

    1.  **`OandaConnector` Enhancement:** Add a `getOpenPositions` method. This will make a `GET` request to the OANDA positions endpoint.
          * **Endpoint:** `GET /v3/accounts/{accountID}/openPositions`
    2.  **Data Storage:** Add a member variable to `OandaTraderApp` to store the positions, like `std::vector<nlohmann::json> open_positions_;`, along with a mutex for thread safety.
    3.  **`OandaTraderApp` Integration:** Create a `refreshPositions()` method in `OandaTraderApp` that calls the new connector method and updates `open_positions_`. Call this method periodically or on a refresh button click.
    4.  **UI Rendering:** In `renderPositions()`, iterate through the `open_positions_` vector and use `ImGui::Text` to display relevant information like instrument, units, and unrealized profit/loss.

  * **Code Snippet (`OandaTraderApp::renderPositions`):**

    ```cpp
    void OandaTraderApp::renderPositions() {
        ImGui::Begin("Open Positions");
        
        if (ImGui::Button("Refresh Positions")) {
            // This would be a new method to fetch positions
            refreshPositions(); 
        }
        
        ImGui::Separator();

        // Assuming open_positions_ is a std::vector<nlohmann::json>
        for (const auto& position : open_positions_) {
            std::string instrument = position["instrument"];
            std::string long_units = position["long"]["units"];
            std::string short_units = position["short"]["units"];
            std::string pnl = position["unrealizedPL"];
            
            if (long_units != "0") {
                ImGui::Text("%s | Units: %s | P/L: %s", instrument.c_str(), long_units.c_str(), pnl.c_str());
            }
            if (short_units != "0") {
                ImGui::Text("%s | Units: %s | P/L: %s", instrument.c_str(), short_units.c_str(), pnl.c_str());
            }
        }
        
        ImGui::End();
    }
    ```

### 3\. Display Order History from OANDA

  * **Objective:** Fetch and display a list of past orders in the "Order History" panel.
  * **Guidance:**
    1.  **`OandaConnector` Enhancement:** Add a `getOrders` method. This will make a `GET` request to the OANDA orders endpoint.
          * **Endpoint:** `GET /v3/accounts/{accountID}/orders?state=FILLED` (you can also query for `PENDING`, `CANCELLED`, etc.)
    2.  **Data Storage:** Add a member like `std::vector<nlohmann::json> order_history_;` to `OandaTraderApp`.
    3.  **Integration & UI:** Follow the same pattern as for displaying positions: create a `refreshOrderHistory()` method, add a refresh button, and iterate through the stored history in `renderOrderHistory()` to display order details.

-----

## `src/memory/memory_tier_manager.cpp` Implementation

These implementations follow the strategy outlined in the `memory_tier_manager_implementation_plan.md`.

### Phase 1: Core Pattern Management

#### 1\. `pruneWeakRelationships()`

  * **Objective:** Remove relationships from `pattern_relationships_` and `data_relationships_` that fall below a configurable strength threshold.

  * **Guidance:**

    1.  Define a `weak_relationship_threshold` (e.g., 0.3f) in the `MemoryTierManager::Config`.
    2.  Iterate through the `pattern_relationships_` map. For each entry, iterate through its nested map of target IDs and strengths.
    3.  Use the `std::erase_if` pattern (or a traditional iterator loop) to remove any relationship where the strength is less than the threshold.
    4.  If a source pattern is left with no relationships, remove its entry from the outer map.
    5.  Repeat the process for `data_relationships_`.

  * **Code Snippet (`MemoryTierManager::pruneWeakRelationships`):**

    ```cpp
    void MemoryTierManager::pruneWeakRelationships() {
        const float threshold = config_.weak_relationship_threshold; // Assuming threshold is in config

        auto prune_map = [&](auto& map) {
            for (auto it = map.begin(); it != map.end();) {
                auto& relationships = it->second;
                std::erase_if(relationships, [&](const auto& item) {
                    auto const& [key, value] = item;
                    return value < threshold;
                });

                if (relationships.empty()) {
                    it = map.erase(it);
                } else {
                    ++it;
                }
            }
        };

        prune_map(pattern_relationships_);
        prune_map(data_relationships_);
    }
    ```

-----

### 2\. `calculateRelationshipScores()`

  * **Objective:** To dynamically update the strength of relationships between patterns using a stateful, quadratic model. This model treats the relationship strength as the magnitude of a vector in a conceptual phase space, reinforcing it based on the patterns' deviation from a target equilibrium (`coherence = 0.5`).

  * **Core Concept:** The goal is not to maximize strength to 1.0 but to create a responsive metric that reflects a system in dynamic equilibrium. The strength evolves according to the formula $S\_{new} = \\sqrt{S\_{old}^2 + \\Delta^2}$, where $S\_{old}$ is the previous strength and $\\Delta$ is the new contribution derived from the patterns' current coherence relative to the 0.5 target. This Pythagorean approach models the evolution of the relationship's magnitude.

  * **Guidance:**

    1.  Iterate through the `pattern_relationships_` map. This will be a nested loop to access each source pattern and its corresponding map of target relationships.
    2.  For each relationship between a source pattern (**A**) and a target pattern (**B**), retrieve their current `PatternData` from the `pattern_registry_` to get their coherence values (`coh_A`, `coh_B`).
    3.  Fetch the relationship's existing strength, which represents the "rolling state" or $S\_{old}$.
    4.  Calculate the **deviation contribution ($\\Delta$)**. This term quantifies how far the related patterns are from the ideal 0.5 coherence target.
        ```cpp
        float deviation_A = std::abs(coh_A - 0.5f);
        float deviation_B = std::abs(coh_B - 0.5f);
        float delta = (deviation_A + deviation_B) / 2.0f; 
        ```
    5.  Apply the **quadratic update rule** to calculate the new, unnormalized strength:
        ```cpp
        float new_strength_raw = std::sqrt(std::pow(old_strength, 2.0f) + std::pow(delta, 2.0f));
        ```
    6.  **Normalize the result** to keep it within the unitized [0, 1] space. A simple clamp is insufficient as the value would always grow. A hyperbolic normalization function is more appropriate to gracefully handle ever-increasing raw scores.
        ```cpp
        float new_strength_normalized = new_strength_raw / std::sqrt(1.0f + std::pow(new_strength_raw, 2.0f));
        ```
    7.  Update the strength value for the relationship in the map with `new_strength_normalized`.

  * **Adaptive Depth of Analysis (Advanced):** To implement the "varying depth of analysis," the calculation of $\\Delta$ can be made more sophisticated when the system is near its equilibrium.

      * First, calculate the system's average coherence.
      * If `abs(average_coherence - 0.5) < some_small_threshold`, the system is stable.
      * In this state, modify the `delta` calculation to include the coherences of **second-degree neighbors** (the neighbors of B), making the analysis deeper and more sensitive to the broader pattern manifold.

  * **Code Snippet (`MemoryTierManager::calculateRelationshipScores`):**

    ```cpp
    void MemoryTierManager::calculateRelationshipScores() {
        for (auto& [source_id, relationships] : pattern_relationships_) {
            const auto* pattern_a_data = getPatternData(source_id);
            if (!pattern_a_data) continue;

            for (auto& [target_id, old_strength] : relationships) {
                const auto* pattern_b_data = getPatternData(target_id);
                if (!pattern_b_data) continue;

                // 1. Calculate deviation from the 0.5 target
                float dev_a = std::abs(pattern_a_data->quantum_state.coherence - 0.5f);
                float dev_b = std::abs(pattern_b_data->quantum_state.coherence - 0.5f);
                float delta = (dev_a + dev_b) / 2.0f;

                // 2. Apply the quadratic update rule
                float new_strength_raw = std::sqrt(std::pow(old_strength, 2.0f) + std::pow(delta, 2.0f));

                // 3. Normalize to keep the value in the [0, 1] range
                float new_strength_normalized = new_strength_raw / std::sqrt(1.0f + std::pow(new_strength_raw, 2.0f));
                
                // 4. Update the strength
                old_strength = new_strength_normalized;
            }
        }
    }
    ```

#### 3\. `calculateRelationshipCoherence()`

  * **Objective:** Update a pattern's overall coherence based on the consistency of its relationships. A pattern with consistently strong relationships is highly coherent.
  * **Guidance:**
    1.  Iterate through each pattern in `pattern_registry_`.
    2.  Find its relationships in `pattern_relationships_`.
    3.  If it has relationships, calculate the mean and standard deviation of the strength values of those relationships.
    4.  Update the pattern's coherence in `pattern_registry_` using a formula like: `coherence = 1.0f - standard_deviation`. A low deviation (consistent strengths) results in high coherence.

### Phase 2 & 3: Cleanup and Data Registration

#### 4\. `cleanupExpiredPatterns()` / `cleanupExpiredData()`

  * **Objective:** Remove patterns and data that haven't been accessed recently.
  * **Guidance:**
    1.  These require a timestamp on the `MemoryBlock` or `PatternData`. The `Pattern` struct in `quantum/types.h` has `last_accessed`. Use this.
    2.  Define a `max_pattern_age_seconds` in the config.
    3.  Get the current time.
    4.  Iterate through `pattern_registry_`. If `(current_time - pattern.last_accessed) > max_age`, call `removePattern(pattern.id)`.

#### 5\. `prunePatternsByPriority()` / `pruneDataByPriority()`

  * **Objective:** Enforce a maximum number of patterns/data entries per tier by removing the least important ones.
  * **Guidance:**
    1.  This function needs to operate on a specific tier's patterns. You will first need a way to map which patterns reside in which tier. The `MemoryBlock` has a `tier` enum; you can use this.
    2.  Gather all patterns for the specified `tier`.
    3.  Calculate a priority score for each pattern. A good starting point is `priority = coherence * stability`.
    4.  If the number of patterns in the tier exceeds `max_patterns_per_tier`, sort them by priority score and remove the ones with the lowest scores.

#### 6\. `registerGenericData()`

  * **Objective:** Store a deep copy of arbitrary data provided as a `const void*`.
  * **Guidance:**
    1.  The function signature must be changed to `registerGenericData(std::size_t id, const void* data, std::size_t size)`. Without the `size`, a deep copy is impossible.
    2.  Inside the function, allocate new memory: `void* new_data = new char[size];`.
    3.  Copy the data: `std::memcpy(new_data, data, size);`.
    4.  Store it in the `data_registry_`. The registry should be a map of `id` to a `std::unique_ptr` or a pair containing the pointer and its size so it can be safely deleted.
          * Example: `std::unordered_map<std::size_t, std::unique_ptr<char[]>> data_registry_;`

### Phase 4: Persistence and Processing

#### 7\. `storeDataToPersistence()` / `loadDataFromPersistence()`

  * **Objective:** Save and load the state of the pattern and data registries using Redis.
  * **Guidance:**
    1.  **Serialization:** Iterate through `pattern_registry_` and `data_registry_`. Convert each entry into a JSON object using `nlohmann/json`. The serialization functions in `quantum/types_serialization.cpp` are a good reference.
    2.  **Storage:** Use the `RedisManager` (from `memory/redis_manager.h`). Store each serialized pattern as a hash in Redis, keyed by its ID (e.g., `HSET pattern:123 coherence 0.8 stability 0.9`).
    3.  **Loading:** Use the `RedisManager` to get all pattern keys. For each key, retrieve the hash data, deserialize it from JSON back into a `PatternData` struct, and repopulate the registries.

#### 8\. `processMemoryBlocks()`

  * **Objective:** Provide a C++ interface for launching the CUDA pattern processing kernels.
  * **Guidance:**
    1.  This function should act as a wrapper around the C-style CUDA API defined in `engine/cuda_api.hpp`.
    2.  It will be responsible for preparing the data on the host, allocating device memory (`DeviceBufferRAII` from `engine/raii.h` is perfect for this), copying data from host to device, launching the appropriate kernel (e.g., `launchQBSAKernel`), and copying the results back.
    3.  The parameters `input_data`, `output_data`, `config`, etc., map directly to the arguments needed by functions like `sep_cuda_process_batch`.