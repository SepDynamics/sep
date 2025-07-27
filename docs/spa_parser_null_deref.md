# PipeWire spa_pod_parser Null Dereference Review

Static analysis flagged a potential null pointer dereference in
`/usr/include/spa-0.2/spa/pod/parser.h`. The function
`spa_pod_parser_deref` may return `NULL` when presented with an
invalid offset. Subsequent helpers such as `spa_pod_parser_get_*`
check for `NULL` before dereferencing the returned pointer.

SEP includes this header via `src/audio/spa_includes.h`, which wraps
the include and documents the safety of these helpers. Because the
library performs its own null checks and SEP does not bypass those
helpers, the reported issue does not surface in SEP's code paths.

No changes are required in the project at this time.
