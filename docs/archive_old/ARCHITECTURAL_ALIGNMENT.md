# SEP Engine Architectural Alignment Document

## Executive Summary

This document ensures the SEP Engine project remains focused on its core objective: creating a production-ready forex trading system that uses the proven signal generation logic from the JavaScript reference implementation (`forex/` directory) while leveraging CUDA-accelerated quantum-inspired pattern analysis.

## Core Vision Alignment

### What We're Building
A high-performance C++ trading system that:
1. **Uses OANDA's real API** for live forex data (EUR/USD focus)
2. **Implements the exact signal logic** from `forex/brains.js` and `forex/processor.js`
3. **Leverages CUDA** for accelerated pattern analysis
4. **Maintains the deterministic chaos approach** (logistic map, r=3.97)
5. **Implements the adaptive feedback loop** for continuous improvement

### What We're NOT Building
- A generic signal generator
- An abstract pattern recognition system
- A theoretical quantum computing framework
- A research prototype

## Critical Success Factors

### 1. JavaScript Reference Implementation Fidelity
The `forex/` directory contains the **algorithmic specification** for our system:

```
forex/
├── brains.js          # Signal generation logic (MUST IMPLEMENT EXACTLY)
├── processor.js       # Validation & feedback loop (CRITICAL REFERENCE)
├── oanda_connector.js # API integration blueprint
├── final_symmetry.json # Pattern definitions
├── truth_kernel.json  # Feedback data store
└── life.md           # System constraints & health protocol
```

### 2. CUDA Integration Priority
- CUDA is **essential** for real-time pattern processing
- The Docker build issues MUST be resolved
- GPU acceleration differentiates us from competitors

### 3. OANDA API Integration
- Real trading, not simulation
- Focus on EUR/USD initially
- Implement exact risk management from `oanda_connector.js`

## Technical Architecture Alignment

### Data Flow (As Specified)
```
OANDA API → OandaConnector → Pattern Processor → SEP Engine (CUDA)
                                     ↓
                            Signal Generation (brains.js logic)
                                     ↓
                            Validation (processor.js logic)
                                     ↓
                            Trade Execution / Feedback Loop
```

### Key Components Mapping

#### 1. Signal Generation (`brains.js` → `src/apps/oanda_trader/`)
```cpp
// Must implement:
- logistic() function with r=3.97
- generateRecursiveSeed() using pattern characteristics
- buildSignals() with exact thresholds:
  - Buy: signalValue > 0.5
  - Sell: signalValue < 0.3
  - Neutral: otherwise
```

#### 2. Validation & Feedback (`processor.js` → `src/memory/memory_tier_manager.cpp`)
```cpp
// Must implement:
- checkMarketCondition() with ATR thresholds
- calculateRiskReward() using confidence & ATR
- applyFeedbackLoop() with exact adjustments:
  - Win: resonance += 0.03
  - Loss: resonance -= 0.04
  - Health Protocol: drop < 0.55, promote > 0.75
```

#### 3. OANDA Integration (`oanda_connector.js` → `src/connectors/oanda_connector.cpp`)
```cpp
// Already partially implemented, needs:
- ATR calculation algorithm
- Position sizing logic
- Stop-loss calculation using ATR
- Exact order JSON structure
```

## Implementation Priorities

### Phase 1: Infrastructure (Immediate)
1. **Fix CUDA Docker Build**
   - Resolve CMake CUDA discovery
   - Fix entrypoint script syntax
   - Ensure GPU access in container

2. **Stabilize OANDA Connection**
   - Debug crash in `connectToOanda()`
   - Implement robust error handling
   - Manage streaming thread properly

### Phase 2: Core Logic Implementation (Week 1)
1. **Port Signal Generation**
   - Implement exact `brains.js` logic in C++
   - Use CUDA for logistic map iterations
   - Maintain deterministic seeding

2. **Port Validation Logic**
   - Implement `processor.js` validation rules
   - Map to memory tier promotion/demotion
   - Implement feedback adjustments

### Phase 3: Integration & Testing (Week 2)
1. **Connect Components**
   - Wire OANDA data to signal generator
   - Feed signals to validator
   - Execute trades based on validation

2. **Verify Against JavaScript**
   - Compare outputs with JS implementation
   - Ensure identical signal generation
   - Validate feedback loop behavior

## Documentation Strategy

### Maintain Focus Documents
1. **This alignment document** - Keep updated
2. **Implementation tracking** - Map JS functions to C++
3. **Performance metrics** - Compare to JS baseline

### Archive Distractions
- Move theoretical discussions to `docs/archive/`
- Keep only actionable, implementation-focused docs active

## Success Metrics

1. **Functional Parity**: C++ implementation produces identical signals to JavaScript
2. **Performance**: CUDA acceleration provides >10x speedup
3. **Reliability**: 99.9% uptime with OANDA connection
4. **Profitability**: Match or exceed JavaScript system's trading performance

## Constraint Lock (from `life.md`)
- No abstraction
- No metaphor
- Execution must produce measurable outputs only

## Next Immediate Actions

1. Fix CUDA discovery in Docker:
   ```bash
   # Add to Dockerfile:
   ENV CUDA_HOME=/usr/local/cuda
   ENV CMAKE_CUDA_COMPILER=/usr/local/cuda/bin/nvcc
   ```

2. Create implementation tracking:
   ```
   docs/IMPLEMENTATION_TRACKING.md
   - [ ] brains.js::logistic() → sep::signals::logistic()
   - [ ] brains.js::generateRecursiveSeed() → sep::signals::generateSeed()
   - [ ] processor.js::checkMarketCondition() → sep::validation::checkMarket()
   ...
   ```

3. Set up comparison testing:
   ```
   tests/js_parity/
   ├── test_signal_generation.cpp
   ├── test_validation_logic.cpp
   └── expected_outputs.json
   ```

## Conclusion

The SEP Engine is a **production trading system**, not a research project. Every line of code must serve the goal of implementing the proven JavaScript trading logic in high-performance C++ with CUDA acceleration. The `forex/` directory is our specification, not inspiration.

Stay focused. Ship working code. Make profitable trades.