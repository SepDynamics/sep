Found no defects in auth_middleware.cpp
Found no defects in crow_error.cpp
Found no defects in pipeline.cpp
Found no defects in js_integration.cpp
Found no defects in compression_utils.cpp
Found no defects in compression.cpp
Found no defects in curl_http_client.cpp
Found no defects in gpu_context.cpp
Found no defects in client.cpp
Found no defects in sep_engine.cpp
Found no defects in bridge.cpp
Found no defects in lock_free_rate_limiter.cpp
Found no defects in api.cpp
Found no defects in blender_integration.cpp
Found no defects in ollama_client.cpp
[LOW] /sep/src/api/bridge_c.cpp:65:7: Value stored to 'processor' is never read [deadcode.DeadStores]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d25b69bf56c879240a29cdbed26a6467
  Steps:
    1, bridge_c.cpp:65:7: Value stored to 'processor' is never read

Found 1 defect(s) in bridge_c.cpp

Found no defects in dag_graph.cpp
Found no defects in rate_limit_middleware.cpp
Found no defects in prometheus_exporter.cpp
Found no defects in tracing.cpp
Found no defects in error_handler.cpp
Found no defects in cycles_renderer.cpp
Found no defects in pattern_visualization_pipeline.cpp
Found no defects in mesh_handler.cpp
Found no defects in stream.cpp
Found no defects in qbsa.cpp
Found no defects in allocation_metrics.cpp
Found no defects in metrics_collector.cpp
Found no defects in raii.cpp
Found no defects in qbsa_qfh.cpp
Found no defects in qfh.cpp
[HIGH] /sep/extern/pipewire/spa/include/spa/pod/parser.h:496:8: Access to field 'type' results in a dereference of a null pointer (loaded from variable 'pod') [core.NullDereference]
      if (pod->type == SPA_TYPE_Choice && *format != 'V')
          ^
  Report hash: 6396cb440c40f5db85f1a9ee33c1714c
  Macro expansions:
    1, parser.h:454:1: Macro 'SPA_API_POD_PARSER' expanded to 'static inline '
  Steps:
     1, parser.h:513:8: Calling 'spa_pod_parser_getv'
     2, parser.h:454:1: Entered call from 'spa_pod_parser_get'
     3, parser.h:457:26: Assuming 'f' is non-null
     4, parser.h:466:12: Assuming 'ftype' is equal to SPA_TYPE_Object
     5, parser.h:470:8: Assuming 'key' is not equal to 0
     6, parser.h:475:10: Assuming 'prop' is null
     7, parser.h:475:4: Null pointer value stored to 'pod'
     8, parser.h:478:7: Assuming the condition is false
     9, parser.h:484:20: Assuming the condition is false
    10, parser.h:484:8: Assuming 'optional' is false
    11, parser.h:487:7: Assuming the condition is false
    12, parser.h:496:8: Access to field 'type' results in a dereference of a null pointer (loaded from variable 'pod')

Found 1 defect(s) in parser.h

Found no defects in component_bridge.cpp
Found no defects in quantum_processor.cpp
Found no defects in quantum_processor_qfh.cpp
Found no defects in main.cpp
Found no defects in quantum_processor_qfh_common.cpp
[HIGH] /usr/lib/clang/20/include/cetintrin.h:49:10: 1st function call argument is an uninitialized value [core.CallAndMessage]
  return __builtin_ia32_rdsspd(t);
         ^
  Report hash: a95dbc87e7d4eb8c0bbf73c1ce08c6bb
  Steps:
    1, cetintrin.h:48:3: 't' declared without an initial value
    2, cetintrin.h:49:10: 1st function call argument is an uninitialized value

[HIGH] /usr/lib/clang/20/include/cetintrin.h:62:10: 1st function call argument is an uninitialized value [core.CallAndMessage]
  return __builtin_ia32_rdsspq(t);
         ^
  Report hash: 076fdab99b1790475cd59e47054be43f
  Steps:
    1, cetintrin.h:61:3: 't' declared without an initial value
    2, cetintrin.h:62:10: 1st function call argument is an uninitialized value

Found 2 defect(s) in cetintrin.h

Found no defects in manager.cpp
Found no defects in evolution.cpp
Found no defects in processor.cpp
Found no defects in pattern_processor.cpp
Found no defects in types_serialization.cpp
[LOW] /sep/src/memory/quantum_coherence_manager.cpp:120:14: Value stored to 'tier_analysis' during its initialization is never read [deadcode.DeadStores]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bd401bfb4e259755fb945e81b4f274cc
  Steps:
    1, quantum_coherence_manager.cpp:120:14: Value stored to 'tier_analysis' during its initialization is never read

Found 1 defect(s) in quantum_coherence_manager.cpp

[MEDIUM] /sep/include/crow/crow_isolation.h:53:63: unused parameter 'key' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 62f61d32c9a1eb93849c6508c799e1f7
  Steps:
    1, crow_isolation.h:53:63: unused parameter 'key'

[MEDIUM] /sep/include/crow/crow_isolation.h:71:50: unused parameter 'key' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2dcbcb3089937a19ab4b9ac68a1a1ed0
  Steps:
    1, crow_isolation.h:71:50: unused parameter 'key'

[MEDIUM] /sep/include/crow/crow_isolation.h:71:80: unused parameter 'value' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c932c497b7c88c3fc859ac59506ed06f
  Steps:
    1, crow_isolation.h:71:80: unused parameter 'value'

[MEDIUM] /sep/include/crow/crow_isolation.h:72:50: unused parameter 'key' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 44d5d1ebd334029d89397f3b39f13a59
  Steps:
    1, crow_isolation.h:72:50: unused parameter 'key'

[MEDIUM] /sep/include/crow/crow_isolation.h:72:80: unused parameter 'value' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 55b258b10389c0d1aefa22553669b74f
  Steps:
    1, crow_isolation.h:72:80: unused parameter 'value'

[MEDIUM] /sep/include/crow/crow_isolation.h:89:53: unused parameter 'text' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5bea7f740f0657009ee1f1afd08ac61e
  Steps:
    1, crow_isolation.h:89:53: unused parameter 'text'

[MEDIUM] /sep/include/crow/crow_isolation.h:90:55: unused parameter 'data' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 58e0946831766745a3fc9d4cd5ebcb3c
  Steps:
    1, crow_isolation.h:90:55: unused parameter 'data'

[MEDIUM] /sep/include/crow/crow_isolation.h:91:49: unused parameter 'msg' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 106bc8660edac42a0fc1f430fe62e2f3
  Steps:
    1, crow_isolation.h:91:49: unused parameter 'msg'

[MEDIUM] /sep/include/crow/crow_isolation.h:116:36: unused parameter 'f' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1e21f1c075e5210a149252ece37c5f4f
  Steps:
    1, crow_isolation.h:116:36: unused parameter 'f'

[MEDIUM] /sep/include/crow/crow_isolation.h:131:28: unused parameter 'p' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b934b84fca2436fdef70c3516da41cb7
  Steps:
    1, crow_isolation.h:131:28: unused parameter 'p'

[MEDIUM] /sep/include/crow/crow_isolation.h:132:48: unused parameter 'addr' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 250fcf67dede6ba4be840f4641fa6e3e
  Steps:
    1, crow_isolation.h:132:48: unused parameter 'addr'

[MEDIUM] /sep/include/crow/crow_isolation.h:172:44: unused parameter 'req' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 331d75dec6c0f922510123a10b531126
  Steps:
    1, crow_isolation.h:172:44: unused parameter 'req'

[MEDIUM] /sep/include/crow/crow_isolation.h:173:39: unused parameter 'res' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c7fc29ac6703380472bc6adf9c1fed11
  Steps:
    1, crow_isolation.h:173:39: unused parameter 'res'

[MEDIUM] /sep/include/crow/crow_isolation.h:174:39: unused parameter 'adaptor' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1190635c4178fb8d0509a679385d250d
  Steps:
    1, crow_isolation.h:174:39: unused parameter 'adaptor'

[MEDIUM] /sep/include/crow/crow_isolation.h:191:44: unused parameter 'req' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 331d75dec6c0f922510123a10b531126
  Steps:
    1, crow_isolation.h:191:44: unused parameter 'req'

[MEDIUM] /sep/include/crow/crow_isolation.h:192:39: unused parameter 'res' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c7fc29ac6703380472bc6adf9c1fed11
  Steps:
    1, crow_isolation.h:192:39: unused parameter 'res'

[MEDIUM] /sep/include/crow/crow_isolation.h:193:39: unused parameter 'adaptor' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1190635c4178fb8d0509a679385d250d
  Steps:
    1, crow_isolation.h:193:39: unused parameter 'adaptor'

Found 17 defect(s) in crow_isolation.h

Found no defects in memory_tier_manager.cpp
Found no defects in quantum_manifold_optimizer.cpp
[MEDIUM] /sep/include/api/server.h:25:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7efebc8bbaef775aeab2e28a25dc4a5c
  Steps:
    1, crow_isolation.h:47:11: previous use is here
    2, server.h:25:1: did you mean class here?
    3, server.h:25:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/server.h:26:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 59bfeb4242018ecdefde0c5a8f41634e
  Steps:
    1, crow_isolation.h:62:11: previous use is here
    2, server.h:26:1: did you mean class here?
    3, server.h:26:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in server.h

[MEDIUM] /sep/include/api/types.h:50:54: unused parameter 'name' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4d36d5ee5324c8a8ec1018571fda19eb
  Steps:
    1, types.h:50:54: unused parameter 'name'

Found 1 defect(s) in types.h

Found no defects in pattern_evolution_bridge.cpp
[LOW] /sep/include/api/bridge_internal.hpp:12:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5ca7fa69e4196bf69f99f847456b1c42
  Steps:
    1, processor.h:80:7: a definition of 'Processor' is found here
    2, bridge_internal.hpp:12:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in bridge_internal.hpp

[MEDIUM] /sep/src/api/client.cpp:143:46: unused parameter 'request' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 91d07ab03d93db50c8196e95accb3896
  Steps:
    1, client.cpp:143:46: unused parameter 'request'

Found 1 defect(s) in client.cpp

[MEDIUM] /sep/include/crow/asio_isolation.h:52:31: all paths through this function will call itself [clang-diagnostic-infinite-recursion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3224361f449a83099b40b66d08419d91
  Steps:
    1, asio_isolation.h:52:31: all paths through this function will call itself

[LOW] /sep/include/crow/asio_isolation.h:64:9: constructor accepting a forwarding reference can hide the copy and move constructors [bugprone-forwarding-reference-overload]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cbbc29282533261ab411323c6ecc789c
  Steps:
    1, asio_isolation.h:64:9: constructor accepting a forwarding reference can hide the copy and move constructors

[MEDIUM] /sep/include/crow/asio_isolation.h:89:59: unused parameter 'ctx' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f3c5052afd97cf4aa573322cb2c80d1a
  Steps:
    1, asio_isolation.h:89:59: unused parameter 'ctx'

[MEDIUM] /sep/include/crow/asio_isolation.h:128:36: unused parameter 'io' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e7dd341eb5bc0746a04737188f427783
  Steps:
    1, asio_isolation.h:128:36: unused parameter 'io'

[MEDIUM] /sep/include/crow/asio_isolation.h:131:57: unused parameter 'ex' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 56b0e986888e6dd3e14c781644929cd4
  Steps:
    1, asio_isolation.h:131:57: unused parameter 'ex'

[MEDIUM] /sep/include/crow/asio_isolation.h:139:58: unused parameter 'type' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 62a23a5fbb5f9103df06be212fb003cb
  Steps:
    1, asio_isolation.h:139:58: unused parameter 'type'

Found 6 defect(s) in asio_isolation.h

[MEDIUM] /sep/include/crow/logging.h:21:50: unused parameter 'message' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d9252f04e6e54412898c044a50e0d541
  Steps:
    1, logging.h:21:50: unused parameter 'message'

[MEDIUM] /sep/include/crow/logging.h:32:37: unused parameter 'value' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4dc92635fe2d82eaccd2033278129f52
  Steps:
    1, logging.h:32:37: unused parameter 'value'

[MEDIUM] /sep/include/crow/logging.h:44:18: private field 'level_' is not used [clang-diagnostic-unused-private-field]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 94dcbf70094f439983d3e049a3f4f5d8
  Steps:
    1, logging.h:44:18: private field 'level_' is not used

[MEDIUM] /sep/include/crow/logging.h:47:20: private field 'handler_' is not used [clang-diagnostic-unused-private-field]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c22310aeee52e4c4a190a94cfd9dbcda
  Steps:
    1, logging.h:47:20: private field 'handler_' is not used

Found 4 defect(s) in logging.h

[MEDIUM] /sep/include/crow/socket_adaptors.h:29:46: unused parameter 'io_context' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 232f9a539f70f2991d47b559d931a0f2
  Steps:
    1, socket_adaptors.h:29:46: unused parameter 'io_context'

Found 1 defect(s) in socket_adaptors.h

[MEDIUM] /sep/src/api/bridge_c.cpp:58:30: variable 'processor' set but not used [clang-diagnostic-unused-but-set-variable]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1d153ece2bc7daa0aa7f8e48fd532480
  Steps:
    1, bridge_c.cpp:58:30: variable 'processor' set but not used

Found 1 defect(s) in bridge_c.cpp

[MEDIUM] /sep/include/memory/memory_tier.hpp:61:41: field 'tier' will be initialized after field 'original_size' [clang-diagnostic-reorder-ctor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5c71e33c2548a8c4133274e264137cc8
  Notes:
    1, memory_tier.hpp:61:41: original_size(s) tier(t) (fixit)
  Steps:
    1, memory_tier.hpp:61:41: field 'tier' will be initialized after field 'original_size'

Found 1 defect(s) in memory_tier.hpp

Found no defects in crow_adapter.cpp
[LOW] /sep/include/api/sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7ddbaf54ea1b7346b97eb4b637f989f0
  Steps:
    1, processor.h:80:7: a definition of 'Processor' is found here
    2, sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in sep_engine.h

[MEDIUM] /sep/src/api/sep_engine.cpp:309:67: unused parameter 'request_data' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d06191f01aa657f541e0410cc16c881a
  Steps:
    1, sep_engine.cpp:309:67: unused parameter 'request_data'

Found 1 defect(s) in sep_engine.cpp

[MEDIUM] /sep/include/blender/gpu_context.h:23:32: unused parameter 'device_index' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 23c51f2f8d9ec7df2beafa42afa62014
  Steps:
    1, gpu_context.h:23:32: unused parameter 'device_index'

[MEDIUM] /sep/include/blender/gpu_context.h:26:60: unused parameter 'path' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 68077b0bb23b247f4345203fbdaffb8d
  Steps:
    1, gpu_context.h:26:60: unused parameter 'path'

[MEDIUM] /sep/include/blender/gpu_context.h:35:42: unused parameter 'buffer' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 966ce6630cd2b7c68963f475b1b4bcf0
  Steps:
    1, gpu_context.h:35:42: unused parameter 'buffer'

[MEDIUM] /sep/include/blender/gpu_context.h:36:40: unused parameter 'buffer' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b9dbd47ac557d5a1b758c3f054a1b8f4
  Steps:
    1, gpu_context.h:36:40: unused parameter 'buffer'

[MEDIUM] /sep/include/blender/gpu_context.h:37:41: unused parameter 'buffer' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9fbb8fa205d3a852dec7cb5e37429a91
  Steps:
    1, gpu_context.h:37:41: unused parameter 'buffer'

Found 5 defect(s) in gpu_context.h

[MEDIUM] /sep/src/audio/config.cpp:50:31: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4e63877f53b67c4fbc4445ca607d7ea4
  Steps:
    1, config.cpp:50:31: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:50:39: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6512e0d023076ff627be170438437f62
  Steps:
    1, config.cpp:50:39: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:50:66: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c6d2f61264b32d1de6f0cf1588bf3524
  Steps:
    1, config.cpp:50:66: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:50:113: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e4cd4b34889d40705121d381bb2d262d
  Steps:
    1, config.cpp:50:113: implicit conversion increases floating-point precision: 'float' to 'double'

Found 4 defect(s) in config.cpp

[MEDIUM] /sep/extern/pipewire/spa/include/spa/buffer/buffer.h:48:2: identifier '_SPA_DATA_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_DATA_LAST,      /**< not part of ABI */
  ^
  Report hash: 03818b597bd5e4204b621d81e5b69bb4
  Steps:
    1, buffer.h:48:2: identifier '_SPA_DATA_LAST' is reserved because it starts with '_' followed by a capital letter

Found 1 defect(s) in buffer.h

[MEDIUM] /sep/extern/pipewire/spa/include/spa/buffer/meta.h:41:2: identifier '_SPA_META_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_META_LAST,      /**< not part of ABI/API */
  ^
  Report hash: fb9bba17242cbd932ec11f23751b377d
  Steps:
    1, meta.h:41:2: identifier '_SPA_META_LAST' is reserved because it starts with '_' followed by a capital letter

Found 1 defect(s) in meta.h

[LOW] /sep/extern/pipewire/spa/include/spa/param/audio/format-utils.h:60:2: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
  switch (info->media_subtype) {
  ^
  Report hash: ffd5b788737352c33aa8985ddfe3e707
  Steps:
    1, format-utils.h:60:2: switching on non-enum value without default case may not cover all cases

[LOW] /sep/extern/pipewire/spa/include/spa/param/audio/format-utils.h:105:2: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
  switch (info->media_subtype) {
  ^
  Report hash: ffd5b788737352c33aa8985ddfe3e707
  Steps:
    1, format-utils.h:105:2: switching on non-enum value without default case may not cover all cases

Found 2 defect(s) in format-utils.h

[LOW] /sep/extern/pipewire/spa/include/spa/param/buffers.h:8:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: 10f6ec61bd6df1a119cf9eea7e68c8d3
  Steps:
    1, param.h:76:10: 'buffers.h' included from here
    2, format.h:8:10: 'param.h' included from here
    3, buffers.h:8:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in buffers.h

[LOW] /sep/extern/pipewire/spa/include/spa/param/port-config.h:8:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: bbf25c04cf6ba9c76091dc26a9c4907c
  Steps:
    1, param.h:78:10: 'port-config.h' included from here
    2, format.h:8:10: 'param.h' included from here
    3, port-config.h:8:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in port-config.h

[LOW] /sep/extern/pipewire/spa/include/spa/param/profile.h:8:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: d543dd0e6a4219f2f281b6f8d2ec2327
  Steps:
    1, param.h:77:10: 'profile.h' included from here
    2, format.h:8:10: 'param.h' included from here
    3, profile.h:8:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in profile.h

[LOW] /sep/extern/pipewire/spa/include/spa/param/route.h:8:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: de3e7bf38f149ce3fcc8ff8ce5654d20
  Steps:
    1, param.h:79:10: 'route.h' included from here
    2, format.h:8:10: 'param.h' included from here
    3, route.h:8:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in route.h

[HIGH] /sep/extern/pipewire/spa/include/spa/pod/builder.h:213:3: suspicious usage of 'sizeof(K)'; did you mean 'K'? [bugprone-sizeof-expression]
    SPA_FLAG_CLEAR(builder->state.flags, SPA_POD_BUILDER_FLAG_FIRST);
    ^
  Report hash: eef99322fff6dd6b95bd63cedded29aa
  Steps:
    1, defs.h:86:26: expanded from macro 'SPA_FLAG_CLEAR'
    2, builder.h:213:3: suspicious usage of 'sizeof(K)'; did you mean 'K'?

[HIGH] /sep/extern/pipewire/spa/include/spa/pod/builder.h:235:2: suspicious usage of 'sizeof(K)'; did you mean 'K'? [bugprone-sizeof-expression]
  SPA_FLAG_CLEAR(builder->state.flags, SPA_POD_BUILDER_FLAG_FIRST);
  ^
  Report hash: 8663eacab3b0ba74521abfbc64e40a60
  Steps:
    1, defs.h:86:26: expanded from macro 'SPA_FLAG_CLEAR'
    2, builder.h:235:2: suspicious usage of 'sizeof(K)'; did you mean 'K'?

[LOW] /sep/extern/pipewire/spa/include/spa/pod/builder.h:661:4: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
      SPA_POD_BUILDER_COLLECT(builder, *format, args);
      ^
  Report hash: 3f893bfb6eace68e3d86b6cf62666a25
  Steps:
    1, builder.h:520:2: expanded from macro 'SPA_POD_BUILDER_COLLECT'
    2, builder.h:661:4: switching on non-enum value without default case may not cover all cases

Found 3 defect(s) in builder.h

[MEDIUM] /sep/extern/pipewire/spa/include/spa/pod/iter.h:314:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_return_val_if_fail(spa_pod_is_fraction(pod), -EINVAL);
  ^
  Report hash: 9600fcd5fbb15cb4c336c03fb2e7cc60
  Steps:
    1, defs.h:381:4: expanded from macro 'spa_return_val_if_fail'
    2, iter.h:314:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/pipewire/spa/include/spa/pod/iter.h:333:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_return_val_if_fail(spa_pod_is_array(pod), NULL);
  ^
  Report hash: 329b2d035b569160577af71457d8348d
  Steps:
    1, defs.h:381:4: expanded from macro 'spa_return_val_if_fail'
    2, iter.h:333:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 2 defect(s) in iter.h

[LOW] /sep/extern/pipewire/spa/include/spa/pod/parser.h:494:4: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
      SPA_POD_PARSER_SKIP(*format, args);
      ^
  Report hash: 936209cc6ea38b2b5f94af2eb67b6ece
  Steps:
    1, parser.h:421:2: expanded from macro 'SPA_POD_PARSER_SKIP'
    2, parser.h:494:4: switching on non-enum value without default case may not cover all cases

Found 1 defect(s) in parser.h

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/cleanup.h:83:20: identifier '_spa_autofree_cleanup_func' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
static inline void _spa_autofree_cleanup_func(void *p)
                   ^
  Report hash: 4e9214c45124f4846bac779aaf0f11b8
  Steps:
    1, cleanup.h:83:20: identifier '_spa_autofree_cleanup_func' is reserved because it starts with '_' at global scope

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/cleanup.h:93:20: identifier '_spa_autoclose_cleanup_func' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
static inline void _spa_autoclose_cleanup_func(int *fd)
                   ^
  Report hash: 062d1e75a6f46d35b6f1ed776d69a0bf
  Steps:
    1, cleanup.h:93:20: identifier '_spa_autoclose_cleanup_func' is reserved because it starts with '_' at global scope

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/cleanup.h:105:1: identifier '_spa_autoptr_cleanup_func_FILE' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(FILE, FILE, {
^
  Report hash: 8c90e8714ce36cbfc016a34d6aafe59b
  Steps:
    1, cleanup.h:68:20: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:105:1: identifier '_spa_autoptr_cleanup_func_FILE' is reserved because it starts with '_' at global scope

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/cleanup.h:105:1: identifier '_spa_autoptr_cleanup_type_FILE' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(FILE, FILE, {
^
  Report hash: 1948d70ed31817f9218c89cacf394cc2
  Steps:
    1, cleanup.h:67:28: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:105:1: identifier '_spa_autoptr_cleanup_type_FILE' is reserved because it starts with '_' at global scope

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/cleanup.h:106:24: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_clear_ptr(*thing, fclose);
                        ^
  Report hash: 6dc3766dd4a30ea03f6b48247051ef9d
  Steps:
    1, cleanup.h:28:3: expanded from macro 'spa_clear_ptr'
    2, cleanup.h:71:2: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    3, cleanup.h:106:24: cast the expression to void to silence this warning
    4, cleanup.h:106:24: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/cleanup.h:113:1: identifier '_spa_autoptr_cleanup_func_DIR' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(DIR, DIR, {
^
  Report hash: e974bef354d1e377f8e0bba41689668c
  Steps:
    1, cleanup.h:68:20: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:113:1: identifier '_spa_autoptr_cleanup_func_DIR' is reserved because it starts with '_' at global scope

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/cleanup.h:113:1: identifier '_spa_autoptr_cleanup_type_DIR' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(DIR, DIR, {
^
  Report hash: 250ac9d4570be92c586f490e4104b572
  Steps:
    1, cleanup.h:67:28: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:113:1: identifier '_spa_autoptr_cleanup_type_DIR' is reserved because it starts with '_' at global scope

Found 7 defect(s) in cleanup.h

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:84:3: identifier '__NONE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
    ^
  Report hash: e169b04a5693dda3263a71a92be29757
  Steps:
    1, json-core.h:84:3: identifier '__NONE' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:84:11: identifier '__STRUCT' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
            ^
  Report hash: a686f6b9af1c086abced146c0c6f2435
  Steps:
    1, json-core.h:84:11: identifier '__STRUCT' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:84:21: identifier '__BARE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                      ^
  Report hash: c36fb0b6ba22ab12a23f29403fa42dc0
  Steps:
    1, json-core.h:84:21: identifier '__BARE' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:84:29: identifier '__STRING' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                              ^
  Report hash: a32457a4f1b73b65aac2a15b621291f6
  Steps:
    1, json-core.h:84:29: identifier '__STRING' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:84:39: identifier '__UTF8' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                                        ^
  Report hash: ff655471788b4ed203ea6836ce8d6ffa
  Steps:
    1, json-core.h:84:39: identifier '__UTF8' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:84:47: identifier '__ESC' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                                                ^
  Report hash: 222e64a766587807525f5bfdb554e6c4
  Steps:
    1, json-core.h:84:47: identifier '__ESC' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:84:54: identifier '__COMMENT' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                                                       ^
  Report hash: 5034e9e0efdb387c2993b905a5c6d2ee
  Steps:
    1, json-core.h:84:54: identifier '__COMMENT' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:85:3: identifier '__ARRAY_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ARRAY_FLAG = 0x10,    /* in array context */
    ^
  Report hash: df193b651db2bc004cfce54a372ee4bf
  Steps:
    1, json-core.h:85:3: identifier '__ARRAY_FLAG' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:86:3: identifier '__PREV_ARRAY_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __PREV_ARRAY_FLAG = 0x20,  /* depth=0 array context flag */
    ^
  Report hash: ea668d02342d7cdd6119ca7e34f58ee0
  Steps:
    1, json-core.h:86:3: identifier '__PREV_ARRAY_FLAG' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:87:3: identifier '__KEY_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __KEY_FLAG = 0x40,    /* inside object key */
    ^
  Report hash: 93a8368e3478ff6d782b10059d0bf152
  Steps:
    1, json-core.h:87:3: identifier '__KEY_FLAG' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:88:3: identifier '__SUB_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __SUB_FLAG = 0x80,    /* not at top-level */
    ^
  Report hash: f83d92cb8f26170b1b651b4cc5257349
  Steps:
    1, json-core.h:88:3: identifier '__SUB_FLAG' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:89:3: identifier '__FLAGS' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __FLAGS = 0xff0,
    ^
  Report hash: 8a480e1edd6a9ea99102e0062640b75d
  Steps:
    1, json-core.h:89:3: identifier '__FLAGS' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:90:3: identifier '__ERROR_SYSTEM' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_SYSTEM = SPA_JSON_ERROR_FLAG,
    ^
  Report hash: 63fc14343b21c8b5954c7a6f23d09d13
  Steps:
    1, json-core.h:90:3: identifier '__ERROR_SYSTEM' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:91:3: identifier '__ERROR_INVALID_ARRAY_SEPARATOR' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_INVALID_ARRAY_SEPARATOR,
    ^
  Report hash: 90e5106128638f8e72fcf1dd0ff5d7eb
  Steps:
    1, json-core.h:91:3: identifier '__ERROR_INVALID_ARRAY_SEPARATOR' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:92:3: identifier '__ERROR_EXPECTED_OBJECT_KEY' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_OBJECT_KEY,
    ^
  Report hash: 0eeebef6a1a296298259434bd9c83b80
  Steps:
    1, json-core.h:92:3: identifier '__ERROR_EXPECTED_OBJECT_KEY' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:93:3: identifier '__ERROR_EXPECTED_OBJECT_VALUE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_OBJECT_VALUE,
    ^
  Report hash: 3701429a89c1e0ae596ec4920c380361
  Steps:
    1, json-core.h:93:3: identifier '__ERROR_EXPECTED_OBJECT_VALUE' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:94:3: identifier '__ERROR_TOO_DEEP_NESTING' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_TOO_DEEP_NESTING,
    ^
  Report hash: edd9a0657db2889b4a6fa1d8926be5f2
  Steps:
    1, json-core.h:94:3: identifier '__ERROR_TOO_DEEP_NESTING' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:95:3: identifier '__ERROR_EXPECTED_ARRAY_CLOSE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_ARRAY_CLOSE,
    ^
  Report hash: 86c42b9d2c3d9b54aab616415c4389e5
  Steps:
    1, json-core.h:95:3: identifier '__ERROR_EXPECTED_ARRAY_CLOSE' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:96:3: identifier '__ERROR_EXPECTED_OBJECT_CLOSE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_OBJECT_CLOSE,
    ^
  Report hash: a20150bb32b72233751ff8c5e718fc0c
  Steps:
    1, json-core.h:96:3: identifier '__ERROR_EXPECTED_OBJECT_CLOSE' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:97:3: identifier '__ERROR_MISMATCHED_BRACKET' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_MISMATCHED_BRACKET,
    ^
  Report hash: 18220c4cdc2c9b88054ae2dc3acfeab3
  Steps:
    1, json-core.h:97:3: identifier '__ERROR_MISMATCHED_BRACKET' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:98:3: identifier '__ERROR_ESCAPE_NOT_ALLOWED' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_ESCAPE_NOT_ALLOWED,
    ^
  Report hash: ed3b1e7cb46d2343adab35416dba18e7
  Steps:
    1, json-core.h:98:3: identifier '__ERROR_ESCAPE_NOT_ALLOWED' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:99:3: identifier '__ERROR_CHARACTERS_NOT_ALLOWED' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_CHARACTERS_NOT_ALLOWED,
    ^
  Report hash: 3011991984ae22eb59322ec4b2c64dd7
  Steps:
    1, json-core.h:99:3: identifier '__ERROR_CHARACTERS_NOT_ALLOWED' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:100:3: identifier '__ERROR_INVALID_ESCAPE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_INVALID_ESCAPE,
    ^
  Report hash: b2d68b5a712901be597bf7b2dadf1427
  Steps:
    1, json-core.h:100:3: identifier '__ERROR_INVALID_ESCAPE' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:101:3: identifier '__ERROR_INVALID_STATE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_INVALID_STATE,
    ^
  Report hash: 34dc1dc7c59fe0e25e45f1c9f7faf58d
  Steps:
    1, json-core.h:101:3: identifier '__ERROR_INVALID_STATE' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:102:3: identifier '__ERROR_UNFINISHED_STRING' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_UNFINISHED_STRING,
    ^
  Report hash: e36b85e610b8c7631cc4c58cc016c648
  Steps:
    1, json-core.h:102:3: identifier '__ERROR_UNFINISHED_STRING' is reserved because it starts with '__'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:115:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define _SPA_ERROR(reason)  { err = __ERROR_ ## reason; goto error; }
        ^
  Report hash: 901bfff7582981f1d0da07bbef233ed0
  Steps:
    1, json-core.h:115:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:324:8: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#undef _SPA_ERROR
       ^
  Report hash: 0bb09aa6989530fb260dc1931e26bbb9
  Steps:
    1, json-core.h:324:8: macro name is a reserved identifier

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:450:29: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
  return spa_dtoa(str, size, val);
                             ^
  Report hash: 57f4ffa6f2535055a551be4e2940a10c
  Steps:
    1, json-core.h:450:29: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:596:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __PUT(c) { if (len < size) *str++ = c; len++; }
        ^
  Report hash: 34755b98cae0df37310c8f6bcf293d28
  Steps:
    1, json-core.h:596:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/json-core.h:633:8: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#undef __PUT
       ^
  Report hash: 42b5c89c010f749d85d0fc0838fd4497
  Steps:
    1, json-core.h:633:8: macro name is a reserved identifier

Found 30 defect(s) in json-core.h

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/string.h:70:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_assert_se(prefix);
  ^
  Report hash: 5019cf126b16580a5adaf138b242121b
  Steps:
    1, defs.h:393:4: expanded from macro 'spa_assert_se'
    2, string.h:70:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/string.h:88:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_assert_se(suffix);
  ^
  Report hash: 65f20f678659dd9b1b5a8318724c88f0
  Steps:
    1, defs.h:393:4: expanded from macro 'spa_assert_se'
    2, string.h:88:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/string.h:225:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_assert_se((ssize_t)size > 0);
  ^
  Report hash: d5a84d0cea4cae9623806e567493b935
  Steps:
    1, defs.h:393:4: expanded from macro 'spa_assert_se'
    2, string.h:225:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 3 defect(s) in string.h

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/type.h:55:2: identifier '_SPA_TYPE_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_LAST,        /**< not part of ABI */
  ^
  Report hash: bdb7800d6f84cfde632ca52fb2f44eab
  Steps:
    1, type.h:55:2: identifier '_SPA_TYPE_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/type.h:62:2: identifier '_SPA_TYPE_POINTER_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_POINTER_LAST,      /**< not part of ABI */
  ^
  Report hash: 1e32f82091e76d3c89ebf1ef8d990e74
  Steps:
    1, type.h:62:2: identifier '_SPA_TYPE_POINTER_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/type.h:68:2: identifier '_SPA_TYPE_EVENT_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_EVENT_LAST,      /**< not part of ABI */
  ^
  Report hash: 90102cb980d9dec27ffe550717137a79
  Steps:
    1, type.h:68:2: identifier '_SPA_TYPE_EVENT_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/type.h:74:2: identifier '_SPA_TYPE_COMMAND_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_COMMAND_LAST,      /**< not part of ABI */
  ^
  Report hash: 08f54e700f5760bf5f2a4537a0e2a915
  Steps:
    1, type.h:74:2: identifier '_SPA_TYPE_COMMAND_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /sep/extern/pipewire/spa/include/spa/utils/type.h:91:2: identifier '_SPA_TYPE_OBJECT_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_OBJECT_LAST,      /**< not part of ABI */
  ^
  Report hash: a68bc4cbf6a446cdbcaf462db3488a0a
  Steps:
    1, type.h:91:2: identifier '_SPA_TYPE_OBJECT_LAST' is reserved because it starts with '_' followed by a capital letter

Found 5 defect(s) in type.h

[LOW] /sep/extern/pipewire/src/pipewire/control.h:25:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: fd8eadb9bf09754217dc928cc66a2a3f
  Steps:
    1, impl.h:24:10: 'control.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, control.h:25:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in control.h

[LOW] /sep/extern/pipewire/src/pipewire/core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path [misc-header-include-cycle]
#include <pipewire/context.h>
         ^
  Report hash: 1daf003ee682828fd0bcf5e77b8a5137
  Steps:
    1, context.h:48:10: 'core.h' included from here
    2, pipewire_includes.h:21:10: 'context.h' included from here
    3, core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path

[MEDIUM] /sep/extern/pipewire/src/pipewire/core.h:396:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  vsnprintf(buffer, sizeof(buffer), message, args);
  ^
  Report hash: 0ed97ae05ae09f99abd5bfafd2bde595
  Steps:
    1, core.h:396:2: cast the expression to void to silence this warning
    2, core.h:396:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 2 defect(s) in core.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: c36587793019ad5530065d0f43f044ca
  Steps:
    1, resource.h:37:10: 'impl-client.h' included from here
    2, impl-core.h:29:10: 'resource.h' included from here
    3, impl.h:25:10: 'impl-core.h' included from here
    4, global.h:39:10: 'impl.h' included from here
    5, pipewire_includes.h:22:10: 'global.h' included from here
    6, impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path

[LOW] /sep/extern/pipewire/src/pipewire/impl-client.h:68:10: circular header file dependency detected while including 'resource.h', please check the include path [misc-header-include-cycle]
#include <pipewire/resource.h>
         ^
  Report hash: 0dbd6e13aed46b4d8732e99ad49ef4c1
  Steps:
    1, resource.h:37:10: 'impl-client.h' included from here
    2, impl-core.h:29:10: 'resource.h' included from here
    3, impl-client.h:68:10: circular header file dependency detected while including 'resource.h', please check the include path

Found 2 defect(s) in impl-client.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: b4e1d1024c73f5543ef983da74c8a645
  Steps:
    1, impl.h:25:10: 'impl-core.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:22:10: 'global.h' included from here
    4, impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-core.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 98baed36db9cbd1ca48fe18b801bea5f
  Steps:
    1, impl.h:27:10: 'impl-device.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:22:10: 'global.h' included from here
    4, impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-device.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: caa7dc7877996ab29c3b85d029b375c4
  Steps:
    1, impl.h:28:10: 'impl-factory.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:22:10: 'global.h' included from here
    4, impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-factory.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-link.h:24:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: 89e4605111d4c286b81e1b516e7160e7
  Steps:
    1, impl.h:30:10: 'impl-link.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, impl-link.h:24:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in impl-link.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-metadata.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 49976cb41585c3805dce471218afb328
  Steps:
    1, impl.h:31:10: 'impl-metadata.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:22:10: 'global.h' included from here
    4, impl-metadata.h:25:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-metadata.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-node.h:28:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: b556a94963dd35d190a0ddd6ee0964b5
  Steps:
    1, impl.h:33:10: 'impl-node.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, impl-node.h:28:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in impl-node.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-port.h:27:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: 3a0828555eb26fb9f898376dc802be93
  Steps:
    1, impl.h:34:10: 'impl-port.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, impl-port.h:27:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in impl-port.h

[LOW] /sep/extern/pipewire/src/pipewire/impl.h:29:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 97378dfee83bf376433e4226817d4af1
  Steps:
    1, global.h:39:10: 'impl.h' included from here
    2, pipewire_includes.h:22:10: 'global.h' included from here
    3, impl.h:29:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl.h

[MEDIUM] /sep/extern/pipewire/src/pipewire/properties.h:192:1: identifier '_spa_autoptr_cleanup_func_pw_properties' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(pw_properties, struct pw_properties, {
^
  Report hash: 83cc49725b0040adc82f792183801b60
  Steps:
    1, cleanup.h:68:20: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, properties.h:192:1: identifier '_spa_autoptr_cleanup_func_pw_properties' is reserved because it starts with '_' at global scope

[MEDIUM] /sep/extern/pipewire/src/pipewire/properties.h:192:1: identifier '_spa_autoptr_cleanup_type_pw_properties' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(pw_properties, struct pw_properties, {
^
  Report hash: 70f2a3d989b98e27528a2b459ef264c4
  Steps:
    1, cleanup.h:67:28: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, properties.h:192:1: identifier '_spa_autoptr_cleanup_type_pw_properties' is reserved because it starts with '_' at global scope

Found 2 defect(s) in properties.h

[LOW] /sep/extern/pipewire/src/pipewire/protocol.h:26:10: circular header file dependency detected while including 'context.h', please check the include path [misc-header-include-cycle]
#include <pipewire/context.h>
         ^
  Report hash: 8a4ca310782449cb3b78489b5a83a9f9
  Steps:
    1, proxy.h:90:10: 'protocol.h' included from here
    2, core.h:79:10: 'proxy.h' included from here
    3, context.h:48:10: 'core.h' included from here
    4, pipewire_includes.h:21:10: 'context.h' included from here
    5, protocol.h:26:10: circular header file dependency detected while including 'context.h', please check the include path

Found 1 defect(s) in protocol.h

[MEDIUM] /sep/extern/pipewire/src/pipewire/utils.h:103:1: identifier '_spa_auto_cleanup_func_pw_strv' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTO_CLEANUP(pw_strv, char **, {
^
  Report hash: d191108c2492e721db5c198011c6f841
  Steps:
    1, cleanup.h:55:20: expanded from macro 'SPA_DEFINE_AUTO_CLEANUP'
    2, utils.h:103:1: identifier '_spa_auto_cleanup_func_pw_strv' is reserved because it starts with '_' at global scope

[MEDIUM] /sep/extern/pipewire/src/pipewire/utils.h:103:1: identifier '_spa_auto_cleanup_type_pw_strv' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTO_CLEANUP(pw_strv, char **, {
^
  Report hash: 286ad65c4b8b98a07728f808e2c0f0cb
  Steps:
    1, cleanup.h:54:26: expanded from macro 'SPA_DEFINE_AUTO_CLEANUP'
    2, utils.h:103:1: identifier '_spa_auto_cleanup_type_pw_strv' is reserved because it starts with '_' at global scope

Found 2 defect(s) in utils.h

Found no defects in pipewire_capture.cpp
[MEDIUM] /sep/src/api/server.cpp:160:84: unused parameter 'body' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99c1cd2d504e0473209900460e079018
  Steps:
    1, server.cpp:160:84: unused parameter 'body'

[MEDIUM] /sep/src/api/server.cpp:220:50: unused parameter 'response_body' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9a6489114b93b9375b7c0c11dd032dae
  Steps:
    1, server.cpp:220:50: unused parameter 'response_body'

Found 2 defect(s) in server.cpp

[MEDIUM] /sep/src/blender/blender_integration.cpp:32:7: field 'thread_running_' will be initialized after field 'gpu_context_' [clang-diagnostic-reorder-ctor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dfc6c665bd56d3f5e87cab3824f381f6
  Notes:
    1, blender_integration.cpp:32:7: gpu_context_(nullptr) (fixit)
    2, blender_integration.cpp:32:7: gpu_context_(nullptr) (fixit)
    3, blender_integration.cpp:32:7: ~~~~~~~~~~~~~~~~~~~~~ (fixit)
    4, blender_integration.cpp:32:7: thread_running_(false) (fixit)
  Steps:
    1, blender_integration.cpp:32:7: field 'thread_running_' will be initialized after field 'gpu_context_'

Found 1 defect(s) in blender_integration.cpp

[MEDIUM] /sep/src/blender/mesh_handler.cpp:303:21: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 96149c0ed1849daeb9b92afc0dcb3cc9
  Steps:
    1, mesh_handler.cpp:303:21: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/blender/mesh_handler.cpp:322:14: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 31bfb7cbaa7554f38c92dbc1014c813e
  Steps:
    1, mesh_handler.cpp:322:14: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in mesh_handler.cpp

Found no defects in logging.cpp
[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:261:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:261:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:305:26: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 005a58abc587efa85ab38a672b887462
  Steps:
    1, memory_tier_manager.cpp:305:26: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:388:23: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0c34aea053751feecbd4d9b55c48af7c
  Steps:
    1, memory_tier_manager.cpp:388:23: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in memory_tier_manager.cpp

[MEDIUM] /sep/src/core/engine.cpp:237:96: unused parameter 'tick' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0c9da63e951de4ba7edf8df7fb562619
  Steps:
    1, engine.cpp:237:96: unused parameter 'tick'

Found 1 defect(s) in engine.cpp

Found no defects in memory_tier.cpp
[MEDIUM] /sep/include/compat/cufft.h:62:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 778d90af82563e80323fc4c239514480
  Steps:
    1, cufft.h:62:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:62:55: unused parameter 'nx' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b476163126b044d8ba41d4a962581631
  Steps:
    1, cufft.h:62:55: unused parameter 'nx'

[MEDIUM] /sep/include/compat/cufft.h:62:63: unused parameter 'type' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5e3348ee34f25afe8fae762c3b7452a5
  Steps:
    1, cufft.h:62:63: unused parameter 'type'

[MEDIUM] /sep/include/compat/cufft.h:62:73: unused parameter 'batch' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c5274b8d065a571b140d8322f057d2e8
  Steps:
    1, cufft.h:62:73: unused parameter 'batch'

[MEDIUM] /sep/include/compat/cufft.h:66:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 17108eb882f5bb6a8b2b9db88957828a
  Steps:
    1, cufft.h:66:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:66:55: unused parameter 'nx' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fde9f899b55f47cddd45027c5389e57e
  Steps:
    1, cufft.h:66:55: unused parameter 'nx'

[MEDIUM] /sep/include/compat/cufft.h:66:63: unused parameter 'ny' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 718e0340681176b0c8200b76210f28bf
  Steps:
    1, cufft.h:66:63: unused parameter 'ny'

[MEDIUM] /sep/include/compat/cufft.h:66:71: unused parameter 'type' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f7a711971179e7029e13c64a411637a3
  Steps:
    1, cufft.h:66:71: unused parameter 'type'

[MEDIUM] /sep/include/compat/cufft.h:70:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9d6175258154b19c50fb5ac3d7e0dec6
  Steps:
    1, cufft.h:70:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:70:55: unused parameter 'nx' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c2f07ccbdc23d01f675aaaa2d3ed138b
  Steps:
    1, cufft.h:70:55: unused parameter 'nx'

[MEDIUM] /sep/include/compat/cufft.h:70:63: unused parameter 'ny' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: aee850b7a6a03122e896b2e1b3b6e78d
  Steps:
    1, cufft.h:70:63: unused parameter 'ny'

[MEDIUM] /sep/include/compat/cufft.h:70:71: unused parameter 'nz' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 829e464e06cec51c751f22f6dee266d6
  Steps:
    1, cufft.h:70:71: unused parameter 'nz'

[MEDIUM] /sep/include/compat/cufft.h:70:79: unused parameter 'type' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b4cff53f1288526fa7e464e4a64ab0ac
  Steps:
    1, cufft.h:70:79: unused parameter 'type'

[MEDIUM] /sep/include/compat/cufft.h:74:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c1463fb0af25faaa7cd8f8892bfc86c7
  Steps:
    1, cufft.h:74:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:78:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 75220db65e344c8c27a290a16ce2ecbb
  Steps:
    1, cufft.h:78:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:78:57: unused parameter 'idata' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9f8d731b236cc34712d826a684e30ca0
  Steps:
    1, cufft.h:78:57: unused parameter 'idata'

[MEDIUM] /sep/include/compat/cufft.h:78:70: unused parameter 'odata' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a454f14b2fea1b6842416ebad447307e
  Steps:
    1, cufft.h:78:70: unused parameter 'odata'

[MEDIUM] /sep/include/compat/cufft.h:78:81: unused parameter 'direction' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 693efb129496780ca772c4b327572393
  Steps:
    1, cufft.h:78:81: unused parameter 'direction'

[MEDIUM] /sep/include/compat/cufft.h:82:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c140b8fed08fab4f88d281deff697e49
  Steps:
    1, cufft.h:82:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:82:57: unused parameter 'idata' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 11e0980bfec3bd8ce185a7592f34f82e
  Steps:
    1, cufft.h:82:57: unused parameter 'idata'

[MEDIUM] /sep/include/compat/cufft.h:82:70: unused parameter 'odata' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1f1c0d2a7218750708f044376383a42c
  Steps:
    1, cufft.h:82:70: unused parameter 'odata'

[MEDIUM] /sep/include/compat/cufft.h:86:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ff4e39ce2b67e163875ca1153065cfe5
  Steps:
    1, cufft.h:86:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:86:57: unused parameter 'idata' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c2765670cf6718cb3a4bad62e43a6ee0
  Steps:
    1, cufft.h:86:57: unused parameter 'idata'

[MEDIUM] /sep/include/compat/cufft.h:86:70: unused parameter 'odata' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8baaf8257d40f376236212d6b9b0b96a
  Steps:
    1, cufft.h:86:70: unused parameter 'odata'

Found 24 defect(s) in cufft.h

[MEDIUM] /sep/include/quantum/quantum_manifold_optimizer.h:9:1: class 'CudaConfig' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fe11538b81a85a9f45f3992aba958340
  Steps:
    1, types.h:89:8: previous use is here
    2, quantum_manifold_optimizer.h:9:1: did you mean struct here?
    3, quantum_manifold_optimizer.h:9:1: class 'CudaConfig' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/quantum/quantum_manifold_optimizer.h:10:1: class 'APIConfig' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e52d51a7b097b27c55280ccbbcf5ba76
  Steps:
    1, types.h:63:8: previous use is here
    2, quantum_manifold_optimizer.h:10:1: did you mean struct here?
    3, quantum_manifold_optimizer.h:10:1: class 'APIConfig' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/quantum/quantum_manifold_optimizer.h:11:1: class 'LogConfig' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7269b2cec7008b529e0dbddd41520da1
  Steps:
    1, types.h:98:8: previous use is here
    2, quantum_manifold_optimizer.h:11:1: did you mean struct here?
    3, quantum_manifold_optimizer.h:11:1: class 'LogConfig' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/quantum/quantum_manifold_optimizer.h:12:1: class 'AnalyticsConfig' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a82c1524d68085f326edaf8c541a99a4
  Steps:
    1, types.h:121:8: previous use is here
    2, quantum_manifold_optimizer.h:12:1: did you mean struct here?
    3, quantum_manifold_optimizer.h:12:1: class 'AnalyticsConfig' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 4 defect(s) in quantum_manifold_optimizer.h

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:120:14: unused variable 'tier_analysis' [clang-diagnostic-unused-variable]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 01708387c81904505d93db53cf5517df
  Steps:
    1, quantum_coherence_manager.cpp:120:14: unused variable 'tier_analysis'

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:548:73: unused parameter 'target_tier' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a560e4c242292f1e5fe22ef0b6668c7b
  Steps:
    1, quantum_coherence_manager.cpp:548:73: unused parameter 'target_tier'

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:573:58: implicit conversion turns floating-point number into integer: 'float' to 'size_t' (aka 'unsigned long') [clang-diagnostic-float-conversion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 29ca0ceabea30d8423bb3e1ab80a80a5
  Steps:
    1, quantum_coherence_manager.cpp:573:58: implicit conversion turns floating-point number into integer: 'float' to 'size_t' (aka 'unsigned long')

Found 3 defect(s) in quantum_coherence_manager.cpp

Found no defects in redis_manager.cpp
Found no defects in manager.cpp
[MEDIUM] /sep/src/quantum/evolution.cpp:127:52: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c5677e825a782969782849e7a3b87e33
  Steps:
    1, evolution.cpp:127:52: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/quantum/evolution.cpp:325:42: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cad84b4fd50cd0e5a22044d2f4918545
  Steps:
    1, evolution.cpp:325:42: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/quantum/evolution.cpp:377:45: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 686c468a0c599aae810685813b0a5284
  Steps:
    1, evolution.cpp:377:45: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in evolution.cpp

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:16:36: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a4a36d003e7c59c4fd7d6b1c343daf57
  Steps:
    1, quantum_manifold_optimizer.cpp:16:36: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:17:24: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 641c514a85911525bc59527032c66824
  Steps:
    1, quantum_manifold_optimizer.cpp:17:24: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:18:34: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c0d904c2c9b962d28959a1f52e0a24c
  Steps:
    1, quantum_manifold_optimizer.cpp:18:34: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:19:31: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 31dc183ddee5964860acdcaaece450bd
  Steps:
    1, quantum_manifold_optimizer.cpp:19:31: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:20:31: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 16d39340a5c9b05f0487c0526cdeb125
  Steps:
    1, quantum_manifold_optimizer.cpp:20:31: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:67:86: unused parameter 'position' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b944f2e95ab8742ef5da653a467252fb
  Steps:
    1, quantum_manifold_optimizer.cpp:67:86: unused parameter 'position'

Found 6 defect(s) in quantum_manifold_optimizer.cpp

Found no defects in pattern_processor_interface.cpp
[MEDIUM] /sep/include/api/types.h:48:54: unused parameter 'name' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4d36d5ee5324c8a8ec1018571fda19eb
  Steps:
    1, types.h:48:54: unused parameter 'name'

Found 1 defect(s) in types.h

[CRITICAL] /sep/include/quantum/data.hpp:22:5: unknown type name 'MemoryTierEnum' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 69ec806177f88d756c73ed48064c98e1
  Steps:
    1, data.hpp:22:5: unknown type name 'MemoryTierEnum'

Found 1 defect(s) in data.hpp

[CRITICAL] /sep/include/quantum/processor.h:94:58: unknown type name 'MemoryTierEnum' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4d396cad09d9d5b95ccff6fde99aa439
  Steps:
    1, processor.h:94:58: unknown type name 'MemoryTierEnum'

Found 1 defect(s) in processor.h

[CRITICAL] /sep/include/quantum/quantum_processor_qfh.h:64:5: no type named 'MemoryTierEnum' in namespace 'sep'; did you mean 'memory::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 91e1c5c5e163601ed45f890542019600
  Notes:
    1, quantum_processor_qfh.h:64:5: memory::MemoryTierEnum (fixit)
  Steps:
    1, types.h:26:12: 'memory::MemoryTierEnum' declared here
    2, quantum_processor_qfh.h:64:5: no type named 'MemoryTierEnum' in namespace 'sep'; did you mean 'memory::MemoryTierEnum'?

Found 1 defect(s) in quantum_processor_qfh.h

[CRITICAL] /sep/include/quantum/types.h:29:11: duplicate member 'evolution_rate' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1d5aedc566860edde6ec91d182dcab08
  Steps:
    1, types.h:23:11: previous declaration is here
    2, types.h:29:11: duplicate member 'evolution_rate'

[CRITICAL] /sep/include/quantum/types.h:30:11: duplicate member 'energy' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8d718523eaa61cda59aad0e46d38a228
  Steps:
    1, types.h:24:11: previous declaration is here
    2, types.h:30:11: duplicate member 'energy'

[CRITICAL] /sep/include/quantum/types.h:31:11: duplicate member 'coupling_strength' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e4ac9d7d740f79b02cf3c6090f064741
  Steps:
    1, types.h:25:11: previous declaration is here
    2, types.h:31:11: duplicate member 'coupling_strength'

[CRITICAL] /sep/include/quantum/types.h:32:5: unknown type name 'MemoryTierEnum' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b3ee43266f04ed3f4e0000d3b102bec8
  Steps:
    1, types.h:32:5: unknown type name 'MemoryTierEnum'

Found 4 defect(s) in types.h

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:258:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:258:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:302:26: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 005a58abc587efa85ab38a672b887462
  Steps:
    1, memory_tier_manager.cpp:302:26: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:385:23: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0c34aea053751feecbd4d9b55c48af7c
  Steps:
    1, memory_tier_manager.cpp:385:23: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in memory_tier_manager.cpp

[LOW] /sep/include/quantum/pattern_evolution_bridge.h:16:7: no definition found for 'QuantumManifoldOptimizer', but a definition with the same name 'QuantumManifoldOptimizer' found in another namespace 'sep::quantum::manifold' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c441508debea10c14cf2d6db211eb7f0
  Steps:
    1, quantum_manifold_optimizer.h:57:7: a definition of 'QuantumManifoldOptimizer' is found here
    2, pattern_evolution_bridge.h:16:7: no definition found for 'QuantumManifoldOptimizer', but a definition with the same name 'QuantumManifoldOptimizer' found in another namespace 'sep::quantum::manifold'

Found 1 defect(s) in pattern_evolution_bridge.h

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:178:5: unknown type name 'AnalyticsConfig'; did you mean 'config::AnalyticsConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 23cdfcefc7dd2fa874c6f40c59cf3b50
  Notes:
    1, quantum_manifold_optimizer.h:178:5: config::AnalyticsConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:12:7: 'config::AnalyticsConfig' declared here
    2, quantum_manifold_optimizer.h:178:5: unknown type name 'AnalyticsConfig'; did you mean 'config::AnalyticsConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:260:5: no type named 'CudaConfig' in 'sep::quantum::manifold::ManifoldConfig'; did you mean simply 'CudaConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 990355e5736f0f0c9878f12aeca26693
  Notes:
    1, quantum_manifold_optimizer.h:260:5: CudaConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:149:8: 'CudaConfig' declared here
    2, quantum_manifold_optimizer.h:260:5: no type named 'CudaConfig' in 'sep::quantum::manifold::ManifoldConfig'; did you mean simply 'CudaConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:288:5: no type named 'APIConfig' in 'sep::quantum::manifold::ManifoldConfig'; did you mean simply 'APIConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 487be4acc14377222787142dccef7b68
  Notes:
    1, quantum_manifold_optimizer.h:288:5: APIConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:158:12: 'APIConfig' declared here
    2, quantum_manifold_optimizer.h:288:5: no type named 'APIConfig' in 'sep::quantum::manifold::ManifoldConfig'; did you mean simply 'APIConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:372:3: unknown type name 'AnalyticsConfig'; did you mean 'config::AnalyticsConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8933a61dae61eca075d99a6920b05b94
  Notes:
    1, quantum_manifold_optimizer.h:372:3: config::AnalyticsConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:12:7: 'config::AnalyticsConfig' declared here
    2, quantum_manifold_optimizer.h:372:3: unknown type name 'AnalyticsConfig'; did you mean 'config::AnalyticsConfig'?

Found 4 defect(s) in quantum_manifold_optimizer.h

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:119:14: unused variable 'tier_analysis' [clang-diagnostic-unused-variable]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 01708387c81904505d93db53cf5517df
  Steps:
    1, quantum_coherence_manager.cpp:119:14: unused variable 'tier_analysis'

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:547:73: unused parameter 'target_tier' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a560e4c242292f1e5fe22ef0b6668c7b
  Steps:
    1, quantum_coherence_manager.cpp:547:73: unused parameter 'target_tier'

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:572:58: implicit conversion turns floating-point number into integer: 'float' to 'size_t' (aka 'unsigned long') [clang-diagnostic-float-conversion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 29ca0ceabea30d8423bb3e1ab80a80a5
  Steps:
    1, quantum_coherence_manager.cpp:572:58: implicit conversion turns floating-point number into integer: 'float' to 'size_t' (aka 'unsigned long')

Found 3 defect(s) in quantum_coherence_manager.cpp

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:76:14: a type specifier is required for all declarations [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 59acc410e6458904ea667d8cdfd5979f
  Steps:
    1, pattern_evolution_bridge.cpp:76:14: a type specifier is required for all declarations

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:77:9: only constructors take base initializers [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 28f45d34f93021fca2136d317d75df35
  Steps:
    1, pattern_evolution_bridge.cpp:77:9: only constructors take base initializers

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:79:15: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1ce98055f1c04dfa73569536f2a7f9f4
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, pattern_evolution_bridge.cpp:79:15: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:87:6: identifier 'Impl' after '~' in destructor name does not name a type [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f1b4bc30003b0f89572c08870ab5be4e
  Notes:
    1, pattern_evolution_bridge.cpp:87:6: PatternEvolutionBridgeImpl (fixit)
  Steps:
    1, pattern_evolution_bridge.cpp:76:14: non-type declaration found by destructor name lookup
    2, pattern_evolution_bridge.cpp:87:6: identifier 'Impl' after '~' in destructor name does not name a type

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:261:19: no member named 'config_' in 'sep::quantum::(anonymous namespace)::PatternEvolutionBridgeImpl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5e1cf502a9fa015a0a193d1fba337c16
  Steps:
    1, pattern_evolution_bridge.cpp:261:19: no member named 'config_' in 'sep::quantum::(anonymous namespace)::PatternEvolutionBridgeImpl'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:271:9: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4187744a588f9e2f5dc27277d7513ba5
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, pattern_evolution_bridge.cpp:271:9: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:272:35: use of undeclared identifier 'config_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 21843ef329f0473bf2d55c90756c8317
  Steps:
    1, pattern_evolution_bridge.cpp:272:35: use of undeclared identifier 'config_'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:273:35: use of undeclared identifier 'config_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4feeaa53c000bae7408a5d0c1367fd1b
  Steps:
    1, pattern_evolution_bridge.cpp:273:35: use of undeclared identifier 'config_'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:275:55: member access into incomplete type 'sep::quantum::QuantumManifoldOptimizer' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 16c512806cdbadd337957bd8bc0fd3b9
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, pattern_evolution_bridge.cpp:275:55: member access into incomplete type 'sep::quantum::QuantumManifoldOptimizer'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:325:61: use of undeclared identifier 'config_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 186a4722e4390209fbc106ef487e4b4d
  Steps:
    1, pattern_evolution_bridge.cpp:325:61: use of undeclared identifier 'config_'

[MEDIUM] /sep/src/quantum/pattern_evolution_bridge.cpp:440:56: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 16ab10153f9ab6bb19dc3eca3d51a4c4
  Steps:
    1, pattern_evolution_bridge.cpp:440:56: implicit conversion increases floating-point precision: 'float' to 'double'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:512:25: cannot define or redeclare 'PatternEvolutionBridge' here because namespace '' does not enclose namespace 'PatternEvolutionBridge' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ede580e412e44812efbd2e445c50e55c
  Steps:
    1, pattern_evolution_bridge.cpp:512:25: cannot define or redeclare 'PatternEvolutionBridge' here because namespace '' does not enclose namespace 'PatternEvolutionBridge'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:513:30: use of undeclared identifier 'Impl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6ad658b161c1b959454eb7f27e60d5a6
  Steps:
    1, pattern_evolution_bridge.cpp:513:30: use of undeclared identifier 'Impl'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:513:36: unexpected namespace name 'config': expected expression [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4e283b3abbf8157b4044109ebdf3b995
  Steps:
    1, pattern_evolution_bridge.cpp:513:36: unexpected namespace name 'config': expected expression

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:515:25: cannot define or redeclare '~PatternEvolutionBridge' here because namespace '' does not enclose namespace 'PatternEvolutionBridge' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 995736136aef174eefb0f34dd262ae46
  Steps:
    1, pattern_evolution_bridge.cpp:515:25: cannot define or redeclare '~PatternEvolutionBridge' here because namespace '' does not enclose namespace 'PatternEvolutionBridge'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:517:30: cannot define or redeclare 'initializeEvolutionState' here because namespace '' does not enclose namespace 'PatternEvolutionBridge' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e3db0956d638ecf7c56f7351adf625dd
  Steps:
    1, pattern_evolution_bridge.cpp:517:30: cannot define or redeclare 'initializeEvolutionState' here because namespace '' does not enclose namespace 'PatternEvolutionBridge'

Found 16 defect(s) in pattern_evolution_bridge.cpp

[CRITICAL] /sep/src/main.cpp:35:8: no member named 'logging' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20b1b32d5dd62e2e122b03252aacfd2a
  Steps:
    1, main.cpp:35:8: no member named 'logging' in namespace 'sep'

[MEDIUM] /sep/src/main.cpp:38:3: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: abda5be677330711b486e3fefd210557
  Steps:
    1, main.cpp:38:3: cast the expression to void to silence this warning
    2, main.cpp:38:3: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/main.cpp:39:3: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 28ce039dab292f009572bd5c2fbc5bc2
  Steps:
    1, main.cpp:39:3: cast the expression to void to silence this warning
    2, main.cpp:39:3: the value returned by this function should not be disregarded; neglecting it may lead to errors

[CRITICAL] /sep/src/main.cpp:73:40: no member named 'initialize' in 'sep::blender::CyclesRenderer' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b6c91a5e8dfac41335b92b6aab13be30
  Steps:
    1, main.cpp:73:40: no member named 'initialize' in 'sep::blender::CyclesRenderer'

[CRITICAL] /sep/src/main.cpp:77:14: no member named 'logging' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 65e404fd506de747095eda688681f609
  Steps:
    1, main.cpp:77:14: no member named 'logging' in namespace 'sep'

[CRITICAL] /sep/src/main.cpp:88:14: no member named 'logging' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 65e404fd506de747095eda688681f609
  Steps:
    1, main.cpp:88:14: no member named 'logging' in namespace 'sep'

[CRITICAL] /sep/src/main.cpp:98:35: no member named 'PatternData' in namespace 'sep::pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a1d51b30a96ae45240bda5a0094c6fc5
  Steps:
    1, main.cpp:98:35: no member named 'PatternData' in namespace 'sep::pattern'

[CRITICAL] /sep/src/main.cpp:103:27: no member named 'PatternData' in namespace 'sep::pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2ce4fff6caafd5cd174c3420962e1266
  Steps:
    1, main.cpp:103:27: no member named 'PatternData' in namespace 'sep::pattern'

[CRITICAL] /sep/src/main.cpp:103:38: expected ';' after expression [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d58534c9d80bf6221f5c95270b65b4d3
  Notes:
    1, main.cpp:103:38: ; (fixit)
  Steps:
    1, main.cpp:103:38: expected ';' after expression

[CRITICAL] /sep/src/main.cpp:103:39: use of undeclared identifier 'pattern'; did you mean 'patterns'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6e9e5a1b8d1a6e5b9048d98708f0511b
  Notes:
    1, main.cpp:103:39: patterns (fixit)
  Steps:
    1, main.cpp:98:48: 'patterns' declared here
    2, main.cpp:103:39: use of undeclared identifier 'pattern'; did you mean 'patterns'?

[CRITICAL] /sep/src/main.cpp:104:13: use of undeclared identifier 'pattern'; did you mean 'patterns'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1ee755483751fefa8519466e7f711d4f
  Notes:
    1, main.cpp:104:13: patterns (fixit)
  Steps:
    1, main.cpp:98:48: 'patterns' declared here
    2, main.cpp:104:13: use of undeclared identifier 'pattern'; did you mean 'patterns'?

[CRITICAL] /sep/src/main.cpp:105:13: use of undeclared identifier 'pattern'; did you mean 'patterns'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9b9f81af6b2fa32cfbb6083e0ff317be
  Notes:
    1, main.cpp:105:13: patterns (fixit)
  Steps:
    1, main.cpp:98:48: 'patterns' declared here
    2, main.cpp:105:13: use of undeclared identifier 'pattern'; did you mean 'patterns'?

[CRITICAL] /sep/src/main.cpp:106:13: use of undeclared identifier 'pattern'; did you mean 'patterns'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 56954188edb14e56cfb72e45dde534e0
  Notes:
    1, main.cpp:106:13: patterns (fixit)
  Steps:
    1, main.cpp:98:48: 'patterns' declared here
    2, main.cpp:106:13: use of undeclared identifier 'pattern'; did you mean 'patterns'?

[CRITICAL] /sep/src/main.cpp:107:32: use of undeclared identifier 'pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 248b7617daac524c55a1f451975385d1
  Steps:
    1, main.cpp:107:32: use of undeclared identifier 'pattern'

[CRITICAL] /sep/src/main.cpp:112:27: no member named 'PatternData' in namespace 'sep::pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2ce4fff6caafd5cd174c3420962e1266
  Steps:
    1, main.cpp:112:27: no member named 'PatternData' in namespace 'sep::pattern'

[CRITICAL] /sep/src/main.cpp:112:38: expected ';' after expression [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d58534c9d80bf6221f5c95270b65b4d3
  Notes:
    1, main.cpp:112:38: ; (fixit)
  Steps:
    1, main.cpp:112:38: expected ';' after expression

[CRITICAL] /sep/src/main.cpp:112:39: use of undeclared identifier 'pattern'; did you mean 'patterns'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6e9e5a1b8d1a6e5b9048d98708f0511b
  Notes:
    1, main.cpp:112:39: patterns (fixit)
  Steps:
    1, main.cpp:98:48: 'patterns' declared here
    2, main.cpp:112:39: use of undeclared identifier 'pattern'; did you mean 'patterns'?

[CRITICAL] /sep/src/main.cpp:113:13: use of undeclared identifier 'pattern'; did you mean 'patterns'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1ee755483751fefa8519466e7f711d4f
  Notes:
    1, main.cpp:113:13: patterns (fixit)
  Steps:
    1, main.cpp:98:48: 'patterns' declared here
    2, main.cpp:113:13: use of undeclared identifier 'pattern'; did you mean 'patterns'?

[CRITICAL] /sep/src/main.cpp:114:13: use of undeclared identifier 'pattern'; did you mean 'patterns'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9b9f81af6b2fa32cfbb6083e0ff317be
  Notes:
    1, main.cpp:114:13: patterns (fixit)
  Steps:
    1, main.cpp:98:48: 'patterns' declared here
    2, main.cpp:114:13: use of undeclared identifier 'pattern'; did you mean 'patterns'?

[CRITICAL] /sep/src/main.cpp:115:13: use of undeclared identifier 'pattern'; did you mean 'patterns'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 56954188edb14e56cfb72e45dde534e0
  Notes:
    1, main.cpp:115:13: patterns (fixit)
  Steps:
    1, main.cpp:98:48: 'patterns' declared here
    2, main.cpp:115:13: use of undeclared identifier 'pattern'; did you mean 'patterns'?

[CRITICAL] /sep/src/main.cpp:116:32: use of undeclared identifier 'pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 248b7617daac524c55a1f451975385d1
  Steps:
    1, main.cpp:116:32: use of undeclared identifier 'pattern'

Found 21 defect(s) in main.cpp

[MEDIUM] /sep/src/api/sep_engine.cpp:304:67: unused parameter 'request_data' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d06191f01aa657f541e0410cc16c881a
  Steps:
    1, sep_engine.cpp:304:67: unused parameter 'request_data'

Found 1 defect(s) in sep_engine.cpp

[MEDIUM] /sep/src/blender/blender_integration.cpp:32:7: field 'thread_running_' will be initialized after field 'gpu_context_' [clang-diagnostic-reorder-ctor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 87e249e0c36b007e21b582c9ae809917
  Notes:
    1, blender_integration.cpp:32:7: gpu_context_(nullptr) (fixit)
    2, blender_integration.cpp:32:5: , gpu_context_(nullptr) (fixit)
    3, blender_integration.cpp:32:7: ~~~~~~~~~~~~~~~~~~~~~ (fixit)
    4, blender_integration.cpp:32:7: thread_running_(false) (fixit)
  Steps:
    1, blender_integration.cpp:32:7: field 'thread_running_' will be initialized after field 'gpu_context_'

Found 1 defect(s) in blender_integration.cpp

[MEDIUM] /sep/src/api/server.cpp:230:50: unused parameter 'response_body' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9a6489114b93b9375b7c0c11dd032dae
  Steps:
    1, server.cpp:230:50: unused parameter 'response_body'

Found 1 defect(s) in server.cpp

[CRITICAL] /sep/src/blender/mesh_handler.cpp:44:42: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c2dda186ff6686f5f89ecaf99bb817e5
  Notes:
    1, mesh_handler.cpp:44:42: sep::pattern (fixit)
  Steps:
    1, types.h:12:11: 'sep::pattern' declared here
    2, mesh_handler.cpp:44:42: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'?

[CRITICAL] /sep/src/blender/mesh_handler.cpp:199:53: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: de6c573da0fa98f9a2af295b476a797b
  Notes:
    1, mesh_handler.cpp:199:53: sep::pattern (fixit)
  Steps:
    1, types.h:12:11: 'sep::pattern' declared here
    2, mesh_handler.cpp:199:53: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'?

[CRITICAL] /sep/src/blender/mesh_handler.cpp:224:50: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7de6c6cffb4fa53a320fb315b82e0eaf
  Notes:
    1, mesh_handler.cpp:224:50: sep::pattern (fixit)
  Steps:
    1, types.h:12:11: 'sep::pattern' declared here
    2, mesh_handler.cpp:224:50: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'?

[CRITICAL] /sep/src/blender/mesh_handler.cpp:240:52: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c9b8bb873914118adc2e0185e4dacdf
  Notes:
    1, mesh_handler.cpp:240:52: sep::pattern (fixit)
  Steps:
    1, types.h:12:11: 'sep::pattern' declared here
    2, mesh_handler.cpp:240:52: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'?

[CRITICAL] /sep/src/blender/mesh_handler.cpp:268:41: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2cab5fe71cf466aed6e947c9e110784e
  Notes:
    1, mesh_handler.cpp:268:41: sep::pattern (fixit)
  Steps:
    1, types.h:12:11: 'sep::pattern' declared here
    2, mesh_handler.cpp:268:41: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'?

[MEDIUM] /sep/src/blender/mesh_handler.cpp:307:21: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 96149c0ed1849daeb9b92afc0dcb3cc9
  Steps:
    1, mesh_handler.cpp:307:21: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/blender/mesh_handler.cpp:326:14: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 31bfb7cbaa7554f38c92dbc1014c813e
  Steps:
    1, mesh_handler.cpp:326:14: implicit conversion increases floating-point precision: 'float' to 'double'

[CRITICAL] /sep/src/blender/mesh_handler.cpp:347:51: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 26ad090d763fd1f3fc74d83cc357e9d7
  Notes:
    1, mesh_handler.cpp:347:51: sep::pattern (fixit)
  Steps:
    1, types.h:12:11: 'sep::pattern' declared here
    2, mesh_handler.cpp:347:51: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'?

[CRITICAL] /sep/src/blender/mesh_handler.cpp:357:47: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 03dd841cbb20447ef97004d0299d211b
  Notes:
    1, mesh_handler.cpp:357:47: sep::pattern (fixit)
  Steps:
    1, types.h:12:11: 'sep::pattern' declared here
    2, mesh_handler.cpp:357:47: use of undeclared identifier 'pattern'; did you mean 'sep::pattern'?

Found 9 defect(s) in mesh_handler.cpp

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:39:5: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 888b92f14d333348a7cbbfb3abf3ebd2
  Steps:
    1, cycles_renderer.cpp:39:5: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:40:5: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d0ab38a29e619ba74232533c3e7a5ecb
  Steps:
    1, cycles_renderer.cpp:40:5: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:71:9: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8ae63feb3b8d13c611f5522559eaaaaa
  Steps:
    1, cycles_renderer.cpp:71:9: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:72:28: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: af0d0693c049e8c9c6b7c102a847bfc7
  Steps:
    1, cycles_renderer.cpp:72:28: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:75:9: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4f921626b454662d408a4d01c3880e3f
  Steps:
    1, cycles_renderer.cpp:75:9: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:79:9: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dad21d9fd7f7b00616d0b1f1ceba809b
  Steps:
    1, cycles_renderer.cpp:79:9: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:80:9: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf2540c9248a3f3c3fecf7de3cd2e3ab
  Steps:
    1, cycles_renderer.cpp:80:9: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:84:25: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bf30a34fc3ae91793f68f122ba15494c
  Steps:
    1, cycles_renderer.cpp:84:25: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:84:47: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 56f63a7f84ea5a5cb0219779147190cc
  Steps:
    1, cycles_renderer.cpp:84:47: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:91:35: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6bf784fa312afcc080c9a3958ec2f6b1
  Steps:
    1, cycles_renderer.cpp:91:35: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:94:9: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6363f807ec38aafffe59522be773a1eb
  Steps:
    1, cycles_renderer.cpp:94:9: use of undeclared identifier 'ccl'

[CRITICAL] /sep/src/blender/cycles_renderer.cpp:101:37: use of undeclared identifier 'ccl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fae51de8d3079f540386df3060357d2a
  Steps:
    1, cycles_renderer.cpp:101:37: use of undeclared identifier 'ccl'

Found 12 defect(s) in cycles_renderer.cpp

[MEDIUM] /sep/src/core/engine.cpp:61:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:61:5: cast the expression to void to silence this warning
    2, engine.cpp:61:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:65:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:65:5: cast the expression to void to silence this warning
    2, engine.cpp:65:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:69:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:69:5: cast the expression to void to silence this warning
    2, engine.cpp:69:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:74:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:74:5: cast the expression to void to silence this warning
    2, engine.cpp:74:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:78:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:78:9: cast the expression to void to silence this warning
    2, engine.cpp:78:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:84:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:84:9: cast the expression to void to silence this warning
    2, engine.cpp:84:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:88:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1b24dfd970075b2d58e9c58b518e9a70
  Steps:
    1, engine.cpp:88:13: cast the expression to void to silence this warning
    2, engine.cpp:88:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:94:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:94:5: cast the expression to void to silence this warning
    2, engine.cpp:94:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:98:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:98:5: cast the expression to void to silence this warning
    2, engine.cpp:98:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:103:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:103:9: cast the expression to void to silence this warning
    2, engine.cpp:103:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:107:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:107:5: cast the expression to void to silence this warning
    2, engine.cpp:107:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:111:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:111:5: cast the expression to void to silence this warning
    2, engine.cpp:111:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:124:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:124:9: cast the expression to void to silence this warning
    2, engine.cpp:124:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:127:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:127:9: cast the expression to void to silence this warning
    2, engine.cpp:127:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:131:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:131:9: cast the expression to void to silence this warning
    2, engine.cpp:131:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:151:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:151:5: cast the expression to void to silence this warning
    2, engine.cpp:151:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:157:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1b24dfd970075b2d58e9c58b518e9a70
  Steps:
    1, engine.cpp:157:13: cast the expression to void to silence this warning
    2, engine.cpp:157:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:162:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 40d0055d6f269f3f48e0ad02217f9686
  Steps:
    1, engine.cpp:162:17: cast the expression to void to silence this warning
    2, engine.cpp:162:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:166:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 40d0055d6f269f3f48e0ad02217f9686
  Steps:
    1, engine.cpp:166:17: cast the expression to void to silence this warning
    2, engine.cpp:166:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:171:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:171:9: cast the expression to void to silence this warning
    2, engine.cpp:171:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:174:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:174:9: cast the expression to void to silence this warning
    2, engine.cpp:174:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:179:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:179:5: cast the expression to void to silence this warning
    2, engine.cpp:179:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:184:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:184:9: cast the expression to void to silence this warning
    2, engine.cpp:184:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:187:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:187:9: cast the expression to void to silence this warning
    2, engine.cpp:187:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:190:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:190:9: cast the expression to void to silence this warning
    2, engine.cpp:190:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:195:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:195:5: cast the expression to void to silence this warning
    2, engine.cpp:195:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:256:94: unused parameter 'tick' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b101bc606cec5d9ec82a9c68115450f0
  Steps:
    1, engine.cpp:256:94: unused parameter 'tick'

Found 27 defect(s) in engine.cpp

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:15:36: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a4a36d003e7c59c4fd7d6b1c343daf57
  Steps:
    1, quantum_manifold_optimizer.cpp:15:36: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:16:24: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 641c514a85911525bc59527032c66824
  Steps:
    1, quantum_manifold_optimizer.cpp:16:24: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:17:34: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c0d904c2c9b962d28959a1f52e0a24c
  Steps:
    1, quantum_manifold_optimizer.cpp:17:34: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:18:31: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 31dc183ddee5964860acdcaaece450bd
  Steps:
    1, quantum_manifold_optimizer.cpp:18:31: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:19:31: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 16d39340a5c9b05f0487c0526cdeb125
  Steps:
    1, quantum_manifold_optimizer.cpp:19:31: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:66:86: unused parameter 'position' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b944f2e95ab8742ef5da653a467252fb
  Steps:
    1, quantum_manifold_optimizer.cpp:66:86: unused parameter 'position'

Found 6 defect(s) in quantum_manifold_optimizer.cpp

[CRITICAL] /sep/src/tests/cycles_test.cpp:50:40: no member named 'isCyclesAvailable' in 'sep::blender::CyclesRenderer' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8f97d9d36c86207ed5cd88828995667c
  Steps:
    1, cycles_test.cpp:50:40: no member named 'isCyclesAvailable' in 'sep::blender::CyclesRenderer'

[CRITICAL] /sep/src/tests/cycles_test.cpp:57:38: no member named 'initialize' in 'sep::blender::CyclesRenderer' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99466ef15b892e5a9a51e67d0070cea4
  Steps:
    1, cycles_test.cpp:57:38: no member named 'initialize' in 'sep::blender::CyclesRenderer'

[CRITICAL] /sep/src/tests/cycles_test.cpp:72:23: no member named 'createSceneFromPatterns' in 'sep::blender::CyclesRenderer' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 742910c36bfa87f6380be26f5d74ec62
  Steps:
    1, cycles_test.cpp:72:23: no member named 'createSceneFromPatterns' in 'sep::blender::CyclesRenderer'

[CRITICAL] /sep/src/tests/cycles_test.cpp:79:35: no type named 'RenderParams' in 'sep::blender::CyclesRenderer' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4e5cfdd3ff359551791830cc5f57d88f
  Steps:
    1, cycles_test.cpp:79:35: no type named 'RenderParams' in 'sep::blender::CyclesRenderer'

[CRITICAL] /sep/src/tests/cycles_test.cpp:93:23: no member named 'renderScene' in 'sep::blender::CyclesRenderer' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 45ae837b8fde2b50ad69c90e1b65e9eb
  Steps:
    1, cycles_test.cpp:93:23: no member named 'renderScene' in 'sep::blender::CyclesRenderer'

Found 5 defect(s) in cycles_test.cpp

[CRITICAL] /sep/include/compat/cuda_impl.h:22:12: no member named 'cudaMemcpyAsync' in the global namespace; did you mean 'sep::cuda::cudaMemcpyAsync'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 090fe72da399bf3f9ed46d260fddcf67
  Notes:
    1, cuda_impl.h:22:12: sep::cuda::cudaMemcpyAsync (fixit)
  Steps:
    1, cuda_runtime.h:96:13: 'sep::cuda::cudaMemcpyAsync' declared here
    2, cuda_impl.h:22:12: no member named 'cudaMemcpyAsync' in the global namespace; did you mean 'sep::cuda::cudaMemcpyAsync'?

Found 1 defect(s) in cuda_impl.h

[MEDIUM] /sep/src/api/client.cpp:141:46: unused parameter 'request' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 91d07ab03d93db50c8196e95accb3896
  Steps:
    1, client.cpp:141:46: unused parameter 'request'

Found 1 defect(s) in client.cpp

[CRITICAL] /sep/src/api/lock_free_rate_limiter.cpp:20:9: use of undeclared identifier 'metrics_mutex_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: caaa666c662948bd157ab08052140f51
  Steps:
    1, lock_free_rate_limiter.cpp:20:9: use of undeclared identifier 'metrics_mutex_'

Found 1 defect(s) in lock_free_rate_limiter.cpp

[MEDIUM] /sep/include/quantum/quantum_manifold_optimizer.h:138:3: variable 'memory' defined in a header file; variable definitions in header files can lead to ODR violations [misc-definitions-in-headers]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: de12231b69e355bebdfb785632ca709d
  Steps:
    1, quantum_manifold_optimizer.h:138:3: variable 'memory' defined in a header file; variable definitions in header files can lead to ODR violations

[MEDIUM] /sep/include/quantum/quantum_manifold_optimizer.h:146:3: variable 'quantum' defined in a header file; variable definitions in header files can lead to ODR violations [misc-definitions-in-headers]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f3ff5e70c604acc2d8966c63164273ba
  Steps:
    1, quantum_manifold_optimizer.h:146:3: variable 'quantum' defined in a header file; variable definitions in header files can lead to ODR violations

[MEDIUM] /sep/include/quantum/quantum_manifold_optimizer.h:155:3: variable 'cuda' defined in a header file; variable definitions in header files can lead to ODR violations [misc-definitions-in-headers]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f03326f55eb9b11c02f97a8026e870ee
  Steps:
    1, quantum_manifold_optimizer.h:155:3: variable 'cuda' defined in a header file; variable definitions in header files can lead to ODR violations

[MEDIUM] /sep/include/quantum/quantum_manifold_optimizer.h:163:7: variable 'api' defined in a header file; variable definitions in header files can lead to ODR violations [misc-definitions-in-headers]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 212813ce4e8acd486e12a3e2ba97a2e3
  Steps:
    1, quantum_manifold_optimizer.h:163:7: variable 'api' defined in a header file; variable definitions in header files can lead to ODR violations

Found 4 defect(s) in quantum_manifold_optimizer.h

[LOW] /sep/src/memory/quantum_coherence_manager.cpp:119:14: Value stored to 'tier_analysis' during its initialization is never read [deadcode.DeadStores]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bd401bfb4e259755fb945e81b4f274cc
  Steps:
    1, quantum_coherence_manager.cpp:119:14: Value stored to 'tier_analysis' during its initialization is never read

Found 1 defect(s) in quantum_coherence_manager.cpp

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:262:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:262:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:306:26: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 005a58abc587efa85ab38a672b887462
  Steps:
    1, memory_tier_manager.cpp:306:26: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:389:23: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0c34aea053751feecbd4d9b55c48af7c
  Steps:
    1, memory_tier_manager.cpp:389:23: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in memory_tier_manager.cpp

[MEDIUM] /sep/src/core/engine.cpp:51:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:51:6: cast the expression to void to silence this warning
    2, engine.cpp:51:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:55:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:55:6: cast the expression to void to silence this warning
    2, engine.cpp:55:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:59:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:59:6: cast the expression to void to silence this warning
    2, engine.cpp:59:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:64:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:64:6: cast the expression to void to silence this warning
    2, engine.cpp:64:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:68:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:68:10: cast the expression to void to silence this warning
    2, engine.cpp:68:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:74:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:74:10: cast the expression to void to silence this warning
    2, engine.cpp:74:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:78:14: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 39120b7c5df7534093597522738993e6
  Steps:
    1, engine.cpp:78:14: cast the expression to void to silence this warning
    2, engine.cpp:78:14: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:84:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:84:6: cast the expression to void to silence this warning
    2, engine.cpp:84:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:88:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:88:6: cast the expression to void to silence this warning
    2, engine.cpp:88:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:93:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:93:10: cast the expression to void to silence this warning
    2, engine.cpp:93:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:97:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:97:6: cast the expression to void to silence this warning
    2, engine.cpp:97:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:101:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c0238b3ff155d08de6c3b8829936ad05
  Steps:
    1, engine.cpp:101:6: cast the expression to void to silence this warning
    2, engine.cpp:101:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:114:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:114:10: cast the expression to void to silence this warning
    2, engine.cpp:114:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:117:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:117:10: cast the expression to void to silence this warning
    2, engine.cpp:117:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:121:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:121:10: cast the expression to void to silence this warning
    2, engine.cpp:121:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:129:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:129:6: cast the expression to void to silence this warning
    2, engine.cpp:129:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:135:14: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 39120b7c5df7534093597522738993e6
  Steps:
    1, engine.cpp:135:14: cast the expression to void to silence this warning
    2, engine.cpp:135:14: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:140:18: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab6f901229b488bfc64f4551fa611064
  Steps:
    1, engine.cpp:140:18: cast the expression to void to silence this warning
    2, engine.cpp:140:18: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:144:18: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab6f901229b488bfc64f4551fa611064
  Steps:
    1, engine.cpp:144:18: cast the expression to void to silence this warning
    2, engine.cpp:144:18: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:149:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:149:10: cast the expression to void to silence this warning
    2, engine.cpp:149:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:152:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:152:10: cast the expression to void to silence this warning
    2, engine.cpp:152:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:157:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:157:6: cast the expression to void to silence this warning
    2, engine.cpp:157:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:162:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:162:10: cast the expression to void to silence this warning
    2, engine.cpp:162:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:165:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:165:10: cast the expression to void to silence this warning
    2, engine.cpp:165:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:168:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:168:10: cast the expression to void to silence this warning
    2, engine.cpp:168:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:173:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:173:6: cast the expression to void to silence this warning
    2, engine.cpp:173:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:240:95: unused parameter 'tick' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d01385ebb281083b3540719cdc37c216
  Steps:
    1, engine.cpp:240:95: unused parameter 'tick'

Found 27 defect(s) in engine.cpp

[CRITICAL] /sep/include/blender/bridge.h:124:33: no member named 'PatternProcessor' in namespace 'sep::quantum' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: af89b32713ff6d0746b5a0492bdb88ff
  Steps:
    1, bridge.h:124:33: no member named 'PatternProcessor' in namespace 'sep::quantum'

Found 1 defect(s) in bridge.h

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:121:14: unused variable 'tier_analysis' [clang-diagnostic-unused-variable]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 01708387c81904505d93db53cf5517df
  Steps:
    1, quantum_coherence_manager.cpp:121:14: unused variable 'tier_analysis'

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:549:73: unused parameter 'tier_analysis' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7b77e7e39fcd03264c4a0bce9f712a22
  Steps:
    1, quantum_coherence_manager.cpp:549:73: unused parameter 'tier_analysis'

Found 2 defect(s) in quantum_coherence_manager.cpp

Found no defects in pattern_evolution.cpp
[MEDIUM] /sep/src/api/server.cpp:164:84: unused parameter 'response_body' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1ba44a0c1e912cab9c3cb4d30b1f4d8d
  Steps:
    1, server.cpp:164:84: unused parameter 'response_body'

[MEDIUM] /sep/src/api/server.cpp:224:50: unused parameter 'response_body' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9a6489114b93b9375b7c0c11dd032dae
  Steps:
    1, server.cpp:224:50: unused parameter 'response_body'

Found 2 defect(s) in server.cpp

[MEDIUM] /sep/src/audio/pipewire_capture.cpp:94:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0e308df13dda55c08b2861ff936dfe97
  Steps:
    1, pipewire_capture.cpp:94:9: cast the expression to void to silence this warning
    2, pipewire_capture.cpp:94:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 1 defect(s) in pipewire_capture.cpp

[MEDIUM] /sep/src/core/engine.cpp:63:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:63:6: cast the expression to void to silence this warning
    2, engine.cpp:63:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:69:14: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 39120b7c5df7534093597522738993e6
  Steps:
    1, engine.cpp:69:14: cast the expression to void to silence this warning
    2, engine.cpp:69:14: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:74:18: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab6f901229b488bfc64f4551fa611064
  Steps:
    1, engine.cpp:74:18: cast the expression to void to silence this warning
    2, engine.cpp:74:18: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:78:18: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab6f901229b488bfc64f4551fa611064
  Steps:
    1, engine.cpp:78:18: cast the expression to void to silence this warning
    2, engine.cpp:78:18: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:83:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:83:10: cast the expression to void to silence this warning
    2, engine.cpp:83:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:86:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:86:10: cast the expression to void to silence this warning
    2, engine.cpp:86:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:91:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:91:6: cast the expression to void to silence this warning
    2, engine.cpp:91:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:96:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:96:10: cast the expression to void to silence this warning
    2, engine.cpp:96:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:99:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:99:10: cast the expression to void to silence this warning
    2, engine.cpp:99:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:102:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:102:10: cast the expression to void to silence this warning
    2, engine.cpp:102:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:107:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:107:6: cast the expression to void to silence this warning
    2, engine.cpp:107:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:168:95: unused parameter 'tick' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d01385ebb281083b3540719cdc37c216
  Steps:
    1, engine.cpp:168:95: unused parameter 'tick'

Found 12 defect(s) in engine.cpp

[MEDIUM] /sep/src/quantum/pattern_evolution.cpp:75:112: unused parameter 'args' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f51780838423b3246fcf33761a5b3e0b
  Steps:
    1, pattern_evolution.cpp:75:112: unused parameter 'args'

[MEDIUM] /sep/src/quantum/pattern_evolution.cpp:82:100: unused parameter 'config' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 774dde4eb08a19ccd3e8af9ae80b6cf9
  Steps:
    1, pattern_evolution.cpp:82:100: unused parameter 'config'

Found 2 defect(s) in pattern_evolution.cpp

[MEDIUM] /sep/src/quantum/pattern_processor.cpp:46:43: unused variable 'previous_tier' [clang-diagnostic-unused-variable]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 430ebc78e5683fa2a3b2d7e9b6b5b5e7
  Steps:
    1, pattern_processor.cpp:46:43: unused variable 'previous_tier'

[MEDIUM] /sep/src/quantum/pattern_processor.cpp:106:46: unused function 'createPatternQuantumProcessor' [clang-diagnostic-unused-function]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 543e72d443d59334c8b1d1b7a9b4a25e
  Steps:
    1, pattern_processor.cpp:106:46: unused function 'createPatternQuantumProcessor'

Found 2 defect(s) in pattern_processor.cpp

[LOW] /sep/src/quantum/pattern_processor.cpp:46:43: Value stored to 'previous_tier' during its initialization is never read [deadcode.DeadStores]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d19cfb2c7cf871062627e2668769db5b
  Steps:
    1, pattern_processor.cpp:46:43: Value stored to 'previous_tier' during its initialization is never read

Found 1 defect(s) in pattern_processor.cpp

[CRITICAL] /sep/tests/blender/mocks/mock_mesh_handler.cpp:1:10: 'tests/blender/mesh_handler.hpp' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2a6b420407b78d6f219ecc455b880ef4
  Steps:
    1, mock_mesh_handler.cpp:1:10: 'tests/blender/mesh_handler.hpp' file not found

Found 1 defect(s) in mock_mesh_handler.cpp

[CRITICAL] /sep/tests/blender/mock_blender_bridge.cpp:3:10: 'blender/bridge.hpp' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 691913b2a5a988836d1d3fafdf75b436
  Steps:
    1, mock_blender_bridge.cpp:3:10: 'blender/bridge.hpp' file not found

Found 1 defect(s) in mock_blender_bridge.cpp

[CRITICAL] /sep/tests/blender/mock_pattern_compression.cpp:3:10: 'quantum/pattern_compression.hpp' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 89d40689e4a6f637db1562d684f5485c
  Steps:
    1, mock_pattern_compression.cpp:3:10: 'quantum/pattern_compression.hpp' file not found

Found 1 defect(s) in mock_pattern_compression.cpp

[CRITICAL] /sep/tests/blender/mock_pattern_evolution.cpp:4:10: 'quantum/pattern_evolution.hpp' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 67b9233f25896f6549986220e2ee6a2c
  Steps:
    1, mock_pattern_evolution.cpp:4:10: 'quantum/pattern_evolution.hpp' file not found

Found 1 defect(s) in mock_pattern_evolution.cpp

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:3:10: 'blender/mesh_handler.hpp' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dcdeaf1fa32b78bebeebd9d1605b3d76
  Steps:
    1, mock_mesh_handler.cpp:3:10: 'blender/mesh_handler.hpp' file not found

Found 1 defect(s) in mock_mesh_handler.cpp

[CRITICAL] /sep/tests/blender/pattern_integration_test.cpp:4:10: 'quantum/pattern_evolution_bridge.hpp' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1ab2c868f6f7b984ec86b5a2822422d5
  Steps:
    1, pattern_integration_test.cpp:4:10: 'quantum/pattern_evolution_bridge.hpp' file not found

Found 1 defect(s) in pattern_integration_test.cpp

[CRITICAL] /sep/tests/blender/mock_cuda_wrappers.cpp:3:10: 'compat/cuda_api.hpp' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fd5d847e6f943aa19d59afb3d54021b9
  Steps:
    1, mock_cuda_wrappers.cpp:3:10: 'compat/cuda_api.hpp' file not found

Found 1 defect(s) in mock_cuda_wrappers.cpp

[CRITICAL] /sep/tests/blender/mock_trace_span.cpp:3:10: 'core/tracing.hpp' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 892a38d80e3dbbabd4daefe0b7fda903
  Steps:
    1, mock_trace_span.cpp:3:10: 'core/tracing.hpp' file not found

Found 1 defect(s) in mock_trace_span.cpp

[HIGH] /usr/include/spa-0.2/spa/pod/parser.h:496:8: Access to field 'type' results in a dereference of a null pointer (loaded from variable 'pod') [core.NullDereference]
      if (pod->type == SPA_TYPE_Choice && *format != 'V')
          ^
  Report hash: 6396cb440c40f5db85f1a9ee33c1714c
  Macro expansions:
    1, parser.h:454:1: Macro 'SPA_API_POD_PARSER' expanded to 'static inline '
  Steps:
     1, parser.h:513:8: Calling 'spa_pod_parser_getv'
     2, parser.h:454:1: Entered call from 'spa_pod_parser_get'
     3, parser.h:457:26: Assuming 'f' is non-null
     4, parser.h:466:12: Assuming 'ftype' is equal to SPA_TYPE_Object
     5, parser.h:470:8: Assuming 'key' is not equal to 0
     6, parser.h:475:10: Assuming 'prop' is null
     7, parser.h:475:4: Null pointer value stored to 'pod'
     8, parser.h:478:7: Assuming the condition is false
     9, parser.h:484:20: Assuming the condition is false
    10, parser.h:484:8: Assuming 'optional' is false
    11, parser.h:487:7: Assuming the condition is false
    12, parser.h:496:8: Access to field 'type' results in a dereference of a null pointer (loaded from variable 'pod')

Found 1 defect(s) in parser.h

[CRITICAL] /sep/tests/blender/mock_pattern_compression.cpp:7:48: expected class name [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 33a4aceec889e72727157c30eb4a4227
  Steps:
    1, mock_pattern_compression.cpp:7:48: expected class name

[CRITICAL] /sep/tests/blender/mock_pattern_compression.cpp:9:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 10cac47b6abd2da8a01459ef7ccda1fa
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_pattern_compression.cpp:9:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_pattern_compression.cpp:9:63: no type named 'PatternData' in 'sep::quantum::Pattern'; did you mean 'persistence::PatternData'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c09b808e1bc7744a479a94f8bf79b983
  Notes:
    1, mock_pattern_compression.cpp:9:63: persistence::PatternData (fixit)
  Steps:
    1, gmock-function-mocker.h:115:59: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:56: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-function-mocker.h:131:60: expanded from macro 'GMOCK_INTERNAL_MOCK_METHOD_ARG_4'
    4, mock_pattern_compression.cpp:9:63: no type named 'PatternData' in 'sep::quantum::Pattern'; did you mean 'persistence::PatternData'?

[CRITICAL] /sep/tests/blender/mock_pattern_compression.cpp:9:63: use of undeclared identifier 'pattern'; did you mean 'quantum::Pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b57609ec50af731a12f400459cc68133
  Steps:
    1, gmock-function-mocker.h:115:59: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:56: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-function-mocker.h:131:60: expanded from macro 'GMOCK_INTERNAL_MOCK_METHOD_ARG_4'
    4, mock_pattern_compression.cpp:9:63: use of undeclared identifier 'pattern'; did you mean 'quantum::Pattern'?

Found 4 defect(s) in mock_pattern_compression.cpp

[CRITICAL] /sep/tests/blender/mock_trace_span.cpp:6:30: use of undeclared identifier 'core' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5ce702bb42d72bede86b48b768cd7fe2
  Steps:
    1, mock_trace_span.cpp:6:30: use of undeclared identifier 'core'

[CRITICAL] /sep/tests/blender/mock_trace_span.cpp:6:36: expected class name [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c22c6f2723618c2443b96add31d2090f
  Steps:
    1, mock_trace_span.cpp:6:36: expected class name

[CRITICAL] /sep/tests/blender/mock_trace_span.cpp:8:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3fd05367d524d0fc740ca7944da59189
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_trace_span.cpp:8:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_trace_span.cpp:9:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 247ea9eae8900f4b196bb11b3409fdf3
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_trace_span.cpp:9:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_trace_span.cpp:10:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6ab22921e9b55b4744b67a4bd93f8452
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_trace_span.cpp:10:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_trace_span.cpp:11:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 426b5393ed56e7221586b15a19a83823
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_trace_span.cpp:11:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_trace_span.cpp:12:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 052b88ebf54547b2c73bc613f566f881
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_trace_span.cpp:12:5: only virtual member functions can be marked 'override'

Found 7 defect(s) in mock_trace_span.cpp

[CRITICAL] /sep/tests/blender/mock_pattern_evolution.cpp:3:10: 'src/quantum/data.hpp' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3015ac0230224947f8badacfee652bf3
  Steps:
    1, mock_pattern_evolution.cpp:3:10: 'src/quantum/data.hpp' file not found

Found 1 defect(s) in mock_pattern_evolution.cpp

[CRITICAL] /sep/tests/blender/mock_blender_bridge.cpp:4:10: 'pattern/pattern_data.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bf5cca67ed23bf8f9eb633a7f188e23c
  Steps:
    1, mock_blender_bridge.cpp:4:10: 'pattern/pattern_data.h' file not found

Found 1 defect(s) in mock_blender_bridge.cpp

[CRITICAL] /sep/tests/blender/mock_cuda_wrappers.cpp:8:32: expected class name [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 135da66486758e301106624506607f8d
  Steps:
    1, mock_cuda_wrappers.cpp:8:32: expected class name

[CRITICAL] /sep/tests/blender/mock_cuda_wrappers.cpp:10:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ed9b428ce3c96db89825d4ce00e82e9f
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_cuda_wrappers.cpp:10:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_cuda_wrappers.cpp:11:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 67688d990c5b1732866dc31d57f81140
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_cuda_wrappers.cpp:11:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_cuda_wrappers.cpp:12:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a6a04436dfef4f6d40f845c4b744da66
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_cuda_wrappers.cpp:12:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_cuda_wrappers.cpp:13:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 770fe4cf5c65955c599a5b6d39fcb935
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_cuda_wrappers.cpp:13:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_cuda_wrappers.cpp:14:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1dd0fb29dc84ce3aca218dc881db944f
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_cuda_wrappers.cpp:14:5: only virtual member functions can be marked 'override'

Found 6 defect(s) in mock_cuda_wrappers.cpp

Found no defects in mesh_handler_test.cpp
[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:7:32: use of undeclared identifier 'blender' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 025cc071920d3faf545bf7cbb0a3b03f
  Steps:
    1, mock_mesh_handler.cpp:7:32: use of undeclared identifier 'blender'

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:7:41: expected class name [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0576a1f2691737dc8e5e79ab90b467c5
  Steps:
    1, mock_mesh_handler.cpp:7:41: expected class name

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:9:5: a type specifier is required for all declarations [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f2aca222d87b1600ed596c1ac6eafbf4
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:9:5: a type specifier is required for all declarations

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:9:5: expected ')' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d69ef9efa52fc2429e63abfb29255005
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:9:5: expected ')'

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:9:5: expected ';' at end of declaration list [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: be1bb3f63345b5afadb58b417d0843ba
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:9:5: expected ';' at end of declaration list

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:9:5: expected parameter declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e80770b626c7cca45b22226425100b87
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:9:5: expected parameter declarator

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:9:5: static assertion failed:  should be enclosed in parentheses. [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 70383ae58a4331ce9134cc62a296de04
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:9:5: static assertion failed:  should be enclosed in parentheses.

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:10:5: a type specifier is required for all declarations [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 844ef9f3b51d2fa7399f2cdf82cd00fd
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:10:5: a type specifier is required for all declarations

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:10:5: expected ')' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1a71343ba01a648748edbe02697abbca
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:10:5: expected ')'

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:10:5: expected ';' at end of declaration list [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 65a75ec23c645bf01561cd694d67164e
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:10:5: expected ';' at end of declaration list

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:10:5: expected parameter declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 93afa5c744029572124146727ee779c5
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:10:5: expected parameter declarator

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:10:5: static assertion failed:  should be enclosed in parentheses. [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c9e2647ac167a4afc0466dc75f2f3457
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:10:5: static assertion failed:  should be enclosed in parentheses.

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:11:5: a type specifier is required for all declarations [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 93047e3e72bf74a9b07f908a77700ec9
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:11:5: a type specifier is required for all declarations

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:11:5: expected ')' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5b7cd79923e96ffa0bf42dedc5519245
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:11:5: expected ')'

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:11:5: expected ';' at end of declaration list [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 89fb74ad7a80a395e814070377dc96bf
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:11:5: expected ';' at end of declaration list

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:11:5: expected parameter declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 27e3dffbe10aaaa57cf62733a574bfbb
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:11:5: expected parameter declarator

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:11:5: static assertion failed:  should be enclosed in parentheses. [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a877f13f564b65a4f375c59f73ff1ac2
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:11:5: static assertion failed:  should be enclosed in parentheses.

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:12:5: expected parameter declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f8fe0e1a675ee4ba4468769ffdbafcbe
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:12:5: expected parameter declarator

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:12:5: static assertion failed:  should be enclosed in parentheses. [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 46119c383ce287c0305a1a924c29bd08
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_mesh_handler.cpp:12:5: static assertion failed:  should be enclosed in parentheses.

Found 19 defect(s) in mock_mesh_handler.cpp

[CRITICAL] /sep/tests/blender/mock_pattern_compression.cpp:9:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 90e5f6385bc9c29bf606fbe4a1a9c287
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_pattern_compression.cpp:9:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_pattern_compression.cpp:9:98: parameter declarator cannot be qualified [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 351094502459c2ca48083a09a94ddb0b
  Steps:
    1, gmock-function-mocker.h:115:59: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:56: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-function-mocker.h:131:60: expanded from macro 'GMOCK_INTERNAL_MOCK_METHOD_ARG_4'
    4, mock_pattern_compression.cpp:9:98: parameter declarator cannot be qualified

[CRITICAL] /sep/tests/blender/mock_pattern_compression.cpp:10:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: df29fa455e49baa93fb2d56324f27ce8
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_pattern_compression.cpp:10:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_pattern_compression.cpp:11:5: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 72f1fd0ce854a156ad15083c6fe99dcc
  Steps:
    1, gmock-function-mocker.h:115:3: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:7: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-pp.h:5:30: expanded from macro 'GMOCK_PP_CAT'
    4, mock_pattern_compression.cpp:11:5: only virtual member functions can be marked 'override'

[CRITICAL] /sep/tests/blender/mock_pattern_compression.cpp:11:93: parameter declarator cannot be qualified [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d96eb8582b31e7ec2279536ecf284bf6
  Steps:
    1, gmock-function-mocker.h:115:59: expanded from macro 'MOCK_METHOD'
    2, gmock-pp.h:49:56: expanded from macro 'GMOCK_PP_VARIADIC_CALL'
    3, gmock-function-mocker.h:131:60: expanded from macro 'GMOCK_INTERNAL_MOCK_METHOD_ARG_4'
    4, mock_pattern_compression.cpp:11:93: parameter declarator cannot be qualified

Found 5 defect(s) in mock_pattern_compression.cpp

[CRITICAL] /sep/tests/blender/mock_pattern_evolution.cpp:6:10: 'pattern/pattern_data.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 470cd91159f1d1c9362efdb152e06de6
  Steps:
    1, mock_pattern_evolution.cpp:6:10: 'pattern/pattern_data.h' file not found

Found 1 defect(s) in mock_pattern_evolution.cpp

[CRITICAL] /sep/tests/blender/mock_mesh_handler.cpp:5:10: 'common/result.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9d1329b39788842f127faaf2d97682da
  Steps:
    1, mock_mesh_handler.cpp:5:10: 'common/result.h' file not found

Found 1 defect(s) in mock_mesh_handler.cpp

[CRITICAL] /sep/src/main.cpp:176:9: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c902c9b48fa5d12977ed652b84c8cac7
  Notes:
    1, main.cpp:176:9: sep::logging::shutdownLogging (fixit)
  Steps:
    1, logging.h:80:13: 'sep::logging::shutdownLogging' declared here
    2, main.cpp:176:9: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'?

[CRITICAL] /sep/src/main.cpp:187:9: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c902c9b48fa5d12977ed652b84c8cac7
  Notes:
    1, main.cpp:187:9: sep::logging::shutdownLogging (fixit)
  Steps:
    1, logging.h:80:13: 'sep::logging::shutdownLogging' declared here
    2, main.cpp:187:9: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'?

[CRITICAL] /sep/src/main.cpp:224:11: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b3835112e03d3b5ec51cf7d206d7b97f
  Notes:
    1, main.cpp:224:11: sep::logging::shutdownLogging (fixit)
  Steps:
    1, logging.h:80:13: 'sep::logging::shutdownLogging' declared here
    2, main.cpp:224:11: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'?

[CRITICAL] /sep/src/main.cpp:241:11: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b3835112e03d3b5ec51cf7d206d7b97f
  Notes:
    1, main.cpp:241:11: sep::logging::shutdownLogging (fixit)
  Steps:
    1, logging.h:80:13: 'sep::logging::shutdownLogging' declared here
    2, main.cpp:241:11: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'?

[CRITICAL] /sep/src/main.cpp:247:9: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c902c9b48fa5d12977ed652b84c8cac7
  Notes:
    1, main.cpp:247:9: sep::logging::shutdownLogging (fixit)
  Steps:
    1, logging.h:80:13: 'sep::logging::shutdownLogging' declared here
    2, main.cpp:247:9: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'?

[CRITICAL] /sep/src/main.cpp:253:9: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c902c9b48fa5d12977ed652b84c8cac7
  Notes:
    1, main.cpp:253:9: sep::logging::shutdownLogging (fixit)
  Steps:
    1, logging.h:80:13: 'sep::logging::shutdownLogging' declared here
    2, main.cpp:253:9: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'?

[CRITICAL] /sep/src/main.cpp:294:5: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b40d11ad7fcebe77f468e8318fb6c7d4
  Notes:
    1, main.cpp:294:5: sep::logging::shutdownLogging (fixit)
  Steps:
    1, logging.h:80:13: 'sep::logging::shutdownLogging' declared here
    2, main.cpp:294:5: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'?

[CRITICAL] /sep/src/main.cpp:300:3: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 299052bffce3943bd7d214b0d696ca24
  Notes:
    1, main.cpp:300:3: sep::logging::shutdownLogging (fixit)
  Steps:
    1, logging.h:80:13: 'sep::logging::shutdownLogging' declared here
    2, main.cpp:300:3: no member named 'shutdownLogging' in 'sep::logging::Manager'; did you mean 'sep::logging::shutdownLogging'?

Found 8 defect(s) in main.cpp

[LOW] /sep/src/memory/quantum_coherence_manager.cpp:121:14: Value stored to 'tier_analysis' during its initialization is never read [deadcode.DeadStores]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bd401bfb4e259755fb945e81b4f274cc
  Steps:
    1, quantum_coherence_manager.cpp:121:14: Value stored to 'tier_analysis' during its initialization is never read

Found 1 defect(s) in quantum_coherence_manager.cpp

[MEDIUM] /sep/src/core/engine.cpp:65:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:65:6: cast the expression to void to silence this warning
    2, engine.cpp:65:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:71:14: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 39120b7c5df7534093597522738993e6
  Steps:
    1, engine.cpp:71:14: cast the expression to void to silence this warning
    2, engine.cpp:71:14: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:76:18: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab6f901229b488bfc64f4551fa611064
  Steps:
    1, engine.cpp:76:18: cast the expression to void to silence this warning
    2, engine.cpp:76:18: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:80:18: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab6f901229b488bfc64f4551fa611064
  Steps:
    1, engine.cpp:80:18: cast the expression to void to silence this warning
    2, engine.cpp:80:18: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:85:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:85:10: cast the expression to void to silence this warning
    2, engine.cpp:85:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:88:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:88:10: cast the expression to void to silence this warning
    2, engine.cpp:88:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:109:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:109:6: cast the expression to void to silence this warning
    2, engine.cpp:109:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:170:95: unused parameter 'tick' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d01385ebb281083b3540719cdc37c216
  Steps:
    1, engine.cpp:170:95: unused parameter 'tick'

Found 8 defect(s) in engine.cpp

Found no defects in crow_request_adapter.cpp
[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:45:20: out-of-line definition of 'MemoryTierManager' does not match any declaration in 'sep::memory::MemoryTierManager' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b52b240fefae2317ad7266a7f46c84a8
  Steps:
    1, memory_tier_manager.hpp:44:7: MemoryTierManager defined here
    2, memory_tier_manager.cpp:45:20: out-of-line definition of 'MemoryTierManager' does not match any declaration in 'sep::memory::MemoryTierManager'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:50:23: no member named 'stm_size' in 'sep::config::MemoryThresholdConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d0bc2ee8efa87111de5ab94020f85af0
  Steps:
    1, memory_tier_manager.cpp:50:23: no member named 'stm_size' in 'sep::config::MemoryThresholdConfig'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:51:23: no member named 'mtm_size' in 'sep::config::MemoryThresholdConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 09d2116d39db6acd81a5fe6abb1107b0
  Steps:
    1, memory_tier_manager.cpp:51:23: no member named 'mtm_size' in 'sep::config::MemoryThresholdConfig'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:52:23: no member named 'ltm_size' in 'sep::config::MemoryThresholdConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 50cc29e8ce7ba32ffe1d89475a4749a8
  Steps:
    1, memory_tier_manager.cpp:52:23: no member named 'ltm_size' in 'sep::config::MemoryThresholdConfig'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:65:23: no member named 'stm_size' in 'sep::config::MemoryThresholdConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d0bc2ee8efa87111de5ab94020f85af0
  Steps:
    1, memory_tier_manager.cpp:65:23: no member named 'stm_size' in 'sep::config::MemoryThresholdConfig'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:66:23: no member named 'mtm_size' in 'sep::config::MemoryThresholdConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 09d2116d39db6acd81a5fe6abb1107b0
  Steps:
    1, memory_tier_manager.cpp:66:23: no member named 'mtm_size' in 'sep::config::MemoryThresholdConfig'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:67:23: no member named 'ltm_size' in 'sep::config::MemoryThresholdConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 50cc29e8ce7ba32ffe1d89475a4749a8
  Steps:
    1, memory_tier_manager.cpp:67:23: no member named 'ltm_size' in 'sep::config::MemoryThresholdConfig'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:289:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:289:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:336:26: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 005a58abc587efa85ab38a672b887462
  Steps:
    1, memory_tier_manager.cpp:336:26: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:420:23: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b88d2d0afa7e06912c1d00d5827b227a
  Steps:
    1, memory_tier_manager.cpp:420:23: implicit conversion increases floating-point precision: 'float' to 'double'

Found 10 defect(s) in memory_tier_manager.cpp

[CRITICAL] /usr/include/c++/15/bits/unique_ptr.h:1085:34: no matching constructor for initialization of 'sep::memory::MemoryTierManager' [clang-diagnostic-error]
    { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }
                                 ^
  Report hash: a2489e18cdf5aef8d201baec5655c583
  Steps:
    1, memory_tier_manager.cpp:40:26: in instantiation of function template specialization 'std::make_unique<sep::memory::MemoryTierManager, const sep::config::MemoryThresholdConfig &>' requested here
    2, memory_tier_manager.hpp:44:7: candidate constructor (the implicit copy constructor) not viable: no known conversion from 'const sep::config::MemoryThresholdConfig' to 'const MemoryTierManager' for 1st argument
    3, memory_tier_manager.hpp:69:5: candidate constructor not viable: no known conversion from 'const sep::config::MemoryThresholdConfig' to 'const Config' for 1st argument
    4, memory_tier_manager.cpp:58:20: candidate constructor not viable: requires 0 arguments, but 1 was provided
    5, unique_ptr.h:1085:34: no matching constructor for initialization of 'sep::memory::MemoryTierManager'

Found 1 defect(s) in unique_ptr.h

[LOW] /sep/include/api/bridge_internal.hpp:12:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5ca7fa69e4196bf69f99f847456b1c42
  Steps:
    1, processor.h:82:7: a definition of 'Processor' is found here
    2, bridge_internal.hpp:12:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in bridge_internal.hpp

[LOW] /sep/include/compat/component_bridge.h:6:8: no definition found for 'SEPBlenderBridge', but a definition with the same name 'SEPBlenderBridge' found in another namespace 'sep' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4a93f265990d8ce27d7d28d2ab5cc95a
  Steps:
    1, types.h:58:8: a definition of 'SEPBlenderBridge' is found here
    2, component_bridge.h:6:8: no definition found for 'SEPBlenderBridge', but a definition with the same name 'SEPBlenderBridge' found in another namespace 'sep'

[CRITICAL] /sep/include/compat/component_bridge.h:6:8: definition of type 'SEPBlenderBridge' conflicts with typedef of the same name [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f4eaf5a49caaca33e720b6d4462a0a3a
  Steps:
    1, types.h:66:31: 'SEPBlenderBridge' declared here
    2, component_bridge.h:6:8: definition of type 'SEPBlenderBridge' conflicts with typedef of the same name

Found 2 defect(s) in component_bridge.h

[CRITICAL] /sep/src/blender/blender_integration.cpp:31:16: definition of implicitly declared default constructor [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 08ee2db41ab2e655d2f774a5ea2e2bdd
  Steps:
    1, blender_integration.cpp:31:16: definition of implicitly declared default constructor

[CRITICAL] /sep/src/blender/blender_integration.cpp:32:7: member initializer 'thread_running_' does not name a non-static data member or base class [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8a05201aa3c256333149cc08b6035a46
  Steps:
    1, blender_integration.cpp:32:7: member initializer 'thread_running_' does not name a non-static data member or base class

[CRITICAL] /sep/src/blender/blender_integration.cpp:33:7: member initializer 'gpu_context_' does not name a non-static data member or base class [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3b1cf6c2cc6fe912759857c09199b073
  Steps:
    1, blender_integration.cpp:33:7: member initializer 'gpu_context_' does not name a non-static data member or base class

[CRITICAL] /sep/src/blender/blender_integration.cpp:38:16: definition of implicitly declared destructor [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 23cfc80e06404fa66bcab340394097f1
  Steps:
    1, blender_integration.cpp:38:16: definition of implicitly declared destructor

[CRITICAL] /sep/src/blender/blender_integration.cpp:41:47: out-of-line definition of 'create' does not match any declaration in 'sep::pattern::BlenderBridge' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 10c7c0ccd9384f344a6277200cfa5904
  Steps:
    1, bridge.h:174:43: BlenderBridge defined here
    2, blender_integration.cpp:41:47: out-of-line definition of 'create' does not match any declaration in 'sep::pattern::BlenderBridge'

[CRITICAL] /sep/src/blender/blender_integration.cpp:47:31: out-of-line definition of 'init' does not match any declaration in 'sep::pattern::BlenderBridge' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 797af22c33c0487ccb36565ebd60c574
  Steps:
    1, bridge.h:174:43: BlenderBridge defined here
    2, blender_integration.cpp:47:31: out-of-line definition of 'init' does not match any declaration in 'sep::pattern::BlenderBridge'

[CRITICAL] /sep/src/blender/blender_integration.cpp:54:9: use of undeclared identifier 'thread_running_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cbea9f139829ef5767b9ac854888ecde
  Steps:
    1, blender_integration.cpp:54:9: use of undeclared identifier 'thread_running_'

[CRITICAL] /sep/src/blender/blender_integration.cpp:60:5: use of undeclared identifier 'gpu_context_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6722d12ac5f13a9fa143f9dbabff8e7c
  Steps:
    1, blender_integration.cpp:60:5: use of undeclared identifier 'gpu_context_'

[CRITICAL] /sep/src/blender/blender_integration.cpp:63:5: use of undeclared identifier 'pattern_processor_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7c08d0d1eb0dfa88af77eaa99d215354
  Steps:
    1, blender_integration.cpp:63:5: use of undeclared identifier 'pattern_processor_'

[CRITICAL] /sep/src/blender/blender_integration.cpp:64:10: use of undeclared identifier 'pattern_processor_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 25e40230e7cb519f9f006571ec03a3b3
  Steps:
    1, blender_integration.cpp:64:10: use of undeclared identifier 'pattern_processor_'

[CRITICAL] /sep/src/blender/blender_integration.cpp:66:9: use of undeclared identifier 'notifyError' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c63d3ebff1a669d028cedb0e18d2856d
  Steps:
    1, blender_integration.cpp:66:9: use of undeclared identifier 'notifyError'

[CRITICAL] /sep/src/blender/blender_integration.cpp:73:5: use of undeclared identifier 'startProcessingThread' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7c8173be47a9e9bce63b220e90b61877
  Steps:
    1, blender_integration.cpp:73:5: use of undeclared identifier 'startProcessingThread'

[CRITICAL] /sep/src/blender/blender_integration.cpp:78:31: out-of-line definition of 'registerObject' does not match any declaration in 'sep::pattern::BlenderBridge' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f4469e6de58e3ac987887ef9e0d3d4d4
  Steps:
    1, bridge.h:174:43: BlenderBridge defined here
    2, blender_integration.cpp:78:31: out-of-line definition of 'registerObject' does not match any declaration in 'sep::pattern::BlenderBridge'

[CRITICAL] /sep/src/blender/blender_integration.cpp:80:10: use of undeclared identifier 'thread_running_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9360b5159eae2f45f6606fed63c57a7f
  Steps:
    1, blender_integration.cpp:80:10: use of undeclared identifier 'thread_running_'

[CRITICAL] /sep/src/blender/blender_integration.cpp:98:9: no type named 'ObjectState' in 'sep::pattern::BlenderBridge'; did you mean simply 'ObjectState'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 02b82a8ffa37a08ec6e9400dbed190fb
  Notes:
    1, blender_integration.cpp:98:9: ObjectState (fixit)
  Steps:
    1, pattern_bridge.h:35:8: 'ObjectState' declared here
    2, blender_integration.cpp:98:9: no type named 'ObjectState' in 'sep::pattern::BlenderBridge'; did you mean simply 'ObjectState'?

[CRITICAL] /sep/src/blender/blender_integration.cpp:114:13: use of undeclared identifier 'notifyError' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2654846bbd58195bde979adb042d80a3
  Steps:
    1, blender_integration.cpp:114:13: use of undeclared identifier 'notifyError'

[CRITICAL] /sep/src/blender/blender_integration.cpp:119:31: use of undeclared identifier 'next_handle_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1e19bb8a10977090da9e647650f86055
  Steps:
    1, blender_integration.cpp:119:31: use of undeclared identifier 'next_handle_'

[MEDIUM] /sep/src/blender/blender_integration.cpp:123:45: parentheses were disambiguated as a function declaration [clang-diagnostic-vexing-parse]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a46728a5f48812a5001b1f61ab8e9d3d
  Steps:
    1, blender_integration.cpp:123:46: add a pair of parentheses to declare a variable
    2, blender_integration.cpp:123:45: parentheses were disambiguated as a function declaration

[CRITICAL] /sep/src/blender/blender_integration.cpp:123:46: unknown type name 'objects_mutex_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8e553d1bfdf0fa0b1c0a9cd74c0f3040
  Steps:
    1, blender_integration.cpp:123:46: unknown type name 'objects_mutex_'

Found 19 defect(s) in blender_integration.cpp

[MEDIUM] /sep/src/blender/blender_integration.cpp:30:7: field 'thread_running_' will be initialized after field 'gpu_context_' [clang-diagnostic-reorder-ctor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dfc6c665bd56d3f5e87cab3824f381f6
  Notes:
    1, blender_integration.cpp:30:7: gpu_context_(nullptr) (fixit)
    2, blender_integration.cpp:30:7: gpu_context_(nullptr) (fixit)
    3, blender_integration.cpp:30:7: ~~~~~~~~~~~~~~~~~~~~~ (fixit)
    4, blender_integration.cpp:30:7: thread_running_(false) (fixit)
  Steps:
    1, blender_integration.cpp:30:7: field 'thread_running_' will be initialized after field 'gpu_context_'

Found 1 defect(s) in blender_integration.cpp

[MEDIUM] /sep/extern/cycles/src/device/device.h:162:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9daf1764d35eb612b90b3de43ceb1766
  Steps:
    1, device.h:162:5: cast the expression to void to silence this warning
    2, device.h:162:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/cycles/src/device/device.h:163:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cb5ae6914026f22d6c49feb9ff4e226f
  Steps:
    1, device.h:163:5: cast the expression to void to silence this warning
    2, device.h:163:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 2 defect(s) in device.h

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:176:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __AO__
        ^
  Report hash: b4de53a194a60d1a19d5ede17a79fac8
  Steps:
    1, types.h:176:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:177:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CAUSTICS_TRICKS__
        ^
  Report hash: db4df5cf1bb11fc9095958e135939d50
  Steps:
    1, types.h:177:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:178:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CLAMP_SAMPLE__
        ^
  Report hash: a8455f4a9f6a3aeaffd3f09fa0ec3871
  Steps:
    1, types.h:178:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:179:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __DENOISING_FEATURES__
        ^
  Report hash: d6ccc908fb0544bd34ec368003ff693c
  Steps:
    1, types.h:179:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:180:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __DPDU__
        ^
  Report hash: b87cd2e6a12c419ad315e5b0ec76acfe
  Steps:
    1, types.h:180:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:181:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __HAIR__
        ^
  Report hash: 2a3bd20338dfa6fbd112bbf6cc3e0c0b
  Steps:
    1, types.h:181:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:182:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __LIGHT_LINKING__
        ^
  Report hash: 1e4dca011c117bf7e03f8302e3281878
  Steps:
    1, types.h:182:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:183:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __SHADOW_LINKING__
        ^
  Report hash: cf10ad33a4d91ccadcd5bcf26975e0e2
  Steps:
    1, types.h:183:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:184:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __LIGHT_TREE__
        ^
  Report hash: 6c97dea5942675c6696f458cfe2d1a0c
  Steps:
    1, types.h:184:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:185:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __OBJECT_MOTION__
        ^
  Report hash: 1fa04db2efa801c0562ea810017e8e39
  Steps:
    1, types.h:185:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:186:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __MNEE__
        ^
  Report hash: cd45058b45877584193933e028e922a9
  Steps:
    1, types.h:186:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:187:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __PASSES__
        ^
  Report hash: 63dbb64d61351f844a37da10bfc6adac
  Steps:
    1, types.h:187:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:188:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __POINTCLOUD__
        ^
  Report hash: 7d385b809df68f2b3a95a46c96161f28
  Steps:
    1, types.h:188:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:189:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __PRINCIPLED_HAIR__
        ^
  Report hash: f9a9aac9b2a74e3e569ab1decf3c8935
  Steps:
    1, types.h:189:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:190:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __RAY_DIFFERENTIALS__
        ^
  Report hash: 9b0dc76a2e8af5874ce0037d6de04501
  Steps:
    1, types.h:190:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:191:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __SHADER_RAYTRACE__
        ^
  Report hash: e8efa4af324f3763d9063446ecb3ac58
  Steps:
    1, types.h:191:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:192:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __SHADOW_CATCHER__
        ^
  Report hash: d5eb6dc9c1960453862926d5c8b24f0c
  Steps:
    1, types.h:192:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:193:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __SHADOW_RECORD_ALL__
        ^
  Report hash: e0306564d8604e3214892dcb8d714f22
  Steps:
    1, types.h:193:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:194:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __SUBSURFACE__
        ^
  Report hash: 3dcd657cef9752dddc7bc7c16df00d1b
  Steps:
    1, types.h:194:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:195:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __SVM__
        ^
  Report hash: 955b48ecb94e1105687e5e2c76a4bfb4
  Steps:
    1, types.h:195:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:196:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __TRANSPARENT_SHADOWS__
        ^
  Report hash: fc861c83c7cd011e28fdcf317f24fd84
  Steps:
    1, types.h:196:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:197:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __VISIBILITY_FLAG__
        ^
  Report hash: fbfb4f97d4475e0735132e00f1fc43a2
  Steps:
    1, types.h:197:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:198:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __VOLUME__
        ^
  Report hash: 27fcecc2f31c7e11b294cfb1f246d96c
  Steps:
    1, types.h:198:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:212:11: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#  define __VOLUME_RECORD_ALL__
          ^
  Report hash: 05466fa229c88e4ae05ba6a79d52a029
  Steps:
    1, types.h:212:11: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/kernel/types.h:276:11: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#  define __BVH_LOCAL__
          ^
  Report hash: 2733b0357272411d955781996f812085
  Steps:
    1, types.h:276:11: macro name is a reserved identifier

Found 25 defect(s) in types.h

[MEDIUM] /sep/extern/cycles/src/util/array.h:57:3: move constructors should be marked noexcept [performance-noexcept-move-constructor]
  array(array &&from)
  ^
  Report hash: 7bc418750c01d3e93819ea035bb3ef22
  Notes:
    1, array.h:57:3: { (fixit)
    2, array.h:57:2: noexcept  (fixit)
  Steps:
    1, array.h:57:3: move constructors should be marked noexcept

Found 1 defect(s) in array.h

[MEDIUM] /sep/extern/cycles/src/util/atomic_ops.h:62:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __ATOMIC_OPS_H__
        ^
  Report hash: 96a7264b76615300167d08297db58108
  Steps:
    1, atomic_ops.h:62:9: macro name is a reserved identifier

Found 1 defect(s) in atomic_ops.h

[MEDIUM] /sep/extern/cycles/src/util/defines.h:16:11: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#  define __KERNEL_64_BIT__
          ^
  Report hash: 4d1c1d73ace1d408ea5f9ac3ef65aa94
  Steps:
    1, defines.h:16:11: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/util/defines.h:54:15: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#      define __forceinline inline __attribute__((always_inline))
              ^
  Report hash: 1febd954762d11cb93384df36ced8d1b
  Steps:
    1, defines.h:54:15: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/util/defines.h:73:11: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#  define __KERNEL_WITH_SSE_ALIGN__
          ^
  Report hash: 176cb53e177d6fa60e9f016d1f8e4dcf
  Steps:
    1, defines.h:73:11: macro name is a reserved identifier

Found 3 defect(s) in defines.h

[LOW] /sep/extern/cycles/src/util/image_impl.h:7:10: circular header file dependency detected while including 'image.h', please check the include path [misc-header-include-cycle]
#include "util/image.h"
         ^
  Report hash: 0e373f7a2d88d640d12e83b2836c594c
  Steps:
    1, image.h:106:10: 'image_impl.h' included from here
    2, tile.h:11:10: 'image.h' included from here
    3, image_impl.h:7:10: circular header file dependency detected while including 'image.h', please check the include path

[MEDIUM] /sep/extern/cycles/src/util/image_impl.h:12:1: do not use unnamed namespaces in header files [google-build-namespaces]
namespace {
^
  Report hash: 512d615d047024aa85fb8181cfa79c09
  Steps:
    1, image_impl.h:12:1: do not use unnamed namespaces in header files

[HIGH] /sep/extern/cycles/src/util/image_impl.h:89:38: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead [bugprone-incorrect-roundings]
  const size_t kernel_size = (size_t)(inv_scale_factor + 0.5f);
                                     ^
  Report hash: e9f3d247fca8dc4a809ef5a019ff2392
  Steps:
    1, image_impl.h:89:38: casting (double + 0.5) to integer leads to incorrect rounding; consider using lround (#include <cmath>) instead

Found 3 defect(s) in image_impl.h

[MEDIUM] /sep/extern/cycles/src/util/intern/atomic_ops_ext.h:53:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __ATOMIC_OPS_EXT_H__
        ^
  Report hash: 4accb800ddba345d4ba86caefaff648b
  Steps:
    1, atomic_ops_ext.h:53:9: macro name is a reserved identifier

Found 1 defect(s) in atomic_ops_ext.h

[MEDIUM] /sep/extern/cycles/src/util/intern/atomic_ops_unix.h:53:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __ATOMIC_OPS_UNIX_H__
        ^
  Report hash: 85ab00e39eac9c5592f2f0501bd60793
  Steps:
    1, atomic_ops_unix.h:53:9: macro name is a reserved identifier

Found 1 defect(s) in atomic_ops_unix.h

[MEDIUM] /sep/extern/cycles/src/util/intern/atomic_ops_utils.h:53:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __ATOMIC_OPS_UTILS_H__
        ^
  Report hash: beba42a545d98f9b0b46faf10e88eca5
  Steps:
    1, atomic_ops_utils.h:53:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/util/intern/atomic_ops_utils.h:70:11: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#  define _ATOMIC_LIKELY(x)       __builtin_expect(!!(x), 1)
          ^
  Report hash: 4e8fffa762aa7e78a4147d7838ebdd77
  Steps:
    1, atomic_ops_utils.h:70:11: macro name is a reserved identifier

[MEDIUM] /sep/extern/cycles/src/util/intern/atomic_ops_utils.h:71:11: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#  define _ATOMIC_UNLIKELY(x)     __builtin_expect(!!(x), 0)
          ^
  Report hash: f70b97fd317706accc302a2f0d262eb9
  Steps:
    1, atomic_ops_utils.h:71:11: macro name is a reserved identifier

Found 3 defect(s) in atomic_ops_utils.h

[MEDIUM] /sep/extern/cycles/src/util/math_base.h:257:23: identifier '__float_as_int' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
ccl_device_inline int __float_as_int(const float f)
                      ^
  Report hash: 5b6ea2e1604ca00bf109310434a694c9
  Steps:
    1, math_base.h:257:23: identifier '__float_as_int' is reserved because it starts with '__'

[MEDIUM] /sep/extern/cycles/src/util/math_base.h:267:25: identifier '__int_as_float' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
ccl_device_inline float __int_as_float(const int i)
                        ^
  Report hash: 232f1f80834c37ce82e83bfbf0c4e299
  Steps:
    1, math_base.h:267:25: identifier '__int_as_float' is reserved because it starts with '__'

[MEDIUM] /sep/extern/cycles/src/util/math_base.h:277:24: identifier '__float_as_uint' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
ccl_device_inline uint __float_as_uint(const float f)
                       ^
  Report hash: f7d0a00e173123acf94e824f417588ce
  Steps:
    1, math_base.h:277:24: identifier '__float_as_uint' is reserved because it starts with '__'

[MEDIUM] /sep/extern/cycles/src/util/math_base.h:287:25: identifier '__uint_as_float' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
ccl_device_inline float __uint_as_float(const uint i)
                        ^
  Report hash: 8e411a78180178ed79d294f0c5288ea8
  Steps:
    1, math_base.h:287:25: identifier '__uint_as_float' is reserved because it starts with '__'

Found 4 defect(s) in math_base.h

[MEDIUM] /sep/extern/cycles/src/util/math_float4.h:618:24: identifier '__float4_as_int4' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
ccl_device_inline int4 __float4_as_int4(const float4 f)
                       ^
  Report hash: da7a0ae3eb41d4c166c659206dc14947
  Steps:
    1, math_float4.h:618:24: identifier '__float4_as_int4' is reserved because it starts with '__'

[MEDIUM] /sep/extern/cycles/src/util/math_float4.h:628:26: identifier '__int4_as_float4' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
ccl_device_inline float4 __int4_as_float4(const int4 i)
                         ^
  Report hash: 2007717ef4861bb32f33ab0990bc1ad1
  Steps:
    1, math_float4.h:628:26: identifier '__int4_as_float4' is reserved because it starts with '__'

Found 2 defect(s) in math_float4.h

[MEDIUM] /sep/extern/cycles/src/util/optimization.h:22:13: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#    define __KERNEL_SSE42__
            ^
  Report hash: 39c1fc4a4b76198e73b4e98e7657cbea
  Steps:
    1, optimization.h:22:13: macro name is a reserved identifier

Found 1 defect(s) in optimization.h

[MEDIUM] /sep/extern/cycles/src/util/progress.h:35:3: operator=() should take 'Progress const&', 'Progress&&' or 'Progress' [misc-unconventional-assign-operator]
  Progress &operator=(Progress &progress)
  ^
  Report hash: 427e0bdc50dd307bf449e84560fb8431
  Steps:
    1, progress.h:35:3: operator=() should take 'Progress const&', 'Progress&&' or 'Progress'

Found 1 defect(s) in progress.h

[MEDIUM] /sep/extern/cycles/src/util/simd.h:397:24: identifier '__bsf' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
__forceinline uint32_t __bsf(const uint32_t x)
                       ^
  Report hash: a599e69f297e7bf13eacd7952e350e0c
  Steps:
    1, simd.h:397:24: identifier '__bsf' is reserved because it starts with '__'

[MEDIUM] /sep/extern/cycles/src/util/simd.h:407:24: identifier '__bsr' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
__forceinline uint32_t __bsr(const uint32_t x)
                       ^
  Report hash: f56bb77a9e8245657d95971e374481d7
  Steps:
    1, simd.h:407:24: identifier '__bsr' is reserved because it starts with '__'

[MEDIUM] /sep/extern/cycles/src/util/simd.h:417:24: identifier '__btc' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
__forceinline uint32_t __btc(const uint32_t x, const uint32_t bit)
                       ^
  Report hash: ea7d78c1ad51445a1be5862d000c17a0
  Steps:
    1, simd.h:417:24: identifier '__btc' is reserved because it starts with '__'

[MEDIUM] /sep/extern/cycles/src/util/simd.h:423:24: identifier '__bsf' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
__forceinline uint32_t __bsf(const uint64_t x)
                       ^
  Report hash: bfce6f5151a6cdbc82c5a955378f40cf
  Steps:
    1, simd.h:423:24: identifier '__bsf' is reserved because it starts with '__'

[MEDIUM] /sep/extern/cycles/src/util/simd.h:433:24: identifier '__bsr' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
__forceinline uint32_t __bsr(const uint64_t x)
                       ^
  Report hash: e53e11bc142e45d7460ac544cb7586d4
  Steps:
    1, simd.h:433:24: identifier '__bsr' is reserved because it starts with '__'

[MEDIUM] /sep/extern/cycles/src/util/simd.h:443:24: identifier '__btc' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
__forceinline uint64_t __btc(const uint64_t x, const uint32_t bit)
                       ^
  Report hash: 33ac7d651e4af8eeb351f3c6dc8d67f6
  Steps:
    1, simd.h:443:24: identifier '__btc' is reserved because it starts with '__'

Found 6 defect(s) in simd.h

[MEDIUM] /sep/extern/cycles/src/util/types_base.h:76:11: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#  define __KERNEL_PRINTF__
          ^
  Report hash: 17646f23c65ee1aeae78ff51c662dc96
  Steps:
    1, types_base.h:76:11: macro name is a reserved identifier

Found 1 defect(s) in types_base.h

[CRITICAL] /sep/extern/crow/utility.h:684:13: use of class template 'crow' requires template arguments [clang-diagnostic-error]
            CROW_LOG_ERROR << "Unhandled character in path sanitization: " << data[i];
            ^
  Report hash: 7aeef18e582125de491e6c084a364b5f
  Steps:
    1, logging.h:52:24: expanded from macro 'CROW_LOG_ERROR'
    2, crow_isolation.h:179:11: template is declared here
    3, utility.h:684:13: use of class template 'crow' requires template arguments

Found 1 defect(s) in utility.h

Found no defects in logging_middleware.cpp
[LOW] /usr/local/include/crow/http_parser_merged.h:54:16: no definition found for 'http_parser', but a definition with the same name 'http_parser' found in another namespace 'crow::http_parser' [bugprone-forward-declaration-namespace]
typedef struct http_parser http_parser;
               ^
  Report hash: 4913a4fabdcbe721f3f7b89c678e682b
  Steps:
    1, crow_isolation.h:223:16: a definition of 'http_parser' is found here
    2, http_parser_merged.h:54:16: no definition found for 'http_parser', but a definition with the same name 'http_parser' found in another namespace 'crow::http_parser'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:54:16: implicit declaration introduced by elaborated type conflicts with a non-class type of the same name [clang-diagnostic-error]
typedef struct http_parser http_parser;
               ^
  Report hash: 1292dfb91345e7746687d0e6b985d3d3
  Steps:
    1, crow_isolation.h:211:15: 'http_parser' declared here
    2, http_parser_merged.h:54:16: implicit declaration introduced by elaborated type conflicts with a non-class type of the same name

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:75:30: unknown type name 'http_parser'; did you mean 'http_parser::http_parser'? [clang-diagnostic-error]
typedef int (*http_data_cb) (http_parser*, const char *at, size_t length);
                             ^
  Report hash: 12acc32d86ae9993e8130809afe9c97e
  Steps:
    1, crow_isolation.h:223:16: 'http_parser::http_parser' declared here
    2, http_parser_merged.h:75:30: unknown type name 'http_parser'; did you mean 'http_parser::http_parser'?

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:76:25: unknown type name 'http_parser'; did you mean 'http_parser::http_parser'? [clang-diagnostic-error]
typedef int (*http_cb) (http_parser*);
                        ^
  Report hash: c1694d89dd340011b5592f253d91a445
  Steps:
    1, crow_isolation.h:223:16: 'http_parser::http_parser' declared here
    2, http_parser_merged.h:76:25: unknown type name 'http_parser'; did you mean 'http_parser::http_parser'?

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:147:12: redefinition of 'http_parser' as different kind of symbol [clang-diagnostic-error]
    struct http_parser
           ^
  Report hash: 90411b4bc45f45a6514b4760fa04df15
  Steps:
    1, crow_isolation.h:211:15: previous definition is here
    2, http_parser_merged.h:147:12: redefinition of 'http_parser' as different kind of symbol

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:485:45: unknown type name 'http_parser'; did you mean 'http_parser::http_parser'? [clang-diagnostic-error]
parse_url_char(enum state s, const char ch, http_parser *parser, const char* url_mark, const char* p)
                                            ^
  Report hash: 506850bd553866521bb4b8fff0eb04f2
  Steps:
    1, crow_isolation.h:223:16: 'http_parser::http_parser' declared here
    2, http_parser_merged.h:485:45: unknown type name 'http_parser'; did you mean 'http_parser::http_parser'?

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:587:19: no member named 'qs_point' in 'crow::http_parser::http_parser' [clang-diagnostic-error]
          parser->qs_point = p - url_mark;
                  ^
  Report hash: f75cade68d24596d646be0d9dde19023
  Steps:
    1, http_parser_merged.h:587:19: no member named 'qs_point' in 'crow::http_parser::http_parser'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:607:19: no member named 'qs_point' in 'crow::http_parser::http_parser' [clang-diagnostic-error]
          parser->qs_point = p - url_mark;
                  ^
  Report hash: f75cade68d24596d646be0d9dde19023
  Steps:
    1, http_parser_merged.h:607:19: no member named 'qs_point' in 'crow::http_parser::http_parser'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:633:36: unexpected namespace name 'http_parser': expected expression [clang-diagnostic-error]
inline size_t http_parser_execute (http_parser *parser,
                                   ^
  Report hash: 6b21c01b019aa0e5bd35f171aa5f8cad
  Steps:
    1, http_parser_merged.h:633:36: unexpected namespace name 'http_parser': expected expression

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:633:49: use of undeclared identifier 'parser'; did you mean 'pause'? [clang-diagnostic-error]
inline size_t http_parser_execute (http_parser *parser,
                                                ^
  Report hash: c5a5757b6befb24633d028a44d913d46
  Steps:
    1, unistd.h:489:12: 'pause' declared here
    2, http_parser_merged.h:633:49: use of undeclared identifier 'parser'; did you mean 'pause'?

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:634:29: expected expression [clang-diagnostic-error]
                            const http_parser_settings *settings,
                            ^
  Report hash: f2a1cd0d755bacbe6ec8865ad4775e79
  Steps:
    1, http_parser_merged.h:634:29: expected expression

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:635:29: expected expression [clang-diagnostic-error]
                            const char *data,
                            ^
  Report hash: 61c1cf3641180ff7c32a67fdcd5363db
  Steps:
    1, http_parser_merged.h:635:29: expected expression

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:636:29: unexpected type name 'size_t': expected expression [clang-diagnostic-error]
                            size_t len)
                            ^
  Report hash: e45a91fbe39414c987572367a1aff62f
  Steps:
    1, http_parser_merged.h:636:29: unexpected type name 'size_t': expected expression

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:636:40: expected ';' after top level declarator [clang-diagnostic-error]
                            size_t len)
                                       ^
  Report hash: 0b7ed04dbfd00dd481fd25dfaa770fda
  Steps:
    1, http_parser_merged.h:636:40: expected ';' after top level declarator

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:1930:3: variable has incomplete type 'void' [clang-diagnostic-error]
  http_parser_init(http_parser* parser)
  ^
  Report hash: 51c9899c9553ee370a72537003f32ea8
  Steps:
    1, http_parser_merged.h:1930:3: variable has incomplete type 'void'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:1930:20: unexpected namespace name 'http_parser': expected expression [clang-diagnostic-error]
  http_parser_init(http_parser* parser)
                   ^
  Report hash: 236d56fcd4f57f34c2f6ec1d069644bc
  Steps:
    1, http_parser_merged.h:1930:20: unexpected namespace name 'http_parser': expected expression

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:1930:33: use of undeclared identifier 'parser'; did you mean 'pause'? [clang-diagnostic-error]
  http_parser_init(http_parser* parser)
                                ^
  Report hash: e7ce575f63450520eeac7b7d342f1b09
  Steps:
    1, unistd.h:489:12: 'pause' declared here
    2, http_parser_merged.h:1930:33: use of undeclared identifier 'parser'; did you mean 'pause'?

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:1930:40: expected ';' after top level declarator [clang-diagnostic-error]
  http_parser_init(http_parser* parser)
                                       ^
  Report hash: 85200c3db78b878d183ae9c08a5532cf
  Steps:
    1, http_parser_merged.h:1930:40: expected ';' after top level declarator

[LOW] /usr/local/include/crow/http_parser_merged.h:1973:33: no definition found for 'http_parser', but a definition with the same name 'http_parser' found in another namespace 'crow::http_parser' [bugprone-forward-declaration-namespace]
http_body_is_final(const struct http_parser *parser) {
                                ^
  Report hash: cb482052e449bd5e945f9a872dc6f00c
  Steps:
    1, crow_isolation.h:223:16: a definition of 'http_parser' is found here
    2, http_parser_merged.h:1973:33: no definition found for 'http_parser', but a definition with the same name 'http_parser' found in another namespace 'crow::http_parser'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:1973:33: typedef 'http_parser' cannot be referenced with the 'struct' specifier [clang-diagnostic-error]
http_body_is_final(const struct http_parser *parser) {
                                ^
  Report hash: 1dcab7be9517ca94fad0521fe66930cc
  Steps:
    1, http_parser_merged.h:54:28: declared here
    2, http_parser_merged.h:1973:33: typedef 'http_parser' cannot be referenced with the 'struct' specifier

Found 20 defect(s) in http_parser_merged.h

[LOW] /sep/src/memory/quantum_coherence_manager.cpp:123:14: Value stored to 'tier_analysis' during its initialization is never read [deadcode.DeadStores]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bd401bfb4e259755fb945e81b4f274cc
  Steps:
    1, quantum_coherence_manager.cpp:123:14: Value stored to 'tier_analysis' during its initialization is never read

Found 1 defect(s) in quantum_coherence_manager.cpp

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:123:14: unused variable 'tier_analysis' [clang-diagnostic-unused-variable]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 01708387c81904505d93db53cf5517df
  Steps:
    1, quantum_coherence_manager.cpp:123:14: unused variable 'tier_analysis'

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:551:73: unused parameter 'tier_analysis' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7b77e7e39fcd03264c4a0bce9f712a22
  Steps:
    1, quantum_coherence_manager.cpp:551:73: unused parameter 'tier_analysis'

Found 2 defect(s) in quantum_coherence_manager.cpp

[MEDIUM] /sep/src/audio/pipewire_capture.cpp:86:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0e308df13dda55c08b2861ff936dfe97
  Steps:
    1, pipewire_capture.cpp:86:9: cast the expression to void to silence this warning
    2, pipewire_capture.cpp:86:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 1 defect(s) in pipewire_capture.cpp

[LOW] /usr/include/pipewire-0.3/pipewire/control.h:25:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: fd8eadb9bf09754217dc928cc66a2a3f
  Steps:
    1, impl.h:24:10: 'control.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, control.h:25:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in control.h

[LOW] /usr/include/pipewire-0.3/pipewire/core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path [misc-header-include-cycle]
#include <pipewire/context.h>
         ^
  Report hash: 1daf003ee682828fd0bcf5e77b8a5137
  Steps:
    1, context.h:48:10: 'core.h' included from here
    2, pipewire_includes.h:21:10: 'context.h' included from here
    3, core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path

[MEDIUM] /usr/include/pipewire-0.3/pipewire/core.h:396:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  vsnprintf(buffer, sizeof(buffer), message, args);
  ^
  Report hash: 0ed97ae05ae09f99abd5bfafd2bde595
  Steps:
    1, core.h:396:2: cast the expression to void to silence this warning
    2, core.h:396:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 2 defect(s) in core.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: c36587793019ad5530065d0f43f044ca
  Steps:
    1, resource.h:37:10: 'impl-client.h' included from here
    2, impl-core.h:29:10: 'resource.h' included from here
    3, impl.h:25:10: 'impl-core.h' included from here
    4, global.h:39:10: 'impl.h' included from here
    5, pipewire_includes.h:22:10: 'global.h' included from here
    6, impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path

[LOW] /usr/include/pipewire-0.3/pipewire/impl-client.h:68:10: circular header file dependency detected while including 'resource.h', please check the include path [misc-header-include-cycle]
#include <pipewire/resource.h>
         ^
  Report hash: 0dbd6e13aed46b4d8732e99ad49ef4c1
  Steps:
    1, resource.h:37:10: 'impl-client.h' included from here
    2, impl-core.h:29:10: 'resource.h' included from here
    3, impl-client.h:68:10: circular header file dependency detected while including 'resource.h', please check the include path

Found 2 defect(s) in impl-client.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: b4e1d1024c73f5543ef983da74c8a645
  Steps:
    1, impl.h:25:10: 'impl-core.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:22:10: 'global.h' included from here
    4, impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-core.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 98baed36db9cbd1ca48fe18b801bea5f
  Steps:
    1, impl.h:27:10: 'impl-device.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:22:10: 'global.h' included from here
    4, impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-device.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: caa7dc7877996ab29c3b85d029b375c4
  Steps:
    1, impl.h:28:10: 'impl-factory.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:22:10: 'global.h' included from here
    4, impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-factory.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-link.h:24:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: 89e4605111d4c286b81e1b516e7160e7
  Steps:
    1, impl.h:30:10: 'impl-link.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, impl-link.h:24:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in impl-link.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-metadata.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 49976cb41585c3805dce471218afb328
  Steps:
    1, impl.h:31:10: 'impl-metadata.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:22:10: 'global.h' included from here
    4, impl-metadata.h:25:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-metadata.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-node.h:28:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: b556a94963dd35d190a0ddd6ee0964b5
  Steps:
    1, impl.h:33:10: 'impl-node.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, impl-node.h:28:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in impl-node.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-port.h:27:10: circular header file dependency detected while including 'impl.h', please check the include path [misc-header-include-cycle]
#include <pipewire/impl.h>
         ^
  Report hash: 3a0828555eb26fb9f898376dc802be93
  Steps:
    1, impl.h:34:10: 'impl-port.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, impl-port.h:27:10: circular header file dependency detected while including 'impl.h', please check the include path

Found 1 defect(s) in impl-port.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl.h:29:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 97378dfee83bf376433e4226817d4af1
  Steps:
    1, global.h:39:10: 'impl.h' included from here
    2, pipewire_includes.h:22:10: 'global.h' included from here
    3, impl.h:29:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl.h

[MEDIUM] /usr/include/pipewire-0.3/pipewire/properties.h:192:1: identifier '_spa_autoptr_cleanup_func_pw_properties' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(pw_properties, struct pw_properties, {
^
  Report hash: 83cc49725b0040adc82f792183801b60
  Steps:
    1, cleanup.h:68:20: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, properties.h:192:1: identifier '_spa_autoptr_cleanup_func_pw_properties' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/pipewire-0.3/pipewire/properties.h:192:1: identifier '_spa_autoptr_cleanup_type_pw_properties' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(pw_properties, struct pw_properties, {
^
  Report hash: 70f2a3d989b98e27528a2b459ef264c4
  Steps:
    1, cleanup.h:67:28: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, properties.h:192:1: identifier '_spa_autoptr_cleanup_type_pw_properties' is reserved because it starts with '_' at global scope

Found 2 defect(s) in properties.h

[LOW] /usr/include/pipewire-0.3/pipewire/protocol.h:26:10: circular header file dependency detected while including 'context.h', please check the include path [misc-header-include-cycle]
#include <pipewire/context.h>
         ^
  Report hash: 8a4ca310782449cb3b78489b5a83a9f9
  Steps:
    1, proxy.h:90:10: 'protocol.h' included from here
    2, core.h:79:10: 'proxy.h' included from here
    3, context.h:48:10: 'core.h' included from here
    4, pipewire_includes.h:21:10: 'context.h' included from here
    5, protocol.h:26:10: circular header file dependency detected while including 'context.h', please check the include path

Found 1 defect(s) in protocol.h

[MEDIUM] /usr/include/pipewire-0.3/pipewire/utils.h:103:1: identifier '_spa_auto_cleanup_func_pw_strv' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTO_CLEANUP(pw_strv, char **, {
^
  Report hash: d191108c2492e721db5c198011c6f841
  Steps:
    1, cleanup.h:55:20: expanded from macro 'SPA_DEFINE_AUTO_CLEANUP'
    2, utils.h:103:1: identifier '_spa_auto_cleanup_func_pw_strv' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/pipewire-0.3/pipewire/utils.h:103:1: identifier '_spa_auto_cleanup_type_pw_strv' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTO_CLEANUP(pw_strv, char **, {
^
  Report hash: 286ad65c4b8b98a07728f808e2c0f0cb
  Steps:
    1, cleanup.h:54:26: expanded from macro 'SPA_DEFINE_AUTO_CLEANUP'
    2, utils.h:103:1: identifier '_spa_auto_cleanup_type_pw_strv' is reserved because it starts with '_' at global scope

Found 2 defect(s) in utils.h

[MEDIUM] /usr/include/spa-0.2/spa/buffer/buffer.h:48:2: identifier '_SPA_DATA_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_DATA_LAST,      /**< not part of ABI */
  ^
  Report hash: 03818b597bd5e4204b621d81e5b69bb4
  Steps:
    1, buffer.h:48:2: identifier '_SPA_DATA_LAST' is reserved because it starts with '_' followed by a capital letter

Found 1 defect(s) in buffer.h

[MEDIUM] /usr/include/spa-0.2/spa/buffer/meta.h:41:2: identifier '_SPA_META_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_META_LAST,      /**< not part of ABI/API */
  ^
  Report hash: fb9bba17242cbd932ec11f23751b377d
  Steps:
    1, meta.h:41:2: identifier '_SPA_META_LAST' is reserved because it starts with '_' followed by a capital letter

Found 1 defect(s) in meta.h

[LOW] /usr/include/spa-0.2/spa/param/audio/format-utils.h:56:2: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
  switch (info->media_subtype) {
  ^
  Report hash: ffd5b788737352c33aa8985ddfe3e707
  Steps:
    1, format-utils.h:56:2: switching on non-enum value without default case may not cover all cases

[LOW] /usr/include/spa-0.2/spa/param/audio/format-utils.h:91:2: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
  switch (info->media_subtype) {
  ^
  Report hash: ffd5b788737352c33aa8985ddfe3e707
  Steps:
    1, format-utils.h:91:2: switching on non-enum value without default case may not cover all cases

Found 2 defect(s) in format-utils.h

[LOW] /usr/include/spa-0.2/spa/param/buffers.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: 10f6ec61bd6df1a119cf9eea7e68c8d3
  Steps:
    1, param.h:75:10: 'buffers.h' included from here
    2, format.h:17:10: 'param.h' included from here
    3, buffers.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in buffers.h

[LOW] /usr/include/spa-0.2/spa/param/port-config.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: bbf25c04cf6ba9c76091dc26a9c4907c
  Steps:
    1, param.h:77:10: 'port-config.h' included from here
    2, format.h:17:10: 'param.h' included from here
    3, port-config.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in port-config.h

[LOW] /usr/include/spa-0.2/spa/param/profile.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: d543dd0e6a4219f2f281b6f8d2ec2327
  Steps:
    1, param.h:76:10: 'profile.h' included from here
    2, format.h:17:10: 'param.h' included from here
    3, profile.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in profile.h

[LOW] /usr/include/spa-0.2/spa/param/route.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: de3e7bf38f149ce3fcc8ff8ce5654d20
  Steps:
    1, param.h:78:10: 'route.h' included from here
    2, format.h:17:10: 'param.h' included from here
    3, route.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in route.h

[HIGH] /usr/include/spa-0.2/spa/pod/builder.h:205:3: suspicious usage of 'sizeof(K)'; did you mean 'K'? [bugprone-sizeof-expression]
    SPA_FLAG_CLEAR(builder->state.flags, SPA_POD_BUILDER_FLAG_FIRST);
    ^
  Report hash: eef99322fff6dd6b95bd63cedded29aa
  Steps:
    1, defs.h:86:26: expanded from macro 'SPA_FLAG_CLEAR'
    2, builder.h:205:3: suspicious usage of 'sizeof(K)'; did you mean 'K'?

[HIGH] /usr/include/spa-0.2/spa/pod/builder.h:227:2: suspicious usage of 'sizeof(K)'; did you mean 'K'? [bugprone-sizeof-expression]
  SPA_FLAG_CLEAR(builder->state.flags, SPA_POD_BUILDER_FLAG_FIRST);
  ^
  Report hash: 8663eacab3b0ba74521abfbc64e40a60
  Steps:
    1, defs.h:86:26: expanded from macro 'SPA_FLAG_CLEAR'
    2, builder.h:227:2: suspicious usage of 'sizeof(K)'; did you mean 'K'?

[LOW] /usr/include/spa-0.2/spa/pod/builder.h:653:4: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
      SPA_POD_BUILDER_COLLECT(builder, *format, args);
      ^
  Report hash: 3f893bfb6eace68e3d86b6cf62666a25
  Steps:
    1, builder.h:512:2: expanded from macro 'SPA_POD_BUILDER_COLLECT'
    2, builder.h:653:4: switching on non-enum value without default case may not cover all cases

Found 3 defect(s) in builder.h

[MEDIUM] /usr/include/spa-0.2/spa/pod/iter.h:314:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_return_val_if_fail(spa_pod_is_fraction(pod), -EINVAL);
  ^
  Report hash: 9600fcd5fbb15cb4c336c03fb2e7cc60
  Steps:
    1, defs.h:381:4: expanded from macro 'spa_return_val_if_fail'
    2, iter.h:314:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /usr/include/spa-0.2/spa/pod/iter.h:333:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_return_val_if_fail(spa_pod_is_array(pod), NULL);
  ^
  Report hash: 329b2d035b569160577af71457d8348d
  Steps:
    1, defs.h:381:4: expanded from macro 'spa_return_val_if_fail'
    2, iter.h:333:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 2 defect(s) in iter.h

[LOW] /usr/include/spa-0.2/spa/pod/parser.h:494:4: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
      SPA_POD_PARSER_SKIP(*format, args);
      ^
  Report hash: 936209cc6ea38b2b5f94af2eb67b6ece
  Steps:
    1, parser.h:421:2: expanded from macro 'SPA_POD_PARSER_SKIP'
    2, parser.h:494:4: switching on non-enum value without default case may not cover all cases

Found 1 defect(s) in parser.h

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:83:20: identifier '_spa_autofree_cleanup_func' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
static inline void _spa_autofree_cleanup_func(void *p)
                   ^
  Report hash: 4e9214c45124f4846bac779aaf0f11b8
  Steps:
    1, cleanup.h:83:20: identifier '_spa_autofree_cleanup_func' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:93:20: identifier '_spa_autoclose_cleanup_func' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
static inline void _spa_autoclose_cleanup_func(int *fd)
                   ^
  Report hash: 062d1e75a6f46d35b6f1ed776d69a0bf
  Steps:
    1, cleanup.h:93:20: identifier '_spa_autoclose_cleanup_func' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:105:1: identifier '_spa_autoptr_cleanup_func_FILE' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(FILE, FILE, {
^
  Report hash: 8c90e8714ce36cbfc016a34d6aafe59b
  Steps:
    1, cleanup.h:68:20: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:105:1: identifier '_spa_autoptr_cleanup_func_FILE' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:105:1: identifier '_spa_autoptr_cleanup_type_FILE' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(FILE, FILE, {
^
  Report hash: 1948d70ed31817f9218c89cacf394cc2
  Steps:
    1, cleanup.h:67:28: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:105:1: identifier '_spa_autoptr_cleanup_type_FILE' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:106:24: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_clear_ptr(*thing, fclose);
                        ^
  Report hash: 6dc3766dd4a30ea03f6b48247051ef9d
  Steps:
    1, cleanup.h:28:3: expanded from macro 'spa_clear_ptr'
    2, cleanup.h:71:2: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    3, cleanup.h:106:24: cast the expression to void to silence this warning
    4, cleanup.h:106:24: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:113:1: identifier '_spa_autoptr_cleanup_func_DIR' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(DIR, DIR, {
^
  Report hash: e974bef354d1e377f8e0bba41689668c
  Steps:
    1, cleanup.h:68:20: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:113:1: identifier '_spa_autoptr_cleanup_func_DIR' is reserved because it starts with '_' at global scope

[MEDIUM] /usr/include/spa-0.2/spa/utils/cleanup.h:113:1: identifier '_spa_autoptr_cleanup_type_DIR' is reserved because it starts with '_' at global scope [clang-diagnostic-reserved-identifier]
SPA_DEFINE_AUTOPTR_CLEANUP(DIR, DIR, {
^
  Report hash: 250ac9d4570be92c586f490e4104b572
  Steps:
    1, cleanup.h:67:28: expanded from macro 'SPA_DEFINE_AUTOPTR_CLEANUP'
    2, cleanup.h:113:1: identifier '_spa_autoptr_cleanup_type_DIR' is reserved because it starts with '_' at global scope

Found 7 defect(s) in cleanup.h

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:3: identifier '__NONE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
    ^
  Report hash: e169b04a5693dda3263a71a92be29757
  Steps:
    1, json-core.h:83:3: identifier '__NONE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:11: identifier '__STRUCT' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
            ^
  Report hash: a686f6b9af1c086abced146c0c6f2435
  Steps:
    1, json-core.h:83:11: identifier '__STRUCT' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:21: identifier '__BARE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                      ^
  Report hash: c36fb0b6ba22ab12a23f29403fa42dc0
  Steps:
    1, json-core.h:83:21: identifier '__BARE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:29: identifier '__STRING' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                              ^
  Report hash: a32457a4f1b73b65aac2a15b621291f6
  Steps:
    1, json-core.h:83:29: identifier '__STRING' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:39: identifier '__UTF8' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                                        ^
  Report hash: ff655471788b4ed203ea6836ce8d6ffa
  Steps:
    1, json-core.h:83:39: identifier '__UTF8' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:47: identifier '__ESC' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                                                ^
  Report hash: 222e64a766587807525f5bfdb554e6c4
  Steps:
    1, json-core.h:83:47: identifier '__ESC' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:83:54: identifier '__COMMENT' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __NONE, __STRUCT, __BARE, __STRING, __UTF8, __ESC, __COMMENT,
                                                       ^
  Report hash: 5034e9e0efdb387c2993b905a5c6d2ee
  Steps:
    1, json-core.h:83:54: identifier '__COMMENT' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:84:3: identifier '__ARRAY_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ARRAY_FLAG = 0x10,    /* in array context */
    ^
  Report hash: df193b651db2bc004cfce54a372ee4bf
  Steps:
    1, json-core.h:84:3: identifier '__ARRAY_FLAG' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:85:3: identifier '__PREV_ARRAY_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __PREV_ARRAY_FLAG = 0x20,  /* depth=0 array context flag */
    ^
  Report hash: ea668d02342d7cdd6119ca7e34f58ee0
  Steps:
    1, json-core.h:85:3: identifier '__PREV_ARRAY_FLAG' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:86:3: identifier '__KEY_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __KEY_FLAG = 0x40,    /* inside object key */
    ^
  Report hash: 93a8368e3478ff6d782b10059d0bf152
  Steps:
    1, json-core.h:86:3: identifier '__KEY_FLAG' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:87:3: identifier '__SUB_FLAG' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __SUB_FLAG = 0x80,    /* not at top-level */
    ^
  Report hash: f83d92cb8f26170b1b651b4cc5257349
  Steps:
    1, json-core.h:87:3: identifier '__SUB_FLAG' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:88:3: identifier '__FLAGS' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __FLAGS = 0xff0,
    ^
  Report hash: 8a480e1edd6a9ea99102e0062640b75d
  Steps:
    1, json-core.h:88:3: identifier '__FLAGS' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:89:3: identifier '__ERROR_SYSTEM' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_SYSTEM = SPA_JSON_ERROR_FLAG,
    ^
  Report hash: 63fc14343b21c8b5954c7a6f23d09d13
  Steps:
    1, json-core.h:89:3: identifier '__ERROR_SYSTEM' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:90:3: identifier '__ERROR_INVALID_ARRAY_SEPARATOR' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_INVALID_ARRAY_SEPARATOR,
    ^
  Report hash: 90e5106128638f8e72fcf1dd0ff5d7eb
  Steps:
    1, json-core.h:90:3: identifier '__ERROR_INVALID_ARRAY_SEPARATOR' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:91:3: identifier '__ERROR_EXPECTED_OBJECT_KEY' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_OBJECT_KEY,
    ^
  Report hash: 0eeebef6a1a296298259434bd9c83b80
  Steps:
    1, json-core.h:91:3: identifier '__ERROR_EXPECTED_OBJECT_KEY' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:92:3: identifier '__ERROR_EXPECTED_OBJECT_VALUE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_OBJECT_VALUE,
    ^
  Report hash: 3701429a89c1e0ae596ec4920c380361
  Steps:
    1, json-core.h:92:3: identifier '__ERROR_EXPECTED_OBJECT_VALUE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:93:3: identifier '__ERROR_TOO_DEEP_NESTING' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_TOO_DEEP_NESTING,
    ^
  Report hash: edd9a0657db2889b4a6fa1d8926be5f2
  Steps:
    1, json-core.h:93:3: identifier '__ERROR_TOO_DEEP_NESTING' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:94:3: identifier '__ERROR_EXPECTED_ARRAY_CLOSE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_ARRAY_CLOSE,
    ^
  Report hash: 86c42b9d2c3d9b54aab616415c4389e5
  Steps:
    1, json-core.h:94:3: identifier '__ERROR_EXPECTED_ARRAY_CLOSE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:95:3: identifier '__ERROR_EXPECTED_OBJECT_CLOSE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_EXPECTED_OBJECT_CLOSE,
    ^
  Report hash: a20150bb32b72233751ff8c5e718fc0c
  Steps:
    1, json-core.h:95:3: identifier '__ERROR_EXPECTED_OBJECT_CLOSE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:96:3: identifier '__ERROR_MISMATCHED_BRACKET' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_MISMATCHED_BRACKET,
    ^
  Report hash: 18220c4cdc2c9b88054ae2dc3acfeab3
  Steps:
    1, json-core.h:96:3: identifier '__ERROR_MISMATCHED_BRACKET' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:97:3: identifier '__ERROR_ESCAPE_NOT_ALLOWED' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_ESCAPE_NOT_ALLOWED,
    ^
  Report hash: ed3b1e7cb46d2343adab35416dba18e7
  Steps:
    1, json-core.h:97:3: identifier '__ERROR_ESCAPE_NOT_ALLOWED' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:98:3: identifier '__ERROR_CHARACTERS_NOT_ALLOWED' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_CHARACTERS_NOT_ALLOWED,
    ^
  Report hash: 3011991984ae22eb59322ec4b2c64dd7
  Steps:
    1, json-core.h:98:3: identifier '__ERROR_CHARACTERS_NOT_ALLOWED' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:99:3: identifier '__ERROR_INVALID_ESCAPE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_INVALID_ESCAPE,
    ^
  Report hash: b2d68b5a712901be597bf7b2dadf1427
  Steps:
    1, json-core.h:99:3: identifier '__ERROR_INVALID_ESCAPE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:100:3: identifier '__ERROR_INVALID_STATE' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_INVALID_STATE,
    ^
  Report hash: 34dc1dc7c59fe0e25e45f1c9f7faf58d
  Steps:
    1, json-core.h:100:3: identifier '__ERROR_INVALID_STATE' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:101:3: identifier '__ERROR_UNFINISHED_STRING' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
    __ERROR_UNFINISHED_STRING,
    ^
  Report hash: e36b85e610b8c7631cc4c58cc016c648
  Steps:
    1, json-core.h:101:3: identifier '__ERROR_UNFINISHED_STRING' is reserved because it starts with '__'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:114:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define _SPA_ERROR(reason)  { err = __ERROR_ ## reason; goto error; }
        ^
  Report hash: 901bfff7582981f1d0da07bbef233ed0
  Steps:
    1, json-core.h:114:9: macro name is a reserved identifier

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:323:8: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#undef _SPA_ERROR
       ^
  Report hash: 0bb09aa6989530fb260dc1931e26bbb9
  Steps:
    1, json-core.h:323:8: macro name is a reserved identifier

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:449:29: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
  return spa_dtoa(str, size, val);
                             ^
  Report hash: 57f4ffa6f2535055a551be4e2940a10c
  Steps:
    1, json-core.h:449:29: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:595:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __PUT(c) { if (len < size) *str++ = c; len++; }
        ^
  Report hash: 34755b98cae0df37310c8f6bcf293d28
  Steps:
    1, json-core.h:595:9: macro name is a reserved identifier

[MEDIUM] /usr/include/spa-0.2/spa/utils/json-core.h:632:8: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#undef __PUT
       ^
  Report hash: 42b5c89c010f749d85d0fc0838fd4497
  Steps:
    1, json-core.h:632:8: macro name is a reserved identifier

Found 30 defect(s) in json-core.h

[MEDIUM] /usr/include/spa-0.2/spa/utils/string.h:70:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_assert_se(prefix);
  ^
  Report hash: 5019cf126b16580a5adaf138b242121b
  Steps:
    1, defs.h:393:4: expanded from macro 'spa_assert_se'
    2, string.h:70:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /usr/include/spa-0.2/spa/utils/string.h:88:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_assert_se(suffix);
  ^
  Report hash: 65f20f678659dd9b1b5a8318724c88f0
  Steps:
    1, defs.h:393:4: expanded from macro 'spa_assert_se'
    2, string.h:88:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /usr/include/spa-0.2/spa/utils/string.h:225:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  spa_assert_se((ssize_t)size > 0);
  ^
  Report hash: d5a84d0cea4cae9623806e567493b935
  Steps:
    1, defs.h:393:4: expanded from macro 'spa_assert_se'
    2, string.h:225:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 3 defect(s) in string.h

[MEDIUM] /usr/include/spa-0.2/spa/utils/type.h:55:2: identifier '_SPA_TYPE_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_LAST,        /**< not part of ABI */
  ^
  Report hash: bdb7800d6f84cfde632ca52fb2f44eab
  Steps:
    1, type.h:55:2: identifier '_SPA_TYPE_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /usr/include/spa-0.2/spa/utils/type.h:62:2: identifier '_SPA_TYPE_POINTER_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_POINTER_LAST,      /**< not part of ABI */
  ^
  Report hash: 1e32f82091e76d3c89ebf1ef8d990e74
  Steps:
    1, type.h:62:2: identifier '_SPA_TYPE_POINTER_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /usr/include/spa-0.2/spa/utils/type.h:68:2: identifier '_SPA_TYPE_EVENT_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_EVENT_LAST,      /**< not part of ABI */
  ^
  Report hash: 90102cb980d9dec27ffe550717137a79
  Steps:
    1, type.h:68:2: identifier '_SPA_TYPE_EVENT_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /usr/include/spa-0.2/spa/utils/type.h:74:2: identifier '_SPA_TYPE_COMMAND_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_COMMAND_LAST,      /**< not part of ABI */
  ^
  Report hash: 08f54e700f5760bf5f2a4537a0e2a915
  Steps:
    1, type.h:74:2: identifier '_SPA_TYPE_COMMAND_LAST' is reserved because it starts with '_' followed by a capital letter

[MEDIUM] /usr/include/spa-0.2/spa/utils/type.h:91:2: identifier '_SPA_TYPE_OBJECT_LAST' is reserved because it starts with '_' followed by a capital letter [clang-diagnostic-reserved-identifier]
  _SPA_TYPE_OBJECT_LAST,      /**< not part of ABI */
  ^
  Report hash: a68bc4cbf6a446cdbcaf462db3488a0a
  Steps:
    1, type.h:91:2: identifier '_SPA_TYPE_OBJECT_LAST' is reserved because it starts with '_' followed by a capital letter

Found 5 defect(s) in type.h

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:283:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:283:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:330:26: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 005a58abc587efa85ab38a672b887462
  Steps:
    1, memory_tier_manager.cpp:330:26: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:414:23: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b88d2d0afa7e06912c1d00d5827b227a
  Steps:
    1, memory_tier_manager.cpp:414:23: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in memory_tier_manager.cpp

Found no defects in factory.cpp
Found no defects in factory.cpp
[CRITICAL] /sep/src/core/engine.cpp:189:24: no member named 'pin_id' in 'sep::PinState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: aa8d51ae3e507e81d50b22012fab6258
  Steps:
    1, engine.cpp:189:24: no member named 'pin_id' in 'sep::PinState'

[CRITICAL] /sep/src/core/engine.cpp:195:23: no member named 'value' in 'sep::PinState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7b0ccd838640e638f50a524efef2f09a
  Steps:
    1, engine.cpp:195:23: no member named 'value' in 'sep::PinState'

[CRITICAL] /sep/src/core/engine.cpp:195:41: no member named 'coherence' in 'sep::PinState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 90fcb885eb5419c0672119c6218a7665
  Steps:
    1, engine.cpp:195:41: no member named 'coherence' in 'sep::PinState'

[CRITICAL] /sep/src/core/engine.cpp:259:28: use of undeclared identifier 'sep_cuda_process_batch' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ba6d27e3b694ff1d4a8ad01ccb27c859
  Steps:
    1, engine.cpp:259:28: use of undeclared identifier 'sep_cuda_process_batch'

[CRITICAL] /sep/src/core/engine.cpp:274:27: use of undeclared identifier 'sep_cuda_process_symmetry' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c35b4832c67e2c7fb4dc20d3e6a69984
  Steps:
    1, engine.cpp:274:27: use of undeclared identifier 'sep_cuda_process_symmetry'

[CRITICAL] /sep/src/core/engine.cpp:287:33: no viable overloaded '=' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 32adf3e053d4d840a7a9c61c2f13018c
  Steps:
    1, stl_vector.h:818:7: candidate function not viable: no known conversion from '::sep::shim::vector<std::uint32_t>' (aka 'vector<unsigned int>') to 'const vector<unsigned int>' for 1st argument
    2, stl_vector.h:833:7: candidate function not viable: no known conversion from '::sep::shim::vector<std::uint32_t>' (aka 'vector<unsigned int>') to 'vector<unsigned int>' (aka 'std::vector<unsigned int>') for 1st argument
    3, stl_vector.h:855:7: candidate function not viable: no known conversion from '::sep::shim::vector<std::uint32_t>' (aka 'vector<unsigned int>') to 'initializer_list<value_type>' (aka 'initializer_list<unsigned int>') for 1st argument
    4, engine.cpp:287:33: no viable overloaded '='

[CRITICAL] /sep/src/core/engine.cpp:291:37: no viable overloaded '=' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 04bb564b0e612066f47c0a331d50ed76
  Steps:
    1, stl_vector.h:818:7: candidate function not viable: no known conversion from '::sep::shim::vector<std::uint32_t>' (aka 'vector<unsigned int>') to 'const vector<vector<unsigned int>>' for 1st argument
    2, stl_vector.h:833:7: candidate function not viable: no known conversion from '::sep::shim::vector<std::uint32_t>' (aka 'vector<unsigned int>') to 'vector<vector<unsigned int>>' for 1st argument
    3, stl_vector.h:855:7: candidate function not viable: no known conversion from '::sep::shim::vector<std::uint32_t>' (aka 'vector<unsigned int>') to 'initializer_list<value_type>' (aka 'initializer_list<std::vector<unsigned int, std::allocator<unsigned int>>>') for 1st argument
    4, engine.cpp:291:37: no viable overloaded '='

[CRITICAL] /sep/src/core/engine.cpp:292:36: no viable overloaded '=' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c9a7e45bcfb290ea546f636d9faad6ae
  Steps:
    1, stl_vector.h:818:7: candidate function not viable: no known conversion from '::sep::shim::vector<std::uint32_t>' (aka 'vector<unsigned int>') to 'const vector<unsigned int>' for 1st argument
    2, stl_vector.h:833:7: candidate function not viable: no known conversion from '::sep::shim::vector<std::uint32_t>' (aka 'vector<unsigned int>') to 'vector<unsigned int>' (aka 'std::vector<unsigned int>') for 1st argument
    3, stl_vector.h:855:7: candidate function not viable: no known conversion from '::sep::shim::vector<std::uint32_t>' (aka 'vector<unsigned int>') to 'initializer_list<value_type>' (aka 'initializer_list<unsigned int>') for 1st argument
    4, engine.cpp:292:36: no viable overloaded '='

[CRITICAL] /sep/src/core/engine.cpp:311:30: no member named 'INTERNAL_ERROR' in 'sep::SEPResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eff06a34945ab920703222322f314263
  Steps:
    1, engine.cpp:311:30: no member named 'INTERNAL_ERROR' in 'sep::SEPResult'

Found 9 defect(s) in engine.cpp

[MEDIUM] /sep/src/core/engine.cpp:66:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:66:6: cast the expression to void to silence this warning
    2, engine.cpp:66:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:72:14: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 39120b7c5df7534093597522738993e6
  Steps:
    1, engine.cpp:72:14: cast the expression to void to silence this warning
    2, engine.cpp:72:14: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:77:18: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab6f901229b488bfc64f4551fa611064
  Steps:
    1, engine.cpp:77:18: cast the expression to void to silence this warning
    2, engine.cpp:77:18: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:81:18: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab6f901229b488bfc64f4551fa611064
  Steps:
    1, engine.cpp:81:18: cast the expression to void to silence this warning
    2, engine.cpp:81:18: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:89:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:89:10: cast the expression to void to silence this warning
    2, engine.cpp:89:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:110:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:110:6: cast the expression to void to silence this warning
    2, engine.cpp:110:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 6 defect(s) in engine.cpp

[CRITICAL] /sep/include/compat/cycles.h:25:10: 'device/device.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 495b27a565034a196c259d35d2d8b3f1
  Steps:
    1, cycles.h:25:10: 'device/device.h' file not found

Found 1 defect(s) in cycles.h

[MEDIUM] /sep/extern/cycles/src/util/hash.h:555:15: 'signed char' to 'uint' (aka 'unsigned int') conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
  while ((c = *str++)) {
              ^
  Report hash: e273fee2a0a290ebd55727a0afef4ea8
  Steps:
    1, hash.h:555:15: 'signed char' to 'uint' (aka 'unsigned int') conversion; consider casting to 'unsigned char' first.

Found 1 defect(s) in hash.h

[MEDIUM] /sep/include/compat/cycles.h:68:34: unused parameter 'row' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fff28953586f6f45c8151653cb8615ab
  Steps:
    1, cycles.h:68:34: unused parameter 'row'

[MEDIUM] /sep/include/compat/cycles.h:68:43: unused parameter 'col' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 38220157262a42481c215ecb43bf31f3
  Steps:
    1, cycles.h:68:43: unused parameter 'col'

[MEDIUM] /sep/include/compat/cycles.h:69:34: unused parameter 'i' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9a788d8abf35ab6cf7290cd9f8d778b3
  Steps:
    1, cycles.h:69:34: unused parameter 'i'

[MEDIUM] /sep/include/compat/cycles.h:70:35: unused parameter 'i' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9f5bf18f51d222e486fe2c7261c8d98d
  Steps:
    1, cycles.h:70:35: unused parameter 'i'

[MEDIUM] /sep/include/compat/cycles.h:76:36: unused parameter 'min' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3491c10f7ed201f307451e8ee2806396
  Steps:
    1, cycles.h:76:36: unused parameter 'min'

[MEDIUM] /sep/include/compat/cycles.h:76:54: unused parameter 'max' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 09194db40cec448620d014aeae2dbad2
  Steps:
    1, cycles.h:76:54: unused parameter 'max'

[MEDIUM] /sep/include/compat/cycles.h:77:51: unused parameter 'other' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a926832fcc449c8a7c6123cd96f5f78f
  Steps:
    1, cycles.h:77:51: unused parameter 'other'

Found 7 defect(s) in cycles.h

[MEDIUM] /sep/src/api/server.cpp:921:17: unused variable 'iterations' [clang-diagnostic-unused-variable]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 754121206c4a3557a6c37fb43c0f40a9
  Steps:
    1, server.cpp:921:17: unused variable 'iterations'

Found 1 defect(s) in server.cpp

[CRITICAL] /sep/include/crow/socket_adaptors.h:34:18: constructor for 'crow::SocketAdaptor' must explicitly initialize the reference member 'io_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9582f428fef619bbcc002c256fd9fa76
  Steps:
    1, socket_adaptors.h:84:32: declared here
    2, socket_adaptors.h:34:18: constructor for 'crow::SocketAdaptor' must explicitly initialize the reference member 'io_'

[MEDIUM] /sep/include/crow/socket_adaptors.h:56:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 65dbab1b803153775327fae80895489b
  Steps:
    1, socket_adaptors.h:56:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/include/crow/socket_adaptors.h:56:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 341bf432f71521ae828cd32f95f83569
  Steps:
    1, socket_adaptors.h:56:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/include/crow/socket_adaptors.h:62:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 75fe484a08a8aa33bc6a26035876a5b2
  Steps:
    1, socket_adaptors.h:62:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/include/crow/socket_adaptors.h:62:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7c3e7bbaf32e4a0f1c75352f1c488bf2
  Steps:
    1, socket_adaptors.h:62:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/include/crow/socket_adaptors.h:68:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2877cf30b3315dd3523808afdd813f89
  Steps:
    1, socket_adaptors.h:68:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/include/crow/socket_adaptors.h:68:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7428d21fcd9f7c0e13567eb402fac556
  Steps:
    1, socket_adaptors.h:68:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/include/crow/socket_adaptors.h:74:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 169bac9ac4685dec4252b21ceac4aae5
  Steps:
    1, socket_adaptors.h:74:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/include/crow/socket_adaptors.h:74:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e74b3d114d1de58a5a9947b02c8b009b
  Steps:
    1, socket_adaptors.h:74:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 9 defect(s) in socket_adaptors.h

[LOW] /usr/include/asio/prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 0eaff2f93cce5d1c85bf380d1aadd780
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/asio/prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: c7bb02861d41c0f3c6e91cb673027038
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 2 defect(s) in prefer.hpp

[LOW] /usr/include/asio/require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 5d62603bc2b367f7b193486c1ed29300
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/asio/require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 68073bea6e9be45b4736e221dcf9ac48
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 2 defect(s) in require.hpp

[LOW] /usr/include/boost/asio/associated_allocator.hpp:91:54: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_allocator.hpp:91:54)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    typename associator<associated_allocator, T, A>::type
                                                     ^
  Report hash: c090761e197917f9d2d7db9ce4db7d76
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, associated_allocator.hpp:91:54: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_allocator.hpp:91:54)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 1 defect(s) in associated_allocator.hpp

[LOW] /usr/include/boost/asio/associated_cancellation_slot.hpp:91:62: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_cancellation_slot.hpp:91:62)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    typename associator<associated_cancellation_slot, T, S>::type
                                                             ^
  Report hash: 74516d15e962f1c7038dcbbebd069366
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, associated_cancellation_slot.hpp:91:62: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_cancellation_slot.hpp:91:62)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 1 defect(s) in associated_cancellation_slot.hpp

[LOW] /usr/include/boost/asio/associated_executor.hpp:93:53: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_executor.hpp:93:53)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    typename associator<associated_executor, T, E>::type
                                                    ^
  Report hash: 6f38914654d381eba47daab48632f296
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, associated_executor.hpp:93:53: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_executor.hpp:93:53)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 1 defect(s) in associated_executor.hpp

[LOW] /usr/include/boost/asio/associated_immediate_executor.hpp:67:21: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:67:21)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    is_executor<E>::value
                    ^
  Report hash: f0c0ae7236dff41c1d7d9010ac024029
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, associated_immediate_executor.hpp:67:21: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:67:21)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/associated_immediate_executor.hpp:156:63: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:156:63)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    typename associator<associated_immediate_executor, T, E>::type
                                                              ^
  Report hash: 3a9f7b4153030f6830777aa37fbe4073
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, associated_immediate_executor.hpp:156:63: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:156:63)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 2 defect(s) in associated_immediate_executor.hpp

[LOW] /usr/include/boost/asio/execution/bulk_execute.hpp:147:35: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:147:35)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    bulk_execute_member<S, F, N>::is_valid
                                  ^
  Report hash: f1b1b82c5537cae85b62009bdb674355
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, bulk_execute.hpp:147:35: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:147:35)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/execution/bulk_execute.hpp:165:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:165:36)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    !bulk_execute_member<S, F, N>::is_valid
                                   ^
  Report hash: 904e45d6e707d7f8d26ee09ddf37795b
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, bulk_execute.hpp:165:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:165:36)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/execution/bulk_execute.hpp:186:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:186:36)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    !bulk_execute_member<S, F, N>::is_valid
                                   ^
  Report hash: d662c6bea9a37d3df6cf394ff80a64d2
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, bulk_execute.hpp:186:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:186:36)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 3 defect(s) in bulk_execute.hpp

[LOW] /usr/include/boost/asio/execution/connect.hpp:180:55: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:180:55)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    is_operation_state<typename connect_member<S, R>::result_type>::value
                                                      ^
  Report hash: 8518e27c743e9f2b38d42b5df711d7f4
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, connect.hpp:180:55: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:180:55)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/execution/connect.hpp:196:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:196:25)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    connect_free<S, R>::is_valid
                        ^
  Report hash: 649f3330e3fa7ad588f98c197cee240b
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, connect.hpp:196:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:196:25)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/execution/connect.hpp:215:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:215:26)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    !connect_free<S, R>::is_valid
                         ^
  Report hash: b044315f4edbdfe8f20db28659d554f2
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, connect.hpp:215:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:215:26)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 3 defect(s) in connect.hpp

[LOW] /usr/include/boost/asio/execution/execute.hpp:145:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:145:25)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    execute_free<T, F>::is_valid
                        ^
  Report hash: c4fccbcb72f740de6d71df434764aa17
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, execute.hpp:145:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:145:25)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/execution/execute.hpp:158:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:158:26)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    !execute_free<T, F>::is_valid
                         ^
  Report hash: 7d268b6df3a90087680a895da2db05d4
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, execute.hpp:158:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:158:26)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 2 defect(s) in execute.hpp

[LOW] /usr/include/boost/asio/execution/executor.hpp:62:43: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/executor.hpp:62:43)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    typename result_of<typename decay<F>::type&()>::type
                                          ^
  Report hash: 62bc001fb04aa973be759ea8c60961e6
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, executor.hpp:62:43: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/executor.hpp:62:43)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 1 defect(s) in executor.hpp

[LOW] /usr/include/boost/asio/execution/schedule.hpp:118:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:118:23)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    schedule_free<S>::is_valid
                      ^
  Report hash: d385e4769eda00d3f5413d68c1afddce
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, schedule.hpp:118:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:118:23)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/execution/schedule.hpp:131:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:131:24)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    !schedule_free<S>::is_valid
                       ^
  Report hash: b704a17e4f964681ff2d6b133e772dc2
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, schedule.hpp:131:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:131:24)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 2 defect(s) in schedule.hpp

[LOW] /usr/include/boost/asio/execution/sender.hpp:90:56: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/sender.hpp:90:56)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    typename has_error_types<S::template error_types>::type,
                                                       ^
  Report hash: cb5ca22a59f224b4fd1d2645fa07a26c
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, sender.hpp:90:56: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/sender.hpp:90:56)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 1 defect(s) in sender.hpp

[LOW] /usr/include/boost/asio/execution/set_done.hpp:115:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_done.hpp:115:23)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    set_done_free<R>::is_valid
                      ^
  Report hash: 6bd1aaf5e48a805acba15120937bd34a
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, set_done.hpp:115:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_done.hpp:115:23)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 1 defect(s) in set_done.hpp

[LOW] /usr/include/boost/asio/execution/set_error.hpp:115:27: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_error.hpp:115:27)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    set_error_free<R, E>::is_valid
                          ^
  Report hash: e97388010105fd4f63ff0cd02e80c59b
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, set_error.hpp:115:27: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_error.hpp:115:27)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 1 defect(s) in set_error.hpp

[LOW] /usr/include/boost/asio/execution/set_value.hpp:118:28: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_value.hpp:118:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    set_value_free<R, Vs>::is_valid
                           ^
  Report hash: 3c6e8b559fb3f678768fd5428fd4c92d
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, set_value.hpp:118:28: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_value.hpp:118:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 1 defect(s) in set_value.hpp

[LOW] /usr/include/boost/asio/execution/start.hpp:112:20: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/start.hpp:112:20)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    start_free<R>::is_valid
                   ^
  Report hash: b233cf5dd3b0b50232cc47cc98eba219
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, start.hpp:112:20: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/start.hpp:112:20)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 1 defect(s) in start.hpp

[LOW] /usr/include/boost/asio/execution/submit.hpp:148:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:148:25)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    is_sender_to<S, R>::value
                        ^
  Report hash: f1da4da400da723b9315fe1a4d7c042d
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, submit.hpp:148:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:148:25)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/execution/submit.hpp:161:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:161:24)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    submit_free<S, R>::is_valid
                       ^
  Report hash: 4fd57a2548622308fb5f14ed4a752b81
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, submit.hpp:161:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:161:24)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/execution/submit.hpp:177:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:177:25)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    !submit_free<S, R>::is_valid
                        ^
  Report hash: b2829f4f2de981fbd9fea0a0827ddbea
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, submit.hpp:177:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:177:25)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 3 defect(s) in submit.hpp

[LOW] /usr/include/boost/asio/executor_work_guard.hpp:162:41: no definition found for '(unnamed class at /usr/include/boost/asio/executor_work_guard.hpp:162:41)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      execution::is_executor<Executor>::value
                                        ^
  Report hash: 45694e5dc8f81c685df12e06f744621a
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, executor_work_guard.hpp:162:41: no definition found for '(unnamed class at /usr/include/boost/asio/executor_work_guard.hpp:162:41)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 1 defect(s) in executor_work_guard.hpp

[LOW] /usr/include/boost/asio/prefer.hpp:168:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:168:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 088b57f91047308a24c38dd7186af7a9
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, prefer.hpp:168:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:168:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/prefer.hpp:193:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:193:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 6d441b36bfceaa47915e1f339db47938
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, prefer.hpp:193:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:193:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/prefer.hpp:215:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:215:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: b766e9da315ab1de7d4a51044cf8cf52
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, prefer.hpp:215:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:215:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/prefer.hpp:240:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:240:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 627846d6585c819257f88b6ac43b027d
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, prefer.hpp:240:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:240:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/prefer.hpp:268:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:268:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 715596fc84ba5b23c3a4541ac715820e
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, prefer.hpp:268:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:268:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/prefer.hpp:299:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:299:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: d640b0742008fc704495645a43730009
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, prefer.hpp:299:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:299:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/prefer.hpp:335:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:335:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 497174dfc847c7c36bdc5905e2b592db
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, prefer.hpp:335:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:335:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/prefer.hpp:371:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:371:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: d572dd44539827ebd20db5e7840af034
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, prefer.hpp:371:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:371:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 8 defect(s) in prefer.hpp

[LOW] /usr/include/boost/asio/query.hpp:142:32: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:142:32)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    static_query<T, Property>::is_valid
                               ^
  Report hash: dde1bd0789d5b2f92a844788b4de613a
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, query.hpp:142:32: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:142:32)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/query.hpp:158:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:158:33)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    !static_query<T, Property>::is_valid
                                ^
  Report hash: 6aa59b7f4e85eb68e435d6060708579a
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, query.hpp:158:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:158:33)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/query.hpp:177:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:177:33)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    !static_query<T, Property>::is_valid
                                ^
  Report hash: a31946e497a7c680b72779bdac855b5c
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, query.hpp:177:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:177:33)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 3 defect(s) in query.hpp

[LOW] /usr/include/boost/asio/require.hpp:151:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:151:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable
                           ^
  Report hash: 6218dcfc0110fe56169e4b0a3cc77966
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, require.hpp:151:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:151:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/require.hpp:176:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:176:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable
                           ^
  Report hash: bb0c9330a3320da0efef02c7cf3997f0
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, require.hpp:176:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:176:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/require.hpp:198:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:198:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable
                           ^
  Report hash: 872d980b73ae60db70fec4d4714cb6db
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, require.hpp:198:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:198:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/require.hpp:222:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:222:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: f83011775dd1d0ed34c3cc6abfc83c8d
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, require.hpp:222:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:222:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/require.hpp:258:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:258:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 22ee3c43ab75ac62bf24664a43273696
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, require.hpp:258:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:258:48)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 5 defect(s) in require.hpp

[LOW] /usr/include/boost/asio/require_concept.hpp:149:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:149:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable_concept
                           ^
  Report hash: ce5a70174017b68a572a7a1108b34b21
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, require_concept.hpp:149:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:149:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/require_concept.hpp:169:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:169:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable_concept
                           ^
  Report hash: f70a9186f3d566204435277962c8dae6
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, require_concept.hpp:169:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:169:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

[LOW] /usr/include/boost/asio/require_concept.hpp:197:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:197:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable_concept
                           ^
  Report hash: 1ad93ee7e33cc7818c1d31a7b6e38aac
  Steps:
    1, http_request.h:27:12: a definition of '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' is found here
    2, require_concept.hpp:197:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:197:28)', but a definition with the same name '(unnamed struct at /sep/include/crow/../../extern/crow/include/crow/http_request.h:27:12)' found in another namespace 'crow'

Found 3 defect(s) in require_concept.hpp

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:716:9: use of undeclared identifier 'CROW_LIKELY' [clang-diagnostic-error]
        CROW_CALLBACK_NOTIFY_NOADVANCE(message_complete);
        ^
  Report hash: 9fbacb52a2695c0d60f35bfc576f1fe6
  Steps:
    1, http_parser_merged.h:242:46: expanded from macro 'CROW_CALLBACK_NOTIFY_NOADVANCE'
    2, http_parser_merged.h:226:7: expanded from macro 'CROW_CALLBACK_NOTIFY_'
    3, http_parser_merged.h:716:9: use of undeclared identifier 'CROW_LIKELY'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:716:9: use of undeclared identifier 'CROW_UNLIKELY' [clang-diagnostic-error]
        CROW_CALLBACK_NOTIFY_NOADVANCE(message_complete);
        ^
  Report hash: 7c7dc0fc53fa913cdaf68e5fe613ed87
  Steps:
    1, http_parser_merged.h:242:46: expanded from macro 'CROW_CALLBACK_NOTIFY_NOADVANCE'
    2, http_parser_merged.h:227:9: expanded from macro 'CROW_CALLBACK_NOTIFY_'
    3, http_parser_merged.h:716:9: use of undeclared identifier 'CROW_UNLIKELY'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:754:7: use of undeclared identifier 'CROW_UNLIKELY' [clang-diagnostic-error]
      CROW_COUNT_HEADER_SIZE(1);
      ^
  Report hash: 402014a2bc721efeb4e9a83d8aeda482
  Steps:
    1, http_parser_merged.h:295:7: expanded from macro 'CROW_COUNT_HEADER_SIZE'
    2, http_parser_merged.h:754:7: use of undeclared identifier 'CROW_UNLIKELY'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:763:31: use of undeclared identifier 'cr' [clang-diagnostic-error]
        if (CROW_LIKELY(ch == cr || ch == lf))
                              ^
  Report hash: 9287bd77613bf7981d2686fcdf1b25a8
  Steps:
    1, http_parser_merged.h:763:31: use of undeclared identifier 'cr'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:763:43: use of undeclared identifier 'lf' [clang-diagnostic-error]
        if (CROW_LIKELY(ch == cr || ch == lf))
                                          ^
  Report hash: a6c78e247c122c2f87d623bfac8de7ca
  Steps:
    1, http_parser_merged.h:763:43: use of undeclared identifier 'lf'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:771:19: use of undeclared identifier 'cr' [clang-diagnostic-error]
        if (ch == cr || ch == lf)
                  ^
  Report hash: b47ad7ce12e8c77995c52061dd5cf32c
  Steps:
    1, http_parser_merged.h:771:19: use of undeclared identifier 'cr'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:771:31: use of undeclared identifier 'lf' [clang-diagnostic-error]
        if (ch == cr || ch == lf)
                              ^
  Report hash: 8867ff0be0a6773ffbd5093a499d729d
  Steps:
    1, http_parser_merged.h:771:31: use of undeclared identifier 'lf'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:777:13: use of undeclared identifier 'CROW_UNLIKELY' [clang-diagnostic-error]
        if (CROW_UNLIKELY(!CROW_IS_ALPHA(ch))) {
            ^
  Report hash: a7669643ff837e4355c1d7e4b3b70bec
  Steps:
    1, http_parser_merged.h:777:13: use of undeclared identifier 'CROW_UNLIKELY'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:785:48: use of undeclared identifier 'HTTPMethod' [clang-diagnostic-error]
          case 'A': parser->method = (unsigned)HTTPMethod::Acl;                                                              break;
                                               ^
  Report hash: 0d347c8747284450b2b88c2dff686409
  Steps:
    1, http_parser_merged.h:785:48: use of undeclared identifier 'HTTPMethod'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:786:48: use of undeclared identifier 'HTTPMethod' [clang-diagnostic-error]
          case 'B': parser->method = (unsigned)HTTPMethod::Bind;                                                             break;
                                               ^
  Report hash: fa577fb00dbbe16d7849f5cfd3bcb76e
  Steps:
    1, http_parser_merged.h:786:48: use of undeclared identifier 'HTTPMethod'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:787:48: use of undeclared identifier 'HTTPMethod' [clang-diagnostic-error]
          case 'C': parser->method = (unsigned)HTTPMethod::Connect;   /* or COPY, CHECKOUT */                                break;
                                               ^
  Report hash: 090f882caa6da9747a5f3838ec95194e
  Steps:
    1, http_parser_merged.h:787:48: use of undeclared identifier 'HTTPMethod'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:788:48: use of undeclared identifier 'HTTPMethod' [clang-diagnostic-error]
          case 'D': parser->method = (unsigned)HTTPMethod::Delete;                                                           break;
                                               ^
  Report hash: 492c123b6fe064b2d277974b8e0650cb
  Steps:
    1, http_parser_merged.h:788:48: use of undeclared identifier 'HTTPMethod'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:789:48: use of undeclared identifier 'HTTPMethod' [clang-diagnostic-error]
          case 'G': parser->method = (unsigned)HTTPMethod::Get;                                                              break;
                                               ^
  Report hash: 1425cdabbd7e59084d35165b14de8984
  Steps:
    1, http_parser_merged.h:789:48: use of undeclared identifier 'HTTPMethod'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:790:48: use of undeclared identifier 'HTTPMethod' [clang-diagnostic-error]
          case 'H': parser->method = (unsigned)HTTPMethod::Head;                                                             break;
                                               ^
  Report hash: bfc2e13fff50f91fba08b9ea582cbb09
  Steps:
    1, http_parser_merged.h:790:48: use of undeclared identifier 'HTTPMethod'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:791:48: use of undeclared identifier 'HTTPMethod' [clang-diagnostic-error]
          case 'L': parser->method = (unsigned)HTTPMethod::Lock;      /* or LINK */                                          break;
                                               ^
  Report hash: 9b3dce45c587975c71c4a7d490ef9025
  Steps:
    1, http_parser_merged.h:791:48: use of undeclared identifier 'HTTPMethod'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:792:48: use of undeclared identifier 'HTTPMethod' [clang-diagnostic-error]
          case 'M': parser->method = (unsigned)HTTPMethod::MkCol;     /* or MOVE, MKACTIVITY, MERGE, M-SEARCH, MKCALENDAR */ break;
                                               ^
  Report hash: 1302570816ab977a1883c4334c69b550
  Steps:
    1, http_parser_merged.h:792:48: use of undeclared identifier 'HTTPMethod'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:793:48: use of undeclared identifier 'HTTPMethod' [clang-diagnostic-error]
          case 'N': parser->method = (unsigned)HTTPMethod::Notify;                                                           break;
                                               ^
  Report hash: d81a0a56a99b932493e5030389c9d6ca
  Steps:
    1, http_parser_merged.h:793:48: use of undeclared identifier 'HTTPMethod'

[CRITICAL] /usr/local/include/crow/http_parser_merged.h:794:48: use of undeclared identifier 'HTTPMethod' [clang-diagnostic-error]
          case 'O': parser->method = (unsigned)HTTPMethod::Options;                                                          break;
                                               ^
  Report hash: a3daea01bbb3c67f310c636e782f2b91
  Steps:
    1, http_parser_merged.h:794:48: use of undeclared identifier 'HTTPMethod'

Found 18 defect(s) in http_parser_merged.h

[CRITICAL] /sep/extern/crow/include/crow/http_connection.h:82:41: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f711d0b8b0403828502e4b676f897237
  Notes:
    1, http_connection.h:82:41: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, http_connection.h:82:41: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[MEDIUM] /sep/extern/crow/include/crow/http_connection.h:82:59: unused parameter 'ec' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b05964ef37119254f64cee3991dcf5c8
  Steps:
    1, http_connection.h:82:59: unused parameter 'ec'

[CRITICAL] /sep/extern/crow/include/crow/http_connection.h:469:28: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 55211ed2d2667ba42703d31408d9ade7
  Notes:
    1, http_connection.h:469:28: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, http_connection.h:469:28: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[MEDIUM] /sep/extern/crow/include/crow/http_connection.h:469:46: unused parameter 'ec' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9f418878e130fc5ece3f29f4df650a91
  Steps:
    1, http_connection.h:469:46: unused parameter 'ec'

[CRITICAL] /sep/extern/crow/include/crow/http_connection.h:512:28: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e7ca7211fbaa7ff83cecf0a64c73c40f
  Notes:
    1, http_connection.h:512:28: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, http_connection.h:512:28: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[MEDIUM] /sep/extern/crow/include/crow/http_connection.h:512:46: unused parameter 'ec' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0f37fc0e0149d8eaea360eccbb8a3f42
  Steps:
    1, http_connection.h:512:46: unused parameter 'ec'

Found 6 defect(s) in http_connection.h

[MEDIUM] /sep/extern/crow/include/crow/http_parser_merged.h:492:14: both sides of operator are equivalent [misc-redundant-expression]
        0    |   0    |   0    |   0    |   0    |   0    |   0    |   0,
             ^
  Report hash: 6dd8dcd1c48cf794602d0c68b5eca375
  Steps:
    1, http_parser_merged.h:492:14: both sides of operator are equivalent

[MEDIUM] /sep/extern/crow/include/crow/http_parser_merged.h:494:14: both sides of operator are equivalent [misc-redundant-expression]
        0    |CROW_T(2)|  0    |   0    |CROW_T(16)| 0    |   0    |   0,
             ^
  Report hash: 1a4f4b417f4130b7f41ebb25b4298a6c
  Steps:
    1, http_parser_merged.h:494:14: both sides of operator are equivalent

[MEDIUM] /sep/extern/crow/include/crow/http_parser_merged.h:496:14: both sides of operator are equivalent [misc-redundant-expression]
        0    |   0    |   0    |   0    |   0    |   0    |   0    |   0,
             ^
  Report hash: 6dd8dcd1c48cf794602d0c68b5eca375
  Steps:
    1, http_parser_merged.h:496:14: both sides of operator are equivalent

[MEDIUM] /sep/extern/crow/include/crow/http_parser_merged.h:498:14: both sides of operator are equivalent [misc-redundant-expression]
        0    |   0    |   0    |   0    |   0    |   0    |   0    |   0,
             ^
  Report hash: 6dd8dcd1c48cf794602d0c68b5eca375
  Steps:
    1, http_parser_merged.h:498:14: both sides of operator are equivalent

[MEDIUM] /sep/extern/crow/include/crow/http_parser_merged.h:500:68: operator has equivalent nested operands [misc-redundant-expression]
        0    |   2    |   4    |   0    |   16   |   32   |   64   |  128,
                                                                   ^
  Report hash: 03be05b3de31bdf4bdd16819f022c485
  Steps:
    1, http_parser_merged.h:500:68: operator has equivalent nested operands

[MEDIUM] /sep/extern/crow/include/crow/http_parser_merged.h:1774:34: 'signed char' to 'uint64_t' (aka 'unsigned long') conversion; consider casting to 'unsigned char' first. [bugprone-signed-char-misuse]
        parser->content_length = unhex_val;
                                 ^
  Report hash: a0d056c9dfb0f50812e56943db870ab4
  Steps:
    1, http_parser_merged.h:1774:34: 'signed char' to 'uint64_t' (aka 'unsigned long') conversion; consider casting to 'unsigned char' first.

Found 6 defect(s) in http_parser_merged.h

[MEDIUM] /sep/extern/crow/include/crow/http_request.h:55:88: passing result of std::move() as a const reference argument; no move will actually happen [performance-move-const-arg]
          method(method), raw_url(std::move(raw_url)), url(std::move(url)), url_params(std::move(url_params)), headers(std::move(headers)), body(std::move(body)), http_ver_major(http_major), http_ver_minor(http_minor), keep_alive(has_keep_alive), close_connection(has_close_connection), upgrade(is_upgrade)
                                                                                       ^
  Report hash: 5641ba77e1813e83f2ec3e6e72a9733e
  Steps:
    1, query_string.h:293:11: 'query_string' is not move constructible
    2, http_request.h:55:88: passing result of std::move() as a const reference argument; no move will actually happen

Found 1 defect(s) in http_request.h

[MEDIUM] /sep/extern/crow/include/crow/http_response.h:147:9: move constructors should be marked noexcept [performance-noexcept-move-constructor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0101bbca4c676ac5d86202b41ef2ebfe
  Notes:
    1, http_response.h:147:9: { (fixit)
    2, http_response.h:147:2: noexcept  (fixit)
  Steps:
    1, http_response.h:147:9: move constructors should be marked noexcept

Found 1 defect(s) in http_response.h

[MEDIUM] /sep/extern/crow/include/crow/http_server.h:85:29: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ef1a549f85028fa92aefe8c1b1b65180
  Steps:
    1, http_server.h:85:29: cast the expression to void to silence this warning
    2, http_server.h:85:29: the value returned by this function should not be disregarded; neglecting it may lead to errors

[STYLE] /sep/extern/crow/include/crow/http_server.h:88:59: 'data' should be used for accessing the data pointer instead of taking the address of the 0-th element [readability-container-data-pointer]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fec0d597eb9af7dadf3cc473c7b7be94
  Notes:
    1, http_server.h:88:59: date_str.data() (fixit)
  Steps:
    1, http_server.h:88:59: 'data' should be used for accessing the data pointer instead of taking the address of the 0-th element

Found 2 defect(s) in http_server.h

[MEDIUM] /sep/extern/crow/include/crow/json.h:133:17: move constructors should be marked noexcept [performance-noexcept-move-constructor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 457d5845e230f31002e7001be4703d8a
  Notes:
    1, json.h:133:17: { (fixit)
    2, json.h:133:2: noexcept  (fixit)
  Steps:
    1, json.h:133:17: move constructors should be marked noexcept

[MEDIUM] /sep/extern/crow/include/crow/json.h:138:27: move assignment operators should be marked noexcept [performance-noexcept-move-constructor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7b10e105c2ba133cc9b629a877e82993
  Notes:
    1, json.h:138:17: { (fixit)
    2, json.h:138:2: noexcept  (fixit)
  Steps:
    1, json.h:138:27: move assignment operators should be marked noexcept

[LOW] /sep/extern/crow/include/crow/json.h:486:29: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b44d6f6b5e24bda1f7dfe15bf049acc4
  Steps:
    1, json.h:486:29: switching on non-enum value without default case may not cover all cases

[MEDIUM] /sep/extern/crow/include/crow/json.h:1472:13: move constructors should be marked noexcept [performance-noexcept-move-constructor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a0703dd62794abfe2d148aa44fcf9776
  Notes:
    1, json.h:1472:32: noexcept  (fixit)
  Steps:
    1, json.h:1472:13: move constructors should be marked noexcept

[MEDIUM] /sep/extern/crow/include/crow/json.h:1478:21: move assignment operators should be marked noexcept [performance-noexcept-move-constructor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 44add3bcaad1805e1a0d907f8939edce
  Notes:
    1, json.h:1478:13: { (fixit)
    2, json.h:1478:2: noexcept  (fixit)
  Steps:
    1, json.h:1478:21: move assignment operators should be marked noexcept

[MEDIUM] /sep/extern/crow/include/crow/json.h:1856:29: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7e5ef8a91462b9206f5cbffa5704a88e
  Steps:
    1, json.h:1856:29: cast the expression to void to silence this warning
    2, json.h:1856:29: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/crow/include/crow/json.h:1871:53: expression is redundant [misc-redundant-expression]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 070b51d785bd4b2d7d772ca9d21d025f
  Steps:
    1, json.h:1871:53: expression is redundant

Found 7 defect(s) in json.h

[MEDIUM] /sep/extern/crow/include/crow/logging.h:86:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            gmtime_r(&t, &my_tm);
            ^
  Report hash: 0e22d3fd93c080184b70561c29821d40
  Steps:
    1, logging.h:86:13: cast the expression to void to silence this warning
    2, logging.h:86:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 1 defect(s) in logging.h

[MEDIUM] /sep/extern/crow/include/crow/query_string.h:322:23: move assignment operators should be marked noexcept [performance-noexcept-move-constructor]
        query_string& operator=(query_string&& qs)
                      ^
  Report hash: 3fae3ea73e7219fe6514aecea9dd6113
  Notes:
    1, query_string.h:322:9: { (fixit)
    2, query_string.h:322:2: noexcept  (fixit)
  Steps:
    1, query_string.h:322:23: move assignment operators should be marked noexcept

[STYLE] /sep/extern/crow/include/crow/query_string.h:343:37: 'data' should be used for accessing the data pointer instead of taking the address of the 0-th element [readability-container-data-pointer]
            size_t count = qs_parse(&url_[0], &key_value_pairs_[0], MAX_KEY_VALUE_PAIRS_COUNT, url);
                                    ^
  Report hash: dbe8f48a7ce02ea036d90057810f79a8
  Notes:
    1, query_string.h:343:37: url_.data() (fixit)
  Steps:
    1, query_string.h:343:37: 'data' should be used for accessing the data pointer instead of taking the address of the 0-th element

[STYLE] /sep/extern/crow/include/crow/query_string.h:343:47: 'data' should be used for accessing the data pointer instead of taking the address of the 0-th element [readability-container-data-pointer]
            size_t count = qs_parse(&url_[0], &key_value_pairs_[0], MAX_KEY_VALUE_PAIRS_COUNT, url);
                                              ^
  Report hash: b80effb4e13485e007ba1b6edee3223d
  Notes:
    1, query_string.h:343:47: key_value_pairs_.data() (fixit)
  Steps:
    1, query_string.h:343:47: 'data' should be used for accessing the data pointer instead of taking the address of the 0-th element

Found 3 defect(s) in query_string.h

[MEDIUM] /sep/extern/crow/include/crow/routing.h:346:20: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead [bugprone-move-forwarding-reference]
              [f = std::move(f)]
                   ^
  Report hash: 83db8cbb8b898c148a04a42c8b292c0f
  Notes:
    1, routing.h:346:20: std::forward<Func> (fixit)
  Steps:
    1, routing.h:346:20: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead

[MEDIUM] /sep/extern/crow/include/crow/routing.h:368:20: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead [bugprone-move-forwarding-reference]
              [f = std::move(f)]
                   ^
  Report hash: 83db8cbb8b898c148a04a42c8b292c0f
  Notes:
    1, routing.h:368:20: std::forward<Func> (fixit)
  Steps:
    1, routing.h:368:20: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead

[MEDIUM] /sep/extern/crow/include/crow/routing.h:390:20: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead [bugprone-move-forwarding-reference]
              [f = std::move(f)]
                   ^
  Report hash: 83db8cbb8b898c148a04a42c8b292c0f
  Notes:
    1, routing.h:390:20: std::forward<Func> (fixit)
  Steps:
    1, routing.h:390:20: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead

[MEDIUM] /sep/extern/crow/include/crow/routing.h:410:24: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead [bugprone-move-forwarding-reference]
            handler_ = std::move(f);
                       ^
  Report hash: 94db3572474d9d59e5683a57267139bb
  Notes:
    1, routing.h:410:24: std::forward<Func> (fixit)
  Steps:
    1, routing.h:410:24: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead

[MEDIUM] /sep/extern/crow/include/crow/routing.h:666:20: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead [bugprone-move-forwarding-reference]
              [f = std::move(f)]
                   ^
  Report hash: 83db8cbb8b898c148a04a42c8b292c0f
  Notes:
    1, routing.h:666:20: std::forward<Func> (fixit)
  Steps:
    1, routing.h:666:20: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead

[MEDIUM] /sep/extern/crow/include/crow/routing.h:1118:9: move constructors should be marked noexcept [performance-noexcept-move-constructor]
        Blueprint(Blueprint&& value)
        ^
  Report hash: 306b4beee599e398d0b97becd3b3f449
  Notes:
    1, routing.h:1118:9: { (fixit)
    2, routing.h:1118:2: noexcept  (fixit)
  Steps:
    1, routing.h:1118:9: move constructors should be marked noexcept

[LOW] /sep/extern/crow/include/crow/routing.h:1287:47: 2nd argument 'RULE_SPECIAL_REDIRECT_SLASH' (passed to 'rule_index') looks like it might be swapped with the 4th, 'BP_index' (passed to 'blueprint_index') [readability-suspicious-call-argument]
                    per_methods_[method].trie.add(rule_without_trailing_slash, RULE_SPECIAL_REDIRECT_SLASH, BP_index != INVALID_BP_ID ? blueprints[BP_index]->prefix().length() : 0, BP_index);
                                              ^
  Report hash: d7460aceb4a8cabc7b0438c50f1c5ed6
  Steps:
    1, routing.h:999:14: in the call to 'add', declared here
    2, routing.h:1287:47: 2nd argument 'RULE_SPECIAL_REDIRECT_SLASH' (passed to 'rule_index') looks like it might be swapped with the 4th, 'BP_index' (passed to 'blueprint_index')

[MEDIUM] /sep/extern/crow/include/crow/routing.h:1437:61: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead [bugprone-move-forwarding-reference]
                rules[rule_index]->handle_upgrade(req, res, std::move(adaptor));
                                                            ^
  Report hash: 61e934ea51e4f4761af5c066e0342941
  Notes:
    1, routing.h:1437:61: std::forward<Adaptor> (fixit)
  Steps:
    1, routing.h:1437:61: forwarding reference passed to std::move(), which may unexpectedly cause lvalues to be moved; use std::forward() instead

Found 8 defect(s) in routing.h

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:55:13: no type named 'error_code' in namespace 'boost::asio'; did you mean 'std::error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 790460d665b14f9f13281acc50e2bb9b
  Notes:
    1, socket_adaptors.h:55:13: std::error_code (fixit)
  Steps:
    1, system_error:225:9: 'std::error_code' declared here
    2, socket_adaptors.h:55:13: no type named 'error_code' in namespace 'boost::asio'; did you mean 'std::error_code'?

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:56:21: no matching member function for call to 'close' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b158f9d04ef0a74d7a2c84f825584aa2
  Steps:
    1, basic_socket.hpp:545:27: candidate function not viable: no known conversion from 'std::error_code' to 'boost::system::error_code &' for 1st argument
    2, basic_socket.hpp:514:8: candidate function not viable: requires 0 arguments, but 1 was provided
    3, socket_adaptors.h:56:21: no matching member function for call to 'close'

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:61:13: no type named 'error_code' in namespace 'boost::asio'; did you mean 'std::error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 790460d665b14f9f13281acc50e2bb9b
  Notes:
    1, socket_adaptors.h:61:13: std::error_code (fixit)
  Steps:
    1, system_error:225:9: 'std::error_code' declared here
    2, socket_adaptors.h:61:13: no type named 'error_code' in namespace 'boost::asio'; did you mean 'std::error_code'?

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:62:21: no matching member function for call to 'shutdown' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 429a5d3c8b95cdeb57624fadccce3692
  Steps:
    1, basic_socket.hpp:1715:27: candidate function not viable: no known conversion from 'std::error_code' to 'boost::system::error_code &' for 2nd argument
    2, socket_adaptors.h:62:21: no matching member function for call to 'shutdown'

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:67:13: no type named 'error_code' in namespace 'boost::asio'; did you mean 'std::error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 790460d665b14f9f13281acc50e2bb9b
  Notes:
    1, socket_adaptors.h:67:13: std::error_code (fixit)
  Steps:
    1, system_error:225:9: 'std::error_code' declared here
    2, socket_adaptors.h:67:13: no type named 'error_code' in namespace 'boost::asio'; did you mean 'std::error_code'?

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:68:21: no matching member function for call to 'shutdown' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f9bcd4bece9a1dc957779bf8f9ee28d5
  Steps:
    1, basic_socket.hpp:1715:27: candidate function not viable: no known conversion from 'std::error_code' to 'boost::system::error_code &' for 2nd argument
    2, socket_adaptors.h:68:21: no matching member function for call to 'shutdown'

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:73:13: no type named 'error_code' in namespace 'boost::asio'; did you mean 'std::error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 790460d665b14f9f13281acc50e2bb9b
  Notes:
    1, socket_adaptors.h:73:13: std::error_code (fixit)
  Steps:
    1, system_error:225:9: 'std::error_code' declared here
    2, socket_adaptors.h:73:13: no type named 'error_code' in namespace 'boost::asio'; did you mean 'std::error_code'?

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:74:21: no matching member function for call to 'shutdown' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c0b3ca17c06cfe8262452fe052c78b23
  Steps:
    1, basic_socket.hpp:1715:27: candidate function not viable: no known conversion from 'std::error_code' to 'boost::system::error_code &' for 2nd argument
    2, socket_adaptors.h:74:21: no matching member function for call to 'shutdown'

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:80:21: no member named 'error_code' in namespace 'boost::asio' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7f7be9300991d8fe56d49cdc0c73099b
  Steps:
    1, socket_adaptors.h:80:21: no member named 'error_code' in namespace 'boost::asio'

Found 9 defect(s) in socket_adaptors.h

[CRITICAL] /sep/extern/crow/include/crow/task_timer.h:115:37: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e5ee45d59b47cff9c7fdf19fdc0846da
  Notes:
    1, task_timer.h:115:37: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, task_timer.h:115:37: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

Found 1 defect(s) in task_timer.h

[MEDIUM] /sep/extern/crow/include/crow/TinySHA1.hpp:23:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define _TINY_SHA1_HPP_
        ^
  Report hash: 32bc17a3e4975e6e0014db5647579c1d
  Steps:
    1, TinySHA1.hpp:23:9: macro name is a reserved identifier

[MEDIUM] /sep/extern/crow/include/crow/TinySHA1.hpp:41:3: operator=() should return 'SHA1&' [misc-unconventional-assign-operator]
    const SHA1& operator = (const SHA1& s) {
    ^
  Report hash: 0be90924bff0e27adeaad44c356eefa0
  Steps:
    1, TinySHA1.hpp:41:3: operator=() should return 'SHA1&'

Found 2 defect(s) in TinySHA1.hpp

[MEDIUM] /sep/extern/crow/include/crow/utility.h:251:25: identifier '__test' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
            static char __test(int);
                        ^
  Report hash: a470313ba96d36a2bcac7f06df5adaf6
  Steps:
    1, utility.h:251:25: identifier '__test' is reserved because it starts with '__'

[MEDIUM] /sep/extern/crow/include/crow/utility.h:254:24: identifier '__test' is reserved because it starts with '__' [clang-diagnostic-reserved-identifier]
            static int __test(...);
                       ^
  Report hash: e6ec93565d8d5c7f2850799586322772
  Steps:
    1, utility.h:254:24: identifier '__test' is reserved because it starts with '__'

[LOW] /sep/extern/crow/include/crow/utility.h:746:21: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
                    switch (toUpper(data[i]))
                    ^
  Report hash: c0e825a6e578409082b7e499822bb485
  Steps:
    1, utility.h:746:21: switching on non-enum value without default case may not cover all cases

Found 3 defect(s) in utility.h

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:303:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 00caf74bfe0a89451a584bf295eddd35
  Notes:
    1, websocket.h:303:40: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, websocket.h:303:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[MEDIUM] /sep/extern/crow/include/crow/websocket.h:303:58: unused parameter 'ec' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b47a9b0af53982050ad90137990ed24e
  Steps:
    1, websocket.h:303:58: unused parameter 'ec'

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:371:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 00caf74bfe0a89451a584bf295eddd35
  Notes:
    1, websocket.h:371:40: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, websocket.h:371:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[MEDIUM] /sep/extern/crow/include/crow/websocket.h:371:58: unused parameter 'ec' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b47a9b0af53982050ad90137990ed24e
  Steps:
    1, websocket.h:371:58: unused parameter 'ec'

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:407:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 00caf74bfe0a89451a584bf295eddd35
  Notes:
    1, websocket.h:407:40: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, websocket.h:407:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[MEDIUM] /sep/extern/crow/include/crow/websocket.h:407:58: unused parameter 'ec' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b47a9b0af53982050ad90137990ed24e
  Steps:
    1, websocket.h:407:58: unused parameter 'ec'

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:451:44: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 117e79ffc1947346aa76ae6767f43ffc
  Notes:
    1, websocket.h:451:44: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, websocket.h:451:44: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[MEDIUM] /sep/extern/crow/include/crow/websocket.h:451:62: unused parameter 'ec' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 44aa3b18673128e86a670551a5d36102
  Steps:
    1, websocket.h:451:62: unused parameter 'ec'

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:493:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3dde67621205128b8ba1ba96c3187ee6
  Notes:
    1, websocket.h:493:40: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, websocket.h:493:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[MEDIUM] /sep/extern/crow/include/crow/websocket.h:493:58: unused parameter 'ec' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 50423260de2015dba715b1106f44fc31
  Steps:
    1, websocket.h:493:58: unused parameter 'ec'

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:645:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: db0d59c440c283bcfec2349f5dde93b2
  Notes:
    1, websocket.h:645:40: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, websocket.h:645:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[MEDIUM] /sep/extern/crow/include/crow/websocket.h:645:58: unused parameter 'ec' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e0511325d3725d6137e8a87c991fc0ca
  Steps:
    1, websocket.h:645:58: unused parameter 'ec'

Found 12 defect(s) in websocket.h

[CRITICAL] /sep/extern/crow/include/crow/http_connection.h:82:41: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f711d0b8b0403828502e4b676f897237
  Notes:
    1, http_connection.h:82:41: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, http_connection.h:82:41: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/http_connection.h:469:28: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 55211ed2d2667ba42703d31408d9ade7
  Notes:
    1, http_connection.h:469:28: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, http_connection.h:469:28: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/http_connection.h:512:28: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e7ca7211fbaa7ff83cecf0a64c73c40f
  Notes:
    1, http_connection.h:512:28: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, http_connection.h:512:28: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/http_connection.h:543:57: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3d00d5e0fee2f4a752652c9c580b6eb8
  Notes:
    1, http_connection.h:543:57: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, http_connection.h:543:57: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[MEDIUM] /sep/extern/crow/include/crow/http_connection.h:543:74: unused parameter 'ec' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: edb8eee3ca7132192bbbc78447771a48
  Steps:
    1, http_connection.h:543:74: unused parameter 'ec'

Found 5 defect(s) in http_connection.h

[CRITICAL] /sep/extern/crow/include/crow/http_server.h:53:49: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bd40368c937f94a6034aa65bb7e958d0
  Notes:
    1, http_server.h:53:49: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, http_server.h:53:49: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/http_server.h:129:32: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f1a7b1aa4bee484654dee2e9f5f0007d
  Notes:
    1, http_server.h:129:32: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, http_server.h:129:32: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/http_server.h:144:25: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1cba3c2ae974b68bef24dfb5e3013c70
  Notes:
    1, http_server.h:144:25: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, http_server.h:144:25: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

Found 3 defect(s) in http_server.h

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:55:13: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 60ba18d6935a2ef2503d3d5f8bdfcf80
  Notes:
    1, socket_adaptors.h:55:13: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, socket_adaptors.h:55:13: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:61:13: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 60ba18d6935a2ef2503d3d5f8bdfcf80
  Notes:
    1, socket_adaptors.h:61:13: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, socket_adaptors.h:61:13: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:67:13: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 60ba18d6935a2ef2503d3d5f8bdfcf80
  Notes:
    1, socket_adaptors.h:67:13: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, socket_adaptors.h:67:13: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/socket_adaptors.h:73:13: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 60ba18d6935a2ef2503d3d5f8bdfcf80
  Notes:
    1, socket_adaptors.h:73:13: error_code (fixit)
  Steps:
    1, http_request.h:16:11: 'error_code' declared here
    2, socket_adaptors.h:73:13: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

Found 4 defect(s) in socket_adaptors.h

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:303:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 00caf74bfe0a89451a584bf295eddd35
  Notes:
    1, websocket.h:303:40: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, websocket.h:303:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:371:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 00caf74bfe0a89451a584bf295eddd35
  Notes:
    1, websocket.h:371:40: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, websocket.h:371:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:407:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 00caf74bfe0a89451a584bf295eddd35
  Notes:
    1, websocket.h:407:40: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, websocket.h:407:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:451:44: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 117e79ffc1947346aa76ae6767f43ffc
  Notes:
    1, websocket.h:451:44: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, websocket.h:451:44: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:493:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3dde67621205128b8ba1ba96c3187ee6
  Notes:
    1, websocket.h:493:40: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, websocket.h:493:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

[CRITICAL] /sep/extern/crow/include/crow/websocket.h:645:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: db0d59c440c283bcfec2349f5dde93b2
  Notes:
    1, websocket.h:645:40: error_code (fixit)
  Steps:
    1, task_timer.h:17:11: 'error_code' declared here
    2, websocket.h:645:40: no type named 'error_code' in namespace 'boost::asio'; did you mean simply 'error_code'?

Found 6 defect(s) in websocket.h

[MEDIUM] /sep/include/api/server.h:25:1: class 'request' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 82fa53630b2d2acc978d65fe1ca496d7
  Steps:
    1, http_request.h:30:12: previous use is here
    2, server.h:25:1: did you mean struct here?
    3, server.h:25:1: class 'request' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/server.h:26:1: class 'response' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5b3fcf90561fa3090a801d6c46ec5b78
  Steps:
    1, http_response.h:33:12: previous use is here
    2, server.h:26:1: did you mean struct here?
    3, server.h:26:1: class 'response' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in server.h

[LOW] /sep/include/compat/cufft.h:14:19: direct self-inclusion of header file 'cufft.h' [misc-header-include-cycle]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a196ddcfc730f5947d1846a5aa3c4ef5
  Steps:
    1, cufft.h:14:19: direct self-inclusion of header file 'cufft.h'

Found 1 defect(s) in cufft.h

[MEDIUM] /sep/include/compat/memory.h:109:63: unused parameter 'stream' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 03fe062d6c7e2fcf37a2b293d4c6d7a1
  Steps:
    1, memory.h:109:63: unused parameter 'stream'

[MEDIUM] /sep/include/compat/memory.h:121:61: unused parameter 'stream' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d7af2a21b214b2470caa64843624d8f9
  Steps:
    1, memory.h:121:61: unused parameter 'stream'

Found 2 defect(s) in memory.h

[CRITICAL] /sep/include/compat/cuda_api.hpp:14:1: unknown type name 'SEP_API' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 69d1cb4d9f645edbfae84f35b2c045a4
  Steps:
    1, cuda_api.hpp:14:1: unknown type name 'SEP_API'

[CRITICAL] /sep/include/compat/cuda_api.hpp:14:14: no member named 'SEPResult' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b8db5709b0e08cca7bf50fe330fcfa3f
  Steps:
    1, cuda_api.hpp:14:14: no member named 'SEPResult' in namespace 'sep'

[CRITICAL] /sep/include/compat/cuda_api.hpp:14:23: expected ';' after top level declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6d06a71d15c4ebacd536c33f2afa2db0
  Notes:
    1, cuda_api.hpp:14:23: ; (fixit)
  Steps:
    1, cuda_api.hpp:14:23: expected ';' after top level declarator

[CRITICAL] /sep/include/compat/cuda_api.hpp:15:1: unknown type name 'SEP_API' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d8fc45801bd8e33475d0c070a631f2b2
  Steps:
    1, cuda_api.hpp:15:1: unknown type name 'SEP_API'

[CRITICAL] /sep/include/compat/cuda_api.hpp:15:14: no member named 'SEPResult' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5d74a21aa27d7effe62414069f8ff0fa
  Steps:
    1, cuda_api.hpp:15:14: no member named 'SEPResult' in namespace 'sep'

[CRITICAL] /sep/include/compat/cuda_api.hpp:15:23: expected ';' after top level declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 531289ab5ff4667fcfa649a58a52576f
  Notes:
    1, cuda_api.hpp:15:23: ; (fixit)
  Steps:
    1, cuda_api.hpp:15:23: expected ';' after top level declarator

[CRITICAL] /sep/include/compat/cuda_api.hpp:18:1: unknown type name 'SEP_API' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4339062ba06ad21e8f15f89410135f20
  Steps:
    1, cuda_api.hpp:18:1: unknown type name 'SEP_API'

[CRITICAL] /sep/include/compat/cuda_api.hpp:18:14: no member named 'SEPResult' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cef9b9f8f823c0447ac806b975ce19ef
  Steps:
    1, cuda_api.hpp:18:14: no member named 'SEPResult' in namespace 'sep'

[CRITICAL] /sep/include/compat/cuda_api.hpp:18:23: expected ';' after top level declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2346e5bc020f7b60e4c22a3e2fb38fe3
  Notes:
    1, cuda_api.hpp:18:23: ; (fixit)
  Steps:
    1, cuda_api.hpp:18:23: expected ';' after top level declarator

[CRITICAL] /sep/include/compat/cuda_api.hpp:27:1: unknown type name 'SEP_API' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4aa89b07178794ddb418fc675129d682
  Steps:
    1, cuda_api.hpp:27:1: unknown type name 'SEP_API'

[CRITICAL] /sep/include/compat/cuda_api.hpp:27:14: no member named 'SEPResult' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dcf194fbaf9230117946f6a2f3ac40d8
  Steps:
    1, cuda_api.hpp:27:14: no member named 'SEPResult' in namespace 'sep'

[CRITICAL] /sep/include/compat/cuda_api.hpp:27:23: expected ';' after top level declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 64e0c02265d0f98c7c9412b77fe62b28
  Notes:
    1, cuda_api.hpp:27:23: ; (fixit)
  Steps:
    1, cuda_api.hpp:27:23: expected ';' after top level declarator

[CRITICAL] /sep/include/compat/cuda_api.hpp:35:1: unknown type name 'SEP_API' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 77b3da4139c4736fcebff774191fa1e9
  Steps:
    1, cuda_api.hpp:35:1: unknown type name 'SEP_API'

[CRITICAL] /sep/include/compat/cuda_api.hpp:35:20: expected ';' after top level declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7a1fed477846733aaa727b374081fc52
  Notes:
    1, cuda_api.hpp:35:20: ; (fixit)
  Steps:
    1, cuda_api.hpp:35:20: expected ';' after top level declarator

[CRITICAL] /sep/include/compat/cuda_api.hpp:36:1: unknown type name 'SEP_API' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 26fafd624aea6b3b31a7b2f5d037dc07
  Steps:
    1, cuda_api.hpp:36:1: unknown type name 'SEP_API'

[CRITICAL] /sep/include/compat/cuda_api.hpp:36:20: expected ';' after top level declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cb006fd8633f3aebc4bb5327cdefe22f
  Notes:
    1, cuda_api.hpp:36:20: ; (fixit)
  Steps:
    1, cuda_api.hpp:36:20: expected ';' after top level declarator

[CRITICAL] /sep/include/compat/cuda_api.hpp:37:1: unknown type name 'SEP_API' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 971f647421101b92f8f4c736db2d22a0
  Steps:
    1, cuda_api.hpp:37:1: unknown type name 'SEP_API'

[CRITICAL] /sep/include/compat/cuda_api.hpp:37:20: expected ';' after top level declarator [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 16416757e0f4e056d25bbd1965333de9
  Notes:
    1, cuda_api.hpp:37:20: ; (fixit)
  Steps:
    1, cuda_api.hpp:37:20: expected ';' after top level declarator

Found 18 defect(s) in cuda_api.hpp

[MEDIUM] /sep/src/core/engine.cpp:68:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:68:6: cast the expression to void to silence this warning
    2, engine.cpp:68:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:74:14: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 39120b7c5df7534093597522738993e6
  Steps:
    1, engine.cpp:74:14: cast the expression to void to silence this warning
    2, engine.cpp:74:14: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:79:18: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab6f901229b488bfc64f4551fa611064
  Steps:
    1, engine.cpp:79:18: cast the expression to void to silence this warning
    2, engine.cpp:79:18: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:83:18: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab6f901229b488bfc64f4551fa611064
  Steps:
    1, engine.cpp:83:18: cast the expression to void to silence this warning
    2, engine.cpp:83:18: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:91:10: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 97b2e01d3d0212a4a0c9e9ec53f3efe5
  Steps:
    1, engine.cpp:91:10: cast the expression to void to silence this warning
    2, engine.cpp:91:10: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:112:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:112:6: cast the expression to void to silence this warning
    2, engine.cpp:112:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 6 defect(s) in engine.cpp

[LOW] /usr/include/glm/gtc/bitfield.inl:343:15: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x' [deadcode.DeadStores]
    uint16 REG2(x >>= 1);
                ^
  Report hash: 1e773bd29984d4372b430ea9f83aa8cf
  Steps:
    1, bitfield.inl:343:15: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x'

[LOW] /usr/include/glm/gtc/bitfield.inl:397:20: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x' [deadcode.DeadStores]
    glm::uint32 REG2(x >>= 1);
                     ^
  Report hash: 031d05d8df11fb2b7b4b94b5bf3e5bc6
  Steps:
    1, bitfield.inl:397:20: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x'

[LOW] /usr/include/glm/gtc/bitfield.inl:451:20: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x' [deadcode.DeadStores]
    glm::uint64 REG2(x >>= 1);
                     ^
  Report hash: 13c9f8fbeed87c6dad3b9d3f53be2237
  Steps:
    1, bitfield.inl:451:20: Although the value stored to 'x' is used in the enclosing expression, the value is never actually read from 'x'

Found 3 defect(s) in bitfield.inl

[MEDIUM] /sep/extern/crow/include/crow/common.h:291:11: 1 uninitialized field at the end of the constructor call [optin.cplusplus.UninitializedObject]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d88b84cca1112223acdd0d15fd5b030d
  Macro expansions:
    1, routing.h:1539:17: Macro 'CROW_UNLIKELY(req.method >= HTTPMethod::InternalMethodCount)' expanded to '__builtin_expect (!!(req .method >=HTTPMethod ::InternalMethodCount ),0)'
  Notes:
    1, common.h:284:20: uninitialized field 'this->method'
  Steps:
     1, routing.h:1539:17: Assuming field 'method' is < InternalMethodCount
     2, routing.h:1541:22: Assuming field 'method' is not equal to Head
     3, routing.h:1562:22: Assuming field 'method' is not equal to Options
     4, routing.h:1620:26: Calling 'Trie::find'
     5, routing.h:993:9: Entered call from 'Router::handle_initial'
     6, routing.h:995:20: Calling 'Trie::find'
     7, routing.h:835:9: Entered call from 'Trie::find'
     8, routing.h:860:17: Assuming the condition is true
     9, routing.h:863:24: Calling constructor for 'routing_handle_result'
    10, common.h:288:9: Entered call from 'Trie::find'
    11, common.h:291:11: 1 uninitialized field at the end of the constructor call

[MEDIUM] /sep/extern/crow/include/crow/common.h:291:11: 1 uninitialized field at the end of the constructor call [optin.cplusplus.UninitializedObject]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a8dbed4ab6c06c009f8d768ae46d7d6c
  Macro expansions:
    1, routing.h:1539:17: Macro 'CROW_UNLIKELY(req.method >= HTTPMethod::InternalMethodCount)' expanded to '__builtin_expect (!!(req .method >=HTTPMethod ::InternalMethodCount ),0)'
  Notes:
    1, common.h:284:20: uninitialized field 'this->method'
  Steps:
     1, routing.h:1539:17: Assuming field 'method' is < InternalMethodCount
     2, routing.h:1541:22: Assuming field 'method' is not equal to Head
     3, routing.h:1562:22: Assuming field 'method' is not equal to Options
     4, routing.h:1620:26: Calling 'Trie::find'
     5, routing.h:993:9: Entered call from 'Router::handle_initial'
     6, routing.h:995:20: Calling 'Trie::find'
     7, routing.h:835:9: Entered call from 'Trie::find'
     8, routing.h:860:17: Assuming the condition is false
     9, routing.h:868:36: Loop body skipped when range is empty
    10, routing.h:990:20: Calling constructor for 'routing_handle_result'
    11, common.h:288:9: Entered call from 'Trie::find'
    12, common.h:291:11: 1 uninitialized field at the end of the constructor call

Found 2 defect(s) in common.h

[HIGH] /sep/extern/crow/include/crow/routing.h:863:63: Moved-from object 'MT' of type 'std::vector' is copied [cplusplus.Move]
                return routing_handle_result{node.rule_index, *blueprints, *params};
                                                              ^
  Report hash: 7b70a7f507f67c3804cc819c07887e3b
  Macro expansions:
    1, routing.h:1539:17: Macro 'CROW_UNLIKELY(req.method >= HTTPMethod::InternalMethodCount)' expanded to '__builtin_expect (!!(req .method >=HTTPMethod ::InternalMethodCount ),0)'
  Steps:
     1, routing.h:1539:17: Assuming field 'method' is < InternalMethodCount
     2, routing.h:1541:22: Assuming field 'method' is not equal to Head
     3, routing.h:1562:22: Assuming field 'method' is not equal to Options
     4, routing.h:1620:26: Calling 'Trie::find'
     5, routing.h:993:9: Entered call from 'Router::handle_initial'
     6, routing.h:995:20: Calling 'Trie::find'
     7, routing.h:835:9: Entered call from 'Trie::find'
     8, routing.h:860:17: Assuming the condition is true
     9, routing.h:862:17: Object 'MT' of type 'std::vector' is left in a valid but unspecified state after move
    10, routing.h:863:63: Moved-from object 'MT' of type 'std::vector' is copied

[HIGH] /sep/extern/crow/include/crow/routing.h:982:51: Method called on moved-from object 'MT' of type 'std::vector' [cplusplus.Move]
                        if (!blueprints->empty()) blueprints->pop_back();
                                                  ^
  Report hash: 7b70a7f507f67c3804cc819c07887e3b
  Macro expansions:
    1, routing.h:1539:17: Macro 'CROW_UNLIKELY(req.method >= HTTPMethod::InternalMethodCount)' expanded to '__builtin_expect (!!(req .method >=HTTPMethod ::InternalMethodCount ),0)'
  Steps:
     1, routing.h:1539:17: Assuming field 'method' is < InternalMethodCount
     2, routing.h:1541:22: Assuming field 'method' is not equal to Head
     3, routing.h:1562:22: Assuming field 'method' is not equal to Options
     4, routing.h:1622:21: Assuming field 'rule_index' is 0
     5, routing.h:1624:43: Assuming '__begin6' is not equal to '__end6'
     6, routing.h:1624:43: Entering loop body
     7, routing.h:1626:29: Calling 'Trie::find'
     8, routing.h:993:9: Entered call from 'Router::handle_initial'
     9, routing.h:995:20: Calling 'Trie::find'
    10, routing.h:835:9: Entered call from 'Trie::find'
    11, routing.h:860:17: Assuming the condition is false
    12, routing.h:868:36: Entering loop body
    13, routing.h:870:21: Assuming field 'param' is equal to MAX
    14, routing.h:976:25: Assuming the condition is true
    15, routing.h:979:29: Assuming 'INVALID_BP_ID' is equal to field 'blueprint_index'
    16, routing.h:980:36: Calling 'Trie::find'
    17, routing.h:835:9: Entered call from 'Trie::find'
    18, routing.h:860:17: Assuming the condition is false
    19, routing.h:868:36: Loop body skipped when range is empty
    20, routing.h:988:17: Object 'MT' of type 'std::vector' is left in a valid but unspecified state after move
    21, routing.h:980:36: Returning from 'Trie::find'
    22, routing.h:982:29: Assuming the condition is true
    23, routing.h:982:51: Method called on moved-from object 'MT' of type 'std::vector'

Found 2 defect(s) in routing.h

[LOW] /usr/include/boost/asio/detail/impl/signal_set_service.ipp:146:12: Call to blocking function 'read' inside of critical section [unix.BlockInCriticalSection]
    while (::read(fd, &signal_number, sizeof(int)) == sizeof(int))
           ^
  Report hash: 5770f89bb4da766be512d01ba33e841c
  Steps:
     1, signal_set_service.ipp:146:12: Assuming the condition is true
     2, signal_set_service.ipp:146:12: Entering loop body
     3, signal_set_service.ipp:147:11: Assuming 'signal_number' is >= 0
     4, signal_set_service.ipp:147:33: Assuming 'signal_number' is < max_signal_number
     5, signal_set_service.ipp:148:9: Calling 'signal_set_service::deliver_signal'
     6, signal_set_service.ipp:611:1: Entered call from 'pipe_read_op::do_perform'
     7, signal_set_service.ipp:617:10: Entering loop body
     8, signal_set_service.ipp:622:12: Entering loop body
     9, signal_set_service.ipp:630:27: Entering loop body
    10, signal_set_service.ipp:630:9: Looping back to the head of the loop
    11, signal_set_service.ipp:630:27: Loop body executed 0 times
    12, signal_set_service.ipp:622:5: Looping back to the head of the loop
    13, signal_set_service.ipp:641:5: Calling 'scheduler::post_deferred_completions'
    14, scheduler.ipp:411:1: Entered call from 'signal_set_service::deliver_signal'
    15, scheduler.ipp:417:9: Assuming field 'one_thread_' is false
    16, scheduler.ipp:427:24: Calling constructor for 'scoped_lock'
    17, conditionally_enabled_mutex.hpp:50:5: Entered call from 'scheduler::post_deferred_completions'
    18, conditionally_enabled_mutex.hpp:53:11: Assuming field 'enabled_' is true
    19, conditionally_enabled_mutex.hpp:55:9: Calling 'posix_mutex::lock'
    20, posix_mutex.hpp:50:3: Entered call from constructor for 'scoped_lock'
    21, posix_mutex.hpp:52:11: Entering critical section here
    22, conditionally_enabled_mutex.hpp:55:9: Returning from 'posix_mutex::lock'
    23, scheduler.ipp:427:24: Returning from constructor for 'scoped_lock'
    24, signal_set_service.ipp:641:5: Returning from 'scheduler::post_deferred_completions'
    25, signal_set_service.ipp:617:3: Looping back to the head of the loop
    26, signal_set_service.ipp:148:9: Returning from 'signal_set_service::deliver_signal'
    27, signal_set_service.ipp:146:5: Looping back to the head of the loop
    28, signal_set_service.ipp:146:12: Call to blocking function 'read' inside of critical section

Found 1 defect(s) in signal_set_service.ipp

[MEDIUM] /sep/src/core/engine.cpp:67:6: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 20320af19fc786b32ca4a60b432107fa
  Steps:
    1, engine.cpp:67:6: cast the expression to void to silence this warning
    2, engine.cpp:67:6: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:71:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:71:5: cast the expression to void to silence this warning
    2, engine.cpp:71:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 2 defect(s) in engine.cpp

[MEDIUM] /sep/_sep/testbed/context_algorithms.hpp:78:44: implicit conversion increases floating-point precision: 'value_type' (aka 'float') to 'value_type' (aka 'double') [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5df491699c147a1a5760c1b3c02927d7
  Steps:
    1, context_algorithms.hpp:78:44: implicit conversion increases floating-point precision: 'value_type' (aka 'float') to 'value_type' (aka 'double')

Found 1 defect(s) in context_algorithms.hpp

[MEDIUM] /sep/extern/crow/include/crow/app.h:103:45: passing result of std::move() as a const reference argument; no move will actually happen [performance-move-const-arg]
            return router_.new_rule_dynamic(std::move(rule));
                                            ^
  Report hash: ba5f3bdcca1eaaf2e3a42b0765ef888c
  Steps:
    1, app.h:103:45: passing result of std::move() as a const reference argument; no move will actually happen

Found 1 defect(s) in app.h

[MEDIUM] /sep/extern/crow/include/crow/http_server.h:86:29: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                            gmtime_r(&last_time_t, &my_tm);
                            ^
  Report hash: ef1a549f85028fa92aefe8c1b1b65180
  Steps:
    1, http_server.h:86:29: cast the expression to void to silence this warning
    2, http_server.h:86:29: the value returned by this function should not be disregarded; neglecting it may lead to errors

[STYLE] /sep/extern/crow/include/crow/http_server.h:89:59: 'data' should be used for accessing the data pointer instead of taking the address of the 0-th element [readability-container-data-pointer]
                            size_t date_str_sz = strftime(&date_str[0], 99, "%a, %d %b %Y %H:%M:%S GMT", &my_tm);
                                                          ^
  Report hash: fec0d597eb9af7dadf3cc473c7b7be94
  Notes:
    1, http_server.h:89:59: date_str.data() (fixit)
  Steps:
    1, http_server.h:89:59: 'data' should be used for accessing the data pointer instead of taking the address of the 0-th element

Found 2 defect(s) in http_server.h

[MEDIUM] /sep/extern/crow/include/crow/socket_adaptors.h:56:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.close(ec);
            ^
  Report hash: 65dbab1b803153775327fae80895489b
  Steps:
    1, socket_adaptors.h:56:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/crow/include/crow/socket_adaptors.h:56:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.close(ec);
            ^
  Report hash: 341bf432f71521ae828cd32f95f83569
  Steps:
    1, socket_adaptors.h:56:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/crow/include/crow/socket_adaptors.h:62:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.shutdown(asio::socket_base::shutdown_type::shutdown_both, ec);
            ^
  Report hash: 1e229a6b766d66c3cacf03134edc0b41
  Steps:
    1, socket_adaptors.h:62:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/crow/include/crow/socket_adaptors.h:62:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.shutdown(asio::socket_base::shutdown_type::shutdown_both, ec);
            ^
  Report hash: bd6c6f371cfa4b64d60792e0ef9354fb
  Steps:
    1, socket_adaptors.h:62:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/crow/include/crow/socket_adaptors.h:68:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.shutdown(asio::socket_base::shutdown_type::shutdown_send, ec);
            ^
  Report hash: 771cb5d8e12cbb28136d5ea60d4825f3
  Steps:
    1, socket_adaptors.h:68:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/crow/include/crow/socket_adaptors.h:68:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.shutdown(asio::socket_base::shutdown_type::shutdown_send, ec);
            ^
  Report hash: b33e477bbb5d1bd31f124d7df3093bd6
  Steps:
    1, socket_adaptors.h:68:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/crow/include/crow/socket_adaptors.h:74:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.shutdown(asio::socket_base::shutdown_type::shutdown_receive, ec);
            ^
  Report hash: 0aa829d6187851394b69c817bfb4cae8
  Steps:
    1, socket_adaptors.h:74:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/crow/include/crow/socket_adaptors.h:74:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.shutdown(asio::socket_base::shutdown_type::shutdown_receive, ec);
            ^
  Report hash: 09ae037f4d546fd27b0ec5e09cdb74bf
  Steps:
    1, socket_adaptors.h:74:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 8 defect(s) in socket_adaptors.h

[MEDIUM] /sep/include/api/crow_adapter.h:15:5: class 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f18998871189c82b7a4da78e55d76987
  Steps:
    1, http_request.h:30:12: previous use is here
    2, crow_adapter.h:15:5: did you mean struct here?
    3, crow_adapter.h:15:5: class 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/crow_adapter.h:16:5: class 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 17aeb083bb607e276f0b48bc7c728902
  Steps:
    1, http_response.h:33:12: previous use is here
    2, crow_adapter.h:16:5: did you mean struct here?
    3, crow_adapter.h:16:5: class 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in crow_adapter.h

[MEDIUM] /sep/include/compat/cycles.h:69:5: unused parameter 'row' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 93c529ac6bc26ac89a838cfb38a846dd
  Steps:
    1, cycles.h:69:5: unused parameter 'row'

[MEDIUM] /sep/include/compat/cycles.h:69:14: unused parameter 'col' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4b18b8d3953c28320a399cdc7a663d59
  Steps:
    1, cycles.h:69:14: unused parameter 'col'

[MEDIUM] /sep/include/compat/cycles.h:72:14: unused parameter 'i' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7ca3cbbb8cff9f3980157fc36cb41c41
  Steps:
    1, cycles.h:72:14: unused parameter 'i'

[MEDIUM] /sep/include/compat/cycles.h:73:4: unused parameter 'i' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a0cb60dbd34f737292f804c947902144
  Steps:
    1, cycles.h:73:4: unused parameter 'i'

[MEDIUM] /sep/include/compat/cycles.h:77:15: unused parameter 'min' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 89dafc703fbaf80b040081f844786739
  Steps:
    1, cycles.h:77:15: unused parameter 'min'

[MEDIUM] /sep/include/compat/cycles.h:78:7: unused parameter 'max' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cd5d8fb74f851b57cefe75c415a7e586
  Steps:
    1, cycles.h:78:7: unused parameter 'max'

[MEDIUM] /sep/include/compat/cycles.h:80:22: unused parameter 'other' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d5fdac0eb2faccec150e2c374d3d38c5
  Steps:
    1, cycles.h:80:22: unused parameter 'other'

Found 7 defect(s) in cycles.h

[CRITICAL] /sep/include/core/dag_graph.h:35:36: no template named 'is_integral_v' in namespace 'std'; did you mean 'is_integral'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4aaa7d030b66d7459be1993064d89a30
  Notes:
    1, dag_graph.h:35:36: is_integral (fixit)
  Steps:
    1, type_traits:466:12: 'is_integral' declared here
    2, dag_graph.h:35:36: no template named 'is_integral_v' in namespace 'std'; did you mean 'is_integral'?

[CRITICAL] /sep/include/core/dag_graph.h:35:55: expected '>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: afcccf58f834632e369e73d456e6a87a
  Steps:
    1, dag_graph.h:35:30: to match this '<'
    2, dag_graph.h:35:55: expected '>'

[CRITICAL] /sep/include/core/dag_graph.h:35:56: expected member name or ';' after declaration specifiers [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 589d97bdc898d82700e3764f13df763d
  Steps:
    1, dag_graph.h:35:56: expected member name or ';' after declaration specifiers

Found 3 defect(s) in dag_graph.h

[CRITICAL] /sep/src/core/dag_graph.cpp:12:19: use of undeclared identifier 'next_id_' [clang-diagnostic-error]
    uint64_t id = next_id_++;
                  ^
  Report hash: ab7d5aac961e9d56e6ad78eaf36733f4
  Steps:
    1, dag_graph.cpp:12:19: use of undeclared identifier 'next_id_'

[CRITICAL] /sep/src/core/dag_graph.cpp:23:15: use of undeclared identifier 'next_id_' [clang-diagnostic-error]
    if (id >= next_id_)
              ^
  Report hash: 1d341f0047b21953446144d7ad695449
  Steps:
    1, dag_graph.cpp:23:15: use of undeclared identifier 'next_id_'

[CRITICAL] /sep/src/core/dag_graph.cpp:25:9: use of undeclared identifier 'next_id_' [clang-diagnostic-error]
        next_id_ = id + 1;
        ^
  Report hash: cd46477606a1f860dbe83e039aa12ecd
  Steps:
    1, dag_graph.cpp:25:9: use of undeclared identifier 'next_id_'

Found 3 defect(s) in dag_graph.cpp

[CRITICAL] /sep/include/memory/redis_manager.h:21:18: no template named 'optional' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d68a6edd5ca6ec03762b094624ae6084
  Steps:
    1, redis_manager.h:21:18: no template named 'optional' in namespace 'std'

[CRITICAL] /sep/include/memory/redis_manager.h:35:10: no template named 'optional' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 862e50a4463bb365d50146254ec84cac
  Steps:
    1, redis_manager.h:35:10: no template named 'optional' in namespace 'std'

[CRITICAL] /sep/include/memory/redis_manager.h:50:14: no template named 'optional' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a27864226859d696a2a321902e6c732f
  Steps:
    1, redis_manager.h:50:14: no template named 'optional' in namespace 'std'

Found 3 defect(s) in redis_manager.h

[CRITICAL] /sep/extern/cycles/src/graph/node.h:110:46: template argument for non-type template parameter must be an expression [clang-diagnostic-error]
  template<class ValueType, std::enable_if_t<std::is_enum_v<ValueType>, bool> = true>
                                             ^
  Report hash: 653b681068d29391f47f99b86c24e3d8
  Steps:
    1, type_traits:2836:17: template parameter is declared here
    2, node.h:110:46: template argument for non-type template parameter must be an expression

[CRITICAL] /sep/extern/cycles/src/graph/node.h:110:51: no template named 'is_enum_v' in namespace 'std'; did you mean 'is_enum'? [clang-diagnostic-error]
  template<class ValueType, std::enable_if_t<std::is_enum_v<ValueType>, bool> = true>
                                                  ^
  Report hash: d0d8b0bbcedef08760c7014d71e1af23
  Notes:
    1, node.h:110:51: is_enum (fixit)
  Steps:
    1, type_traits:642:12: 'is_enum' declared here
    2, node.h:110:51: no template named 'is_enum_v' in namespace 'std'; did you mean 'is_enum'?

[CRITICAL] /sep/extern/cycles/src/graph/node.h:113:19: static assertion failed due to requirement 'sizeof(ccl::Shader *) <= sizeof(int)': Enumerator type should fit int [clang-diagnostic-error]
    static_assert(sizeof(ValueType) <= sizeof(int), "Enumerator type should fit int");
                  ^
  Report hash: 48f27f8f8adb34d0405267fb1099590f
  Steps:
    1, background.h:25:3: in instantiation of function template specialization 'ccl::Node::set<ccl::Shader *, 1>' requested here
    2, node.h:61:11: expanded from macro 'NODE_SOCKET_API'
    3, node.h:113:37: expression evaluates to '8 <= 4'
    4, node.h:113:19: static assertion failed due to requirement 'sizeof(ccl::Shader *) <= sizeof(int)': Enumerator type should fit int

[CRITICAL] /sep/extern/cycles/src/graph/node.h:114:16: static_cast from 'ccl::Shader *' to 'int' is not allowed [clang-diagnostic-error]
    set(input, static_cast<int>(value));
               ^
  Report hash: 7f2d83b7bc6037eb020b24a32a103366
  Steps:
    1, node.h:114:16: static_cast from 'ccl::Shader *' to 'int' is not allowed

Found 4 defect(s) in node.h

[CRITICAL] /sep/extern/cycles/src/util/math_base.h:158:45: template argument for non-type template parameter must be an expression [clang-diagnostic-error]
ccl_device_inline typename std::enable_if_t<std::is_same_v<T, size_t>, T> max(T a, T b)
                                            ^
  Report hash: 1286e8b7cd325b649f5361393b6346e3
  Steps:
    1, type_traits:2836:17: template parameter is declared here
    2, math_base.h:158:45: template argument for non-type template parameter must be an expression

[CRITICAL] /sep/extern/cycles/src/util/math_base.h:158:50: no template named 'is_same_v' in namespace 'std'; did you mean 'is_same'? [clang-diagnostic-error]
ccl_device_inline typename std::enable_if_t<std::is_same_v<T, size_t>, T> max(T a, T b)
                                                 ^
  Report hash: c19579e6d98d540f2f8281f7e4f9b07e
  Notes:
    1, math_base.h:158:50: is_same (fixit)
  Steps:
    1, type_traits:1533:12: 'is_same' declared here
    2, math_base.h:158:50: no template named 'is_same_v' in namespace 'std'; did you mean 'is_same'?

[CRITICAL] /sep/extern/cycles/src/util/math_base.h:164:45: template argument for non-type template parameter must be an expression [clang-diagnostic-error]
ccl_device_inline typename std::enable_if_t<std::is_same_v<T, size_t>, T> min(T a, T b)
                                            ^
  Report hash: d1fae1a2fedfcc63efb86d3d33ad3548
  Steps:
    1, type_traits:2836:17: template parameter is declared here
    2, math_base.h:164:45: template argument for non-type template parameter must be an expression

[CRITICAL] /sep/extern/cycles/src/util/math_base.h:164:50: no template named 'is_same_v' in namespace 'std'; did you mean 'is_same'? [clang-diagnostic-error]
ccl_device_inline typename std::enable_if_t<std::is_same_v<T, size_t>, T> min(T a, T b)
                                                 ^
  Report hash: 4e800270e0231be8e83d9a8044b98100
  Notes:
    1, math_base.h:164:50: is_same (fixit)
  Steps:
    1, type_traits:1533:12: 'is_same' declared here
    2, math_base.h:164:50: no template named 'is_same_v' in namespace 'std'; did you mean 'is_same'?

Found 4 defect(s) in math_base.h

[MEDIUM] /sep/include/compat/cycles.h:72:34: unused parameter 'row' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fff28953586f6f45c8151653cb8615ab
  Steps:
    1, cycles.h:72:34: unused parameter 'row'

[MEDIUM] /sep/include/compat/cycles.h:72:43: unused parameter 'col' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 38220157262a42481c215ecb43bf31f3
  Steps:
    1, cycles.h:72:43: unused parameter 'col'

[MEDIUM] /sep/include/compat/cycles.h:73:34: unused parameter 'i' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9a788d8abf35ab6cf7290cd9f8d778b3
  Steps:
    1, cycles.h:73:34: unused parameter 'i'

[MEDIUM] /sep/include/compat/cycles.h:74:35: unused parameter 'i' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9f5bf18f51d222e486fe2c7261c8d98d
  Steps:
    1, cycles.h:74:35: unused parameter 'i'

[MEDIUM] /sep/include/compat/cycles.h:80:36: unused parameter 'min' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3491c10f7ed201f307451e8ee2806396
  Steps:
    1, cycles.h:80:36: unused parameter 'min'

[MEDIUM] /sep/include/compat/cycles.h:80:54: unused parameter 'max' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 09194db40cec448620d014aeae2dbad2
  Steps:
    1, cycles.h:80:54: unused parameter 'max'

[MEDIUM] /sep/include/compat/cycles.h:81:51: unused parameter 'other' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a926832fcc449c8a7c6123cd96f5f78f
  Steps:
    1, cycles.h:81:51: unused parameter 'other'

Found 7 defect(s) in cycles.h

[CRITICAL] /sep/include/core/system_hooks.h:13:43: no type named 'string_view' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 50b67af3573782b2443ac2f5449313ca
  Steps:
    1, system_hooks.h:13:43: no type named 'string_view' in namespace 'std'

[CRITICAL] /sep/include/core/system_hooks.h:14:40: no type named 'string_view' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 04783e7ed9d1231a5b18fafb707211a3
  Steps:
    1, system_hooks.h:14:40: no type named 'string_view' in namespace 'std'

[CRITICAL] /sep/include/core/system_hooks.h:15:40: no type named 'string_view' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7b6b3e8468473fecd448820af7f0a482
  Steps:
    1, system_hooks.h:15:40: no type named 'string_view' in namespace 'std'

[CRITICAL] /sep/include/core/system_hooks.h:16:39: no type named 'string_view' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4c19106e254df73084482cfbaa282551
  Steps:
    1, system_hooks.h:16:39: no type named 'string_view' in namespace 'std'

Found 4 defect(s) in system_hooks.h

[MEDIUM] /sep/src/main.cpp:49:22: functions without C linkage are not allowed as signal handler (until C++17) [bugprone-signal-handler]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1af451b9d2543658c3b1c9aea86e6a6f
  Steps:
    1, main.cpp:49:22: functions without C linkage are not allowed as signal handler (until C++17)

[MEDIUM] /sep/src/main.cpp:53:23: functions without C linkage are not allowed as signal handler (until C++17) [bugprone-signal-handler]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1357dd0356b09644fe183d0e2d40117e
  Steps:
    1, main.cpp:53:23: functions without C linkage are not allowed as signal handler (until C++17)

Found 2 defect(s) in main.cpp

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:282:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:282:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:329:26: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 005a58abc587efa85ab38a672b887462
  Steps:
    1, memory_tier_manager.cpp:329:26: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:413:23: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b88d2d0afa7e06912c1d00d5827b227a
  Steps:
    1, memory_tier_manager.cpp:413:23: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in memory_tier_manager.cpp

[CRITICAL] /sep/src/memory/redis_manager.cpp:99:6: no template named 'optional' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 801b78a296242127b8811696012712d7
  Steps:
    1, redis_manager.cpp:99:6: no template named 'optional' in namespace 'std'

[CRITICAL] /sep/src/memory/redis_manager.cpp:103:21: no member named 'nullopt' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 477cb81c5cd7d5a485bd4815facc0208
  Steps:
    1, redis_manager.cpp:103:21: no member named 'nullopt' in namespace 'std'

[CRITICAL] /sep/src/memory/redis_manager.cpp:114:21: no member named 'nullopt' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 477cb81c5cd7d5a485bd4815facc0208
  Steps:
    1, redis_manager.cpp:114:21: no member named 'nullopt' in namespace 'std'

[CRITICAL] /sep/src/memory/redis_manager.cpp:126:12: no viable conversion from returned value of type 'PersistentPatternData' to function return type 'int' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: af9d34572b89e21656d5a504e97aca05
  Steps:
    1, redis_manager.cpp:126:12: no viable conversion from returned value of type 'PersistentPatternData' to function return type 'int'

[CRITICAL] /sep/src/memory/redis_manager.cpp:213:6: no template named 'optional' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 216b3f30f565b154787bcc2189bfef6c
  Steps:
    1, redis_manager.cpp:213:6: no template named 'optional' in namespace 'std'

Found 5 defect(s) in redis_manager.cpp

[CRITICAL] /sep/include/quantum/qfh.h:46:10: no template named 'optional' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 498af51d222fa2492750dd2fba9498a4
  Steps:
    1, qfh.h:46:10: no template named 'optional' in namespace 'std'

[CRITICAL] /sep/include/quantum/qfh.h:50:10: no template named 'optional' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: beabc8f6c1d269e26e95e5c09a7efb8f
  Steps:
    1, qfh.h:50:10: no template named 'optional' in namespace 'std'

Found 2 defect(s) in qfh.h

[CRITICAL] /sep/src/quantum/quantum_processor_qfh_common.cpp:23:23: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'? [clang-diagnostic-error]
    float cos_theta = std::clamp(dot / mags, -1.0f, 1.0f);
                      ^
  Report hash: 8184471f4c737d15185344d93e150bd7
  Notes:
    1, quantum_processor_qfh_common.cpp:23:23: glm::clamp (fixit)
  Steps:
    1, func_common.inl:505:43: 'glm::clamp' declared here
    2, quantum_processor_qfh_common.cpp:23:23: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'?

[CRITICAL] /sep/src/quantum/quantum_processor_qfh_common.cpp:29:18: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'? [clang-diagnostic-error]
    float freq = std::clamp(interaction_frequency, 0.0f, 1.0f);
                 ^
  Report hash: ee5aaaa7dcc1587da75e135396654697
  Notes:
    1, quantum_processor_qfh_common.cpp:29:18: glm::clamp (fixit)
  Steps:
    1, func_common.inl:505:43: 'glm::clamp' declared here
    2, quantum_processor_qfh_common.cpp:29:18: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'?

[CRITICAL] /sep/src/quantum/quantum_processor_qfh_common.cpp:30:12: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'? [clang-diagnostic-error]
    return std::clamp(coherence_similarity * freq, 0.0f, 1.0f);
           ^
  Report hash: 5138c89afa13090331b1cffbc66dedad
  Notes:
    1, quantum_processor_qfh_common.cpp:30:12: glm::clamp (fixit)
  Steps:
    1, func_common.inl:505:43: 'glm::clamp' declared here
    2, quantum_processor_qfh_common.cpp:30:12: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'?

[CRITICAL] /sep/src/quantum/quantum_processor_qfh_common.cpp:35:18: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'? [clang-diagnostic-error]
    float freq = std::clamp(access_frequency, 0.0f, 1.0f);
                 ^
  Report hash: 1581738c75c811256a5af492e91e377c
  Notes:
    1, quantum_processor_qfh_common.cpp:35:18: glm::clamp (fixit)
  Steps:
    1, func_common.inl:505:43: 'glm::clamp' declared here
    2, quantum_processor_qfh_common.cpp:35:18: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'?

[CRITICAL] /sep/src/quantum/quantum_processor_qfh_common.cpp:36:17: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'? [clang-diagnostic-error]
    float gen = std::clamp(generation_count / 100.0f, 0.0f, 1.0f);
                ^
  Report hash: 349a0d462d108f2c478cd8b29a831cb5
  Notes:
    1, quantum_processor_qfh_common.cpp:36:17: glm::clamp (fixit)
  Steps:
    1, func_common.inl:505:43: 'glm::clamp' declared here
    2, quantum_processor_qfh_common.cpp:36:17: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'?

[CRITICAL] /sep/src/quantum/quantum_processor_qfh_common.cpp:37:12: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'? [clang-diagnostic-error]
    return std::clamp(base * freq + gen, 0.0f, 1.0f);
           ^
  Report hash: c3f8060a5d79c0a0465113d91c30ba33
  Notes:
    1, quantum_processor_qfh_common.cpp:37:12: glm::clamp (fixit)
  Steps:
    1, func_common.inl:505:43: 'glm::clamp' declared here
    2, quantum_processor_qfh_common.cpp:37:12: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'?

[CRITICAL] /sep/src/quantum/quantum_processor_qfh_common.cpp:57:12: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'? [clang-diagnostic-error]
    return std::clamp(rate, 0.0f, 1.0f);
           ^
  Report hash: f6337e03a8936beed425cc075b08f728
  Notes:
    1, quantum_processor_qfh_common.cpp:57:12: glm::clamp (fixit)
  Steps:
    1, func_common.inl:505:43: 'glm::clamp' declared here
    2, quantum_processor_qfh_common.cpp:57:12: no member named 'clamp' in namespace 'std'; did you mean 'glm::clamp'?

Found 7 defect(s) in quantum_processor_qfh_common.cpp

[MEDIUM] /sep/src/quantum/processor.cpp:229:14: lambda capture 'this' is not used [clang-diagnostic-unused-lambda-capture]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a8cf365dcc1115f5118987a48af80ea4
  Steps:
    1, processor.cpp:229:14: lambda capture 'this' is not used

Found 1 defect(s) in processor.cpp

[MEDIUM] /sep/include/api/crow_adapter.h:15:5: class 'request' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1cb338fa7be79e9fa5863ab96c341985
  Steps:
    1, http_request.h:30:12: previous use is here
    2, crow_adapter.h:15:5: did you mean struct here?
    3, crow_adapter.h:15:5: class 'request' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/crow_adapter.h:16:5: class 'response' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f276dcea83ad465537894f487b61c63a
  Steps:
    1, http_response.h:33:12: previous use is here
    2, crow_adapter.h:16:5: did you mean struct here?
    3, crow_adapter.h:16:5: class 'response' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in crow_adapter.h

[MEDIUM] /sep/include/api/server.h:25:1: class 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 43e01467f44228aa9369964f4818966f
  Steps:
    1, http_request.h:30:12: previous use is here
    2, server.h:25:1: did you mean struct here?
    3, server.h:25:1: class 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/server.h:26:1: class 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eb6ec7bb6fa28f8f77c9a2afb301fc43
  Steps:
    1, http_response.h:33:12: previous use is here
    2, server.h:26:1: did you mean struct here?
    3, server.h:26:1: class 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in server.h

[CRITICAL] /sep/include/crow/asio_isolation.h:13:11: redefinition of 'asio' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fcbfbae73c870fb54ab77338ea345b92
  Steps:
    1, stream.hpp:39:11: previous definition is here
    2, asio_isolation.h:13:11: redefinition of 'asio'

Found 1 defect(s) in asio_isolation.h

[CRITICAL] /sep/src/api/server.cpp:782:26: no member named 'clamp' in namespace 'std' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1f977cf2eae44aec8c59a525e073723f
  Steps:
    1, server.cpp:782:26: no member named 'clamp' in namespace 'std'

[MEDIUM] /sep/src/api/server.cpp:788:29: functions without C linkage are not allowed as signal handler (until C++17) [bugprone-signal-handler]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 89d7f380a2c649eb6bd893689f51003b
  Steps:
    1, server.cpp:788:29: functions without C linkage are not allowed as signal handler (until C++17)

[MEDIUM] /sep/src/api/server.cpp:789:30: functions without C linkage are not allowed as signal handler (until C++17) [bugprone-signal-handler]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fa0863d64668b6b840a3d8db6d477f59
  Steps:
    1, server.cpp:789:30: functions without C linkage are not allowed as signal handler (until C++17)

Found 3 defect(s) in server.cpp

[MEDIUM] /sep/include/compat/memory.h:112:63: unused parameter 'stream' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 03fe062d6c7e2fcf37a2b293d4c6d7a1
  Steps:
    1, memory.h:112:63: unused parameter 'stream'

[MEDIUM] /sep/include/compat/memory.h:124:61: unused parameter 'stream' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d7af2a21b214b2470caa64843624d8f9
  Steps:
    1, memory.h:124:61: unused parameter 'stream'

Found 2 defect(s) in memory.h

[MEDIUM] /sep/src/core/engine.cpp:90:3: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6e12c4fd64fc76995ebc3b185ba355a3
  Steps:
    1, engine.cpp:90:3: cast the expression to void to silence this warning
    2, engine.cpp:90:3: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 1 defect(s) in engine.cpp

[MEDIUM] /sep/include/compat/kernels.cuh:47:6: unused function 'process_pattern_kernel' [clang-diagnostic-unused-function]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 55269f362da979aebf488ceb4a65c5d9
  Steps:
    1, kernels.cuh:47:6: unused function 'process_pattern_kernel'

[MEDIUM] /sep/include/compat/kernels.cuh:48:27: unused parameter 'pattern' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 31d5f2cb4a0073e44a643481c4cc0d9c
  Steps:
    1, kernels.cuh:48:27: unused parameter 'pattern'

[MEDIUM] /sep/include/compat/kernels.cuh:49:27: unused parameter 'result' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5e3e639c90699be9f1e68511d19a7617
  Steps:
    1, kernels.cuh:49:27: unused parameter 'result'

[MEDIUM] /sep/include/compat/kernels.cuh:50:35: unused parameter 'config' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eb015f5d301c785fe9ee54e041abb80c
  Steps:
    1, kernels.cuh:50:35: unused parameter 'config'

[MEDIUM] /sep/include/compat/kernels.cuh:51:12: unused parameter 'pattern_count' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bbdf69baf0632c6003f677ee2c7c0b3f
  Steps:
    1, kernels.cuh:51:12: unused parameter 'pattern_count'

[MEDIUM] /sep/include/compat/kernels.cuh:52:33: unused parameter 'previous_patterns' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e90d8ad10723fe736ad64b04db58160a
  Steps:
    1, kernels.cuh:52:33: unused parameter 'previous_patterns'

Found 6 defect(s) in kernels.cuh

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:284:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:284:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:331:26: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 005a58abc587efa85ab38a672b887462
  Steps:
    1, memory_tier_manager.cpp:331:26: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:415:23: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b88d2d0afa7e06912c1d00d5827b227a
  Steps:
    1, memory_tier_manager.cpp:415:23: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in memory_tier_manager.cpp

[MEDIUM] /sep/src/quantum/evolution.cpp:324:42: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cad84b4fd50cd0e5a22044d2f4918545
  Steps:
    1, evolution.cpp:324:42: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/quantum/evolution.cpp:376:45: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 686c468a0c599aae810685813b0a5284
  Steps:
    1, evolution.cpp:376:45: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in evolution.cpp

[MEDIUM] /sep/extern/crow/include/crow/json.h:135:17: move constructors should be marked noexcept [performance-noexcept-move-constructor]
                r_string(r_string&& r)
                ^
  Report hash: 457d5845e230f31002e7001be4703d8a
  Notes:
    1, json.h:135:17: { (fixit)
    2, json.h:135:2: noexcept  (fixit)
  Steps:
    1, json.h:135:17: move constructors should be marked noexcept

[MEDIUM] /sep/extern/crow/include/crow/json.h:140:27: move assignment operators should be marked noexcept [performance-noexcept-move-constructor]
                r_string& operator=(r_string&& r)
                          ^
  Report hash: 7b10e105c2ba133cc9b629a877e82993
  Notes:
    1, json.h:140:17: { (fixit)
    2, json.h:140:2: noexcept  (fixit)
  Steps:
    1, json.h:140:27: move assignment operators should be marked noexcept

[LOW] /sep/extern/crow/include/crow/json.h:488:29: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
                            switch (*++head)
                            ^
  Report hash: b44d6f6b5e24bda1f7dfe15bf049acc4
  Steps:
    1, json.h:488:29: switching on non-enum value without default case may not cover all cases

[MEDIUM] /sep/extern/crow/include/crow/json.h:1474:13: move constructors should be marked noexcept [performance-noexcept-move-constructor]
            wvalue(wvalue&& r):
            ^
  Report hash: a0703dd62794abfe2d148aa44fcf9776
  Notes:
    1, json.h:1474:32: noexcept  (fixit)
  Steps:
    1, json.h:1474:13: move constructors should be marked noexcept

[MEDIUM] /sep/extern/crow/include/crow/json.h:1480:21: move assignment operators should be marked noexcept [performance-noexcept-move-constructor]
            wvalue& operator=(wvalue&& r)
                    ^
  Report hash: 44add3bcaad1805e1a0d907f8939edce
  Notes:
    1, json.h:1480:13: { (fixit)
    2, json.h:1480:2: noexcept  (fixit)
  Steps:
    1, json.h:1480:21: move assignment operators should be marked noexcept

[MEDIUM] /sep/extern/crow/include/crow/json.h:1858:29: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
                            snprintf(outbuf, sizeof(outbuf), "%f", v.num.d);
                            ^
  Report hash: 7e5ef8a91462b9206f5cbffa5704a88e
  Steps:
    1, json.h:1858:29: cast the expression to void to silence this warning
    2, json.h:1858:29: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/extern/crow/include/crow/json.h:1873:53: expression is redundant [misc-redundant-expression]
                                            if (fch != '\0' && fch == '0') p++;
                                                    ^
  Report hash: 070b51d785bd4b2d7d772ca9d21d025f
  Steps:
    1, json.h:1873:53: expression is redundant

Found 7 defect(s) in json.h

[MEDIUM] /sep/src/blender/blender_integration.cpp:27:7: field 'thread_running_' will be initialized after field 'gpu_context_' [clang-diagnostic-reorder-ctor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dfc6c665bd56d3f5e87cab3824f381f6
  Notes:
    1, blender_integration.cpp:27:7: gpu_context_(nullptr) (fixit)
    2, blender_integration.cpp:27:7: gpu_context_(nullptr) (fixit)
    3, blender_integration.cpp:27:7: ~~~~~~~~~~~~~~~~~~~~~ (fixit)
    4, blender_integration.cpp:27:7: thread_running_(false) (fixit)
  Steps:
    1, blender_integration.cpp:27:7: field 'thread_running_' will be initialized after field 'gpu_context_'

Found 1 defect(s) in blender_integration.cpp

[MEDIUM] /sep/src/blender/mesh_handler.cpp:304:21: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 96149c0ed1849daeb9b92afc0dcb3cc9
  Steps:
    1, mesh_handler.cpp:304:21: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/blender/mesh_handler.cpp:323:14: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 31bfb7cbaa7554f38c92dbc1014c813e
  Steps:
    1, mesh_handler.cpp:323:14: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in mesh_handler.cpp

[MEDIUM] /sep/include/api/server.h:222:38: private field 'cycles_renderer_' is not used [clang-diagnostic-unused-private-field]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3a19fccf9675b2d499ff0269f54d5087
  Steps:
    1, server.h:222:38: private field 'cycles_renderer_' is not used

Found 1 defect(s) in server.h

[CRITICAL] /sep/extern/cycles/src/util/simd.h:59:1: unknown type name 'CCL_NAMESPACE_BEGIN' [clang-diagnostic-error]
CCL_NAMESPACE_BEGIN
^
  Report hash: 379d2bdd1d99d1afa065872dcc6ca2f1
  Steps:
    1, simd.h:59:1: unknown type name 'CCL_NAMESPACE_BEGIN'

[CRITICAL] /sep/extern/cycles/src/util/simd.h:397:23: expected ';' after top level declarator [clang-diagnostic-error]
__forceinline uint32_t __bsf(const uint32_t x)
                      ^
  Report hash: ba081144ad5b73318216f3cb3f21a0fb
  Steps:
    1, simd.h:397:23: expected ';' after top level declarator

[CRITICAL] /sep/extern/cycles/src/util/simd.h:484:1: unknown type name 'CCL_NAMESPACE_END' [clang-diagnostic-error]
CCL_NAMESPACE_END
^
  Report hash: 1bee200428f197027166010eaa7021ca
  Steps:
    1, simd.h:484:1: unknown type name 'CCL_NAMESPACE_END'

Found 3 defect(s) in simd.h

[CRITICAL] /sep/extern/cycles/src/util/types_base.h:25:20: expected ';' after top level declarator [clang-diagnostic-error]
CCL_NAMESPACE_BEGIN
                   ^
  Report hash: 6b629fdcb6e7c2643b6746fc06ebf3da
  Steps:
    1, types_base.h:25:20: expected ';' after top level declarator

[CRITICAL] /sep/extern/cycles/src/util/types_base.h:72:1: unknown type name 'CCL_NAMESPACE_END' [clang-diagnostic-error]
CCL_NAMESPACE_END
^
  Report hash: ab8c3eb44e98f3ae5832095351810273
  Steps:
    1, types_base.h:72:1: unknown type name 'CCL_NAMESPACE_END'

Found 2 defect(s) in types_base.h

[CRITICAL] /sep/extern/cycles/src/util/types_uchar2.h:9:1: unknown type name 'CCL_NAMESPACE_BEGIN' [clang-diagnostic-error]
CCL_NAMESPACE_BEGIN
^
  Report hash: 38cf2661476921cb42d99ca97f0b4530
  Steps:
    1, types_uchar2.h:9:1: unknown type name 'CCL_NAMESPACE_BEGIN'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar2.h:13:3: unknown type name 'uchar' [clang-diagnostic-error]
  uchar x, y;
  ^
  Report hash: ee7463005743f4cbf2f6dae19f481e7b
  Steps:
    1, types_uchar2.h:13:3: unknown type name 'uchar'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar2.h:16:17: unknown type name 'uchar' [clang-diagnostic-error]
  __forceinline uchar operator[](int i) const
                ^
  Report hash: b48542448b51342db6c875f439dedcd5
  Steps:
    1, types_uchar2.h:16:17: unknown type name 'uchar'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar2.h:23:17: unknown type name 'uchar' [clang-diagnostic-error]
  __forceinline uchar &operator[](int i)
                ^
  Report hash: 871aca4c013a0e53338a20460f30791e
  Steps:
    1, types_uchar2.h:23:17: unknown type name 'uchar'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar2.h:32:44: unknown type name 'uchar' [clang-diagnostic-error]
ccl_device_inline uchar2 make_uchar2(const uchar x, const uchar y)
                                           ^
  Report hash: 53ad9c262c6d6034e80946eebc4aa02c
  Steps:
    1, types_uchar2.h:32:44: unknown type name 'uchar'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar2.h:32:59: unknown type name 'uchar' [clang-diagnostic-error]
ccl_device_inline uchar2 make_uchar2(const uchar x, const uchar y)
                                                          ^
  Report hash: e1c74384922b361a6ce82aef08d1f1fc
  Steps:
    1, types_uchar2.h:32:59: unknown type name 'uchar'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar2.h:39:1: unknown type name 'CCL_NAMESPACE_END' [clang-diagnostic-error]
CCL_NAMESPACE_END
^
  Report hash: cbd7cda84ce4cdcd323114cd20d69b7f
  Steps:
    1, types_uchar2.h:39:1: unknown type name 'CCL_NAMESPACE_END'

Found 7 defect(s) in types_uchar2.h

[CRITICAL] /sep/extern/cycles/src/util/types_uchar3.h:9:20: expected ';' after top level declarator [clang-diagnostic-error]
CCL_NAMESPACE_BEGIN
                   ^
  Report hash: c47fae072fc060244c7c38b4fb8f92de
  Steps:
    1, types_uchar3.h:9:20: expected ';' after top level declarator

[CRITICAL] /sep/extern/cycles/src/util/types_uchar3.h:13:3: unknown type name 'uchar' [clang-diagnostic-error]
  uchar x, y, z;
  ^
  Report hash: 9faeb56dd68ae60b0500d7e59c6dbc62
  Steps:
    1, types_uchar3.h:13:3: unknown type name 'uchar'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar3.h:16:17: unknown type name 'uchar' [clang-diagnostic-error]
  __forceinline uchar operator[](int i) const
                ^
  Report hash: e58e7222eb571a276c8f776c4fe6aeb1
  Steps:
    1, types_uchar3.h:16:17: unknown type name 'uchar'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar3.h:23:17: unknown type name 'uchar' [clang-diagnostic-error]
  __forceinline uchar &operator[](int i)
                ^
  Report hash: 177f9491222f3af406076313de1d9104
  Steps:
    1, types_uchar3.h:23:17: unknown type name 'uchar'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar3.h:32:44: unknown type name 'uchar' [clang-diagnostic-error]
ccl_device_inline uchar3 make_uchar3(const uchar x, const uchar y, uchar z)
                                           ^
  Report hash: 957cb471a93705a27df5668b02dd1ab2
  Steps:
    1, types_uchar3.h:32:44: unknown type name 'uchar'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar3.h:32:59: unknown type name 'uchar' [clang-diagnostic-error]
ccl_device_inline uchar3 make_uchar3(const uchar x, const uchar y, uchar z)
                                                          ^
  Report hash: b08e03cfd72c91b009c9c840282667c4
  Steps:
    1, types_uchar3.h:32:59: unknown type name 'uchar'

[CRITICAL] /sep/extern/cycles/src/util/types_uchar3.h:32:68: unknown type name 'uchar' [clang-diagnostic-error]
ccl_device_inline uchar3 make_uchar3(const uchar x, const uchar y, uchar z)
                                                                   ^
  Report hash: cd3322bf44386055efa0243514661304
  Steps:
    1, types_uchar3.h:32:68: unknown type name 'uchar'

Found 7 defect(s) in types_uchar3.h

[LOW] /usr/include/asio/detail/impl/signal_set_service.ipp:145:12: Call to blocking function 'read' inside of critical section [unix.BlockInCriticalSection]
    while (::read(fd, &signal_number, sizeof(int)) == sizeof(int))
           ^
  Report hash: 4d4048db5a8e07237305e792933e397a
  Steps:
     1, signal_set_service.ipp:145:12: Assuming the condition is true
     2, signal_set_service.ipp:145:12: Entering loop body
     3, signal_set_service.ipp:146:11: Assuming 'signal_number' is >= 0
     4, signal_set_service.ipp:146:33: Assuming 'signal_number' is < max_signal_number
     5, signal_set_service.ipp:147:9: Calling 'signal_set_service::deliver_signal'
     6, signal_set_service.ipp:610:1: Entered call from 'pipe_read_op::do_perform'
     7, signal_set_service.ipp:616:10: Entering loop body
     8, signal_set_service.ipp:621:12: Entering loop body
     9, signal_set_service.ipp:629:27: Entering loop body
    10, signal_set_service.ipp:629:9: Looping back to the head of the loop
    11, signal_set_service.ipp:629:27: Loop body executed 0 times
    12, signal_set_service.ipp:621:5: Looping back to the head of the loop
    13, signal_set_service.ipp:640:5: Calling 'scheduler::post_deferred_completions'
    14, scheduler.ipp:410:1: Entered call from 'signal_set_service::deliver_signal'
    15, scheduler.ipp:416:9: Assuming field 'one_thread_' is false
    16, scheduler.ipp:426:24: Calling constructor for 'scoped_lock'
    17, conditionally_enabled_mutex.hpp:49:5: Entered call from 'scheduler::post_deferred_completions'
    18, conditionally_enabled_mutex.hpp:52:11: Assuming field 'enabled_' is true
    19, conditionally_enabled_mutex.hpp:54:9: Calling 'posix_mutex::lock'
    20, posix_mutex.hpp:49:3: Entered call from constructor for 'scoped_lock'
    21, posix_mutex.hpp:51:11: Entering critical section here
    22, conditionally_enabled_mutex.hpp:54:9: Returning from 'posix_mutex::lock'
    23, scheduler.ipp:426:24: Returning from constructor for 'scoped_lock'
    24, signal_set_service.ipp:640:5: Returning from 'scheduler::post_deferred_completions'
    25, signal_set_service.ipp:616:3: Looping back to the head of the loop
    26, signal_set_service.ipp:147:9: Returning from 'signal_set_service::deliver_signal'
    27, signal_set_service.ipp:145:5: Looping back to the head of the loop
    28, signal_set_service.ipp:145:12: Call to blocking function 'read' inside of critical section

Found 1 defect(s) in signal_set_service.ipp

[MEDIUM] /sep/src/api/client.cpp:145:46: unused parameter 'request' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 91d07ab03d93db50c8196e95accb3896
  Steps:
    1, client.cpp:145:46: unused parameter 'request'

Found 1 defect(s) in client.cpp

[CRITICAL] /sep/src/blender/oiio_output_driver.cpp:28:1: use of undeclared identifier 'OIIOOutputDriver' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7c6b499db5e372075a1ff7236333fad0
  Steps:
    1, oiio_output_driver.cpp:28:1: use of undeclared identifier 'OIIOOutputDriver'

[CRITICAL] /sep/src/blender/oiio_output_driver.cpp:28:42: unknown type name 'string_view' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fb711e602ac48d455b908d13deaeb16f
  Steps:
    1, oiio_output_driver.cpp:28:42: unknown type name 'string_view'

[CRITICAL] /sep/src/blender/oiio_output_driver.cpp:29:42: unknown type name 'string_view' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ca1a71f931f7a98cf4be02202b4727d9
  Steps:
    1, oiio_output_driver.cpp:29:42: unknown type name 'string_view'

[CRITICAL] /sep/src/blender/oiio_output_driver.cpp:30:36: unknown type name 'LogFunction' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4d57351c8f1651f64815e94f0fd1dce8
  Steps:
    1, oiio_output_driver.cpp:30:36: unknown type name 'LogFunction'

[CRITICAL] /sep/src/blender/oiio_output_driver.cpp:33:1: use of undeclared identifier 'OIIOOutputDriver' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 89489aa8f89951d61c43ca01737b5e21
  Steps:
    1, oiio_output_driver.cpp:33:1: use of undeclared identifier 'OIIOOutputDriver'

[CRITICAL] /sep/src/blender/oiio_output_driver.cpp:35:6: use of undeclared identifier 'OIIOOutputDriver' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5f55a3655f7fd05b839e446a128bfafa
  Steps:
    1, oiio_output_driver.cpp:35:6: use of undeclared identifier 'OIIOOutputDriver'

[CRITICAL] /sep/src/blender/oiio_output_driver.cpp:35:48: unknown type name 'Tile' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 468b5fbe6eaac966e3269fd43047ff4d
  Steps:
    1, oiio_output_driver.cpp:35:48: unknown type name 'Tile'

Found 7 defect(s) in oiio_output_driver.cpp

[MEDIUM] /sep/extern/crow/include/crow/http_request.h:49:9: 5 uninitialized fields at the end of the constructor call [optin.cplusplus.UninitializedObject]
        request():
        ^
  Report hash: d86a921d55e9e714b05c4811c3e97dc5
  Notes:
    1, http_request.h:39:39: uninitialized field 'this->http_ver_minor'
    2, http_request.h:40:14: uninitialized field 'this->keep_alive'
    3, http_request.h:41:11: uninitialized field 'this->close_connection'
    4, http_request.h:42:11: uninitialized field 'this->upgrade'
    5, http_request.h:39:23: uninitialized field 'this->http_ver_major'
  Steps:
    1, http_connection.h:516:23: Assuming field 'continue_requested' is false
    2, http_connection.h:518:23: Calling 'HTTPParser::clear'
    3, parser.h:139:9: Entered call from 'operator()'
    4, parser.h:141:19: Calling default constructor for 'request'
    5, http_request.h:49:9: Entered call from 'HTTPParser::clear'
    6, http_request.h:49:9: 5 uninitialized fields at the end of the constructor call

Found 1 defect(s) in http_request.h

[HIGH] /sep/extern/crow/include/crow/routing.h:863:63: Moved-from object 'MT' of type 'std::vector' is copied [cplusplus.Move]
                return routing_handle_result{node.rule_index, *blueprints, *params};
                                                              ^
  Report hash: 7b70a7f507f67c3804cc819c07887e3b
  Steps:
     1, parser.h:99:13: Calling 'HTTPParser::process_message'
     2, parser.h:160:9: Entered call from 'HTTPParser::on_message_complete'
     3, parser.h:162:13: Calling 'Connection::handle'
     4, http_connection.h:122:9: Entered call from 'HTTPParser::process_message'
     5, http_connection.h:136:21: Assuming the condition is false
     6, http_connection.h:141:26: Assuming field 'upgrade' is true
     7, http_connection.h:152:25: Calling 'Crow::handle_upgrade'
     8, app.h:75:9: Entered call from 'Connection::handle'
     9, app.h:77:13: Calling 'Router::handle_upgrade'
    10, routing.h:1383:9: Entered call from 'Crow::handle_upgrade'
    11, routing.h:1385:17: Assuming field 'method' is < InternalMethodCount
    12, routing.h:1390:35: Calling 'Trie::find'
    13, routing.h:993:9: Entered call from 'Router::handle_upgrade'
    14, routing.h:995:20: Calling 'Trie::find'
    15, routing.h:835:9: Entered call from 'Trie::find'
    16, routing.h:860:17: Assuming the condition is true
    17, routing.h:862:17: Object 'MT' of type 'std::vector' is left in a valid but unspecified state after move
    18, routing.h:863:63: Moved-from object 'MT' of type 'std::vector' is copied

[HIGH] /sep/extern/crow/include/crow/routing.h:982:51: Method called on moved-from object 'MT' of type 'std::vector' [cplusplus.Move]
                        if (!blueprints->empty()) blueprints->pop_back();
                                                  ^
  Report hash: 7b70a7f507f67c3804cc819c07887e3b
  Steps:
     1, parser.h:99:13: Calling 'HTTPParser::process_message'
     2, parser.h:160:9: Entered call from 'HTTPParser::on_message_complete'
     3, parser.h:162:13: Calling 'Connection::handle'
     4, http_connection.h:122:9: Entered call from 'HTTPParser::process_message'
     5, http_connection.h:136:21: Assuming the condition is false
     6, http_connection.h:141:26: Assuming field 'upgrade' is true
     7, http_connection.h:152:25: Calling 'Crow::handle_upgrade'
     8, app.h:75:9: Entered call from 'Connection::handle'
     9, app.h:77:13: Calling 'Router::handle_upgrade'
    10, routing.h:1383:9: Entered call from 'Crow::handle_upgrade'
    11, routing.h:1385:17: Assuming field 'method' is < InternalMethodCount
    12, routing.h:1390:35: Calling 'Trie::find'
    13, routing.h:993:9: Entered call from 'Router::handle_upgrade'
    14, routing.h:995:20: Calling 'Trie::find'
    15, routing.h:835:9: Entered call from 'Trie::find'
    16, routing.h:860:17: Assuming the condition is false
    17, routing.h:868:36: Entering loop body
    18, routing.h:870:21: Assuming field 'param' is equal to MAX
    19, routing.h:976:25: Assuming the condition is true
    20, routing.h:979:29: Assuming 'INVALID_BP_ID' is equal to field 'blueprint_index'
    21, routing.h:980:36: Calling 'Trie::find'
    22, routing.h:835:9: Entered call from 'Trie::find'
    23, routing.h:860:17: Assuming the condition is false
    24, routing.h:868:36: Loop body skipped when range is empty
    25, routing.h:988:17: Object 'MT' of type 'std::vector' is left in a valid but unspecified state after move
    26, routing.h:980:36: Returning from 'Trie::find'
    27, routing.h:982:29: Assuming the condition is true
    28, routing.h:982:51: Method called on moved-from object 'MT' of type 'std::vector'

Found 2 defect(s) in routing.h

[MEDIUM] /sep/include/api/server.h:23:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7efebc8bbaef775aeab2e28a25dc4a5c
  Steps:
    1, crow_isolation.h:47:11: previous use is here
    2, server.h:23:1: did you mean class here?
    3, server.h:23:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/server.h:24:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 59bfeb4242018ecdefde0c5a8f41634e
  Steps:
    1, crow_isolation.h:62:11: previous use is here
    2, server.h:24:1: did you mean class here?
    3, server.h:24:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in server.h

[CRITICAL] /sep/include/memory/manager.h:78:3: no type named 'level_enum' in 'spdlog::level::level_enum'; did you mean '::spdlog::level::level_enum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6a24ea08ee9012b106f7c286eb3df7ef
  Notes:
    1, manager.h:78:3: ::spdlog::level::level_enum (fixit)
  Steps:
    1, common.h:245:6: '::spdlog::level::level_enum' declared here
    2, manager.h:78:3: no type named 'level_enum' in 'spdlog::level::level_enum'; did you mean '::spdlog::level::level_enum'?

Found 1 defect(s) in manager.h

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:60:7: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e8753866e5db361edd170b4ad09eb127
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, quantum_manifold_optimizer.cpp:60:7: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:518:1: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 33988d65f9c61fdcb13397e01e02748b
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, quantum_manifold_optimizer.cpp:518:1: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:518:58: unknown type name 'Config'; did you mean 'PatternEvolutionBridge::Config'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cef0b46cc39833a6d103ef837804d531
  Notes:
    1, quantum_manifold_optimizer.cpp:518:58: PatternEvolutionBridge::Config (fixit)
  Steps:
    1, pattern_evolution_bridge.h:56:12: 'PatternEvolutionBridge::Config' declared here
    2, quantum_manifold_optimizer.cpp:518:58: unknown type name 'Config'; did you mean 'PatternEvolutionBridge::Config'?

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:521:1: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 519d067060c648b334af9d85785fc821
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, quantum_manifold_optimizer.cpp:521:1: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:523:1: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 16834158cb6395aa7c714e4696bb8bb2
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, quantum_manifold_optimizer.cpp:523:1: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:524:1: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b9fa7f3518737b4e8b5905b11f79ca8c
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, quantum_manifold_optimizer.cpp:524:1: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:525:42: unknown type name 'OptimizationTarget' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9b5567666b7480acd1aa19973813eb81
  Steps:
    1, quantum_manifold_optimizer.cpp:525:42: unknown type name 'OptimizationTarget'

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:529:6: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f343cd45978b7c96132e6adcb5cc18c8
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, quantum_manifold_optimizer.cpp:529:6: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:533:7: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c29ec2ccf4206d3ac04a6f6aadaa0b65
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, quantum_manifold_optimizer.cpp:533:7: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:537:24: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f332ecde57eddd4857736e9239eca201
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, quantum_manifold_optimizer.cpp:537:24: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/quantum_manifold_optimizer.cpp:543:80: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b8effffc16b6b9983300d26ef77a003f
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, quantum_manifold_optimizer.cpp:543:80: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

Found 11 defect(s) in quantum_manifold_optimizer.cpp

[CRITICAL] /sep/include/memory/quantum_coherence_manager.hpp:13:7: redefinition of 'QuantumCoherenceManager' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9a9bf6489fa5cbbfd4ecf8b0e7d5d66a
  Steps:
    1, quantum_coherence_manager.h:13:7: previous definition is here
    2, quantum_coherence_manager.hpp:13:7: redefinition of 'QuantumCoherenceManager'

Found 1 defect(s) in quantum_coherence_manager.hpp

[CRITICAL] /sep/include/quantum/qfh.h:65:11: duplicate member 'collapse_threshold' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a3cc0cc42ce1b80825c58fee3ee4ae1e
  Steps:
    1, qfh.h:63:11: previous declaration is here
    2, qfh.h:65:11: duplicate member 'collapse_threshold'

Found 1 defect(s) in qfh.h

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:23:7: redefinition of 'QuantumCoherenceManager' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4696f3c37da245582d6595644526d9a7
  Steps:
    1, quantum_coherence_manager.h:13:7: previous definition is here
    2, quantum_coherence_manager.cpp:23:7: redefinition of 'QuantumCoherenceManager'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:156:50: no type named 'AnomalyType' in 'sep::memory::QuantumCoherenceManager' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 955108cb20db43a6cd89c7f973c16373
  Steps:
    1, quantum_coherence_manager.cpp:156:50: no type named 'AnomalyType' in 'sep::memory::QuantumCoherenceManager'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:211:20: no member named 'tier_fragmentation' in 'sep::memory::QuantumCoherenceManager::CoherenceResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: df1008afb6ecfb517ee90b210e3d561f
  Steps:
    1, quantum_coherence_manager.cpp:211:20: no member named 'tier_fragmentation' in 'sep::memory::QuantumCoherenceManager::CoherenceResult'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:211:53: no member named 'tier_fragmentation' in 'sep::memory::QuantumCoherenceManager::CoherenceMetrics' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 44f92c33c61e7ad7a5dd2d802671c74d
  Steps:
    1, quantum_coherence_manager.cpp:211:53: no member named 'tier_fragmentation' in 'sep::memory::QuantumCoherenceManager::CoherenceMetrics'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:212:20: no member named 'tier_pattern_count' in 'sep::memory::QuantumCoherenceManager::CoherenceResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bbb8c7025cf238126e3f1d6c004bec73
  Steps:
    1, quantum_coherence_manager.cpp:212:20: no member named 'tier_pattern_count' in 'sep::memory::QuantumCoherenceManager::CoherenceResult'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:225:24: no member named 'for_each' in 'tbb::detail::d2::concurrent_hash_map<std::basic_string<char>, sep::memory::QuantumCoherenceManager::PatternCoherenceData>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9dcff80b5b1e9028a3d624e12b9ab152
  Steps:
    1, quantum_coherence_manager.cpp:225:24: no member named 'for_each' in 'tbb::detail::d2::concurrent_hash_map<std::basic_string<char>, sep::memory::QuantumCoherenceManager::PatternCoherenceData>'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:290:24: no member named 'for_each' in 'tbb::detail::d2::concurrent_hash_map<std::basic_string<char>, sep::memory::QuantumCoherenceManager::PatternCoherenceData>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 629ff8ff6faa029942d2a34d12a69b2a
  Steps:
    1, quantum_coherence_manager.cpp:290:24: no member named 'for_each' in 'tbb::detail::d2::concurrent_hash_map<std::basic_string<char>, sep::memory::QuantumCoherenceManager::PatternCoherenceData>'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:310:24: no member named 'for_each' in 'tbb::detail::d2::concurrent_hash_map<std::basic_string<char>, sep::memory::QuantumCoherenceManager::PatternCoherenceData>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 51aacee0fe80dfaeb7917a5342fcfee5
  Steps:
    1, quantum_coherence_manager.cpp:310:24: no member named 'for_each' in 'tbb::detail::d2::concurrent_hash_map<std::basic_string<char>, sep::memory::QuantumCoherenceManager::PatternCoherenceData>'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:353:29: no member named 'tier_fragmentation' in 'sep::memory::QuantumCoherenceManager::CoherenceMetrics' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0007110f53d86ef7ab8becc392103a05
  Steps:
    1, quantum_coherence_manager.cpp:353:29: no member named 'tier_fragmentation' in 'sep::memory::QuantumCoherenceManager::CoherenceMetrics'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:377:18: no member named 'fragmented_patterns' in 'sep::memory::QuantumCoherenceManager::CoherenceMetrics' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7caded37ae62aec0f7e456765d4e39bc
  Steps:
    1, quantum_coherence_manager.cpp:377:18: no member named 'fragmented_patterns' in 'sep::memory::QuantumCoherenceManager::CoherenceMetrics'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:384:22: no member named 'tier_fragmentation' in 'sep::memory::QuantumCoherenceManager::CoherenceMetrics' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0a7a62de72bccd17a3314f507bc41d9d
  Steps:
    1, quantum_coherence_manager.cpp:384:22: no member named 'tier_fragmentation' in 'sep::memory::QuantumCoherenceManager::CoherenceMetrics'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:405:35: cannot initialize object parameter of type 'sep::quantum::QuantumProcessorQFHCommon' with an expression of type 'sep::quantum::QuantumProcessorQFH' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 52e0f7a45e4e93fafc0aa70ec873e238
  Steps:
    1, quantum_coherence_manager.cpp:405:35: cannot initialize object parameter of type 'sep::quantum::QuantumProcessorQFHCommon' with an expression of type 'sep::quantum::QuantumProcessorQFH'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:409:30: cannot initialize object parameter of type 'sep::quantum::QuantumProcessorQFHCommon' with an expression of type 'sep::quantum::QuantumProcessorQFH' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: be27e66835ac489914cee4765bfb84f3
  Steps:
    1, quantum_coherence_manager.cpp:409:30: cannot initialize object parameter of type 'sep::quantum::QuantumProcessorQFHCommon' with an expression of type 'sep::quantum::QuantumProcessorQFH'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:420:18: no member named 'fragmentation_score' in 'sep::memory::QuantumCoherenceManager::PatternCoherenceData' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e30f80689de987a519c0f758469e8b04
  Steps:
    1, quantum_coherence_manager.cpp:420:18: no member named 'fragmentation_score' in 'sep::memory::QuantumCoherenceManager::PatternCoherenceData'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:433:22: no member named 'fragmentation_score' in 'sep::memory::QuantumCoherenceManager::PatternCoherenceData' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 77e2a3abf817ce4379ae0d9f3b051d03
  Steps:
    1, quantum_coherence_manager.cpp:433:22: no member named 'fragmentation_score' in 'sep::memory::QuantumCoherenceManager::PatternCoherenceData'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:447:24: no member named 'for_each' in 'tbb::detail::d2::concurrent_hash_map<std::basic_string<char>, sep::memory::QuantumCoherenceManager::PatternCoherenceData>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9dcff80b5b1e9028a3d624e12b9ab152
  Steps:
    1, quantum_coherence_manager.cpp:447:24: no member named 'for_each' in 'tbb::detail::d2::concurrent_hash_map<std::basic_string<char>, sep::memory::QuantumCoherenceManager::PatternCoherenceData>'

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:472:22: no member named 'tier_fragmentation' in 'sep::memory::QuantumCoherenceManager::CoherenceMetrics' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1ab1903ef1b3e22bb91e90eed08e1332
  Steps:
    1, quantum_coherence_manager.cpp:472:22: no member named 'tier_fragmentation' in 'sep::memory::QuantumCoherenceManager::CoherenceMetrics'

Found 17 defect(s) in quantum_coherence_manager.cpp

[MEDIUM] /sep/src/quantum/pattern_evolution_bridge.cpp:216:33: unused parameter 'worker_id' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 896d943e57dff0ea5f6bf9e7b0866a30
  Steps:
    1, pattern_evolution_bridge.cpp:216:33: unused parameter 'worker_id'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:233:35: no member named 'phase' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0c260a7d2f1260f1f92d30bbb5cba472
  Steps:
    1, pattern_evolution_bridge.cpp:233:35: no member named 'phase' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:242:31: no member named 'phase' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8243989bc6ff52a8d5d95e3412d8868a
  Steps:
    1, pattern_evolution_bridge.cpp:242:31: no member named 'phase' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:254:31: no member named 'evolution_rate' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 68627cc700ef4317887d6462efcf8dc2
  Steps:
    1, pattern_evolution_bridge.cpp:254:31: no member named 'evolution_rate' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:258:9: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4187744a588f9e2f5dc27277d7513ba5
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, pattern_evolution_bridge.cpp:258:9: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:262:55: member access into incomplete type 'sep::quantum::QuantumManifoldOptimizer' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 16c512806cdbadd337957bd8bc0fd3b9
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, pattern_evolution_bridge.cpp:262:55: member access into incomplete type 'sep::quantum::QuantumManifoldOptimizer'

[STYLE] /sep/src/quantum/pattern_evolution_bridge.cpp:283:34: 'data' should be used for accessing the data pointer instead of taking the address of the 0-th element [readability-container-data-pointer]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d6a15f09f35ba9c90fb1fe5a5d031988
  Notes:
    1, pattern_evolution_bridge.cpp:283:34: patterns.data() (fixit)
  Steps:
    1, pattern_evolution_bridge.cpp:283:34: 'data' should be used for accessing the data pointer instead of taking the address of the 0-th element

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:296:39: no member named 'energy' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c0e18dba234c0d74131e2e715ed5e5d1
  Steps:
    1, pattern_evolution_bridge.cpp:296:39: no member named 'energy' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:299:78: no member named 'energy' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bc2d99fa3e4012ab3ecf111cb5d67118
  Steps:
    1, pattern_evolution_bridge.cpp:299:78: no member named 'energy' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:300:76: no member named 'energy' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8a6d10c9aff70fd6b540d3aa677f0914
  Steps:
    1, pattern_evolution_bridge.cpp:300:76: no member named 'energy' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:301:70: no member named 'energy' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5618fbfeeed45344584c9581b320857f
  Steps:
    1, pattern_evolution_bridge.cpp:301:70: no member named 'energy' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:311:49: no member named 'momentum' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 863dba0f5dca8fe71acd79caaee4b028
  Steps:
    1, pattern_evolution_bridge.cpp:311:49: no member named 'momentum' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:311:67: no member named 'momentum' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: de0339b2064fdb88e1806a5b77e8ca5c
  Steps:
    1, pattern_evolution_bridge.cpp:311:67: no member named 'momentum' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:312:69: no member named 'coupling_strength' in 'sep::quantum::PatternEvolutionBridge::Config' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 78339eb9bceb1cb6f6bdd1e84256c2a4
  Steps:
    1, pattern_evolution_bridge.cpp:312:69: no member named 'coupling_strength' in 'sep::quantum::PatternEvolutionBridge::Config'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:326:58: no member named 'coupling_strength' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9cc252e882d0903d52f1ee7d2b2ac73f
  Steps:
    1, pattern_evolution_bridge.cpp:326:58: no member named 'coupling_strength' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:337:54: no member named 'phase' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c8eb183adb05e107d33f5e33610bca55
  Steps:
    1, pattern_evolution_bridge.cpp:337:54: no member named 'phase' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:337:79: no member named 'phase' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 082cd88b85b5bd31b61483882fa7a644
  Steps:
    1, pattern_evolution_bridge.cpp:337:79: no member named 'phase' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:345:42: no member named 'phase' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b653adf5b7ee31f14ffb1ec03afa8b6e
  Steps:
    1, pattern_evolution_bridge.cpp:345:42: no member named 'phase' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:345:67: no member named 'phase' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 707ffdab930fb736453a224581909387
  Steps:
    1, pattern_evolution_bridge.cpp:345:67: no member named 'phase' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_evolution_bridge.cpp:464:5: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 25d13f953027ebf17e1dbe74b94b696f
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, pattern_evolution_bridge.cpp:464:5: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

Found 20 defect(s) in pattern_evolution_bridge.cpp

[CRITICAL] /sep/include/blender/pattern_bridge.h:16:10: 'memory/memory_tier_manager.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ef6959e99135d74a3fd769b907e2fa62
  Steps:
    1, pattern_bridge.h:16:10: 'memory/memory_tier_manager.h' file not found

Found 1 defect(s) in pattern_bridge.h

[CRITICAL] /sep/src/main.cpp:34:17: no member named 'initializeLogging' in namespace 'sep::logging' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9ff3098bd20a06ef1a66af5601e35744
  Steps:
    1, main.cpp:34:17: no member named 'initializeLogging' in namespace 'sep::logging'

[MEDIUM] /sep/src/main.cpp:37:3: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d0691438712cca465e870fb019786858
  Steps:
    1, main.cpp:37:3: cast the expression to void to silence this warning
    2, main.cpp:37:3: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/main.cpp:38:3: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 59d4612f70dfb5ad3f0fb7538bb91b6b
  Steps:
    1, main.cpp:38:3: cast the expression to void to silence this warning
    2, main.cpp:38:3: the value returned by this function should not be disregarded; neglecting it may lead to errors

[CRITICAL] /sep/src/main.cpp:76:23: no member named 'shutdownLogging' in namespace 'sep::logging' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d50e4bc5001dcd72f747af67d2c92621
  Steps:
    1, main.cpp:76:23: no member named 'shutdownLogging' in namespace 'sep::logging'

[CRITICAL] /sep/src/main.cpp:87:23: no member named 'shutdownLogging' in namespace 'sep::logging' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d50e4bc5001dcd72f747af67d2c92621
  Steps:
    1, main.cpp:87:23: no member named 'shutdownLogging' in namespace 'sep::logging'

[CRITICAL] /sep/src/main.cpp:124:25: no member named 'shutdownLogging' in namespace 'sep::logging' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 072a06603f30b77cd367e2271d7f5e7a
  Steps:
    1, main.cpp:124:25: no member named 'shutdownLogging' in namespace 'sep::logging'

[CRITICAL] /sep/src/main.cpp:141:25: no member named 'shutdownLogging' in namespace 'sep::logging' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 072a06603f30b77cd367e2271d7f5e7a
  Steps:
    1, main.cpp:141:25: no member named 'shutdownLogging' in namespace 'sep::logging'

[CRITICAL] /sep/src/main.cpp:147:23: no member named 'shutdownLogging' in namespace 'sep::logging' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d50e4bc5001dcd72f747af67d2c92621
  Steps:
    1, main.cpp:147:23: no member named 'shutdownLogging' in namespace 'sep::logging'

[CRITICAL] /sep/src/main.cpp:153:23: no member named 'shutdownLogging' in namespace 'sep::logging' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d50e4bc5001dcd72f747af67d2c92621
  Steps:
    1, main.cpp:153:23: no member named 'shutdownLogging' in namespace 'sep::logging'

[CRITICAL] /sep/src/main.cpp:287:19: no member named 'shutdownLogging' in namespace 'sep::logging' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f5a09005224c796e3aad4984eeecd310
  Steps:
    1, main.cpp:287:19: no member named 'shutdownLogging' in namespace 'sep::logging'

[CRITICAL] /sep/src/main.cpp:293:17: no member named 'shutdownLogging' in namespace 'sep::logging' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1e83f43849b9e746e117e446bec93234
  Steps:
    1, main.cpp:293:17: no member named 'shutdownLogging' in namespace 'sep::logging'

Found 11 defect(s) in main.cpp

[CRITICAL] /sep/src/api/server.cpp:2:10: 'memory/memory_tier_manager.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1a555418b559c959a39d92aa652d150e
  Steps:
    1, server.cpp:2:10: 'memory/memory_tier_manager.h' file not found

Found 1 defect(s) in server.cpp

[CRITICAL] /sep/src/api/crow_adapter.cpp:28:10: 'memory/memory_tier_manager.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: acfcb993a55e3ac6016a78905ded7119
  Steps:
    1, crow_adapter.cpp:28:10: 'memory/memory_tier_manager.h' file not found

Found 1 defect(s) in crow_adapter.cpp

[MEDIUM] /sep/src/core/engine.cpp:58:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:58:5: cast the expression to void to silence this warning
    2, engine.cpp:58:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:62:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:62:5: cast the expression to void to silence this warning
    2, engine.cpp:62:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:66:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:66:5: cast the expression to void to silence this warning
    2, engine.cpp:66:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:75:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:75:9: cast the expression to void to silence this warning
    2, engine.cpp:75:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:81:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:81:9: cast the expression to void to silence this warning
    2, engine.cpp:81:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:85:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1b24dfd970075b2d58e9c58b518e9a70
  Steps:
    1, engine.cpp:85:13: cast the expression to void to silence this warning
    2, engine.cpp:85:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:91:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:91:5: cast the expression to void to silence this warning
    2, engine.cpp:91:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:95:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:95:5: cast the expression to void to silence this warning
    2, engine.cpp:95:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:100:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:100:9: cast the expression to void to silence this warning
    2, engine.cpp:100:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:104:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:104:5: cast the expression to void to silence this warning
    2, engine.cpp:104:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:108:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:108:5: cast the expression to void to silence this warning
    2, engine.cpp:108:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:121:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:121:9: cast the expression to void to silence this warning
    2, engine.cpp:121:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:128:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:128:9: cast the expression to void to silence this warning
    2, engine.cpp:128:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:148:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:148:5: cast the expression to void to silence this warning
    2, engine.cpp:148:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:154:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1b24dfd970075b2d58e9c58b518e9a70
  Steps:
    1, engine.cpp:154:13: cast the expression to void to silence this warning
    2, engine.cpp:154:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:159:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 40d0055d6f269f3f48e0ad02217f9686
  Steps:
    1, engine.cpp:159:17: cast the expression to void to silence this warning
    2, engine.cpp:159:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:163:17: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 40d0055d6f269f3f48e0ad02217f9686
  Steps:
    1, engine.cpp:163:17: cast the expression to void to silence this warning
    2, engine.cpp:163:17: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:168:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:168:9: cast the expression to void to silence this warning
    2, engine.cpp:168:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:176:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:176:5: cast the expression to void to silence this warning
    2, engine.cpp:176:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:181:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47db47d195ea360ec887c53149f2cfa1
  Steps:
    1, engine.cpp:181:9: cast the expression to void to silence this warning
    2, engine.cpp:181:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/core/engine.cpp:192:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99265dd40e0853d07ef36894c97a1e7e
  Steps:
    1, engine.cpp:192:5: cast the expression to void to silence this warning
    2, engine.cpp:192:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 21 defect(s) in engine.cpp

[CRITICAL] /sep/include/compat/cuda_impl.h:24:35: no type named 'cudaDeviceProp' in namespace 'sep::cuda' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a2ea76e1ad4b29b0c8eebb0c42f90d44
  Steps:
    1, cuda_impl.h:24:35: no type named 'cudaDeviceProp' in namespace 'sep::cuda'

[CRITICAL] /sep/include/compat/cuda_impl.h:75:39: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c95260136296f5290b1e281345041272
  Steps:
    1, cuda_impl.h:75:39: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:79:22: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 23b54740dde9f2913a67264864aceb7f
  Steps:
    1, cuda_impl.h:79:22: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/cuda_impl.h:85:8: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b7f7267d707260f8858cc341ba898c81
  Steps:
    1, cuda_impl.h:85:8: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:90:12: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eeb22b7a002445ea9dc6cc78d24133e8
  Steps:
    1, cuda_impl.h:90:12: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/cuda_impl.h:96:8: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 676ec5ae7cecacd3d8d36a86f5f3ccc5
  Steps:
    1, cuda_impl.h:96:8: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:103:12: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eeb22b7a002445ea9dc6cc78d24133e8
  Steps:
    1, cuda_impl.h:103:12: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/cuda_impl.h:108:8: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fd828a160ccd420aa470f5ecee633cd5
  Steps:
    1, cuda_impl.h:108:8: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:108:36: unknown type name 'cudaEvent_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a58272eb5fc9cbe53d735fbf296826b1
  Steps:
    1, cuda_impl.h:108:36: unknown type name 'cudaEvent_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:115:12: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eeb22b7a002445ea9dc6cc78d24133e8
  Steps:
    1, cuda_impl.h:115:12: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/cuda_impl.h:119:8: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: aa7bc03bb4c7d96958105a61bfff178b
  Steps:
    1, cuda_impl.h:119:8: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:119:37: unknown type name 'cudaEvent_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 887c9d802cdf1502f888bbd89f44abb2
  Steps:
    1, cuda_impl.h:119:37: unknown type name 'cudaEvent_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:124:12: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eeb22b7a002445ea9dc6cc78d24133e8
  Steps:
    1, cuda_impl.h:124:12: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/cuda_impl.h:128:8: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f11ae6a236235f1e32087ee4689d1cdc
  Steps:
    1, cuda_impl.h:128:8: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:128:36: unknown type name 'cudaEvent_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 247ea6c85a2427333174b90ca5f1957f
  Steps:
    1, cuda_impl.h:128:36: unknown type name 'cudaEvent_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:128:55: unknown type name 'cudaStream_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 647b763edafcadbbb066348c155fa35a
  Steps:
    1, cuda_impl.h:128:55: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:134:12: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eeb22b7a002445ea9dc6cc78d24133e8
  Steps:
    1, cuda_impl.h:134:12: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/cuda_impl.h:138:8: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 70681d237f87fa2cec7773ad11c6445d
  Steps:
    1, cuda_impl.h:138:8: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:138:41: unknown type name 'cudaEvent_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ed66cc15f2e8e914389b6641e8d19267
  Steps:
    1, cuda_impl.h:138:41: unknown type name 'cudaEvent_t'

Found 19 defect(s) in cuda_impl.h

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:137:54: no type named 'CudaConfig' in 'sep::quantum::manifold::ManifoldConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f64e50a9e5a8b2aafd4a4ea13670f58b
  Steps:
    1, quantum_manifold_optimizer.h:137:54: no type named 'CudaConfig' in 'sep::quantum::manifold::ManifoldConfig'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:154:5: unknown type name 'cufftHandle' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9735e5dc1075b98480f5710a54979256
  Steps:
    1, quantum_manifold_optimizer.h:154:5: unknown type name 'cufftHandle'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:155:21: no type named 'CudaConfig' in 'sep::quantum::manifold::ManifoldConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 95736f842b5c9fad0bbeb7c2f75a47f4
  Steps:
    1, quantum_manifold_optimizer.h:155:21: no type named 'CudaConfig' in 'sep::quantum::manifold::ManifoldConfig'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:164:42: no type named 'APIConfig' in 'sep::quantum::manifold::ManifoldConfig'; did you mean 'config::APIConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9eb61477ba0834f6e4eda6cee1f65f4b
  Notes:
    1, quantum_manifold_optimizer.h:164:42: config::APIConfig (fixit)
  Steps:
    1, types.h:61:8: 'config::APIConfig' declared here
    2, quantum_manifold_optimizer.h:164:42: no type named 'APIConfig' in 'sep::quantum::manifold::ManifoldConfig'; did you mean 'config::APIConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:181:5: no type named 'APIConfig' in 'sep::quantum::manifold::ManifoldConfig'; did you mean 'config::APIConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e42a0c9e28155855f5a2000e75260b58
  Notes:
    1, quantum_manifold_optimizer.h:181:5: config::APIConfig (fixit)
  Steps:
    1, types.h:61:8: 'config::APIConfig' declared here
    2, quantum_manifold_optimizer.h:181:5: no type named 'APIConfig' in 'sep::quantum::manifold::ManifoldConfig'; did you mean 'config::APIConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:191:54: no type named 'SemanticConfig' in 'sep::quantum::manifold::ManifoldConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c3900088c1fb251814f03fe843d3948
  Steps:
    1, quantum_manifold_optimizer.h:191:54: no type named 'SemanticConfig' in 'sep::quantum::manifold::ManifoldConfig'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:218:21: no type named 'SemanticConfig' in 'sep::quantum::manifold::ManifoldConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9d361573740e50d0d39f1886a284bd87
  Steps:
    1, quantum_manifold_optimizer.h:218:21: no type named 'SemanticConfig' in 'sep::quantum::manifold::ManifoldConfig'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:228:56: no type named 'AnalyticsConfig' in 'sep::quantum::manifold::ManifoldConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dc208bccc39508757523f4e916155b54
  Steps:
    1, quantum_manifold_optimizer.h:228:56: no type named 'AnalyticsConfig' in 'sep::quantum::manifold::ManifoldConfig'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:261:21: no type named 'AnalyticsConfig' in 'sep::quantum::manifold::ManifoldConfig' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dbbbf15cbb04c3bd81703413ca62a259
  Steps:
    1, quantum_manifold_optimizer.h:261:21: no type named 'AnalyticsConfig' in 'sep::quantum::manifold::ManifoldConfig'

[MEDIUM] /sep/include/quantum/quantum_manifold_optimizer.h:306:57: unused parameter 'seed' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7cff759f82d82e6db00e346efab34033
  Steps:
    1, quantum_manifold_optimizer.h:306:57: unused parameter 'seed'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:306:68: invalid suffix 'QUANTUM42' on integer constant [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b3bfca10c3c62bae1336c19ad23b6afe
  Steps:
    1, quantum_manifold_optimizer.h:306:68: invalid suffix 'QUANTUM42' on integer constant

Found 11 defect(s) in quantum_manifold_optimizer.h

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:42:29: use of undeclared identifier 'streamStateChanged'; did you mean 'sep::audio::PipeWireCapture::streamStateChanged'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3b06fe051872e2b65920b87181bb1c77
  Notes:
    1, pipewire_capture.cpp:42:29: sep::audio::PipeWireCapture::streamStateChanged (fixit)
  Steps:
    1, pipewire_capture.h:44:15: 'sep::audio::PipeWireCapture::streamStateChanged' declared here
    2, pipewire_capture.cpp:42:29: use of undeclared identifier 'streamStateChanged'; did you mean 'sep::audio::PipeWireCapture::streamStateChanged'?

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:43:29: use of undeclared identifier 'streamProcess'; did you mean 'sep::audio::PipeWireCapture::streamProcess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 001920528c208428370bb78ead242cd4
  Notes:
    1, pipewire_capture.cpp:43:29: sep::audio::PipeWireCapture::streamProcess (fixit)
  Steps:
    1, pipewire_capture.h:47:15: 'sep::audio::PipeWireCapture::streamProcess' declared here
    2, pipewire_capture.cpp:43:29: use of undeclared identifier 'streamProcess'; did you mean 'sep::audio::PipeWireCapture::streamProcess'?

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:48:6: reference to 'PipeWireCapture' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 93e56b1c27b4a2dd77d98ffb8b491d13
  Steps:
    1, pipewire_capture.cpp:37:11: candidate found by name lookup is 'PipeWireCapture'
    2, pipewire_capture.h:31:7: candidate found by name lookup is 'sep::audio::PipeWireCapture'
    3, pipewire_capture.cpp:48:6: reference to 'PipeWireCapture' is ambiguous

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:77:1: reference to 'PipeWireCapture' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 416023f92d6c521deac6a67cce2941bd
  Steps:
    1, pipewire_capture.cpp:37:11: candidate found by name lookup is 'PipeWireCapture'
    2, pipewire_capture.h:31:7: candidate found by name lookup is 'sep::audio::PipeWireCapture'
    3, pipewire_capture.cpp:77:1: reference to 'PipeWireCapture' is ambiguous

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:82:12: reference to 'PipeWireCapture' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: df99d3fc23a6468a56c5ca5c89762672
  Steps:
    1, pipewire_capture.cpp:37:11: candidate found by name lookup is 'PipeWireCapture'
    2, pipewire_capture.h:31:7: candidate found by name lookup is 'sep::audio::PipeWireCapture'
    3, pipewire_capture.cpp:82:12: reference to 'PipeWireCapture' is ambiguous

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:120:12: reference to 'PipeWireCapture' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d33f0c2a9e8ad1e2ac2e41ea2dfd2cc3
  Steps:
    1, pipewire_capture.cpp:37:11: candidate found by name lookup is 'PipeWireCapture'
    2, pipewire_capture.h:31:7: candidate found by name lookup is 'sep::audio::PipeWireCapture'
    3, pipewire_capture.cpp:120:12: reference to 'PipeWireCapture' is ambiguous

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:201:12: reference to 'PipeWireCapture' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eb8b24207588195226c0bbc9e621c314
  Steps:
    1, pipewire_capture.cpp:37:11: candidate found by name lookup is 'PipeWireCapture'
    2, pipewire_capture.h:31:7: candidate found by name lookup is 'sep::audio::PipeWireCapture'
    3, pipewire_capture.cpp:201:12: reference to 'PipeWireCapture' is ambiguous

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:221:12: reference to 'PipeWireCapture' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6f520e1d1d4333990b3344c7fddbc563
  Steps:
    1, pipewire_capture.cpp:37:11: candidate found by name lookup is 'PipeWireCapture'
    2, pipewire_capture.h:31:7: candidate found by name lookup is 'sep::audio::PipeWireCapture'
    3, pipewire_capture.cpp:221:12: reference to 'PipeWireCapture' is ambiguous

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:233:6: reference to 'PipeWireCapture' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4cf9545004e952036eb60c35b7a0b4cb
  Steps:
    1, pipewire_capture.cpp:37:11: candidate found by name lookup is 'PipeWireCapture'
    2, pipewire_capture.h:31:7: candidate found by name lookup is 'sep::audio::PipeWireCapture'
    3, pipewire_capture.cpp:233:6: reference to 'PipeWireCapture' is ambiguous

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:233:35: unknown type name 'AudioCallback'; did you mean 'sep::audio::AudioCapture::AudioCallback'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a8a48c8427e4b570c0023061f039e668
  Notes:
    1, pipewire_capture.cpp:233:35: sep::audio::AudioCapture::AudioCallback (fixit)
  Steps:
    1, capture.h:14:9: 'sep::audio::AudioCapture::AudioCallback' declared here
    2, pipewire_capture.cpp:233:35: unknown type name 'AudioCallback'; did you mean 'sep::audio::AudioCapture::AudioCallback'?

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:239:14: reference to 'PipeWireCapture' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 700851ce955b4a1a5ac7c17072cb7828
  Steps:
    1, pipewire_capture.cpp:37:11: candidate found by name lookup is 'PipeWireCapture'
    2, pipewire_capture.h:31:7: candidate found by name lookup is 'sep::audio::PipeWireCapture'
    3, pipewire_capture.cpp:239:14: reference to 'PipeWireCapture' is ambiguous

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:245:6: reference to 'PipeWireCapture' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eed1845faa1bf234f52a89fb6f27aa57
  Steps:
    1, pipewire_capture.cpp:37:11: candidate found by name lookup is 'PipeWireCapture'
    2, pipewire_capture.h:31:7: candidate found by name lookup is 'sep::audio::PipeWireCapture'
    3, pipewire_capture.cpp:245:6: reference to 'PipeWireCapture' is ambiguous

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:263:6: reference to 'PipeWireCapture' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3b03202c9c5a9f96a32544efffc99d0b
  Steps:
    1, pipewire_capture.cpp:37:11: candidate found by name lookup is 'PipeWireCapture'
    2, pipewire_capture.h:31:7: candidate found by name lookup is 'sep::audio::PipeWireCapture'
    3, pipewire_capture.cpp:263:6: reference to 'PipeWireCapture' is ambiguous

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:324:45: cannot define or redeclare 'create' here because namespace 'PipeWireCapture' does not enclose namespace 'AudioCapture' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 60f64ec830472ce846eb11f0232483f0
  Steps:
    1, pipewire_capture.cpp:324:45: cannot define or redeclare 'create' here because namespace 'PipeWireCapture' does not enclose namespace 'AudioCapture'

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:326:12: use of undeclared identifier 'compat'; did you mean 'sep::compat'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3189ff38a50f4142ec0a80c2426c02ca
  Notes:
    1, pipewire_capture.cpp:326:12: sep::compat (fixit)
  Steps:
    1, component_bridge.h:14:11: 'sep::compat' declared here
    2, pipewire_capture.cpp:326:12: use of undeclared identifier 'compat'; did you mean 'sep::compat'?

Found 15 defect(s) in pipewire_capture.cpp

[LOW] /usr/include/pipewire-0.3/pipewire/core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path [misc-header-include-cycle]
#include <pipewire/context.h>
         ^
  Report hash: 1daf003ee682828fd0bcf5e77b8a5137
  Steps:
    1, context.h:48:10: 'core.h' included from here
    2, pipewire_includes.h:24:10: 'context.h' included from here
    3, core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path

Found 1 defect(s) in core.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: c36587793019ad5530065d0f43f044ca
  Steps:
    1, resource.h:37:10: 'impl-client.h' included from here
    2, impl-core.h:29:10: 'resource.h' included from here
    3, impl.h:25:10: 'impl-core.h' included from here
    4, global.h:39:10: 'impl.h' included from here
    5, pipewire_includes.h:25:10: 'global.h' included from here
    6, impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-client.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: b4e1d1024c73f5543ef983da74c8a645
  Steps:
    1, impl.h:25:10: 'impl-core.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:25:10: 'global.h' included from here
    4, impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-core.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 98baed36db9cbd1ca48fe18b801bea5f
  Steps:
    1, impl.h:27:10: 'impl-device.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:25:10: 'global.h' included from here
    4, impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-device.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: caa7dc7877996ab29c3b85d029b375c4
  Steps:
    1, impl.h:28:10: 'impl-factory.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:25:10: 'global.h' included from here
    4, impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-factory.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-metadata.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 49976cb41585c3805dce471218afb328
  Steps:
    1, impl.h:31:10: 'impl-metadata.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:25:10: 'global.h' included from here
    4, impl-metadata.h:25:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-metadata.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl.h:29:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 97378dfee83bf376433e4226817d4af1
  Steps:
    1, global.h:39:10: 'impl.h' included from here
    2, pipewire_includes.h:25:10: 'global.h' included from here
    3, impl.h:29:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl.h

[LOW] /usr/include/pipewire-0.3/pipewire/protocol.h:26:10: circular header file dependency detected while including 'context.h', please check the include path [misc-header-include-cycle]
#include <pipewire/context.h>
         ^
  Report hash: 8a4ca310782449cb3b78489b5a83a9f9
  Steps:
    1, proxy.h:90:10: 'protocol.h' included from here
    2, core.h:79:10: 'proxy.h' included from here
    3, context.h:48:10: 'core.h' included from here
    4, pipewire_includes.h:24:10: 'context.h' included from here
    5, protocol.h:26:10: circular header file dependency detected while including 'context.h', please check the include path

Found 1 defect(s) in protocol.h

[CRITICAL] /sep/src/quantum/types_serialization.cpp:16:34: no member named 'evolution_rate' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 445e748c3b548bb0200d80445552de8d
  Steps:
    1, types_serialization.cpp:16:34: no member named 'evolution_rate' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/types_serialization.cpp:17:37: no member named 'coupling_strength' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d90c2f1b2c956356f1bddc30269c85c9
  Steps:
    1, types_serialization.cpp:17:37: no member named 'coupling_strength' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/types_serialization.cpp:30:41: no member named 'evolution_rate' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: df52456c47e32c68c99c3839711ce67e
  Steps:
    1, types_serialization.cpp:30:41: no member named 'evolution_rate' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/types_serialization.cpp:31:44: no member named 'coupling_strength' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3531cc5c6c77b40210aa4b89040687fd
  Steps:
    1, types_serialization.cpp:31:44: no member named 'coupling_strength' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/types_serialization.cpp:54:31: no member named 'momentum' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4804f4195a413efe3da303ef5a96112c
  Steps:
    1, types_serialization.cpp:54:31: no member named 'momentum' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/quantum/types_serialization.cpp:54:51: no member named 'momentum' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6f5731cb8e49aa48fabaa248084bf060
  Steps:
    1, types_serialization.cpp:54:51: no member named 'momentum' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/quantum/types_serialization.cpp:54:71: no member named 'momentum' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c317b4e1272d2272f235318f6ef3b156
  Steps:
    1, types_serialization.cpp:54:71: no member named 'momentum' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/quantum/types_serialization.cpp:54:91: no member named 'momentum' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9d8d77ddabcd2dcc26c9808a3552a476
  Steps:
    1, types_serialization.cpp:54:91: no member named 'momentum' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/quantum/types_serialization.cpp:68:53: no member named 'get_to' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d5a14ba57c153b7e3586796a8f50b5ad
  Steps:
    1, types_serialization.cpp:68:53: no member named 'get_to' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/types_serialization.cpp:69:80: no member named 'get' in 'std::vector<float>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4b8812c38dd8aa81c6678e6891702ebb
  Steps:
    1, types_serialization.cpp:69:80: no member named 'get' in 'std::vector<float>'

[CRITICAL] /sep/src/quantum/types_serialization.cpp:69:102: expected '(' for function-style cast or type construction [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dea1339d312352ebc6120cbe91db4e4d
  Steps:
    1, types_serialization.cpp:69:102: expected '(' for function-style cast or type construction

[CRITICAL] /sep/src/quantum/types_serialization.cpp:69:104: expected expression [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: edafa795ba8f95290fb786c7993ec06b
  Steps:
    1, types_serialization.cpp:69:104: expected expression

[CRITICAL] /sep/src/quantum/types_serialization.cpp:70:13: no member named 'momentum' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a258ed453b12c13cc3f9de2b1eeb0dc4
  Steps:
    1, types_serialization.cpp:70:13: no member named 'momentum' in 'sep::quantum::Pattern'

Found 13 defect(s) in types_serialization.cpp

[LOW] /sep/include/api/bridge_internal.hpp:12:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5ca7fa69e4196bf69f99f847456b1c42
  Steps:
    1, processor.h:79:7: a definition of 'Processor' is found here
    2, bridge_internal.hpp:12:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in bridge_internal.hpp

[CRITICAL] /sep/src/api/bridge_c.cpp:94:48: no member named 'error_code' in 'sep::quantum::BatchProcessingResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dc06901f870dab032e680944093b3675
  Steps:
    1, bridge_c.cpp:94:48: no member named 'error_code' in 'sep::quantum::BatchProcessingResult'

Found 1 defect(s) in bridge_c.cpp

[CRITICAL] /sep/src/compat/raii.cpp:16:10: 'memory/memory_tier_manager.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 03a3eb2a6675535e9dda6d16a523908e
  Steps:
    1, raii.cpp:16:10: 'memory/memory_tier_manager.h' file not found

Found 1 defect(s) in raii.cpp

[LOW] /sep/include/api/sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7ddbaf54ea1b7346b97eb4b637f989f0
  Steps:
    1, processor.h:79:7: a definition of 'Processor' is found here
    2, sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in sep_engine.h

[CRITICAL] /sep/src/api/sep_engine.cpp:21:10: 'memory/memory_tier_manager.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3829bf2569b53ede39e00e6b3e9f80eb
  Steps:
    1, sep_engine.cpp:21:10: 'memory/memory_tier_manager.h' file not found

Found 1 defect(s) in sep_engine.cpp

[LOW] /sep/include/blender/cycles_renderer.h:43:5: class 'CyclesRenderer' can be made trivially destructible by defaulting the destructor on its first declaration [performance-trivially-destructible]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ccf5d5c66a925f99227ef33b940650a5
  Notes:
    1, cycles_renderer.h:43:23: = default (fixit)
  Steps:
    1, cycles_renderer.cpp:50:17: destructor definition is here
    2, cycles_renderer.h:43:5: class 'CyclesRenderer' can be made trivially destructible by defaulting the destructor on its first declaration

Found 1 defect(s) in cycles_renderer.h

[LOW] /sep/include/compat/cuda_helpers.h:8:10: circular header file dependency detected while including 'cuda_impl.h', please check the include path [misc-header-include-cycle]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 16ba3ed71858f79fc44ba8c54e534373
  Steps:
    1, cuda_impl.h:11:10: 'cuda_helpers.h' included from here
    2, cuda_common.h:10:10: 'cuda_impl.h' included from here
    3, cuda_helpers.h:8:10: circular header file dependency detected while including 'cuda_impl.h', please check the include path

[LOW] /sep/include/compat/cuda_helpers.h:13:10: circular header file dependency detected while including 'cuda_common.h', please check the include path [misc-header-include-cycle]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6aff1d68f03fcba304b633225d55bc1f
  Steps:
    1, cuda_impl.h:11:10: 'cuda_helpers.h' included from here
    2, cuda_common.h:10:10: 'cuda_impl.h' included from here
    3, core.h:5:10: 'cuda_common.h' included from here
    4, cuda_helpers.h:13:10: circular header file dependency detected while including 'cuda_common.h', please check the include path

Found 2 defect(s) in cuda_helpers.h

[LOW] /sep/include/quantum/quantum_manifold_optimizer.h:25:7: declaration 'HamiltonianEvolver' is never referenced, but a declaration with the same name found in another namespace '(global)' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0e26b14df021ed412b3c1a0ba0af70b6
  Steps:
    1, quantum_manifold_optimizer.h:76:7: a declaration of 'HamiltonianEvolver' is found here
    2, quantum_manifold_optimizer.h:25:7: declaration 'HamiltonianEvolver' is never referenced, but a declaration with the same name found in another namespace '(global)'

[LOW] /sep/include/quantum/quantum_manifold_optimizer.h:26:7: declaration 'CUDAQuantumKernel' is never referenced, but a declaration with the same name found in another namespace '(global)' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c7cdc921649e7183f81df1147f8d538
  Steps:
    1, quantum_manifold_optimizer.h:77:7: a declaration of 'CUDAQuantumKernel' is found here
    2, quantum_manifold_optimizer.h:26:7: declaration 'CUDAQuantumKernel' is never referenced, but a declaration with the same name found in another namespace '(global)'

[LOW] /sep/include/quantum/quantum_manifold_optimizer.h:26:7: no definition found for 'CUDAQuantumKernel', but a definition with the same name 'CUDAQuantumKernel' found in another namespace '(global)' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 61406a218cd09da4bbcdde659a947ffd
  Steps:
    1, quantum_manifold_optimizer.h:263:7: a definition of 'CUDAQuantumKernel' is found here
    2, quantum_manifold_optimizer.h:26:7: no definition found for 'CUDAQuantumKernel', but a definition with the same name 'CUDAQuantumKernel' found in another namespace '(global)'

[LOW] /sep/include/quantum/quantum_manifold_optimizer.h:27:7: declaration 'SemanticProcessor' is never referenced, but a declaration with the same name found in another namespace '(global)' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c131d48cfc11ef0a3884b0dbd7dbc86c
  Steps:
    1, quantum_manifold_optimizer.h:78:7: a declaration of 'SemanticProcessor' is found here
    2, quantum_manifold_optimizer.h:27:7: declaration 'SemanticProcessor' is never referenced, but a declaration with the same name found in another namespace '(global)'

[LOW] /sep/include/quantum/quantum_manifold_optimizer.h:27:7: no definition found for 'SemanticProcessor', but a definition with the same name 'SemanticProcessor' found in another namespace '(global)' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cd3159bc36d035046de0e85fd4646eef
  Steps:
    1, quantum_manifold_optimizer.h:317:7: a definition of 'SemanticProcessor' is found here
    2, quantum_manifold_optimizer.h:27:7: no definition found for 'SemanticProcessor', but a definition with the same name 'SemanticProcessor' found in another namespace '(global)'

[LOW] /sep/include/quantum/quantum_manifold_optimizer.h:28:7: declaration 'PerformanceAnalyzer' is never referenced, but a declaration with the same name found in another namespace '(global)' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 50b6cb674de9511e6f54ce32601b624b
  Steps:
    1, quantum_manifold_optimizer.h:79:7: a declaration of 'PerformanceAnalyzer' is found here
    2, quantum_manifold_optimizer.h:28:7: declaration 'PerformanceAnalyzer' is never referenced, but a declaration with the same name found in another namespace '(global)'

[LOW] /sep/include/quantum/quantum_manifold_optimizer.h:28:7: no definition found for 'PerformanceAnalyzer', but a definition with the same name 'PerformanceAnalyzer' found in another namespace '(global)' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c768fc368a8b655c499c8cb0ff8da88d
  Steps:
    1, quantum_manifold_optimizer.h:354:7: a definition of 'PerformanceAnalyzer' is found here
    2, quantum_manifold_optimizer.h:28:7: no definition found for 'PerformanceAnalyzer', but a definition with the same name 'PerformanceAnalyzer' found in another namespace '(global)'

[LOW] /sep/include/quantum/quantum_manifold_optimizer.h:34:16: no definition found for 'CUDAConfig', but a definition with the same name 'CUDAConfig' found in another namespace 'sep::config' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5c2ab5042dcb54c51dee8715ac890555
  Steps:
    1, types.h:88:8: a definition of 'CUDAConfig' is found here
    2, quantum_manifold_optimizer.h:34:16: no definition found for 'CUDAConfig', but a definition with the same name 'CUDAConfig' found in another namespace 'sep::config'

[LOW] /sep/include/quantum/quantum_manifold_optimizer.h:35:16: no definition found for 'APIConfig', but a definition with the same name 'APIConfig' found in another namespace 'sep::config' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f4b3d362780600501d0f88a15c7c18f6
  Steps:
    1, types.h:62:8: a definition of 'APIConfig' is found here
    2, quantum_manifold_optimizer.h:35:16: no definition found for 'APIConfig', but a definition with the same name 'APIConfig' found in another namespace 'sep::config'

[LOW] /sep/include/quantum/quantum_manifold_optimizer.h:36:16: no definition found for 'LogConfig', but a definition with the same name 'LogConfig' found in another namespace 'sep::config' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2ef5499006b248aeab03c642a2246529
  Steps:
    1, types.h:97:8: a definition of 'LogConfig' is found here
    2, quantum_manifold_optimizer.h:36:16: no definition found for 'LogConfig', but a definition with the same name 'LogConfig' found in another namespace 'sep::config'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:115:7: no member named 'MemoryTierEnum' in namespace 'sep::memory'; did you mean 'sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 94d625835bdb5ae66805bd57371b57dc
  Notes:
    1, quantum_manifold_optimizer.h:115:7: sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: 'sep::MemoryTierEnum' declared here
    2, quantum_manifold_optimizer.h:115:7: no member named 'MemoryTierEnum' in namespace 'sep::memory'; did you mean 'sep::MemoryTierEnum'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:146:5: unknown type name 'CUDAConfig'; did you mean 'sep::config::CUDAConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9ebcbd364ee24a01c500c36e77ca8607
  Notes:
    1, quantum_manifold_optimizer.h:146:5: sep::config::CUDAConfig (fixit)
  Steps:
    1, types.h:88:8: 'sep::config::CUDAConfig' declared here
    2, quantum_manifold_optimizer.h:146:5: unknown type name 'CUDAConfig'; did you mean 'sep::config::CUDAConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:147:5: unknown type name 'APIConfig'; did you mean 'APIConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ae686bb80790d57cc589b37e03343faf
  Notes:
    1, quantum_manifold_optimizer.h:147:5: APIConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:108:12: 'APIConfig' declared here
    2, quantum_manifold_optimizer.h:147:5: unknown type name 'APIConfig'; did you mean 'APIConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:148:5: unknown type name 'LogConfig'; did you mean 'sep::config::LogConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bffbb737a054c778a59c35b4e2885214
  Notes:
    1, quantum_manifold_optimizer.h:148:5: sep::config::LogConfig (fixit)
  Steps:
    1, types.h:97:8: 'sep::config::LogConfig' declared here
    2, quantum_manifold_optimizer.h:148:5: unknown type name 'LogConfig'; did you mean 'sep::config::LogConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:158:7: use of undeclared identifier 'QuantumManifoldOptimizer'; did you mean 'sep::quantum::manifold::QuantumManifoldOptimizer'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 427a68ae8258990d2af6ad934bac5284
  Notes:
    1, quantum_manifold_optimizer.h:158:7: sep::quantum::manifold::QuantumManifoldOptimizer (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:40:7: 'sep::quantum::manifold::QuantumManifoldOptimizer' declared here
    2, quantum_manifold_optimizer.h:158:7: use of undeclared identifier 'QuantumManifoldOptimizer'; did you mean 'sep::quantum::manifold::QuantumManifoldOptimizer'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:214:47: use of undeclared identifier 'QuantumPattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6bda9b26ba7593d1dc4f12555465aa20
  Steps:
    1, quantum_manifold_optimizer.h:214:47: use of undeclared identifier 'QuantumPattern'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:220:40: unknown type name 'QuantumPattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dd4b01ee83cae9bae6845206bd44bc44
  Steps:
    1, quantum_manifold_optimizer.h:220:40: unknown type name 'QuantumPattern'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:228:45: unknown type name 'QuantumPattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b566dc9a4b741c205aa94d14a7bf1e10
  Steps:
    1, quantum_manifold_optimizer.h:228:45: unknown type name 'QuantumPattern'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:229:41: unknown type name 'QuantumPattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f2499ac1944e67d7c590bdf4adffc343
  Steps:
    1, quantum_manifold_optimizer.h:229:41: unknown type name 'QuantumPattern'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:233:41: unknown class name 'QuantumProcessorQFH'; did you mean 'sep::quantum::QuantumProcessorQFH'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d437dfc72188cd500c869dcc967dd0b1
  Notes:
    1, quantum_manifold_optimizer.h:233:41: sep::quantum::QuantumProcessorQFH (fixit)
  Steps:
    1, quantum_processor_qfh.h:59:7: 'sep::quantum::QuantumProcessorQFH' declared here
    2, quantum_manifold_optimizer.h:233:41: unknown class name 'QuantumProcessorQFH'; did you mean 'sep::quantum::QuantumProcessorQFH'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:246:65: use of undeclared identifier 'QuantumPattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8fbefd2d62a0e7594ecc675e5876ff8c
  Steps:
    1, quantum_manifold_optimizer.h:246:65: use of undeclared identifier 'QuantumPattern'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:249:5: unknown type name 'QFHResult'; did you mean 'sep::quantum::QFHResult'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ac09e757b964d4425495cdb41ca51c83
  Notes:
    1, quantum_manifold_optimizer.h:249:5: sep::quantum::QFHResult (fixit)
  Steps:
    1, qfh.h:55:8: 'sep::quantum::QFHResult' declared here
    2, quantum_manifold_optimizer.h:249:5: unknown type name 'QFHResult'; did you mean 'sep::quantum::QFHResult'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:265:38: no type named 'CudaConfig' in 'ManifoldConfig'; did you mean simply 'CudaConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fae3d6c466a13ec02f0ad3deea51b0db
  Notes:
    1, quantum_manifold_optimizer.h:265:38: CudaConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:99:12: 'CudaConfig' declared here
    2, quantum_manifold_optimizer.h:265:38: no type named 'CudaConfig' in 'ManifoldConfig'; did you mean simply 'CudaConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:283:5: no type named 'CudaConfig' in 'ManifoldConfig'; did you mean simply 'CudaConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0c89d7ed40eab865fc9c6f34699bdb96
  Notes:
    1, quantum_manifold_optimizer.h:283:5: CudaConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:99:12: 'CudaConfig' declared here
    2, quantum_manifold_optimizer.h:283:5: no type named 'CudaConfig' in 'ManifoldConfig'; did you mean simply 'CudaConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:292:42: no type named 'APIConfig' in 'ManifoldConfig'; did you mean simply 'APIConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 516dbf16f023a92b72a7175ae868277f
  Notes:
    1, quantum_manifold_optimizer.h:292:42: APIConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:108:12: 'APIConfig' declared here
    2, quantum_manifold_optimizer.h:292:42: no type named 'APIConfig' in 'ManifoldConfig'; did you mean simply 'APIConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:309:5: no type named 'APIConfig' in 'ManifoldConfig'; did you mean simply 'APIConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2a8eefaea8ef3ae37e59a2ec87f943fa
  Notes:
    1, quantum_manifold_optimizer.h:309:5: APIConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:108:12: 'APIConfig' declared here
    2, quantum_manifold_optimizer.h:309:5: no type named 'APIConfig' in 'ManifoldConfig'; did you mean simply 'APIConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:319:38: no type named 'SemanticConfig' in 'ManifoldConfig'; did you mean simply 'SemanticConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 265fb06b20950286a1c5c3f14c7c1192
  Notes:
    1, quantum_manifold_optimizer.h:319:38: SemanticConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:136:8: 'SemanticConfig' declared here
    2, quantum_manifold_optimizer.h:319:38: no type named 'SemanticConfig' in 'ManifoldConfig'; did you mean simply 'SemanticConfig'?

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:339:62: use of undeclared identifier 'QuantumPattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e21a2cae64b24afe76a496e276ab614a
  Steps:
    1, quantum_manifold_optimizer.h:339:62: use of undeclared identifier 'QuantumPattern'

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:346:5: no type named 'SemanticConfig' in 'ManifoldConfig'; did you mean simply 'SemanticConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dda67b5ae1b4273ff5179a51ed42dbca
  Notes:
    1, quantum_manifold_optimizer.h:346:5: SemanticConfig (fixit)
  Steps:
    1, quantum_manifold_optimizer.h:136:8: 'SemanticConfig' declared here
    2, quantum_manifold_optimizer.h:346:5: no type named 'SemanticConfig' in 'ManifoldConfig'; did you mean simply 'SemanticConfig'?

Found 29 defect(s) in quantum_manifold_optimizer.h

[LOW] /sep/include/compat/cuda_helpers.h:13:10: circular header file dependency detected while including 'cuda_common.h', please check the include path [misc-header-include-cycle]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6aff1d68f03fcba304b633225d55bc1f
  Steps:
    1, cuda_impl.h:11:10: 'cuda_helpers.h' included from here
    2, cuda_common.h:10:10: 'cuda_impl.h' included from here
    3, memory_tier_manager.cpp:6:10: 'cuda_common.h' included from here
    4, cuda_helpers.h:13:10: circular header file dependency detected while including 'cuda_common.h', please check the include path

Found 1 defect(s) in cuda_helpers.h

[MEDIUM] /sep/include/memory/memory_tier_manager.hpp:36:1: class 'Pattern' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 785e58236b5f6cd58d06bbe6359648ad
  Steps:
    1, types.h:44:8: previous use is here
    2, memory_tier_manager.hpp:36:1: did you mean struct here?
    3, memory_tier_manager.hpp:36:1: class 'Pattern' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 1 defect(s) in memory_tier_manager.hpp

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:251:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:251:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:295:26: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 005a58abc587efa85ab38a672b887462
  Steps:
    1, memory_tier_manager.cpp:295:26: implicit conversion increases floating-point precision: 'const float' to 'double'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:306:43: member access into incomplete type 'sep::persistence::RedisManager' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a9a65e1faa70326b467ab4b0957efd3c
  Steps:
    1, memory_tier_manager.hpp:44:7: forward declaration of 'sep::persistence::RedisManager'
    2, memory_tier_manager.cpp:306:43: member access into incomplete type 'sep::persistence::RedisManager'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:308:30: member access into incomplete type 'sep::persistence::RedisManager' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ae5d6cd63bb0fec22101b056d3955a6b
  Steps:
    1, memory_tier_manager.hpp:44:7: forward declaration of 'sep::persistence::RedisManager'
    2, memory_tier_manager.cpp:308:30: member access into incomplete type 'sep::persistence::RedisManager'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:310:39: member access into incomplete type 'sep::persistence::RedisManager' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 86545758100aba0909a5bec1e588612a
  Steps:
    1, memory_tier_manager.hpp:44:7: forward declaration of 'sep::persistence::RedisManager'
    2, memory_tier_manager.cpp:310:39: member access into incomplete type 'sep::persistence::RedisManager'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:335:43: member access into incomplete type 'sep::persistence::RedisManager' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a9a65e1faa70326b467ab4b0957efd3c
  Steps:
    1, memory_tier_manager.hpp:44:7: forward declaration of 'sep::persistence::RedisManager'
    2, memory_tier_manager.cpp:335:43: member access into incomplete type 'sep::persistence::RedisManager'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:338:19: member access into incomplete type 'sep::persistence::RedisManager' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: de51fa85d9e27117424e7cbfd7f2b8cc
  Steps:
    1, memory_tier_manager.hpp:44:7: forward declaration of 'sep::persistence::RedisManager'
    2, memory_tier_manager.cpp:338:19: member access into incomplete type 'sep::persistence::RedisManager'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:378:23: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0c34aea053751feecbd4d9b55c48af7c
  Steps:
    1, memory_tier_manager.cpp:378:23: implicit conversion increases floating-point precision: 'float' to 'double'

Found 8 defect(s) in memory_tier_manager.cpp

[CRITICAL] /usr/include/c++/15/bits/unique_ptr.h:91:16: invalid application of 'sizeof' to an incomplete type 'sep::persistence::RedisManager' [clang-diagnostic-error]
  static_assert(sizeof(_Tp)>0,
                ^
  Report hash: aebc07f0e07a6a237e3efb4488278821
  Steps:
    1, unique_ptr.h:399:4: in instantiation of member function 'std::default_delete<sep::persistence::RedisManager>::operator()' requested here
    2, memory_tier_manager.cpp:29:20: in instantiation of member function 'std::unique_ptr<sep::persistence::RedisManager>::~unique_ptr' requested here
    3, memory_tier_manager.hpp:44:7: forward declaration of 'sep::persistence::RedisManager'
    4, unique_ptr.h:91:16: invalid application of 'sizeof' to an incomplete type 'sep::persistence::RedisManager'

Found 1 defect(s) in unique_ptr.h

[LOW] /sep/extern/pipewire/src/pipewire/core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path [misc-header-include-cycle]
#include <pipewire/context.h>
         ^
  Report hash: 1daf003ee682828fd0bcf5e77b8a5137
  Steps:
    1, context.h:48:10: 'core.h' included from here
    2, pipewire_includes.h:24:10: 'context.h' included from here
    3, core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path

Found 1 defect(s) in core.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: c36587793019ad5530065d0f43f044ca
  Steps:
    1, resource.h:37:10: 'impl-client.h' included from here
    2, impl-core.h:29:10: 'resource.h' included from here
    3, impl.h:25:10: 'impl-core.h' included from here
    4, global.h:39:10: 'impl.h' included from here
    5, pipewire_includes.h:25:10: 'global.h' included from here
    6, impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-client.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: b4e1d1024c73f5543ef983da74c8a645
  Steps:
    1, impl.h:25:10: 'impl-core.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:25:10: 'global.h' included from here
    4, impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-core.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 98baed36db9cbd1ca48fe18b801bea5f
  Steps:
    1, impl.h:27:10: 'impl-device.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:25:10: 'global.h' included from here
    4, impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-device.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: caa7dc7877996ab29c3b85d029b375c4
  Steps:
    1, impl.h:28:10: 'impl-factory.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:25:10: 'global.h' included from here
    4, impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-factory.h

[LOW] /sep/extern/pipewire/src/pipewire/impl-metadata.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 49976cb41585c3805dce471218afb328
  Steps:
    1, impl.h:31:10: 'impl-metadata.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:25:10: 'global.h' included from here
    4, impl-metadata.h:25:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-metadata.h

[LOW] /sep/extern/pipewire/src/pipewire/impl.h:29:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 97378dfee83bf376433e4226817d4af1
  Steps:
    1, global.h:39:10: 'impl.h' included from here
    2, pipewire_includes.h:25:10: 'global.h' included from here
    3, impl.h:29:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl.h

[LOW] /sep/extern/pipewire/src/pipewire/protocol.h:26:10: circular header file dependency detected while including 'context.h', please check the include path [misc-header-include-cycle]
#include <pipewire/context.h>
         ^
  Report hash: 8a4ca310782449cb3b78489b5a83a9f9
  Steps:
    1, proxy.h:90:10: 'protocol.h' included from here
    2, core.h:79:10: 'proxy.h' included from here
    3, context.h:48:10: 'core.h' included from here
    4, pipewire_includes.h:24:10: 'context.h' included from here
    5, protocol.h:26:10: circular header file dependency detected while including 'context.h', please check the include path

Found 1 defect(s) in protocol.h

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:41:29: use of undeclared identifier 'streamStateChanged'; did you mean 'PipeWireCapture::streamStateChanged'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 02cc8874ba1c322f366de26e87196dcf
  Notes:
    1, pipewire_capture.cpp:41:29: PipeWireCapture::streamStateChanged (fixit)
  Steps:
    1, pipewire_capture.h:45:17: 'PipeWireCapture::streamStateChanged' declared here
    2, pipewire_capture.cpp:41:29: use of undeclared identifier 'streamStateChanged'; did you mean 'PipeWireCapture::streamStateChanged'?

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:42:29: use of undeclared identifier 'streamProcess'; did you mean 'PipeWireCapture::streamProcess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ad6c52cb2fa45175c11d1ee7748931f8
  Notes:
    1, pipewire_capture.cpp:42:29: PipeWireCapture::streamProcess (fixit)
  Steps:
    1, pipewire_capture.h:48:17: 'PipeWireCapture::streamProcess' declared here
    2, pipewire_capture.cpp:42:29: use of undeclared identifier 'streamProcess'; did you mean 'PipeWireCapture::streamProcess'?

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:243:23: out-of-line definition of 'streamStateChanged' does not match any declaration in 'sep::audio::PipeWireCapture' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d47dab97d74c891b012185b03f900c6c
  Steps:
    1, pipewire_capture.h:32:7: PipeWireCapture defined here
    2, pipewire_capture.cpp:243:23: out-of-line definition of 'streamStateChanged' does not match any declaration in 'sep::audio::PipeWireCapture'

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:245:48: use of undeclared identifier 'data' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e1962d9f9ee317ef9fc8d187c0633da3
  Steps:
    1, pipewire_capture.cpp:245:48: use of undeclared identifier 'data'

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:248:44: use of undeclared identifier 'old_state' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f350fd09ab835857ac166465398122b9
  Steps:
    1, pipewire_capture.cpp:248:44: use of undeclared identifier 'old_state'

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:249:44: use of undeclared identifier 'new_state' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c9fc16e57e299586b893c750d21b410
  Steps:
    1, pipewire_capture.cpp:249:44: use of undeclared identifier 'new_state'

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:252:9: use of undeclared identifier 'error' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7b97fa30461402c37e8e4cc3e44490a2
  Steps:
    1, pipewire_capture.cpp:252:9: use of undeclared identifier 'error'

[CRITICAL] /sep/src/audio/pipewire_capture.cpp:254:43: use of undeclared identifier 'error' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f8ed63e1fbfe5a7bd8169e7c725ef091
  Steps:
    1, pipewire_capture.cpp:254:43: use of undeclared identifier 'error'

Found 8 defect(s) in pipewire_capture.cpp

[CRITICAL] /sep/src/memory/redis_manager.cpp:35:21: redefinition of 'Impl' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e25971c7766343269202fd07735144f1
  Steps:
    1, redis_manager.h:41:11: previous definition is here
    2, redis_manager.cpp:35:21: redefinition of 'Impl'

Found 1 defect(s) in redis_manager.cpp

[MEDIUM] /sep/src/api/sep_engine.cpp:293:67: unused parameter 'request_data' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d06191f01aa657f541e0410cc16c881a
  Steps:
    1, sep_engine.cpp:293:67: unused parameter 'request_data'

[CRITICAL] /sep/src/api/sep_engine.cpp:403:85: use of undeclared identifier 'startTime' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e75e0838708d986cc3121a0a285ccf34
  Steps:
    1, sep_engine.cpp:403:85: use of undeclared identifier 'startTime'

[CRITICAL] /sep/src/api/sep_engine.cpp:406:60: use of undeclared identifier 'totalRequests' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 13c4c9082e86bb77f0c07b088cd65647
  Steps:
    1, sep_engine.cpp:406:60: use of undeclared identifier 'totalRequests'

[CRITICAL] /sep/src/api/sep_engine.cpp:407:60: use of undeclared identifier 'successfulRequests' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 59edfb7c3f9a1bec6b93007ddaeb56d4
  Steps:
    1, sep_engine.cpp:407:60: use of undeclared identifier 'successfulRequests'

[CRITICAL] /sep/src/api/sep_engine.cpp:408:60: use of undeclared identifier 'failedRequests' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0f3095710b547aab94be83466e1a2251
  Steps:
    1, sep_engine.cpp:408:60: use of undeclared identifier 'failedRequests'

[CRITICAL] /sep/src/api/sep_engine.cpp:409:60: use of undeclared identifier 'timeoutRequests' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bc774a6c0afcde213622c6d971595f73
  Steps:
    1, sep_engine.cpp:409:60: use of undeclared identifier 'timeoutRequests'

[CRITICAL] /sep/src/api/sep_engine.cpp:410:60: use of undeclared identifier 'rateLimitedCount' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 30262dedf85f1da19531927170d8e4c0
  Steps:
    1, sep_engine.cpp:410:60: use of undeclared identifier 'rateLimitedCount'

[CRITICAL] /sep/src/api/sep_engine.cpp:411:60: use of undeclared identifier 'averageResponseTime' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1a1c000214ac83ce2788cc923ae71e53
  Steps:
    1, sep_engine.cpp:411:60: use of undeclared identifier 'averageResponseTime'

[CRITICAL] /sep/src/api/sep_engine.cpp:412:60: use of undeclared identifier 'lastResponseTime' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5e596702876b4facce82cd01691dbf18
  Steps:
    1, sep_engine.cpp:412:60: use of undeclared identifier 'lastResponseTime'

[CRITICAL] /sep/src/api/sep_engine.cpp:413:60: 'ErrorCode' does not refer to a value [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 16a32f5560b8bb04c1d254a3dac837bf
  Steps:
    1, types.h:68:12: declared here
    2, sep_engine.cpp:413:60: 'ErrorCode' does not refer to a value

[CRITICAL] /sep/src/api/sep_engine.cpp:426:10: invalid use of member 'impl_' in static member function [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 571a18c11842aadba960330823a22a4d
  Steps:
    1, sep_engine.cpp:426:10: invalid use of member 'impl_' in static member function

[CRITICAL] /sep/src/api/sep_engine.cpp:462:10: invalid use of member 'impl_' in static member function [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 571a18c11842aadba960330823a22a4d
  Steps:
    1, sep_engine.cpp:462:10: invalid use of member 'impl_' in static member function

Found 12 defect(s) in sep_engine.cpp

[LOW] /sep/include/memory/memory_tier_manager.hpp:34:8: no definition found for 'PatternConfig', but a definition with the same name 'PatternConfig' found in another namespace 'sep::sep::pattern' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9c0087d6132ff6337397a19a909eddf2
  Steps:
    1, data.hpp:26:8: a definition of 'PatternConfig' is found here
    2, memory_tier_manager.hpp:34:8: no definition found for 'PatternConfig', but a definition with the same name 'PatternConfig' found in another namespace 'sep::sep::pattern'

[CRITICAL] /sep/include/memory/memory_tier_manager.hpp:122:38: no type named 'Pattern' in namespace 'sep::quantum'; did you mean '::sep::sep::quantum::Pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ede1fb1fc11135155e511288e55cda38
  Notes:
    1, memory_tier_manager.hpp:122:38: ::sep::sep::quantum::Pattern (fixit)
  Steps:
    1, types.h:44:8: '::sep::sep::quantum::Pattern' declared here
    2, memory_tier_manager.hpp:122:38: no type named 'Pattern' in namespace 'sep::quantum'; did you mean '::sep::sep::quantum::Pattern'?

[CRITICAL] /sep/include/memory/memory_tier_manager.hpp:123:5: no type named 'Pattern' in namespace 'sep::quantum'; did you mean '::sep::sep::quantum::Pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ee81809f41c61e6b24a3ebff4055ecfa
  Notes:
    1, memory_tier_manager.hpp:123:5: ::sep::sep::quantum::Pattern (fixit)
  Steps:
    1, types.h:44:8: '::sep::sep::quantum::Pattern' declared here
    2, memory_tier_manager.hpp:123:5: no type named 'Pattern' in namespace 'sep::quantum'; did you mean '::sep::sep::quantum::Pattern'?

[CRITICAL] /sep/include/memory/memory_tier_manager.hpp:124:11: no type named 'Pattern' in namespace 'sep::quantum'; did you mean '::sep::sep::quantum::Pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 808a2601585f516f952f79204f3d7129
  Notes:
    1, memory_tier_manager.hpp:124:11: ::sep::sep::quantum::Pattern (fixit)
  Steps:
    1, types.h:44:8: '::sep::sep::quantum::Pattern' declared here
    2, memory_tier_manager.hpp:124:11: no type named 'Pattern' in namespace 'sep::quantum'; did you mean '::sep::sep::quantum::Pattern'?

Found 4 defect(s) in memory_tier_manager.hpp

[CRITICAL] /sep/include/memory/quantum_coherence_manager.h:47:9: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1f998a07442d27c817acbaf3365ce56a
  Notes:
    1, quantum_coherence_manager.h:47:9: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_coherence_manager.h:47:9: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

[CRITICAL] /sep/include/memory/quantum_coherence_manager.h:47:34: use of undeclared identifier 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ea027fdf43164be5bcafb144b3fc0e79
  Notes:
    1, quantum_coherence_manager.h:47:34: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_coherence_manager.h:47:34: use of undeclared identifier 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

[CRITICAL] /sep/include/memory/quantum_coherence_manager.h:48:9: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5a10f16a2cff9e6bdf4fa153e1f80219
  Notes:
    1, quantum_coherence_manager.h:48:9: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_coherence_manager.h:48:9: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

[CRITICAL] /sep/include/memory/quantum_coherence_manager.h:48:32: use of undeclared identifier 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: aa18c6c72170f2e4eb325ac4bd8f0fd7
  Notes:
    1, quantum_coherence_manager.h:48:32: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_coherence_manager.h:48:32: use of undeclared identifier 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

[CRITICAL] /sep/include/memory/quantum_coherence_manager.h:89:9: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cdf20ffd17bf11bf1e8140d3cad4b3db
  Notes:
    1, quantum_coherence_manager.h:89:9: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_coherence_manager.h:89:9: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

[CRITICAL] /sep/include/memory/quantum_coherence_manager.h:89:37: use of undeclared identifier 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4cfbcb4fe2eeca516defd45017fbe253
  Notes:
    1, quantum_coherence_manager.h:89:37: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_coherence_manager.h:89:37: use of undeclared identifier 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

Found 6 defect(s) in quantum_coherence_manager.h

[CRITICAL] /sep/include/memory/spdlog_isolation.h:17:51: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9f86aaa4d8c7d342c8fbe88298a14111
  Notes:
    1, spdlog_isolation.h:17:51: shim (fixit)
  Steps:
    1, shim.h:778:11: 'shim' declared here
    2, spdlog_isolation.h:17:51: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'?

[CRITICAL] /sep/include/memory/spdlog_isolation.h:19:33: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dd35d49936c2d84502862409a253c142
  Notes:
    1, spdlog_isolation.h:19:33: shim (fixit)
  Steps:
    1, shim.h:778:11: 'shim' declared here
    2, spdlog_isolation.h:19:33: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'?

[CRITICAL] /sep/include/memory/spdlog_isolation.h:26:43: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e7d2b3bb87256c88d0928e6db77e11bd
  Notes:
    1, spdlog_isolation.h:26:43: shim (fixit)
  Steps:
    1, shim.h:778:11: 'shim' declared here
    2, spdlog_isolation.h:26:43: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'?

[CRITICAL] /sep/include/memory/spdlog_isolation.h:31:25: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f7d39dbf2735194d0511faabfd933efa
  Notes:
    1, spdlog_isolation.h:31:25: shim (fixit)
  Steps:
    1, shim.h:778:11: 'shim' declared here
    2, spdlog_isolation.h:31:25: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'?

Found 4 defect(s) in spdlog_isolation.h

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:97:5: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2e011097a587bf2d25119b1adfdfd564
  Notes:
    1, quantum_manifold_optimizer.h:97:5: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_manifold_optimizer.h:97:5: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

Found 1 defect(s) in quantum_manifold_optimizer.h

[CRITICAL] /sep/include/quantum/quantum_processor_qfh.h:64:5: no type named 'MemoryTierEnum' in namespace 'sep'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3f36a36ae0b04d49e0dd23b03ea8c761
  Notes:
    1, quantum_processor_qfh.h:64:5: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_processor_qfh.h:64:5: no type named 'MemoryTierEnum' in namespace 'sep'; did you mean '::sep::sep::MemoryTierEnum'?

Found 1 defect(s) in quantum_processor_qfh.h

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:234:36: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 37b6a483d3bc66250b943b1aea2f0ba0
  Notes:
    1, quantum_coherence_manager.cpp:234:36: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_coherence_manager.cpp:234:36: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:238:32: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7eafff9bf9af09f9070d3f06299e2649
  Notes:
    1, quantum_coherence_manager.cpp:238:32: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_coherence_manager.cpp:238:32: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:522:23: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 428da16d88bb42118ff9b9ae48da5f32
  Notes:
    1, quantum_coherence_manager.cpp:522:23: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_coherence_manager.cpp:522:23: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

[CRITICAL] /sep/src/memory/quantum_coherence_manager.cpp:523:23: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1b1cb33a67e0214e3d5f3f831934c79b
  Notes:
    1, quantum_coherence_manager.cpp:523:23: ::sep::sep::MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: '::sep::sep::MemoryTierEnum' declared here
    2, quantum_coherence_manager.cpp:523:23: unknown type name 'MemoryTierEnum'; did you mean '::sep::sep::MemoryTierEnum'?

Found 4 defect(s) in quantum_coherence_manager.cpp

[CRITICAL] /sep/include/api/client.h:117:31: no member named 'ollama' in namespace 'sep::sep'; did you mean simply 'ollama'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c7d579597e882b12835daa87fc5e8d6e
  Notes:
    1, client.h:117:31: ollama (fixit)
  Steps:
    1, client.h:113:11: 'ollama' declared here
    2, client.h:117:31: no member named 'ollama' in namespace 'sep::sep'; did you mean simply 'ollama'?

[CRITICAL] /sep/include/api/client.h:132:3: no member named 'ollama' in namespace 'sep::sep'; did you mean simply 'ollama'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1a37b33812d21c669c09a2c0bf672a5f
  Notes:
    1, client.h:132:3: ollama (fixit)
  Steps:
    1, client.h:113:11: 'ollama' declared here
    2, client.h:132:3: no member named 'ollama' in namespace 'sep::sep'; did you mean simply 'ollama'?

[CRITICAL] /sep/include/api/client.h:133:18: no member named 'ollama' in namespace 'sep::sep'; did you mean simply 'ollama'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c7d34ada1046f79d5f4e7827d500cb32
  Notes:
    1, client.h:133:18: ollama (fixit)
  Steps:
    1, client.h:113:11: 'ollama' declared here
    2, client.h:133:18: no member named 'ollama' in namespace 'sep::sep'; did you mean simply 'ollama'?

[CRITICAL] /sep/include/api/client.h:135:3: no member named 'ollama' in namespace 'sep::sep'; did you mean simply 'ollama'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 065dcfda1181cefc518d5a79cf161a2e
  Notes:
    1, client.h:135:3: ollama (fixit)
  Steps:
    1, client.h:113:11: 'ollama' declared here
    2, client.h:135:3: no member named 'ollama' in namespace 'sep::sep'; did you mean simply 'ollama'?

[CRITICAL] /sep/include/api/client.h:136:22: no member named 'ollama' in namespace 'sep::sep'; did you mean simply 'ollama'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 31ff2212874ad04cbda79cbe5d701c2b
  Notes:
    1, client.h:136:22: ollama (fixit)
  Steps:
    1, client.h:113:11: 'ollama' declared here
    2, client.h:136:22: no member named 'ollama' in namespace 'sep::sep'; did you mean simply 'ollama'?

Found 5 defect(s) in client.h

[CRITICAL] /sep/include/api/json_helpers.h:10:40: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c26fd612278a1e8ea44f2ede7a7fe9b3
  Notes:
    1, json_helpers.h:10:40: shim (fixit)
  Steps:
    1, shim.h:778:11: 'shim' declared here
    2, json_helpers.h:10:40: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'?

[CRITICAL] /sep/include/api/json_helpers.h:33:40: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6558b6355e856c69925905589ceab0ed
  Notes:
    1, json_helpers.h:33:40: shim (fixit)
  Steps:
    1, shim.h:778:11: 'shim' declared here
    2, json_helpers.h:33:40: no member named 'shim' in namespace 'sep::sep'; did you mean simply 'shim'?

Found 2 defect(s) in json_helpers.h

[CRITICAL] /sep/include/api/rate_limit_middleware.h:25:27: no member named 'config' in namespace 'sep::sep'; did you mean simply 'config'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 514bc4dc3deb0c608a1945aea69aaae6
  Notes:
    1, rate_limit_middleware.h:25:27: config (fixit)
  Steps:
    1, types.h:25:11: 'config' declared here
    2, rate_limit_middleware.h:25:27: no member named 'config' in namespace 'sep::sep'; did you mean simply 'config'?

[CRITICAL] /sep/include/api/rate_limit_middleware.h:32:5: no member named 'config' in namespace 'sep::sep'; did you mean simply 'config'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0a5428817be4c1e647f0c1cfab86fe51
  Notes:
    1, rate_limit_middleware.h:32:5: config (fixit)
  Steps:
    1, types.h:25:11: 'config' declared here
    2, rate_limit_middleware.h:32:5: no member named 'config' in namespace 'sep::sep'; did you mean simply 'config'?

Found 2 defect(s) in rate_limit_middleware.h

[CRITICAL] /sep/include/memory/memory_tier_manager.hpp:122:38: use of undeclared identifier 'quantum'; did you mean '::sep::sep::quantum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e3eb8001a9b1e1bb81618a9037ac80ff
  Notes:
    1, memory_tier_manager.hpp:122:38: ::sep::sep::quantum (fixit)
  Steps:
    1, types.h:18:16: '::sep::sep::quantum' declared here
    2, memory_tier_manager.hpp:122:38: use of undeclared identifier 'quantum'; did you mean '::sep::sep::quantum'?

[CRITICAL] /sep/include/memory/memory_tier_manager.hpp:123:5: use of undeclared identifier 'quantum'; did you mean '::sep::sep::quantum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4ad1a904a44a70b87383c99043e592a3
  Notes:
    1, memory_tier_manager.hpp:123:5: ::sep::sep::quantum (fixit)
  Steps:
    1, types.h:18:16: '::sep::sep::quantum' declared here
    2, memory_tier_manager.hpp:123:5: use of undeclared identifier 'quantum'; did you mean '::sep::sep::quantum'?

[CRITICAL] /sep/include/memory/memory_tier_manager.hpp:124:11: use of undeclared identifier 'quantum'; did you mean '::sep::sep::quantum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d72dba0312a0bc9b38765d19b91185ab
  Notes:
    1, memory_tier_manager.hpp:124:11: ::sep::sep::quantum (fixit)
  Steps:
    1, types.h:18:16: '::sep::sep::quantum' declared here
    2, memory_tier_manager.hpp:124:11: use of undeclared identifier 'quantum'; did you mean '::sep::sep::quantum'?

Found 3 defect(s) in memory_tier_manager.hpp

[MEDIUM] /sep/src/api/server.cpp:36:24: field 'running_' will be initialized after field 'logger_' [clang-diagnostic-reorder-ctor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6306fcfef0d316871d0440c1f7b55802
  Notes:
    1, server.cpp:36:24: logger_(nullptr) running_(false) (fixit)
  Steps:
    1, server.cpp:36:24: field 'running_' will be initialized after field 'logger_'

[CRITICAL] /sep/src/api/server.cpp:82:20: return type of out-of-line definition of 'sep::api::SEPApiServer::start' differs from that in the declaration [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 778d339cee32c3a3435897cc74358032
  Steps:
    1, server.cpp:64:20: previous definition is here
    2, server.cpp:82:20: return type of out-of-line definition of 'sep::api::SEPApiServer::start' differs from that in the declaration

[MEDIUM] /sep/src/api/server.cpp:168:84: unused parameter 'body' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99c1cd2d504e0473209900460e079018
  Steps:
    1, server.cpp:168:84: unused parameter 'body'

[MEDIUM] /sep/src/api/server.cpp:228:50: unused parameter 'response_body' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9a6489114b93b9375b7c0c11dd032dae
  Steps:
    1, server.cpp:228:50: unused parameter 'response_body'

[MEDIUM] /sep/src/api/server.cpp:292:10: unused variable 'rate_limit_mw' [clang-diagnostic-unused-variable]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e3e4ad56877412f3b8b4bd5437469c50
  Steps:
    1, server.cpp:292:10: unused variable 'rate_limit_mw'

Found 5 defect(s) in server.cpp

[LOW] /sep/include/compat/cuda_helpers.h:13:10: circular header file dependency detected while including 'cuda_common.h', please check the include path [misc-header-include-cycle]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6aff1d68f03fcba304b633225d55bc1f
  Steps:
    1, cuda_impl.h:11:10: 'cuda_helpers.h' included from here
    2, cuda_common.h:10:10: 'cuda_impl.h' included from here
    3, memory_tier.cpp:20:10: 'cuda_common.h' included from here
    4, cuda_helpers.h:13:10: circular header file dependency detected while including 'cuda_common.h', please check the include path

Found 1 defect(s) in cuda_helpers.h

[CRITICAL] /sep/src/api/crow_adapter.cpp:86:37: no member named 'api' in namespace 'sep::sep'; did you mean simply 'api'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bd5fd764bd9124e7e212a61609338622
  Notes:
    1, crow_adapter.cpp:86:37: api (fixit)
  Steps:
    1, crow_adapter.cpp:30:16: 'api' declared here
    2, crow_adapter.cpp:86:37: no member named 'api' in namespace 'sep::sep'; did you mean simply 'api'?

[CRITICAL] /sep/src/api/crow_adapter.cpp:87:5: no member named 'config' in namespace 'sep::sep'; did you mean simply 'config'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4240b3cd4b8f3ade7fd28931f57e221c
  Notes:
    1, crow_adapter.cpp:87:5: config (fixit)
  Steps:
    1, types.h:25:11: 'config' declared here
    2, crow_adapter.cpp:87:5: no member named 'config' in namespace 'sep::sep'; did you mean simply 'config'?

Found 2 defect(s) in crow_adapter.cpp

[LOW] /sep/include/compat/cuda_helpers.h:13:10: circular header file dependency detected while including 'cuda_common.h', please check the include path [misc-header-include-cycle]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6aff1d68f03fcba304b633225d55bc1f
  Steps:
    1, cuda_impl.h:11:10: 'cuda_helpers.h' included from here
    2, cuda_common.h:10:10: 'cuda_impl.h' included from here
    3, metrics_collector.cpp:2:10: 'cuda_common.h' included from here
    4, cuda_helpers.h:13:10: circular header file dependency detected while including 'cuda_common.h', please check the include path

Found 1 defect(s) in cuda_helpers.h

[CRITICAL] /sep/src/core/metrics_collector.cpp:181:16: call to 'cudaMemGetInfo' is ambiguous [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f2cabd88df954f72e9996d6256ce0c4f
  Steps:
    1, cuda_helpers.h:30:29: expanded from macro 'CUDA_CHECK'
    2, cuda_runtime.h:98:13: candidate function
    3, cuda_impl.h:130:20: candidate function
    4, metrics_collector.cpp:181:16: call to 'cudaMemGetInfo' is ambiguous

[CRITICAL] /sep/src/core/metrics_collector.cpp:238:19: definition of explicitly defaulted default constructor [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d31290e6fd3bc73182c2919aa77b3ca2
  Steps:
    1, metrics_collector.cpp:238:19: definition of explicitly defaulted default constructor

Found 2 defect(s) in metrics_collector.cpp

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:158:7: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2d428e89c1d285e733798605145e6d28
  Steps:
    1, pattern_evolution_bridge.h:16:7: forward declaration of 'sep::quantum::QuantumManifoldOptimizer'
    2, quantum_manifold_optimizer.h:158:7: incomplete type 'sep::quantum::QuantumManifoldOptimizer' named in nested name specifier

[CRITICAL] /sep/include/quantum/quantum_manifold_optimizer.h:158:7: use of undeclared identifier 'QuantumManifoldOptimizer'; did you mean 'sep::quantum::QuantumManifoldOptimizer'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dc71249867471eb71f0a539795043dfe
  Notes:
    1, quantum_manifold_optimizer.h:158:7: sep::quantum::QuantumManifoldOptimizer (fixit)
  Steps:
    1, pattern_evolution_bridge.h:16:7: 'sep::quantum::QuantumManifoldOptimizer' declared here
    2, quantum_manifold_optimizer.h:158:7: use of undeclared identifier 'QuantumManifoldOptimizer'; did you mean 'sep::quantum::QuantumManifoldOptimizer'?

Found 2 defect(s) in quantum_manifold_optimizer.h

[CRITICAL] /sep/src/compat/component_bridge.cpp:26:11: member access into incomplete type 'sep::SEPBlenderBridge' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bdd2393ae4faa8c0810e6ebd74800694
  Steps:
    1, component_bridge.h:11:8: forward declaration of 'sep::SEPBlenderBridge'
    2, component_bridge.cpp:26:11: member access into incomplete type 'sep::SEPBlenderBridge'

Found 1 defect(s) in component_bridge.cpp

[CRITICAL] /usr/include/c++/15/bits/unique_ptr.h:91:16: invalid application of 'sizeof' to an incomplete type 'sep::SEPBlenderBridge' [clang-diagnostic-error]
  static_assert(sizeof(_Tp)>0,
                ^
  Report hash: 654bce7b5b5f01b55679e0ab4b642762
  Steps:
    1, unique_ptr.h:399:4: in instantiation of member function 'std::default_delete<sep::SEPBlenderBridge>::operator()' requested here
    2, component_bridge.cpp:25:19: in instantiation of member function 'std::unique_ptr<sep::SEPBlenderBridge>::~unique_ptr' requested here
    3, component_bridge.h:11:8: forward declaration of 'sep::SEPBlenderBridge'
    4, unique_ptr.h:91:16: invalid application of 'sizeof' to an incomplete type 'sep::SEPBlenderBridge'

[CRITICAL] /usr/include/c++/15/bits/unique_ptr.h:1085:34: allocation of incomplete type 'sep::SEPBlenderBridge' [clang-diagnostic-error]
    { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }
                                 ^
  Report hash: 54fd7132ccde0fcb7fa39f53e7399754
  Steps:
    1, component_bridge.cpp:25:24: in instantiation of function template specialization 'std::make_unique<sep::SEPBlenderBridge>' requested here
    2, component_bridge.h:11:8: forward declaration of 'sep::SEPBlenderBridge'
    3, unique_ptr.h:1085:34: allocation of incomplete type 'sep::SEPBlenderBridge'

Found 2 defect(s) in unique_ptr.h

Found no defects in hip_compat.cpp
Found no defects in config_manager_stub.cpp
Found no defects in memory_tier_manager_serialization.cpp
[LOW] /sep/src/memory/memory_tier_manager.cpp:322:24: Value stored to 'target' during its initialization is never read [deadcode.DeadStores]
        MemoryTierEnum target = block->tier;
                       ^
  Report hash: 5d186a9d3ec5a8bb03f55279bb54002a
  Steps:
    1, memory_tier_manager.cpp:322:24: Value stored to 'target' during its initialization is never read

Found 1 defect(s) in memory_tier_manager.cpp

Found no defects in manifold_config.cpp
Found no defects in simple_embedding_model.cpp
Found no defects in dataset_loader.cpp

----==== Severity Statistics ====----
----------------------------
Severity | Number of reports
----------------------------
LOW      |               151
HIGH     |                13
MEDIUM   |               552
CRITICAL |               532
STYLE    |                 5
----------------------------
----=================----

----==== Checker Statistics ====----
-------------------------------------------------------------------------
Checker name                               | Severity | Number of reports
-------------------------------------------------------------------------
deadcode.DeadStores                        | LOW      |                10
core.NullDereference                       | HIGH     |                 2
core.CallAndMessage                        | HIGH     |                 2
clang-diagnostic-unused-parameter          | MEDIUM   |               131
clang-diagnostic-mismatched-tags           | MEDIUM   |                17
bugprone-forward-declaration-namespace     | LOW      |                68
clang-diagnostic-infinite-recursion        | MEDIUM   |                 1
bugprone-forwarding-reference-overload     | LOW      |                 1
clang-diagnostic-unused-private-field      | MEDIUM   |                 3
clang-diagnostic-unused-but-set-variable   | MEDIUM   |                 1
clang-diagnostic-reorder-ctor              | MEDIUM   |                 6
clang-diagnostic-double-promotion          | MEDIUM   |                45
clang-diagnostic-reserved-identifier       | MEDIUM   |                98
bugprone-switch-missing-default-case       | LOW      |                11
misc-header-include-cycle                  | LOW      |                57
bugprone-sizeof-expression                 | HIGH     |                 4
cert-err33-c                               | MEDIUM   |               141
clang-diagnostic-reserved-macro-identifier | MEDIUM   |                45
clang-diagnostic-unused-variable           | MEDIUM   |                 7
clang-diagnostic-float-conversion          | MEDIUM   |                 2
clang-diagnostic-error                     | CRITICAL |               532
misc-definitions-in-headers                | MEDIUM   |                 4
clang-diagnostic-unused-function           | MEDIUM   |                 2
clang-diagnostic-vexing-parse              | MEDIUM   |                 1
performance-noexcept-move-constructor      | MEDIUM   |                12
google-build-namespaces                    | MEDIUM   |                 1
bugprone-incorrect-roundings               | HIGH     |                 1
misc-unconventional-assign-operator        | MEDIUM   |                 2
bugprone-signed-char-misuse                | MEDIUM   |                 2
bugprone-unused-return-value               | MEDIUM   |                 8
misc-redundant-expression                  | MEDIUM   |                 7
performance-move-const-arg                 | MEDIUM   |                 2
readability-container-data-pointer         | STYLE    |                 5
bugprone-move-forwarding-reference         | MEDIUM   |                 6
readability-suspicious-call-argument       | LOW      |                 1
optin.cplusplus.UninitializedObject        | MEDIUM   |                 3
unix.BlockInCriticalSection                | LOW      |                 2
cplusplus.Move                             | HIGH     |                 4
bugprone-signal-handler                    | MEDIUM   |                 4
clang-diagnostic-unused-lambda-capture     | MEDIUM   |                 1
performance-trivially-destructible         | LOW      |                 1
-------------------------------------------------------------------------
----=================----

----==== File Statistics ====----
-----------------------------------------------------
File name                         | Number of reports
-----------------------------------------------------
bridge_c.cpp                      |                 3
parser.h                          |                 2
cetintrin.h                       |                 2
quantum_coherence_manager.cpp     |                35
crow_isolation.h                  |                17
server.h                          |                 9
types.h                           |                 2
bridge_internal.hpp               |                 3
client.cpp                        |                 3
asio_isolation.h                  |                 7
logging.h                         |                 4
socket_adaptors.h                 |                10
memory_tier.hpp                   |                 1
sep_engine.h                      |                 2
sep_engine.cpp                    |                15
gpu_context.h                     |                 5
config.cpp                        |                 4
buffer.h                          |                 1
meta.h                            |                 1
format-utils.h                    |                 2
buffers.h                         |                 1
port-config.h                     |                 1
profile.h                         |                 1
route.h                           |                 1
builder.h                         |                 3
iter.h                            |                 2
cleanup.h                         |                 7
json-core.h                       |                30
string.h                          |                 3
type.h                            |                 5
control.h                         |                 1
core.h                            |                 3
impl-client.h                     |                 3
impl-core.h                       |                 2
impl-device.h                     |                 2
impl-factory.h                    |                 2
impl-link.h                       |                 1
impl-metadata.h                   |                 2
impl-node.h                       |                 1
impl-port.h                       |                 1
impl.h                            |                 2
properties.h                      |                 2
protocol.h                        |                 2
utils.h                           |                 2
server.cpp                        |                15
blender_integration.cpp           |                23
mesh_handler.cpp                  |                13
memory_tier_manager.cpp           |                37
engine.cpp                        |               120
cufft.h                           |                25
quantum_manifold_optimizer.h      |                55
evolution.cpp                     |                 5
quantum_manifold_optimizer.cpp    |                23
data.hpp                          |                 1
processor.h                       |                 1
quantum_processor_qfh.h           |                 2
types.h                           |                 4
pattern_evolution_bridge.h        |                 1
pattern_evolution_bridge.cpp      |                36
main.cpp                          |                42
cycles_renderer.cpp               |                12
cycles_test.cpp                   |                 5
cuda_impl.h                       |                20
lock_free_rate_limiter.cpp        |                 1
bridge.h                          |                 1
pipewire_capture.cpp              |                25
pattern_evolution.cpp             |                 2
pattern_processor.cpp             |                 3
mock_mesh_handler.cpp             |                 1
mock_blender_bridge.cpp           |                 2
mock_pattern_compression.cpp      |                10
mock_pattern_evolution.cpp        |                 3
mock_mesh_handler.cpp             |                21
pattern_integration_test.cpp      |                 1
mock_cuda_wrappers.cpp            |                 7
mock_trace_span.cpp               |                 8
parser.h                          |                 2
unique_ptr.h                      |                 4
component_bridge.h                |                 2
device.h                          |                 2
types.h                           |                25
types_base.h                      |                 3
array.h                           |                 1
atomic_ops.h                      |                 1
defines.h                         |                 3
image_impl.h                      |                 3
atomic_ops_ext.h                  |                 1
atomic_ops_unix.h                 |                 1
atomic_ops_utils.h                |                 3
math_base.h                       |                 8
math_float4.h                     |                 2
optimization.h                    |                 1
progress.h                        |                 1
simd.h                            |                 9
utility.h                         |                 1
http_parser_merged.h              |                38
control.h                         |                 1
core.h                            |                 3
impl-client.h                     |                 3
impl-core.h                       |                 2
impl-device.h                     |                 2
impl-factory.h                    |                 2
impl-link.h                       |                 1
impl-metadata.h                   |                 2
impl-node.h                       |                 1
impl-port.h                       |                 1
impl.h                            |                 2
properties.h                      |                 2
protocol.h                        |                 2
utils.h                           |                 2
buffer.h                          |                 1
meta.h                            |                 1
format-utils.h                    |                 2
buffers.h                         |                 1
port-config.h                     |                 1
profile.h                         |                 1
route.h                           |                 1
builder.h                         |                 3
iter.h                            |                 2
cleanup.h                         |                 7
json-core.h                       |                30
string.h                          |                 3
type.h                            |                 5
cycles.h                          |                22
hash.h                            |                 1
prefer.hpp                        |                 2
require.hpp                       |                 2
associated_allocator.hpp          |                 1
associated_cancellation_slot.hpp  |                 1
associated_executor.hpp           |                 1
associated_immediate_executor.hpp |                 2
bulk_execute.hpp                  |                 3
connect.hpp                       |                 3
execute.hpp                       |                 2
executor.hpp                      |                 1
schedule.hpp                      |                 2
sender.hpp                        |                 1
set_done.hpp                      |                 1
set_error.hpp                     |                 1
set_value.hpp                     |                 1
start.hpp                         |                 1
submit.hpp                        |                 3
executor_work_guard.hpp           |                 1
prefer.hpp                        |                 8
query.hpp                         |                 3
require.hpp                       |                 5
require_concept.hpp               |                 3
TinySHA1.hpp                      |                 2
http_connection.h                 |                11
http_parser_merged.h              |                 6
http_request.h                    |                 2
http_response.h                   |                 1
http_server.h                     |                 7
json.h                            |                14
logging.h                         |                 1
query_string.h                    |                 3
routing.h                         |                12
socket_adaptors.h                 |                21
task_timer.h                      |                 1
utility.h                         |                 3
websocket.h                       |                18
memory.h                          |                 4
cuda_api.hpp                      |                18
bitfield.inl                      |                 3
common.h                          |                 2
signal_set_service.ipp            |                 1
context_algorithms.hpp            |                 1
app.h                             |                 1
crow_adapter.h                    |                 4
dag_graph.h                       |                 3
dag_graph.cpp                     |                 3
redis_manager.h                   |                 3
node.h                            |                 4
system_hooks.h                    |                 4
redis_manager.cpp                 |                 6
qfh.h                             |                 3
quantum_processor_qfh_common.cpp  |                 7
processor.cpp                     |                 1
kernels.cuh                       |                 6
types_uchar2.h                    |                 7
types_uchar3.h                    |                 7
signal_set_service.ipp            |                 1
oiio_output_driver.cpp            |                 7
manager.h                         |                 1
quantum_coherence_manager.hpp     |                 1
pattern_bridge.h                  |                 1
crow_adapter.cpp                  |                 3
types_serialization.cpp           |                13
raii.cpp                          |                 1
cycles_renderer.h                 |                 1
cuda_helpers.h                    |                 5
memory_tier_manager.hpp           |                 8
quantum_coherence_manager.h       |                 6
spdlog_isolation.h                |                 4
client.h                          |                 5
json_helpers.h                    |                 2
rate_limit_middleware.h           |                 2
metrics_collector.cpp             |                 2
component_bridge.cpp              |                 1
-----------------------------------------------------
----=================----

----======== Summary ========----
------------------------------------------------
Number of processed analyzer result files | 1378
Number of analyzer reports                | 1253
------------------------------------------------
----=================----
[WARNING 2025-07-09 08:11] - The following source file contents changed or missing since the latest analysis:
 - /sep/tests/blender/mock_blender_bridge.cpp
 - /sep/src/quantum/pattern_evolution.cpp
 - /sep/include/quantum/processor.h
 - /sep/tests/blender/pattern_integration_test.cpp
 - /sep/extern/crow/include/crow/json.h
 - /sep/include/quantum/quantum_manifold_optimizer.h
 - /sep/src/blender/mesh_handler.cpp
 - /sep/include/memory/memory_tier.hpp
 - /sep/src/quantum/pattern_processor.cpp
 - /sep/include/memory/quantum_coherence_manager.hpp
 - /sep/include/compat/cufft.h
 - /sep/src/compat/raii.cpp
 - /sep/include/compat/cuda_helpers.h
 - /sep/extern/crow/include/crow/common.h
 - /sep/include/audio/pipewire_includes.h
 - /sep/include/quantum/pattern_evolution_bridge.h
 - /sep/extern/crow/include/crow/task_timer.h
 - /sep/src/api/bridge_c.cpp
 - /sep/tests/blender/mock_mesh_handler.cpp
 - /sep/include/quantum/qfh.h
 - /sep/src/api/crow_adapter.cpp
 - /sep/include/blender/types.h
 - /sep/include/memory/redis_manager.h
 - /sep/src/tests/cycles_test.cpp
 - /sep/include/compat/memory.h
 - /sep/src/quantum/evolution.cpp
 - /sep/extern/cycles/src/device/device.h
 - /sep/include/quantum/data.hpp
 - /sep/include/audio/capture.h
 - /sep/include/compat/cuda_common.h
 - /sep/src/blender/oiio_output_driver.cpp
 - /sep/include/compat/component_bridge.h
 - /sep/src/quantum/pattern_evolution_bridge.cpp
 - /sep/src/memory/redis_manager.cpp
 - /sep/tests/blender/mock_trace_span.cpp
 - /sep/_sep/testbed/context_algorithms.hpp
 - /sep/include/memory/quantum_coherence_manager.h
 - /sep/include/crow/asio_isolation.h
 - /sep/tests/blender/mock_pattern_evolution.cpp
 - /sep/include/core/logging.h
 - /sep/include/memory/memory_tier_manager.hpp
 - /sep/tests/blender/mock_pattern_compression.cpp
 - /sep/include/api/sep_engine.h
 - /sep/include/blender/gpu_context.h
 - /sep/extern/crow/include/crow/http_request.h
 - /sep/extern/crow/include/crow/websocket.h
 - /sep/include/quantum/types.h
 - /sep/src/audio/config.cpp
 - /sep/include/core/system_hooks.h
 - /sep/include/crow/crow_isolation.h
 - /sep/src/api/server.cpp
 - /sep/include/crow/logging.h
 - /sep/include/memory/types.h
 - /sep/include/compat/shim.h
 - /sep/include/api/server.h
 - /sep/tests/blender/mocks/mock_mesh_handler.cpp
 - /sep/include/blender/bridge.h
 - /sep/extern/crow/include/crow/http_connection.h
 - /sep/tests/blender/mock_cuda_wrappers.cpp
 - /sep/src/api/sep_engine.cpp
 - /sep/src/blender/blender_integration.cpp
 - /sep/include/api/json_helpers.h
 - /sep/src/memory/memory_tier.cpp
 - /sep/src/compat/component_bridge.cpp
 - /sep/include/compat/cuda_api.hpp
 - /sep/src/core/metrics_collector.cpp
 - /sep/include/api/bridge_internal.hpp
 - /sep/src/memory/memory_tier_manager.cpp
 - /sep/src/api/client.cpp
 - /sep/src/quantum/processor.cpp
 - /sep/include/api/client.h
 - /sep/include/compat/kernels.cuh
 - /sep/src/core/engine.cpp
 - /sep/include/memory/manager.h
 - /sep/src/quantum/quantum_manifold_optimizer.cpp
 - /sep/include/blender/cycles_renderer.h
 - /sep/include/crow/socket_adaptors.h
 - /sep/extern/crow/include/crow/socket_adaptors.h
 - /sep/src/blender/cycles_renderer.cpp
 - /sep/src/quantum/types_serialization.cpp
 - /sep/extern/crow/include/crow/http_server.h
 - /sep/include/api/types.h
 - /sep/include/quantum/quantum_processor_qfh.h
 - /sep/include/memory/spdlog_isolation.h
 - /sep/include/api/rate_limit_middleware.h
 - /sep/include/compat/cuda_impl.h
 - /sep/include/compat/core.h
 - /sep/src/main.cpp
 - /sep/src/memory/quantum_coherence_manager.cpp
 - /sep/include/compat/cuda_runtime.h
 - /sep/include/core/dag_graph.h
 - /sep/include/core/types.h
 - /sep/src/audio/pipewire_capture.cpp
 - /sep/src/api/lock_free_rate_limiter.cpp
 - /sep/include/audio/pipewire_capture.h
 - /sep/extern/crow/include/crow/http_response.h
 - /sep/include/compat/cycles.h
 - /sep/include/blender/pattern_bridge.h
 - /sep/include/api/crow_adapter.h
Please re-analyze your project to update the reports!
