# API Module Overview

The API layer is split into a public header tree under `include/api` and
matching implementations in `src/api`.  Each header exposes declarations only
so other modules can depend on the API without pulling in implementation
details.  Previous helper sources like `connection_manager.cpp` have been
removed, leaving `connection_manager.h` as an interface used solely by tests.

Generated build files (`cmake_install.cmake`, `CMakeFiles/` directories, etc.)
were removed from version control to keep the repository clean.

The following table lists the remaining headers and their corresponding
implementations:

## Public interface headers

| Header | Purpose | Implementation |
|-------|---------|---------------|
| `api_exception.h` | Custom exception class. | Header only |
| `auth_middleware.h` | Crow authentication middleware. | `src/api/auth_middleware.cpp` |
| `background_cleanup.h` | Utility for periodic cleanup in a background thread. | Header only |
| `bridge.h` | C API for processing contexts. | `src/api/bridge_c.cpp` |
| `bridge.hpp` | C++ helpers and error utilities for the bridge. | `src/api/bridge.cpp`, `src/api/bridge_c.cpp` |
| `bridge_internal.hpp` | Internal shared state for the bridge. | Header only |
| `client.h` | High level HTTP client and Curl transport. | `src/api/client.cpp`, `src/api/curl_http_client.cpp`, `src/api/ollama_client.cpp` |
| `connection_manager.h` | Connection pooling interface. | Removed implementation |
| `crow_adapter.h` | Crow route helpers and adapter classes. | `src/api/crow_adapter.cpp` |
| `crow_request.h` | Implementation of `IRequest` for Crow. | Header only |
| `js_integration.h` | JavaScript bridge wrappers. | `src/api/js_integration.cpp` |
| `lock_free_rate_limiter.h` | Lock-free rate limiter implementation. | `src/api/lock_free_rate_limiter.cpp` |
| `rate_limit_middleware.h` | Crow middleware that applies a rate limiter. | `src/api/rate_limit_middleware.cpp` |
| `rate_limiter.h` | Abstract rate limiter interface. | Implemented inside `src/api/lock_free_rate_limiter.cpp` |
| `request_interface.h` | Abstract HTTP request representation. | Header only |
| `sep_engine.h` | Main engine façade. | `src/api/sep_engine.cpp` |
| `server.h` | Crow server wrapper. | `src/api/server.cpp` |
| `types.h` | Common API types and helpers. | Helper functions implemented in `src/api/sep_engine.cpp` |

## Observations

- `crow_adapter.h` declares `CrowRequestAdapter`, `CrowResponseAdapter`, `makeRequest` and `makeResponse` (see lines 38‑65) but these symbols have no implementation in the source tree.【F:include/api/crow_adapter.h†L38-L65】
- `setupSepApiRoutes` is implemented in `src/api/crow_adapter.cpp` and provides example routes for a Crow application.【F:src/api/crow_adapter.cpp†L45-L80】
- The server implementation in `server.cpp` duplicates much of the route logic from `crow_adapter.cpp`; consolidating these could reduce maintenance overhead.
- `bridge.cpp` and `bridge_c.cpp` each contain similar exception handling macros and could potentially share a common header.
- `makeRequest` is referenced from `src/memory/manager.cpp` but no definition exists in `src/api`; this suggests dead code or a missing implementation.【F:src/memory/manager.cpp†L152-L160】
- Several helper headers (`api_exception.h`, `background_cleanup.h`, `crow_request.h`, `request_interface.h`) are header‑only and do not require separate `.cpp` files.

## Potential cleanup

- Remove or implement the unused adapter classes in `crow_adapter.h` along with the `makeRequest`/`makeResponse` helpers.
- Investigate whether the example routes in `crow_adapter.cpp` are still needed now that `server.cpp` defines a full set of routes. If redundant, consider deleting `crow_adapter.cpp`.
- Consider centralising bridge error macros shared between `bridge.cpp` and `bridge_c.cpp`.

## Server, middleware and client

The API stack is built around `SEPApiServer` (declared in `server.h`).
It configures Crow middleware for authentication and rate limiting,
then delegates all heavy lifting to `SepEngine`.

Middleware components (`auth_middleware.cpp`,
`rate_limit_middleware.cpp` and `lock_free_rate_limiter.cpp`)
only depend on the lightweight `crow_request.h` interface and the
`rate_limiter.h` abstractions. The server uses `client.h` to talk to
external services such as Ollama via `curl_http_client.cpp`.

The removal of `connection_manager.cpp` leaves `connection_manager.h`
as a pure interface used only in tests. No production code references
it, so cross‑module coupling is minimal.



