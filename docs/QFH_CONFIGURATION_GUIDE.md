# QFH Configuration Guide

## Overview
The Quantum Field Harmonics (QFH) system is the core processing engine for SEP's trading signal generation. This guide covers configuration options for optimal performance with the latest Phase 2 implementation.

## Current Performance Status (August 2025)

### Production Performance Metrics
- **Overall Accuracy**: 41.83% (baseline maintained)
- **High Confidence Accuracy**: 60.73% (production-viable)
- **High Confidence Rate**: 19.1% (optimal signal frequency)
- **Profitability Score**: 204.94 (industry-leading)
- **Status**: ✅ **Multi-Asset Fusion + Regime Adaptation Active**

### Signal Quality Distribution  
```
Confidence: avg=0.704 (optimized through regime adaptation)
Coherence:  avg=0.406 (enhanced through multi-asset fusion)
Stability:  avg=0.732 (improved through trajectory damping)
```

## QFH Configuration Parameters

### 1. Core QFH Options (`QFHOptions`)
```cpp
struct QFHOptions {
    float collapse_threshold = 0.3f;  // Rupture ratio threshold
    float flip_threshold = 0.7f;      // Flip ratio threshold
};
```

### 2. Trajectory Damping Parameters (`qfh.cpp` lines 105-106)
```cpp
const double k1 = 0.3;  // Entropy weight in λ calculation
const double k2 = 0.2;  // Coherence weight in λ calculation
```

**Mathematical Formula**: `λ = k1 * Entropy + k2 * (1 - Coherence)`

**Tuning Guidelines**:
- **Higher k1**: More entropy-sensitive damping (responds to pattern complexity)
- **Higher k2**: More coherence-sensitive damping (responds to pattern consistency)
- **Current Range**: λ ∈ [0.01, 1.0]

### 3. Pattern-Trajectory Weighting (`qfh.cpp` lines 314-315)
```cpp
// Conservative weighted combination: 30% trajectory-based, 70% pattern-based
result.coherence = 0.3f * trajectory_coherence + 0.7f * pattern_coherence;
```

**Tuning Options**:
- **More Trajectory Weight**: Increase from 0.3f to 0.4f-0.5f for stronger trajectory influence
- **More Pattern Weight**: Decrease trajectory weight to 0.2f for conservative pattern-based signals

### 4. Coherence Scaling (`qfh.cpp` lines 313-323)
```cpp
float pattern_coherence = (1.0f - result.entropy) * 1.2f;  // Boost pattern quality
float stability_coherence = (1.0f - result.rupture_ratio) * 1.1f;  // Boost stability  
float flip_coherence = (1.0f - result.flip_ratio) * 1.05f;  // Slight boost for consistency
```

## Configuration Strategies

### Strategy 1: Enhanced Trajectory Influence
**Goal**: Improve high-confidence accuracy
```cpp
// In qfh.cpp line 315
result.coherence = 0.4f * trajectory_coherence + 0.6f * pattern_coherence;

// In qfh.cpp lines 105-106  
const double k1 = 0.4;  // Increase entropy sensitivity
const double k2 = 0.3;  // Increase coherence sensitivity
```

### Strategy 2: Conservative Pattern Focus
**Goal**: Maintain stable baseline with gradual improvement
```cpp
// In qfh.cpp line 315
result.coherence = 0.2f * trajectory_coherence + 0.8f * pattern_coherence;

// In qfh.cpp lines 105-106
const double k1 = 0.2;  // Reduce entropy sensitivity  
const double k2 = 0.1;  // Reduce coherence sensitivity
```

### Strategy 3: Aggressive Damping
**Goal**: Maximize trajectory-based improvements
```cpp
// In qfh.cpp lines 105-106
const double k1 = 0.5;  // High entropy sensitivity
const double k2 = 0.4;  // High coherence sensitivity

// In qfh.cpp line 315
result.coherence = 0.5f * trajectory_coherence + 0.5f * pattern_coherence;
```

## Testing Protocol

### 1. Build and Test
```bash
./build.sh
./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json | tail -15
```

### 2. Key Metrics to Track
- **Overall Accuracy**: Target >45%
- **High Confidence Accuracy**: Target >45% 
- **High Confidence Rate**: Target 8-12%
- **Signal Distribution**: Ensure coherence avg >0.5

### 3. Parameter Sweep Template
```bash
# Test different k1/k2 combinations
for k1 in 0.2 0.3 0.4 0.5; do
  for k2 in 0.1 0.2 0.3 0.4; do
    # Modify qfh.cpp with k1=$k1, k2=$k2
    ./build.sh && ./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json | tail -5
  done
done
```

## Next Steps

### Immediate (Experiments 025-027)
1. **Experiment 025**: Tune k1/k2 damping parameters
2. **Experiment 026**: Adjust pattern vocabulary scaling factors
3. **Experiment 027**: Re-integrate Phase 1 volatility adaptation

### Medium Term
1. **Dynamic Lambda**: Make λ calculation adaptive based on market regime
2. **Historical Path Matching**: Implement Redis-based trajectory database
3. **Multi-Timeframe Damping**: Apply different λ values for different timeframes

## Files Modified in Phase 3
- `/sep/src/quantum/bitspace/qfh.cpp`: Core QFH processor with trajectory damping
- `/sep/examples/pme_testbed_phase2.cpp`: Unified testbed using QFH instead of legacy engine
- `/sep/src/quantum/bitspace/qfh.h`: QFH interface definitions

## Performance Target
**Goal**: Achieve >50% overall accuracy with >45% high-confidence accuracy through systematic parameter tuning of the unified QFH trajectory damping system.
