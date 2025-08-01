# Experiment 001: Phase 1 Parameters in Phase 2 Framework

## Hypothesis
Phase 2's regression is due to parameter drift from Phase 1's proven configuration. Testing Phase 1's exact parameters in Phase 2's enhanced framework should recover performance.

## Configuration Changes
```cpp
// Phase 1 proven weights
double stability_w = 0.4;    // Phase 1 value
double coherence_w = 0.4;    // Phase 1 value  
double entropy_w = 0.2;      // Phase 1 value

// Phase 1 proven thresholds
double base_buy_threshold = 0.50;   // Phase 1 value
double base_sell_threshold = 0.52;  // Phase 1 asymmetric threshold

// Keep volatility adaptation (Phase 1 success factor)
// Remove quality filtering (hypothesis: adds noise)
// Remove regime complexity (hypothesis: adds noise)
```

## Expected Outcome
- Accuracy should approach Phase 1's 50.94%
- Signal count may be higher due to Phase 2's framework
- Quality metrics should improve with proven parameters

## Test Command
```bash
./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json
```
