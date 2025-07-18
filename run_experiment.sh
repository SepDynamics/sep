#!/bin/bash

# Run the pattern metric example and pipe the output to the conversion script
# ./docs/proofs/pattern_metric_example /workspace/train_data_2021_to_2025 | python3 ./convert_metrics.py > ./metrics.json
# ./build/examples/pattern_metric_example /workspace/train_data_2021_to_2025 | python3 ./convert_metrics.py > ./metrics.json

# Run the financial backtest script
# python3 ./financial_backtest.py ./metrics.json
python3 ./chunk_and_analyze.py /workspace/train_data_2021_to_2025 10000
