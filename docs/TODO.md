# Development Plan

The SEP Engine is stable and ready for further experimentation. This list tracks ongoing work and where to find supporting material.

## Short-Term Goals

1. **Verify the metrics pipeline**
   - Run `test_metrics.py` to confirm the workbench outputs coherence, stability and entropy.
   - Ensure `metrics.json` feeds into `financial_backtest.py` for analysis.
2. **Execute the OANDA challenge experiment**
   - `python3 run_alpha_experiment.py`
   - Record results in `docs/proofs/`.

## Upcoming Improvements

- Optimize CUDA kernels to achieve sub-5 ms processing.
- Add dashboards for backtest visualization.
- Harden error handling and set up CI.
- Expand signal generation using the reference logic in [`../forex/`](../forex/).

Historic notes and prior experiments live in `docs/proofs/` and `docs/archive_old/`.
