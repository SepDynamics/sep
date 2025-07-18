#!/bin/bash

# Run the pattern metric example and pipe the output to the conversion script
/sep/docs/proofs/pattern_metric_example | python3 /sep/convert_metrics.py > /sep/metrics.json

# Run the financial backtest script
python3 /sep/financial_backtest.py /sep/metrics.json /sep/market_data.csv