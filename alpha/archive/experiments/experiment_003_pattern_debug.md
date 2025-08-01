# Experiment 003: Pattern Creation Debug

## Hypothesis
Phase 2 creates patterns for every single candle, while Phase 1 uses a different pattern creation strategy, leading to the signal explosion.

## Investigation Plan
1. **Add debug output** to count patterns created in both phases
2. **Compare pattern creation logic** - Phase 1 vs Phase 2
3. **Test selective pattern creation** - only create patterns under certain conditions

## Key Observation
Phase 2 code shows: `for (size_t i = 0; i < candles.size(); ++i)` - creates pattern for EVERY candle!

This could be creating ~1400+ patterns (one per candle) vs Phase 1's selective approach.

## Test: Add Pattern Count Debug
```cpp
std::cout << "DEBUG: Created " << quantum_patterns.size() << " patterns from " 
          << candles.size() << " candles" << std::endl;
```

## Expected Finding
Phase 2 creates 1400+ patterns, Phase 1 creates ~76 patterns.
