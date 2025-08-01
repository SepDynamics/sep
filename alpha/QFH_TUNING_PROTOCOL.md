# QFH Tuning Protocol
**Alpha Development Guide for Quantum Field Harmonics Configuration**

## Quick Reference

### Current Baseline (Experiment 024)
- **Overall**: 41.35% accuracy
- **High-Conf**: 35.35% accuracy (6.9% of signals)
- **Target**: >50% overall, >45% high-confidence

### Key Configuration Files
- **Core Engine**: `/sep/src/quantum/bitspace/qfh.cpp`
- **Testbed**: `/sep/examples/pme_testbed_phase2.cpp`
- **Headers**: `/sep/src/quantum/bitspace/qfh.h`

## Tuning Parameters

### 1. Trajectory Damping Weights (`qfh.cpp:105-106`)
```cpp
const double k1 = 0.3;  // Entropy weight [TUNE THIS]
const double k2 = 0.2;  // Coherence weight [TUNE THIS]
```

**Effect**: Controls how `λ = k1 * Entropy + k2 * (1 - Coherence)` shapes trajectory damping
- **Higher k1**: More sensitive to pattern complexity
- **Higher k2**: More sensitive to pattern consistency

### 2. Pattern-Trajectory Blend (`qfh.cpp:315`)
```cpp
result.coherence = 0.3f * trajectory_coherence + 0.7f * pattern_coherence;
//                  ↑ TUNE THIS              ↑ TUNE THIS
```

**Effect**: Balances trajectory-based vs pattern-based signal quality
- **More trajectory weight**: Stronger future-prediction influence
- **More pattern weight**: Conservative pattern-recognition focus

### 3. Coherence Scaling (`qfh.cpp:313-323`)
```cpp
float pattern_coherence = (1.0f - result.entropy) * 1.2f;     // [TUNE 1.2f]
float stability_coherence = (1.0f - result.rupture_ratio) * 1.1f;  // [TUNE 1.1f]
float flip_coherence = (1.0f - result.flip_ratio) * 1.05f;    // [TUNE 1.05f]
```

**Effect**: Amplifies different aspects of pattern quality

## Rapid Testing Workflow

### 1. Quick Test Command
```bash
./build.sh && ./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json 2>&1 | grep -E "(Overall Accuracy|High Confidence)"
```

### 2. Parameter Sweep Template
```bash
#!/bin/bash
# Save current values
cp /sep/src/quantum/bitspace/qfh.cpp /sep/src/quantum/bitspace/qfh.cpp.backup

for k1 in 0.2 0.3 0.4 0.5; do
  for k2 in 0.1 0.2 0.3 0.4; do
    # Modify parameters
    sed -i "s/const double k1 = 0.3;/const double k1 = $k1;/" /sep/src/quantum/bitspace/qfh.cpp
    sed -i "s/const double k2 = 0.2;/const double k2 = $k2;/" /sep/src/quantum/bitspace/qfh.cpp
    
    # Test
    ./build.sh >/dev/null 2>&1
    result=$(./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json 2>&1 | grep "Overall Accuracy" | cut -d' ' -f3)
    echo "k1=$k1 k2=$k2 accuracy=$result"
    
    # Restore for next iteration
    cp /sep/src/quantum/bitspace/qfh.cpp.backup /sep/src/quantum/bitspace/qfh.cpp
  done
done
```

### 3. Results Tracking
```bash
# Log format
echo "$(date) k1=$k1 k2=$k2 overall=$overall high_conf=$high_conf" >> /sep/alpha/tuning_results.log
```

## Configuration Strategies

### Strategy A: Aggressive Trajectory Focus
**Goal**: Maximize trajectory damping influence
```cpp
// qfh.cpp:105-106
const double k1 = 0.5;  // High entropy sensitivity
const double k2 = 0.4;  // High coherence sensitivity

// qfh.cpp:315
result.coherence = 0.5f * trajectory_coherence + 0.5f * pattern_coherence;
```

### Strategy B: Conservative Pattern Enhancement
**Goal**: Gradual improvement while maintaining stability
```cpp
// qfh.cpp:105-106
const double k1 = 0.2;  // Lower entropy sensitivity
const double k2 = 0.1;  // Lower coherence sensitivity

// qfh.cpp:315
result.coherence = 0.2f * trajectory_coherence + 0.8f * pattern_coherence;
```

### Strategy C: Balanced Optimization
**Goal**: Find optimal balance between trajectory and pattern
```cpp
// qfh.cpp:105-106
const double k1 = 0.35; // Moderate entropy sensitivity
const double k2 = 0.25; // Moderate coherence sensitivity

// qfh.cpp:315
result.coherence = 0.4f * trajectory_coherence + 0.6f * pattern_coherence;
```

## Expected Outcomes by Experiment

### Experiment 025: Damping Parameter Tuning
**Target Metrics**:
- Overall accuracy: 43-47%
- High-confidence accuracy: 40-50%
- High-confidence rate: 7-10%

**Success Criteria**: 
- Overall >45% OR High-confidence >45%
- No significant degradation in signal count

### Experiment 026: Pattern Vocabulary Optimization
**Target Metrics**:
- Coherence average: >0.5 (currently 0.406)
- Stability maintenance: >0.7 (currently 0.724)

**Success Criteria**:
- Improved coherence distribution
- Maintained or improved accuracy

### Experiment 027: Volatility Re-Integration
**Target Metrics**:
- Overall accuracy: >50%
- Combined with previous optimizations

**Success Criteria**:
- Beat best historical result (46.59% high-confidence)
- Robust performance across market conditions

## Debug Commands

### 1. Signal Quality Analysis
```bash
./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json 2>&1 | grep -A5 "Signal Distribution Analysis"
```

### 2. Coherence Distribution Check
```bash
./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json | awk -F',' 'NR>1 {print $8}' | sort -n | tail -20
```

### 3. Pattern Count Verification
```bash
./build/examples/pme_testbed_phase2 Testing/OANDA/O-test-2.json 2>&1 | grep "DEBUG: Created"
```

## Safety Checks

### 1. Before Tuning
- [ ] Backup current `qfh.cpp`
- [ ] Confirm baseline: 41.35% overall accuracy
- [ ] Verify build system works

### 2. During Tuning
- [ ] Track each configuration change
- [ ] Monitor for segfaults or crashes
- [ ] Log all parameter combinations tested

### 3. After Tuning
- [ ] Document best configuration found
- [ ] Update AGENT.md with results
- [ ] Commit successful parameter values

## File Modification Checklist

### Core QFH Engine (`/sep/src/quantum/bitspace/qfh.cpp`)
- [ ] Lines 105-106: k1, k2 damping weights
- [ ] Line 315: trajectory/pattern blend ratio
- [ ] Lines 313-323: coherence scaling factors

### Testing Verification (`/sep/examples/pme_testbed_phase2.cpp`)
- [ ] Confirm QFHBasedProcessor is active
- [ ] Verify no fallback to legacy engine
- [ ] Check output format for tracking

**Ready for systematic parameter optimization targeting >50% overall accuracy.**
