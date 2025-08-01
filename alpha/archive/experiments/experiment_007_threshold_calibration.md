# Experiment 007: Threshold Calibration

## Hypothesis
The alpha analysis thresholds (confidence ≥ 0.6, coherence ≥ 0.9) are too restrictive for our current data. Need to find realistic thresholds that balance signal quality with reasonable signal count.

## Approach
**Progressive threshold testing** to understand signal distribution:

### Phase 1: Signal Distribution Analysis
Add debug output to understand actual ranges:
- Min/max/average confidence values
- Min/max/average coherence values  
- Min/max/average stability values

### Phase 2: Threshold Sweep
Test different combinations:
- Coherence: 0.5, 0.6, 0.7, 0.8
- Confidence: 0.4, 0.5, 0.6, 0.7
- Find combinations that yield 10-100 high-quality signals

### Phase 3: Accuracy Optimization
Compare accuracy of different threshold combinations to find optimal balance.

## Expected Outcome
Find realistic thresholds that identify 5-20% of signals as "high confidence" with improved accuracy over the baseline 38.99%.
