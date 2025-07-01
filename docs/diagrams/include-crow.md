# Crow Integration Overview

This document illustrates how the Crow HTTP framework is integrated into the SEP Engine and
what data each piece exposes or consumes from other components. Crow is used to provide the
HTTP API layer that external clients interact with.

## High-Level Flow

```
+-----------+        +-------------+        +----------------+
| External  | -----> |  Crow App   | -----> |   Route        |
| Clients   | HTTP   | (src/api)   |        |  Handlers      |
+-----------+        +------+------+        +----+-----------+
                               |                   |
                               v                   v
                         +-----+-------+     +-----+---------+
                         | Crow Adapter|     | SepEngine API |
                         | (src/api)   |     | (src/api)     |
                         +-----+-------+     +-----+---------+
                               |                   |
                               v                   v
                       +-------+---------+  +------+------+
                       | Memory Manager  |  | Quantum     |
                       | (memory)        |  | Module      |
                       +-----------------+  +-------------+
```

1. **Crow App** – Configured in `src/api/server.cpp` and `crow_adapter.cpp`. It
   receives HTTP requests from external clients.
2. **Route Handlers** – Defined in `crow_adapter.cpp` and `server.cpp`. They parse
   incoming JSON, invoke `SepEngine` methods, and build responses.
3. **Crow Adapter** – `crow_adapter.h/cpp` bridges Crow’s `request` and `response`
   objects with the internal `HttpRequest`/`HttpResponse` interfaces.
4. **SepEngine API** – Implements the high‑level operations. It consumes pattern
   data and returns JSON results. Located under `src/api` and uses
   headers in `include/api`.
5. **Memory Manager / Quantum Modules** – Lower‑level modules (`src/memory`,
   `src/quantum`). These modules provide data storage and algorithmic
   processing. The `SepEngine` API passes data from HTTP requests down to these
   subsystems and aggregates their results.

## Data Exposed and Consumed

- **HTTP JSON Payloads** – Route handlers accept JSON bodies containing context
  objects, pattern data, or control commands. Parsing utilities in
  `json_helpers.h` convert them to C++ structures.
- **Engine Responses** – Results from `SepEngine` (e.g., similarity scores, pattern
  history) are serialized back to JSON and sent through `CrowResponseAdapter` to
  the client.
- **Metrics and Health Data** – `server.cpp` exposes health endpoints and gathers
  metrics such as request counts and error codes, which other components can
  query via the API.
- **Configuration** – `config::APIConfig` values are consumed by `SEPApiServer`
  to set up ports, logging, and optional middlewares (authentication and rate
  limiting).

This integration keeps the Crow framework isolated from core engine logic. The
`crow` headers live under `include/crow`, while API-specific adapters and
servers reside under `src/api` and `include/api`. External components interact
with the engine solely through the defined HTTP routes.


