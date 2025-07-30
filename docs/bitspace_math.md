# Bitspace Mathematical Specification

This document provides the formal mathematical definitions for the core concepts in the bitspace metrics pipeline: Damped Value, Confidence, and the decay factor.

## 1. Damped Value Calculation

The primary value of a signal at a given point in time is calculated by integrating the impacts of future events, with each event's influence decaying over time. This produces a "damped" value that represents the stabilized, forward-looking potential of the signal.

Let $P = \{p_1, p_2, ..., p_n\}$ be a time-series of prices.
The damped value $V_i$ at index $i$ is the sum of discounted future price changes:

$$ V_i = \sum_{j=i+1}^{n} (p_j - p_i) \cdot e^{-\lambda(j-i)} $$

Where:
- $p_j$ is the price at a future time $j$.
- $p_i$ is the price at the current time $i$.
- $\lambda$ is the decay constant, derived from entropy and coherence.
- $(j-i)$ is the time difference between the future event and the present.

## 2. Decay Factor (λ)

The decay factor $\lambda$ determines how quickly the influence of future events diminishes. It is a function of the signal's entropy and coherence. High entropy or low coherence will lead to a faster decay.

$$ \lambda = k_1 \cdot \text{Entropy} + k_2 \cdot (1 - \text{Coherence}) $$

Where:
- **Entropy** measures the randomness or unpredictability of the signal.
- **Coherence** measures the internal structure and self-similarity of the signal.
- $k_1$ and $k_2$ are weighting constants to tune the sensitivity of the decay to entropy and coherence.

## 3. Confidence Score

The confidence score measures how closely a newly calculated trajectory path matches known historical paths. A higher score indicates that the current market behavior is following a recognized, predictable pattern.

Let $T_{current}$ be the vector representing the current signal's trajectory path during damping, and $T_{hist}$ be a historical trajectory path from the database.

The confidence score is calculated using cosine similarity:

$$ \text{Confidence} = \frac{T_{current} \cdot T_{hist}}{\|T_{current}\| \|T_{hist}\|} $$

A high confidence score (close to 1) implies a strong match, increasing the reliability of the trading signal.