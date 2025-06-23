# Core Module

This document describes the runtime responsibilities of the `core` module,
focusing on engine initialization, metrics collection and tracing utilities.

## Engine Initialization

The `Engine` class defined in `include/core/engine.h` owns the CUDA stream,
memory buffers and optional audio/blender subsystems. `Engine::init` receives
an `APIConfig` and performs the following steps:

1. Initializes the CUDA runtime via `CudaCore`.
2. Creates a default stream and allocates device buffers.
3. Starts the `AudioCapture` and connects the `BlenderBridge` if available.
4. Marks the engine as initialized so `run()` can start the processing loop.

The implementation lives in `src/core/engine.cpp` and keeps state in a
private `Impl` struct.

## Metrics

Metrics utilities reside under the `sep::metrics` namespace. The
`MetricsCollector` singleton gathers counters and gauges while
`PrometheusExporter` exposes them for scraping. Implementations are in
`src/core/metrics_collector.cpp` and `src/core/prometheus_exporter.cpp`
with lightweight declarations in the corresponding headers.

Common helpers such as `allocationFailures()` live in
`allocation_metrics.*` and are registered with the exporter during startup.

## Tracing

The tracing system provides a simple RAII span class in
`include/core/tracing.h`. A `TraceSpan` records its start time when
constructed and logs its duration upon destruction when
`SEP_VERBOSE_TRACE` is enabled. The source file `src/core/tracing.cpp`
contains these logging calls.

Together these components allow the engine to report runtime behaviour
without exposing implementation details through the headers.

Note: Obsolete `cmake_install.cmake` files have been removed from the
repository, keeping build artifacts out of version control.
