# Testbed Scripts

This directory contains experimental Python utilities used to validate new ideas before integrating them into the core SEP Engine. Each script focuses on a specific aspect of strategy development or data processing.

## Available tools

- `advanced_backtest_integration.py` – expanded validation harness built on `financial_backtest.py`.
- `backtest_compare.py` – helper functions for strategy A/B comparisons.
- `strategy_comparison.py` – command line interface for side-by-side performance reports.
- `strategy_matrix_compare.py` – grid search across threshold parameters.
- `threshold_optimizer.py` / `threshold_tuning.py` – helpers for refining BUY/SELL decision thresholds.
- `multi_stream_pipeline.py` – prototype for multi-currency data ingestion.
- `market_data_normalizer.py` – utility to normalize column names across datasets.
- `data_quality_tools.py` – gap detection, interpolation and signal smoothing functions.

These scripts operate outside of the main build and are safe places to iterate on new algorithms. Once validated, functionality can be migrated into `scripts/` or `src/` as appropriate.
