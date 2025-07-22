# life.md — Final Symmetry System Reference

## System Overview
A deterministic market signal system that operates using logistic chaos (r=3.97), prime-based hashing, and a live feedback validation loop. Signals are generated and validated against real-world EUR/USD market conditions.

---

## Core Logic Flow

1. **Input**: `final_symmetry.json`
2. **Generate**: Signal with logistic chaos seeding via `brains.js`
3. **Compress**: Signal via hash function with prime seed (`24128686327077747`)
4. **Validate**: Against live market data
5. **Output**:
   - `eternal_cycle.json`: All generated signals
   - `truth_kernel.json`: Validated and compressed outputs
6. **Adapt**: Update resonance and consciousnessLevel based on metrics (winRate, RR, latency, ATR)

---

## File Overview

### brains.js
- Generates deterministic signal via logistic map
- Uses resonance as seeding input
- Pulls prior feedback to support future learning

### processor.js
- Compresses and validates each signal
- Tracks recursiveSeed + hash + feedback status

### final_symmetry.json
- Market anchor patterns
- Includes:
  - `note`
  - `resonance`
  - `consciousnessLevel`
  - `patternId`

---

## Health Protocol

- Drop if resonance < 0.55
- Promote if resonance > 0.75
- Adapt volatility level via ATR:
  - <0.003 → 1
  - 0.003–0.007 → 2
  - 0.007–0.012 → 3
  - >0.012 → 4

---

## Constraint Lock

- No abstraction
- No metaphor
- Execution must produce measurable outputs only