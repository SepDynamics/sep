# Experiment 002: Minimal Phase 2 Framework

## Hypothesis
Phase 2's market regime analysis is fundamentally interfering with signal generation. Create a minimal Phase 2 that removes all market analysis components and mirrors Phase 1's logic exactly.

## Changes to Implement
1. **Remove Market Regime Analysis**: Comment out all `AdvancedMarketAnalyzer` calls
2. **Remove Signal Quality**: Eliminate quality calculation and scoring modifications  
3. **Pure Phase 1 Logic**: Use exact Phase 1 signal generation with Phase 2's structure
4. **Isolate Framework**: Test if Phase 2's basic framework can match Phase 1

## Code Modifications
```cpp
// Remove market state analysis
// std::vector<AdvancedMarketAnalyzer::MarketState> market_states = 
//     AdvancedMarketAnalyzer::analyzeMarketRegimes(candles, 20);

// Remove signal quality calculation
// double signal_quality = AdvancedMarketAnalyzer::calculateSignalQuality(metric, market_state);

// Pure Phase 1 scoring
double buy_score = (metric.stability * stability_w) + 
                  (metric.coherence * coherence_w) + 
                  ((1.0 - metric.phase) * entropy_w);

double sell_score = ((1.0 - metric.stability) * stability_w) + 
                   ((1.0 - metric.coherence) * coherence_w) + 
                   (metric.phase * entropy_w);
```

## Expected Outcome
If framework differences are the issue, this should recover Phase 1's 50.94% accuracy.
