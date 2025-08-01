# Experiment 003 Results: Pattern Creation Debug

## Key Discovery
**Both phases create identical pattern counts**: 1,440 patterns from 1,440 candles

**But signal generation is drastically different:**
- Phase 1: 1,440 patterns → ~76 signals (5.3% conversion rate)
- Phase 2: 1,440 patterns → 1,439 signals (99.9% conversion rate)

## Root Cause Identified
**Signal Filtering Issue**: Phase 1 has strict criteria that filters out 95% of patterns before they become signals, while Phase 2 converts nearly every pattern to a signal.

## Analysis
The issue is NOT in pattern creation but in **signal generation selectivity**:

1. **Phase 1**: Very selective - only generates signals for high-quality patterns
2. **Phase 2**: Too permissive - generates signals for almost every pattern

## Hypothesis for Experiment 004
**Signal Threshold Investigation**: Phase 1's thresholds and scoring are much more restrictive than Phase 2's implementation, even with identical parameters.

**Need to debug:**
1. Why does Phase 1 filter out 95% of patterns?
2. What makes Phase 1's signal generation so selective?
3. Are the threshold applications different between phases?

## Next Steps
1. Add signal scoring debug output to both phases
2. Compare threshold behavior and score distributions
3. Identify the filtering mechanism in Phase 1 that Phase 2 lacks
