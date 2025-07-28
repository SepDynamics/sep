# Multi-Asset Pipeline Design

This document outlines the planned expansion from single-instrument processing to a
scalable, multi-asset pipeline. The initial implementation targets additional
forex pairs and prepares the architecture for commodity and equity futures.

## Supported Currency Pairs
- EUR/USD (existing)
- GBP/USD
- USD/JPY
- AUD/USD

## Pipeline Overview
The prototype implementation lives in `_sep/testbed/multi_stream_pipeline.py`.
It demonstrates an asynchronous queue that aggregates price updates from multiple
instruments concurrently. Each stream is produced by an independent coroutine and
processed by a single consumer task. The `asyncio.Queue` guarantees thread-safe
handling of events without requiring explicit locks.

## Futures Integration Plan
Commodity and equity futures will follow the same streaming interface. A generic
`MarketEvent` structure will normalize fields such as timestamp, price and
volume so that analysis modules can remain asset-agnostic. Future connectors will
push events into the same asynchronous pipeline, enabling unified processing
across asset classes.
