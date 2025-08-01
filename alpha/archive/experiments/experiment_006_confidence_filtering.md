# Experiment 006: Confidence-Based Filtering

## Hypothesis
Based on alpha analysis docs, applying confidence-based filtering (confidence ≥ 0.6, coherence ≥ 0.9, stability ≥ 0.0) should improve signal quality and accuracy by filtering out low-quality predictions.

## Key Insight from Alpha Analysis
The docs show successful alpha generation using:
- **Signal Confidence ≥ 0.6**: Based on default `collapse_threshold` from QBSA patent
- **Coherence ≥ 0.9**: High-quality pattern requirement
- **Stability ≥ 0.0**: Minimum baseline requirement

## Implementation Plan
1. **Add confidence filtering** to Phase 2 backtesting
2. **Apply multi-factor thresholds** from alpha analysis
3. **Compare filtered vs unfiltered** performance
4. **Test if this recovers Phase 1's quality**

## Expected Outcome
Should significantly improve accuracy by trading fewer but higher-quality signals, similar to the alpha analysis approach that generated positive returns.
