# Configuration Options

The engine reads settings from `config/default.json`. New sections allow tuning
memory promotion thresholds and quantum processing limits.

## memory
- `promote_stm_to_mtm`: coherence threshold for promoting a block from STM to MTM.
- `promote_mtm_to_ltm`: coherence threshold for promoting MTM blocks to LTM.
- `demote_threshold`: relationships below this value are pruned.
- `fragmentation_threshold`: fragmentation level that triggers defragmentation.
- `stm_to_mtm_min_gen`: minimum generations before promotion from STM.
- `mtm_to_ltm_min_gen`: minimum generations before promotion from MTM.

## quantum
- `ltm_coherence_threshold`: coherence needed for patterns to reach LTM.
- `mtm_coherence_threshold`: coherence needed for MTM.
- `stability_threshold`: minimum stability for promotion.
