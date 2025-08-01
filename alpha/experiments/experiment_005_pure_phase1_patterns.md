# Experiment 005: Pure Phase 1 Pattern Logic

## Hypothesis
Based on docs analysis, Phase 2's market regime analysis in pattern creation is corrupting base patterns. Use pure Phase 1 pattern logic in Phase 2 framework to recover baseline accuracy.

## Key Insights from Docs
1. **Confidence Filtering**: Alpha analysis shows using confidence ≥ 0.6, coherence ≥ 0.9 filters for high-quality signals
2. **Pattern Enhancement**: Optimization strategy suggests adding pattern types, but current types are validated
3. **Multi-factor Thresholds**: Need stability ≥ 0.0 as minimum baseline

## Implementation Plan
1. **Remove market regime from pattern creation** entirely in Phase 2
2. **Use exact Phase 1 pattern processing** for coherence, stability, entropy
3. **Apply confidence-based filtering** from alpha analysis
4. **Test multi-threshold filtering** approach

## Expected Outcome
Should recover Phase 1's 50.94% accuracy by removing pattern corruption while maintaining Phase 2's framework benefits.
