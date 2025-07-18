# Predictive Gauge Definition

The predictive gauge is a composite metric derived from the primary outputs of the SEP Engine: coherence, stability, and entropy. It is designed to provide a single, smoothed value that can be used as a leading indicator for financial market analysis.

## Formula

The gauge is calculated as a weighted sum of the normalized primary metrics:

`gauge = (w_c * coherence_norm) + (w_s * stability_norm) - (w_e * entropy_norm)`

Where:

- `w_c`: Weight for coherence (default: 0.5)
- `w_s`: Weight for stability (default: 0.3)
- `w_e`: Weight for entropy (default: 0.2)

## Normalization

Each primary metric is normalized using a rolling Z-score to account for changing market conditions:

`metric_norm = (metric - rolling_mean(metric, 20)) / rolling_std(metric, 20)`

## Smoothing

The final gauge value is smoothed using a 20-period simple moving average (SMA) to reduce noise and identify underlying trends.