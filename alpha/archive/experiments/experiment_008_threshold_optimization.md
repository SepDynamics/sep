# Experiment 008: Threshold Optimization

## Success Baseline
Experiment 007 proved filtering works: 42.33% vs 38.99% (+3.34 points) with 14.9% signal selection.

## Current Signal Distribution
- **Confidence**: avg=0.566 (range: 0.409-1.106)
- **Coherence**: avg=0.460 (range: 0.000-0.889)
- **Stability**: avg=0.543 (range: 0.000-1.000)

## Optimization Strategy
**Target**: >45% accuracy with 5-15% signal selection

### Test Matrix
1. **confidence≥0.6, coherence≥0.5** (above avg confidence, avg coherence)
2. **confidence≥0.65, coherence≥0.55** (higher selectivity)
3. **confidence≥0.7, coherence≥0.5** (very high confidence)

## Implementation
Test each combination and track:
- Accuracy improvement
- Signal count (target: 70-215 signals, 5-15%)
- Balance between selectivity and performance

## Success Criteria
- High confidence accuracy >45%
- Signal selection 5-15% of total
- Consistent improvement over baseline
