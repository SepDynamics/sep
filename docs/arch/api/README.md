# `src/api`

This folder implements the public API layer for the SEP Engine. It exposes HTTP endpoints through the Crow framework and bridges external requests to the engine internals. Key pieces include authentication middleware, rate-limiting utilities, an HTTP client wrapper, and a thin C API (`bridge_c.cpp`) used by other languages. The `sep_engine.h` header defines the main interface that orchestrates initialization, pattern processing, and health checks. Overall this directory encapsulates all server-side glue for interacting with the engine.
