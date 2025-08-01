# Experiment 010: Ultra-High Selectivity

## Objective
Test the hypothesis that increasing the confidence threshold to >= 0.7 will result in a predictive accuracy of >= 50%.

## Hypothesis
A higher confidence threshold will filter out lower-quality signals, leading to a smaller but more accurate set of predictions.

## Methodology
- Run the `pattern_metric_example` with a configuration that sets the minimum confidence threshold to 0.7.
- All other parameters will be kept consistent with Experiment 009 for a controlled comparison.
- The output will be logged to `results/experiment_010_results.md`.