# Experiment 007 Results: Threshold Calibration

## Signal Distribution Analysis
**Actual signal ranges in our data:**
- **Confidence**: min=0.409, max=1.106, avg=0.566
- **Coherence**: min=0.000, max=0.889, avg=0.460  
- **Stability**: min=0.000, max=1.000, avg=0.543

## Calibrated Thresholds
- **Confidence**: ≥0.50 (vs 0.6 in alpha analysis)
- **Coherence**: ≥0.60 (vs 0.9 in alpha analysis)
- **Stability**: ≥0.00 (unchanged)

## Results
- **Overall Accuracy**: 38.99%
- **High Confidence Accuracy**: 42.33% ✅ (+3.34 percentage points)
- **High Confidence Signals**: 215 (14.9% of total)

## Key Success
**FILTERING WORKS**: High confidence signals outperform overall accuracy by 3.34 percentage points, validating the alpha analysis approach with realistic thresholds.

## Optimization Opportunities
1. **Coherence threshold**: Could be lowered to 0.5 (current avg=0.460)
2. **Confidence threshold**: Could be raised to 0.6 (above average)
3. **Combination testing**: Find optimal balance

## Next Steps - Experiment 008
Test threshold combinations to maximize high confidence accuracy:
- confidence≥0.6, coherence≥0.5
- confidence≥0.65, coherence≥0.55
- Target: >45% accuracy with 5-15% signal selection
