# Configuration Reference

The engine loads settings from `config/default.json` and overrides
values via environment variables or command line flags. Two new
sections expose memory manager and quantum processor thresholds.

## Memory

```json
"memory": {
    "promote_stm_to_mtm": 0.7,
    "promote_mtm_to_ltm": 0.9,
    "demote_threshold": 0.3,
    "fragmentation_threshold": 0.3,
    "stm_to_mtm_min_gen": 5,
    "mtm_to_ltm_min_gen": 100
}
```

These values control promotion and demotion between STM, MTM and LTM
memory tiers.

## Processor

```json
"processor": {
    "ltm_coherence_threshold": 0.9,
    "mtm_coherence_threshold": 0.6,
    "stability_threshold": 0.8
}
```

The quantum processor uses these thresholds when evolving patterns.
