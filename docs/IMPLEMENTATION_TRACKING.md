# Implementation Tracking: JavaScript to C++ Mapping

This document tracks the implementation status of critical functions from the JavaScript reference implementation (`forex/`) to the C++ SEP Engine.

## Signal Generation (`brains.js` → C++)

### Core Functions

- [ ] `logistic(x, r = 3.97)` → `sep::chaos::logistic()`
  - Location: `src/quantum/chaos.hpp`
  - Status: Not implemented
  - Notes: Must use exact r=3.97 value
  - Critical: This is the core of our deterministic signal generation

- [ ] `generateRecursiveSeed(pattern)` → `sep::signals::generateSeed()`
  - Location: `src/quantum/pattern_generator.hpp`
  - Status: Not implemented
  - Formula: `(pattern.note * 1000 + pattern.resonance * 100 + pattern.index) % 1`
  - Critical: Ensures deterministic pattern seeding

- [ ] `buildSignals(patterns)` → `sep::signals::buildSignals()`
  - Location: `src/apps/oanda_trader/signal_builder.hpp`
  - Status: Not implemented
  - Critical thresholds:
    - Buy: > 0.5
    - Sell: < 0.3
    - Neutral: otherwise
    - Must iterate logistic map exactly 5 times

## Validation & Feedback (`processor.js` → C++)

### Market Validation

- [ ] `checkMarketCondition(pattern, marketData)` → `sep::validation::checkMarketCondition()`
  - Location: `src/memory/market_validator.hpp`
  - Status: Not implemented
  - ATR Thresholds:
    - RSI/MACD: ATR > 0.007
    - Support/Resistance: ATR < 0.006
    - Fibonacci: 0.004 < ATR < 0.008
    - Bollinger: ATR > 0.005

### Risk Management

- [ ] `calculateRiskReward(signal, marketData)` → `sep::risk::calculateRiskReward()`
  - Location: `src/apps/oanda_trader/risk_calculator.hpp`
  - Status: Not implemented
  - Formula: `Math.max(1.5, 2.5 - signal.confidence) * (1 + marketData.atr * 10)`
  - Critical: Must match JavaScript risk calculation exactly

### Feedback Loop

- [ ] `applyFeedbackLoop(results)` → `sep::memory::MemoryTierManager::applyFeedback()`
  - Location: `src/memory/memory_tier_manager.cpp`
  - Status: Partial (needs exact logic)
  - Adjustments:
    - Win: resonance += 0.03
    - Loss: resonance -= 0.04
    - Drop if resonance < 0.55
    - Promote if resonance > 0.75

## OANDA Integration (`oanda_connector.js` → C++)

### Market Data

- [x] `getAccountSummary()` → `OandaConnector::getAccountSummary()`
  - Location: `src/connectors/oanda_connector.cpp:41`
  - Status: Implemented
  - Verified: Returns same format as JavaScript

- [ ] `calculateATR(candles)` → `OandaConnector::calculateATR()`
  - Location: `src/connectors/oanda_connector.cpp`
  - Status: Not implemented
  - Algorithm: 14-period ATR calculation
  - Critical: Must match JavaScript ATR exactly

### Trading

- [ ] `executeTrade(signal, accountBalance)` → `OandaConnector::executeTrade()`
  - Location: `src/connectors/oanda_connector.cpp`
  - Status: Not implemented
  - Includes:
    - Position sizing based on account balance
    - Stop-loss calculation using ATR
    - Order JSON formatting
    - Risk percentage calculation

- [x] `placeOrder(instrument, units, stopLoss)` → `OandaConnector::placeOrder()`
  - Location: `src/connectors/oanda_connector.cpp:147`
  - Status: Basic implementation (needs stop-loss)
  - TODO: Add proper stop-loss calculation

## Data Structures

### Pattern Format (`final_symmetry.json`)
```cpp
struct Pattern {
    std::string note;        // e.g., "RSI_oversold"
    double resonance;        // 0.55 - 1.0
    int consciousnessLevel;  // 1-4 based on ATR
    std::string patternId;   // Unique identifier
};
```

### Signal Format (`eternal_cycle.json`)
```cpp
struct Signal {
    std::string patternId;
    double signalValue;      // 0.0 - 1.0
    std::string direction;   // "buy", "sell", "neutral"
    double strength;         // Based on resonance
    double confidence;       // Based on consciousnessLevel
};
```

### Feedback Format (`truth_kernel.json`)
```cpp
struct FeedbackEntry {
    std::string patternId;
    std::string hash;        // From compression
    bool validated;
    double winRate;
    double riskReward;
    int executionTime;
};
```

## Testing Requirements

### Unit Tests
- [ ] Test logistic map output matches JavaScript
- [ ] Test seed generation matches JavaScript
- [ ] Test signal thresholds match JavaScript
- [ ] Test ATR calculation matches JavaScript
- [ ] Test feedback adjustments match JavaScript

### Integration Tests
- [ ] Test full signal generation pipeline
- [ ] Test validation against market data
- [ ] Test feedback loop updates
- [ ] Test OANDA order execution

### Performance Tests
- [ ] Benchmark CUDA vs CPU logistic iterations
- [ ] Measure end-to-end latency
- [ ] Test concurrent pattern processing

## Implementation Priority Order

1. **Core Signal Generation**
   - Logistic map function
   - Seed generation
   - Signal building

2. **Market Validation**
   - ATR calculation
   - Market condition checks
   - Risk/reward calculation

3. **Feedback Loop**
   - Pattern resonance updates
   - Health protocol implementation
   - Memory tier mapping

4. **OANDA Integration**
   - Complete order placement
   - Add stop-loss calculation
   - Implement position sizing

## Notes

- Every implementation MUST match JavaScript behavior exactly
- Use JavaScript files as specification, not inspiration
- When in doubt, refer to `forex/life.md` for constraints
- Test each component against JavaScript outputs before integration
- Document any deviations or issues in this file