# SEP Documentation Overview

This directory centralizes all project documents. The [root README](../README.md) covers the overall build and execution instructions.
Use this file as a map for locating more detailed material.

## Quick Links

- **Proof-of-Concepts:** [`proofs/`](proofs/) – chronological experiments validating engine performance and predictive capability.
- **Architecture Notes:** [`integrated_trading_platform_architecture.md`](integrated_trading_platform_architecture.md), [`signal_generation_architecture.md`](signal_generation_architecture.md), [`trading_hud_features.md`](trading_hud_features.md)
- **Forex Prototype:** [`../forex/`](../forex/) – JavaScript signal generation reference from earlier work.

## Building and Running

```bash
# Build the engine
./build.sh

# Start the workbench (requires keys.txt with OANDA credentials)
./run_workbench.sh

# Backtest using metrics
python3 financial_backtest.py metrics.json
```

Metrics such as coherence, stability and entropy stream to the console when the workbench runs.

## Document Index

- [`proofs/README.md`](proofs/README.md) – explains each PoC document.
- [`archive_old/`](archive_old/) – historic planning material.
- [`TODO.md`](TODO.md) – active task list.

Refer to the PoC documents for verified benchmarks and previous analyses.
