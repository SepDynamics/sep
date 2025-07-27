# SEP Engine Data Pipeline

```mermaid
graph TD
    MD[Market Data Sources] --> OC[OandaConnector]
    OC --> DP[DataParser]
    DP --> PME[PatternMetricEngine]
    PME --> MC[Metrics Computation]
    MC --> SG[Signal Generation]
    SG --> GUI[Workbench GUI]
```

## Testbed Promotion Workflow

Experimental components start in `_sep/testbed/`. Once validated by tests and
benchmarks, they are moved to `src/` for full integration with the main engine.
