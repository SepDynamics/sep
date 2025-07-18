# Predictive Gauge Definition

**Version:** 1.0
**Date:** July 18, 2025
**Status:** Draft

## 1. Overview

The Predictive Gauge is a composite metric derived from the Self-Emergent Processor (SEP) Engine's core outputs. It is designed to act as a leading indicator for financial market volatility and potential trend changes. By combining multiple quantum-inspired metrics, it aims to provide a more robust signal than any single metric could alone.

The gauge is calculated from the following primary metrics produced by the SEP Engine for each window of data:
- **Coherence:** Measures the degree of similarity or self-correlation within the data. High coherence suggests a strong, repeating trend.
- **Stability:** Measures the resistance of patterns to mutation over time. High stability suggests a persistent, non-volatile regime.
- **Entropy:** Measures the information density and unpredictability of the data. High entropy suggests chaotic or random price action.

## 2. Gauge Formula

The Predictive Gauge is calculated as a weighted average of the normalized primary metrics.

```
Gauge = (w_c * Coherence_norm) + (w_s * Stability_norm) - (w_e * Entropy_norm)
```

Where:
- `w_c`, `w_s`, `w_e` are the weights for Coherence, Stability, and Entropy, respectively.
- `_norm` indicates that the metric has been normalized to a [0, 1] range using a rolling Z-score or similar technique over a defined lookback period.

**Initial Weights:**
- `w_c` = 0.5
- `w_s` = 0.3
- `w_e` = 0.2

The entropy metric is subtracted because high entropy (randomness) is expected to be inversely correlated with predictable market movements.

## 3. Smoothing

To reduce noise and identify more significant trends, the raw Gauge value will be smoothed using a **20-period Simple Moving Average (SMA)**.

```
Smoothed_Gauge = SMA(Gauge, 20)
```

This `Smoothed_Gauge` will be the final output used in the backtesting strategies.

## 4. Interpretation

- **High Gauge Value (> 0.7):** Indicates a strong, stable, and persistent trend. This is a potential signal for a "buy" or "hold" in a trend-following strategy.
- **Low Gauge Value (< 0.3):** Indicates a breakdown of trends, high entropy, and instability. This could signal a market reversal or a period of high volatility, suggesting a "sell" or "stay out" signal.
- **Mid-Range (0.3 - 0.7):** A neutral or consolidating market.

These thresholds will be tested and optimized during the backtesting phase.