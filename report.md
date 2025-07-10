Found no defects in hip_compat.cpp
Found no defects in tracing.cpp
Found no defects in prometheus_exporter.cpp
Found no defects in simple_embedding_model.cpp
[CRITICAL] /sep/src/workbench_main.cpp:6:10: 'backends/imgui_impl_glfw.h' file not found [clang-diagnostic-error]
#include "backends/imgui_impl_glfw.h"
         ^
  Report hash: 597b6b2df006854b59915f486eb67eef
  Steps:
    1, workbench_main.cpp:6:10: 'backends/imgui_impl_glfw.h' file not found

Found 1 defect(s) in workbench_main.cpp

Found no defects in metrics_collector.cpp
Found no defects in error_handler.cpp
Found no defects in dag_graph.cpp
Found no defects in qbsa.cpp
Found no defects in stream.cpp
Found no defects in allocation_metrics.cpp
Found no defects in qbsa_qfh.cpp
Found no defects in qfh.cpp
Found no defects in manager.cpp
[MEDIUM] /sep/src/core/config_manager_stub.cpp:5:1: 'Impl' defined as a class here but previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
class ConfigManager::Impl {
^
  Report hash: a52f1bed6757a6bdcae97e461dc010a2
  Steps:
    1, manager.h:66:3: did you mean class here?
    2, config_manager_stub.cpp:5:1: 'Impl' defined as a class here but previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 1 defect(s) in config_manager_stub.cpp

Found no defects in logging.cpp
Found no defects in memory_tier_manager_serialization.cpp
[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:367:31: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
                   out_block->coherence, out_block->stability);
                              ^
  Report hash: 37e385ab7388829d43ef552db7143b61
  Notes:
    1, memory_tier_manager.cpp:367:13: ~~~~~~ (fixit)
    2, memory_tier_manager.cpp:367:20: out_block->coherence, out_block->stability); (fixit)
    3, memory_tier_manager.cpp:367:20: ~~~~~~~~~~~^~~~~~~~~ (fixit)
  Steps:
    1, memory_tier_manager.cpp:367:31: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:367:53: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
                   out_block->coherence, out_block->stability);
                                                    ^
  Report hash: 9506b31759ee71ddc2d04f59ed8c691e
  Notes:
    1, memory_tier_manager.cpp:367:13: ~~~~~~ (fixit)
    2, memory_tier_manager.cpp:367:20: out_block->coherence, out_block->stability); (fixit)
    3, memory_tier_manager.cpp:367:42: ~~~~~~~~~~~^~~~~~~~~ (fixit)
  Steps:
    1, memory_tier_manager.cpp:367:53: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in memory_tier_manager.cpp

Found no defects in quantum_processor_qfh.cpp
Found no defects in raii.cpp
[MEDIUM] /sep/src/quantum/evolution.cpp:127:52: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        state.phase += (nextFloat() * 2.0f - 1.0f) * sigma * M_PI; // Add phase mutation
                                                   ^
  Report hash: c5677e825a782969782849e7a3b87e33
  Steps:
    1, evolution.cpp:127:52: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/quantum/evolution.cpp:324:42: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    state.phase += (rnd() * 2.0f - 1.0f) * sigma * M_PI; // Add phase mutation
                                         ^
  Report hash: cad84b4fd50cd0e5a22044d2f4918545
  Steps:
    1, evolution.cpp:324:42: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/quantum/evolution.cpp:376:45: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        pattern.quantum_state.phase = rnd() * 2.0f * M_PI; // Initialize phase
                                            ^
  Report hash: 686c468a0c599aae810685813b0a5284
  Steps:
    1, evolution.cpp:376:45: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in evolution.cpp

Found no defects in crow_error.cpp
Found no defects in manifold_config.cpp
Found no defects in types_serialization.cpp
Found no defects in redis_manager.cpp
[LOW] /sep/src/memory/types.h:28:15: no definition found for 'IRedisManager', but a definition with the same name 'IRedisManager' found in another namespace 'sep::persistence' [bugprone-forward-declaration-namespace]
        class IRedisManager;
              ^
  Report hash: bb6220cfbb10b287bf51f1bcfb170272
  Steps:
    1, redis_manager.h:23:7: a definition of 'IRedisManager' is found here
    2, types.h:28:15: no definition found for 'IRedisManager', but a definition with the same name 'IRedisManager' found in another namespace 'sep::persistence'

Found 1 defect(s) in types.h

[MEDIUM] /sep/src/memory/memory_tier.cpp:700:26: std::move of the variable 'pattern' of the trivially-copyable type '::sep::persistence::PersistentPatternData' has no effect; remove std::move() [performance-move-const-arg]
        m_patterns[id] = std::move(pattern);
                         ^
  Report hash: db3b6b6749836b7ff0214ba208ff5162
  Steps:
    1, memory_tier.cpp:700:26: std::move of the variable 'pattern' of the trivially-copyable type '::sep::persistence::PersistentPatternData' has no effect; remove std::move()

Found 1 defect(s) in memory_tier.cpp

Found no defects in js_integration.cpp
[MEDIUM] /sep/src/compat/memory.h:112:63: unused parameter 'stream' [clang-diagnostic-unused-parameter]
bool copyToDevice(T* dst, const T* src, size_t count, Stream* stream  = nullptr) {
                                                              ^
  Report hash: 03fe062d6c7e2fcf37a2b293d4c6d7a1
  Steps:
    1, memory.h:112:63: unused parameter 'stream'

[MEDIUM] /sep/src/compat/memory.h:124:61: unused parameter 'stream' [clang-diagnostic-unused-parameter]
bool copyToHost(T* dst, const T* src, size_t count, Stream* stream  = nullptr) {
                                                            ^
  Report hash: d7af2a21b214b2470caa64843624d8f9
  Steps:
    1, memory.h:124:61: unused parameter 'stream'

Found 2 defect(s) in memory.h

[MEDIUM] /sep/src/core/engine.cpp:97:3: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  fflush(stdout);
  ^
  Report hash: 6e12c4fd64fc76995ebc3b185ba355a3
  Steps:
    1, engine.cpp:97:3: cast the expression to void to silence this warning
    2, engine.cpp:97:3: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 1 defect(s) in engine.cpp

Found no defects in dataset_loader.cpp
Found no defects in quantum_processor_qfh_common.cpp
Found no defects in lock_free_rate_limiter.cpp
Found no defects in pattern_processor.cpp
Found no defects in pattern_evolution_bridge.cpp
Found no defects in pattern_evolution.cpp
Found no defects in quantum_processor.cpp
Found no defects in rate_limit_middleware.cpp
Found no defects in auth_middleware.cpp
Found no defects in processor.cpp
Found no defects in quantum_manifold_optimizer.cpp
Found no defects in logging_middleware.cpp
Found no defects in factory.cpp
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
    2, protocol.h:26:10: 'context.h' included from here
    3, core.h:77:10: circular header file dependency detected while including 'context.h', please check the include path

[LOW] /usr/include/pipewire-0.3/pipewire/core.h:79:10: circular header file dependency detected while including 'proxy.h', please check the include path [misc-header-include-cycle]
#include <pipewire/proxy.h>
         ^
  Report hash: 82346608a24564d39352cc5913ffbb82
  Steps:
    1, context.h:48:10: 'core.h' included from here
    2, protocol.h:26:10: 'context.h' included from here
    3, proxy.h:90:10: 'protocol.h' included from here
    4, client.h:16:10: 'proxy.h' included from here
    5, core.h:79:10: circular header file dependency detected while including 'proxy.h', please check the include path

[MEDIUM] /usr/include/pipewire-0.3/pipewire/core.h:396:2: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
  vsnprintf(buffer, sizeof(buffer), message, args);
  ^
  Report hash: 0ed97ae05ae09f99abd5bfafd2bde595
  Steps:
    1, core.h:396:2: cast the expression to void to silence this warning
    2, core.h:396:2: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 3 defect(s) in core.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-client.h:66:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: c36587793019ad5530065d0f43f044ca
  Steps:
    1, resource.h:37:10: 'impl-client.h' included from here
    2, impl-core.h:29:10: 'resource.h' included from here
    3, impl.h:25:10: 'impl-core.h' included from here
    4, global.h:39:10: 'impl.h' included from here
    5, pipewire_includes.h:17:10: 'global.h' included from here
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
    3, pipewire_includes.h:17:10: 'global.h' included from here
    4, impl-core.h:27:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-core.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: 98baed36db9cbd1ca48fe18b801bea5f
  Steps:
    1, impl.h:27:10: 'impl-device.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:17:10: 'global.h' included from here
    4, impl-device.h:31:10: circular header file dependency detected while including 'global.h', please check the include path

Found 1 defect(s) in impl-device.h

[LOW] /usr/include/pipewire-0.3/pipewire/impl-factory.h:25:10: circular header file dependency detected while including 'global.h', please check the include path [misc-header-include-cycle]
#include <pipewire/global.h>
         ^
  Report hash: caa7dc7877996ab29c3b85d029b375c4
  Steps:
    1, impl.h:28:10: 'impl-factory.h' included from here
    2, global.h:39:10: 'impl.h' included from here
    3, pipewire_includes.h:17:10: 'global.h' included from here
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
    3, pipewire_includes.h:17:10: 'global.h' included from here
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
    2, pipewire_includes.h:17:10: 'global.h' included from here
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
    2, client.h:13:10: 'param.h' included from here
    3, buffers.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in buffers.h

[LOW] /usr/include/spa-0.2/spa/param/port-config.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: bbf25c04cf6ba9c76091dc26a9c4907c
  Steps:
    1, param.h:77:10: 'port-config.h' included from here
    2, client.h:13:10: 'param.h' included from here
    3, port-config.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in port-config.h

[LOW] /usr/include/spa-0.2/spa/param/profile.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: d543dd0e6a4219f2f281b6f8d2ec2327
  Steps:
    1, param.h:76:10: 'profile.h' included from here
    2, client.h:13:10: 'param.h' included from here
    3, profile.h:17:10: circular header file dependency detected while including 'param.h', please check the include path

Found 1 defect(s) in profile.h

[LOW] /usr/include/spa-0.2/spa/param/route.h:17:10: circular header file dependency detected while including 'param.h', please check the include path [misc-header-include-cycle]
#include <spa/param/param.h>
         ^
  Report hash: de3e7bf38f149ce3fcc8ff8ce5654d20
  Steps:
    1, param.h:78:10: 'route.h' included from here
    2, client.h:13:10: 'param.h' included from here
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

Found no defects in quantum_coherence_manager.cpp
Found no defects in crow_request_adapter.cpp
[MEDIUM] /sep/src/workbench/demos/annealing_demo.cpp:32:30: implicit conversion increases floating-point precision: 'float' to 'typename __gnu_cxx::__promote_2<float, int>::__type' (aka 'double') [clang-diagnostic-double-promotion]
            float invDist6 = 1.0f / std::pow(dist, 6);
                             ^
  Report hash: 145ff45a1fe785c34fd9d4a7e29bef4c
  Steps:
    1, annealing_demo.cpp:32:30: implicit conversion increases floating-point precision: 'float' to 'typename __gnu_cxx::__promote_2<float, int>::__type' (aka 'double')

Found 1 defect(s) in annealing_demo.cpp

Found no defects in bridge.cpp
Found no defects in curl_http_client.cpp
Found no defects in client.cpp
Found no defects in ollama_client.cpp
Found no defects in pipeline.cpp
Found no defects in pattern_processor_interface.cpp
Found no defects in cosmo_demo.cpp
Found no defects in demo_manager.cpp
[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:12:21: unknown type name 'AudioCapture'; did you mean '::sep::audio::AudioCapture'? [clang-diagnostic-error]
    std::unique_ptr<AudioCapture> AudioCapture::create() {
                    ^
  Report hash: 56847568c21961edbf50d9f21e6e6e43
  Notes:
    1, audio_visualizer_simple.cpp:12:21: ::sep::audio::AudioCapture (fixit)
  Steps:
    1, capture.h:15:7: '::sep::audio::AudioCapture' declared here
    2, audio_visualizer_simple.cpp:12:21: unknown type name 'AudioCapture'; did you mean '::sep::audio::AudioCapture'?

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:12:35: use of undeclared identifier 'AudioCapture'; did you mean '::sep::audio::AudioCapture'? [clang-diagnostic-error]
    std::unique_ptr<AudioCapture> AudioCapture::create() {
                                  ^
  Report hash: 4e221ba61111c6a1cf4665a7bcaecfce
  Notes:
    1, audio_visualizer_simple.cpp:12:35: ::sep::audio::AudioCapture (fixit)
  Steps:
    1, capture.h:15:7: '::sep::audio::AudioCapture' declared here
    2, audio_visualizer_simple.cpp:12:35: use of undeclared identifier 'AudioCapture'; did you mean '::sep::audio::AudioCapture'?

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:12:49: cannot define or redeclare 'create' here because namespace 'audio' does not enclose namespace 'AudioCapture' [clang-diagnostic-error]
    std::unique_ptr<AudioCapture> AudioCapture::create() {
                                                ^
  Report hash: 31c34e1f80be66a76f730d51605c085f
  Steps:
    1, audio_visualizer_simple.cpp:12:49: cannot define or redeclare 'create' here because namespace 'audio' does not enclose namespace 'AudioCapture'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:14:41: unknown class name 'AudioCapture'; did you mean '::sep::audio::AudioCapture'? [clang-diagnostic-error]
        class StubAudioCapture : public AudioCapture {
                                        ^
  Report hash: ddb9f99f98cd2206e3b68448c019d859
  Notes:
    1, audio_visualizer_simple.cpp:14:41: ::sep::audio::AudioCapture (fixit)
  Steps:
    1, capture.h:15:7: '::sep::audio::AudioCapture' declared here
    2, audio_visualizer_simple.cpp:14:41: unknown class name 'AudioCapture'; did you mean '::sep::audio::AudioCapture'?

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:16:29: unknown type name 'AudioConfig' [clang-diagnostic-error]
            bool init(const AudioConfig& config) override { return true; }
                            ^
  Report hash: 9adb42ca8e158aa15907e271872cbfa9
  Steps:
    1, audio_visualizer_simple.cpp:16:29: unknown type name 'AudioConfig'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:17:26: only virtual member functions can be marked 'override' [clang-diagnostic-error]
            bool start() override { return true; }
                         ^
  Report hash: 6e961558c38d2313377252097c909f0e
  Steps:
    1, audio_visualizer_simple.cpp:17:26: only virtual member functions can be marked 'override'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:18:25: only virtual member functions can be marked 'override' [clang-diagnostic-error]
            void stop() override {}
                        ^
  Report hash: f2a315955ca453da398e4e65f5e64f35
  Steps:
    1, audio_visualizer_simple.cpp:18:25: only virtual member functions can be marked 'override'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:25:21: unknown type name 'AudioPipeline'; did you mean '::sep::audio::AudioPipeline'? [clang-diagnostic-error]
    std::unique_ptr<AudioPipeline> AudioPipeline::create(int spectrum_size) {
                    ^
  Report hash: 098281f4df7bb87383cf582968ac0b83
  Notes:
    1, audio_visualizer_simple.cpp:25:21: ::sep::audio::AudioPipeline (fixit)
  Steps:
    1, pipeline.h:29:21: '::sep::audio::AudioPipeline' declared here
    2, audio_visualizer_simple.cpp:25:21: unknown type name 'AudioPipeline'; did you mean '::sep::audio::AudioPipeline'?

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:25:36: use of undeclared identifier 'AudioPipeline'; did you mean '::sep::audio::AudioPipeline'? [clang-diagnostic-error]
    std::unique_ptr<AudioPipeline> AudioPipeline::create(int spectrum_size) {
                                   ^
  Report hash: 8d10812104459be18e010018fa91e180
  Notes:
    1, audio_visualizer_simple.cpp:25:36: ::sep::audio::AudioPipeline (fixit)
  Steps:
    1, pipeline.h:29:21: '::sep::audio::AudioPipeline' declared here
    2, audio_visualizer_simple.cpp:25:36: use of undeclared identifier 'AudioPipeline'; did you mean '::sep::audio::AudioPipeline'?

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:25:51: cannot define or redeclare 'create' here because namespace 'audio' does not enclose namespace 'AudioPipeline' [clang-diagnostic-error]
    std::unique_ptr<AudioPipeline> AudioPipeline::create(int spectrum_size) {
                                                  ^
  Report hash: 1eb3a9a17d56577891bf106b7f9685c5
  Steps:
    1, audio_visualizer_simple.cpp:25:51: cannot define or redeclare 'create' here because namespace 'audio' does not enclose namespace 'AudioPipeline'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:27:42: unknown class name 'AudioPipeline'; did you mean '::sep::audio::AudioPipeline'? [clang-diagnostic-error]
        class StubAudioPipeline : public AudioPipeline {
                                         ^
  Report hash: f5bf4759598d831a81b42eb2e8340a62
  Notes:
    1, audio_visualizer_simple.cpp:27:42: ::sep::audio::AudioPipeline (fixit)
  Steps:
    1, pipeline.h:29:21: '::sep::audio::AudioPipeline' declared here
    2, audio_visualizer_simple.cpp:27:42: unknown class name 'AudioPipeline'; did you mean '::sep::audio::AudioPipeline'?

[MEDIUM] /sep/src/workbench/demos/audio_visualizer_simple.cpp:31:59: unused parameter 'samples' [clang-diagnostic-unused-parameter]
            void processSamples(const std::vector<float>& samples) override {
                                                          ^
  Report hash: b69a620494a229b4b1ace8cfcdcb6042
  Steps:
    1, audio_visualizer_simple.cpp:31:59: unused parameter 'samples'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:31:68: only virtual member functions can be marked 'override' [clang-diagnostic-error]
            void processSamples(const std::vector<float>& samples) override {
                                                                   ^
  Report hash: 3f7c8e25dc37afb2d7a21de3e95f1db1
  Steps:
    1, audio_visualizer_simple.cpp:31:68: only virtual member functions can be marked 'override'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:45:62: only virtual member functions can be marked 'override' [clang-diagnostic-error]
            std::vector<glm::vec3> getVisualPatterns() const override {
                                                             ^
  Report hash: d6b9ae430936877a002f79c1b745fdf0
  Steps:
    1, audio_visualizer_simple.cpp:45:62: only virtual member functions can be marked 'override'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:54:52: use of undeclared identifier 'spectrum_size' [clang-diagnostic-error]
        return std::make_unique<StubAudioPipeline>(spectrum_size);
                                                   ^
  Report hash: 863af1fbd5cc97ee775cd4e9e1ce101f
  Steps:
    1, audio_visualizer_simple.cpp:54:52: use of undeclared identifier 'spectrum_size'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:59:27: out-of-line definition of 'init' does not match any declaration in 'sep::workbench::AudioVisualizerDemo' [clang-diagnostic-error]
void AudioVisualizerDemo::init() {
                          ^
  Report hash: dddb709a09070072fd0d98c85c360385
  Steps:
    1, audio_visualizer.hpp:13:7: AudioVisualizerDemo defined here
    2, audio_visualizer_simple.cpp:59:27: out-of-line definition of 'init' does not match any declaration in 'sep::workbench::AudioVisualizerDemo'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:63:23: use of undeclared identifier 'Config' [clang-diagnostic-error]
    const auto& cfg = Config::getInstance().audio_visualizer();
                      ^
  Report hash: a553ef91c1f695fb59cda9c0bb15f16a
  Steps:
    1, audio_visualizer_simple.cpp:63:23: use of undeclared identifier 'Config'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer_simple.cpp:66:23: no member named 'AudioCapture' in namespace 'sep::workbench::audio' [clang-diagnostic-error]
    capture_ = audio::AudioCapture::create();
                      ^
  Report hash: e4e320885754bc47a204eed1f420fff4
  Steps:
    1, audio_visualizer_simple.cpp:66:23: no member named 'AudioCapture' in namespace 'sep::workbench::audio'

[LOW] /sep/src/workbench/demos/audio_visualizer_simple.cpp:138:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (key) {
    ^
  Report hash: 660425c6524ea691df39414a470ea58d
  Steps:
    1, audio_visualizer_simple.cpp:138:5: switching on non-enum value without default case may not cover all cases

Found 19 defect(s) in audio_visualizer_simple.cpp

Found no defects in crow_adapter.cpp
[LOW] /sep/src/crow/common.h:5:10: direct self-inclusion of header file 'common.h' [misc-header-include-cycle]
#include <crow/common.h>
         ^
  Report hash: 4d05be001d84b3afeb8864f287a93dcd
  Steps:
    1, common.h:5:10: direct self-inclusion of header file 'common.h'

Found 1 defect(s) in common.h

[MEDIUM] /sep/src/crow/http_response.h:10:5: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
    struct response;
    ^
  Report hash: 66dd6ae82c650fb3018a7c958a82ca37
  Steps:
    1, crow_isolation.h:91:11: previous use is here
    2, http_response.h:10:5: did you mean class here?
    3, http_response.h:10:5: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 1 defect(s) in http_response.h

[MEDIUM] /sep/src/workbench/demos/annealing_sim.cpp:32:30: implicit conversion increases floating-point precision: 'float' to 'typename __gnu_cxx::__promote_2<float, int>::__type' (aka 'double') [clang-diagnostic-double-promotion]
            float invDist6 = 1.0f / std::pow(dist, 6);
                             ^
  Report hash: b184919ceaf7e8ac15a99df24654ffac
  Steps:
    1, annealing_sim.cpp:32:30: implicit conversion increases floating-point precision: 'float' to 'typename __gnu_cxx::__promote_2<float, int>::__type' (aka 'double')

Found 1 defect(s) in annealing_sim.cpp

[MEDIUM] /sep/src/audio/config.cpp:52:31: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                              ^
  Report hash: 4e63877f53b67c4fbc4445ca607d7ea4
  Steps:
    1, config.cpp:52:31: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:52:39: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                                      ^
  Report hash: 6512e0d023076ff627be170438437f62
  Steps:
    1, config.cpp:52:39: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:52:66: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                                                                 ^
  Report hash: c6d2f61264b32d1de6f0cf1588bf3524
  Steps:
    1, config.cpp:52:66: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:52:113: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                                                                                                                ^
  Report hash: e4cd4b34889d40705121d381bb2d262d
  Steps:
    1, config.cpp:52:113: implicit conversion increases floating-point precision: 'float' to 'double'

Found 4 defect(s) in config.cpp

[CRITICAL] /sep/src/workbench/demos/audio_visualizer.cpp:17:5: unknown type name 'AudioConfig' [clang-diagnostic-error]
    AudioConfig acfg;
    ^
  Report hash: 484aae2bdb0b4d1f675c4589a52dddbd
  Steps:
    1, audio_visualizer.cpp:17:5: unknown type name 'AudioConfig'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer.cpp:23:15: no member named 'setCallback' in 'sep::audio::AudioCapture' [clang-diagnostic-error]
    capture_->setCallback([this](const float* data, size_t len) {
              ^
  Report hash: a1d2cc616b204749c817f18464bcc7cb
  Steps:
    1, audio_visualizer.cpp:23:15: no member named 'setCallback' in 'sep::audio::AudioCapture'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer.cpp:28:15: no member named 'init' in 'sep::audio::AudioCapture' [clang-diagnostic-error]
    capture_->init(acfg);
              ^
  Report hash: 8fcb4e4a3b0ace1edbffd73bd0a12d66
  Steps:
    1, audio_visualizer.cpp:28:15: no member named 'init' in 'sep::audio::AudioCapture'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer.cpp:29:15: no member named 'start' in 'sep::audio::AudioCapture' [clang-diagnostic-error]
    capture_->start();
              ^
  Report hash: a1f1420fae159a128af739781786cea3
  Steps:
    1, audio_visualizer.cpp:29:15: no member named 'start' in 'sep::audio::AudioCapture'

[CRITICAL] /sep/src/workbench/demos/audio_visualizer.cpp:68:19: no member named 'stop' in 'sep::audio::AudioCapture' [clang-diagnostic-error]
        capture_->stop();
                  ^
  Report hash: 531108cce6bdb3931dea64cc57aaa554
  Steps:
    1, audio_visualizer.cpp:68:19: no member named 'stop' in 'sep::audio::AudioCapture'

[LOW] /sep/src/workbench/demos/audio_visualizer.cpp:77:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (key) {
    ^
  Report hash: f1b9a1ded2e0ee7c6494d7541f1eab45
  Steps:
    1, audio_visualizer.cpp:77:5: switching on non-enum value without default case may not cover all cases

Found 6 defect(s) in audio_visualizer.cpp

[MEDIUM] /sep/src/audio/pipewire_capture.cpp:223:9: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
        snprintf(runtime_path, sizeof(runtime_path), "/run/user/%d", uid);
        ^
  Report hash: 0e308df13dda55c08b2861ff936dfe97
  Steps:
    1, pipewire_capture.cpp:223:9: cast the expression to void to silence this warning
    2, pipewire_capture.cpp:223:9: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 1 defect(s) in pipewire_capture.cpp

[MEDIUM] /sep/src/audio/pipewire_capture.h:45:21: private field 'pod_builder_' is not used [clang-diagnostic-unused-private-field]
    spa_pod_builder pod_builder_{};
                    ^
  Report hash: d0182a6fb517cfc232d08869a1078af4
  Steps:
    1, pipewire_capture.h:45:21: private field 'pod_builder_' is not used

[MEDIUM] /sep/src/audio/pipewire_capture.h:46:22: private field 'stream_events_' is not used [clang-diagnostic-unused-private-field]
    pw_stream_events stream_events_{};
                     ^
  Report hash: 98f83c942ea1f2cf984aa08ddd1e2213
  Steps:
    1, pipewire_capture.h:46:22: private field 'stream_events_' is not used

Found 2 defect(s) in pipewire_capture.h

Found no defects in drug_discovery_demo.cpp
Found no defects in digital_physics_demo.cpp
[CRITICAL] /usr/include/glm/gtx/component_wise.hpp:22:3: "GLM: GLM_GTX_component_wise is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it." [clang-diagnostic-error]
#  error "GLM: GLM_GTX_component_wise is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
   ^
  Report hash: 32e2eb68d64f82ca226b68432af56ac9
  Steps:
    1, component_wise.hpp:22:3: "GLM: GLM_GTX_component_wise is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."

Found 1 defect(s) in component_wise.hpp

[CRITICAL] /usr/include/glm/gtx/norm.hpp:23:3: "GLM: GLM_GTX_norm is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it." [clang-diagnostic-error]
#  error "GLM: GLM_GTX_norm is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
   ^
  Report hash: 54764508c4ad9a126f0e619d23fbba85
  Steps:
    1, norm.hpp:23:3: "GLM: GLM_GTX_norm is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."

Found 1 defect(s) in norm.hpp

[CRITICAL] /usr/include/glm/gtx/quaternion.hpp:24:3: "GLM: GLM_GTX_quaternion is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it." [clang-diagnostic-error]
#  error "GLM: GLM_GTX_quaternion is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."
   ^
  Report hash: d07bae92a561ccc2889f82f484daae87
  Steps:
    1, quaternion.hpp:24:3: "GLM: GLM_GTX_quaternion is an experimental extension and may change in the future. Use #define GLM_ENABLE_EXPERIMENTAL before including it, if you really want to use it."

Found 1 defect(s) in quaternion.hpp

[LOW] /sep/src/workbench/renderer.cpp:28:9: 2nd argument 'width' (passed to 'right') looks like it might be swapped with the 4th, 'height' (passed to 'top') [readability-suspicious-call-argument]
        glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
        ^
  Report hash: 6e12c29fdb52a44b0e713e5eebb1bb19
  Steps:
    1, glew.h:1052:23: in the call to 'glOrtho', declared here
    2, renderer.cpp:28:9: 2nd argument 'width' (passed to 'right') looks like it might be swapped with the 4th, 'height' (passed to 'top')

[MEDIUM] /sep/src/workbench/renderer.cpp:74:20: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
        glVertex2f(width / 2 - 100, height / 2 - 100);
                   ^
  Report hash: 33916e9483fc65358d2d78f8385e47bc
  Steps:
    1, renderer.cpp:74:20: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:74:37: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
        glVertex2f(width / 2 - 100, height / 2 - 100);
                                    ^
  Report hash: 7b2b650e74ef932dc86b6f17b3b0f359
  Steps:
    1, renderer.cpp:74:37: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:75:20: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
        glVertex2f(width / 2 + 100, height / 2 - 100);
                   ^
  Report hash: 65e93cf78178a7ef2909924097abec11
  Steps:
    1, renderer.cpp:75:20: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:75:37: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
        glVertex2f(width / 2 + 100, height / 2 - 100);
                                    ^
  Report hash: d8817646f98e67b5ca471c660c400081
  Steps:
    1, renderer.cpp:75:37: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:76:20: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
        glVertex2f(width / 2 + 100, height / 2 + 100);
                   ^
  Report hash: aadf5bf001bbfcda2e4c3fc46a014072
  Steps:
    1, renderer.cpp:76:20: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:76:37: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
        glVertex2f(width / 2 + 100, height / 2 + 100);
                                    ^
  Report hash: b0c9c5f6ba97002f4402f002ca3c8a7c
  Steps:
    1, renderer.cpp:76:37: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:77:20: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
        glVertex2f(width / 2 - 100, height / 2 + 100);
                   ^
  Report hash: 6e8d469302240451ced1d78381aae59b
  Steps:
    1, renderer.cpp:77:20: result of integer division used in a floating point context; possible loss of precision

[MEDIUM] /sep/src/workbench/renderer.cpp:77:37: result of integer division used in a floating point context; possible loss of precision [bugprone-integer-division]
        glVertex2f(width / 2 - 100, height / 2 + 100);
                                    ^
  Report hash: 39738617439108e19f40e31844b27957
  Steps:
    1, renderer.cpp:77:37: result of integer division used in a floating point context; possible loss of precision

Found 9 defect(s) in renderer.cpp

[MEDIUM] /sep/src/workbench/renderer.h:42:12: private field 'vao' is not used [clang-diagnostic-unused-private-field]
    GLuint vao = 0;
           ^
  Report hash: 4e72f892291787c9c03d4f3e5cb3375f
  Steps:
    1, renderer.h:42:12: private field 'vao' is not used

[MEDIUM] /sep/src/workbench/renderer.h:43:12: private field 'vbo' is not used [clang-diagnostic-unused-private-field]
    GLuint vbo = 0;
           ^
  Report hash: 10531eb564683048bf4cad3eb15643ac
  Steps:
    1, renderer.h:43:12: private field 'vbo' is not used

Found 2 defect(s) in renderer.h

[MEDIUM] /sep/src/api/server.cpp:778:53: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
  if (coherence_score < config_.response_modulation.coherence_threshold) {
                                                    ^
  Report hash: 9d1543cea2f85bd11b627de29ba7cef4
  Steps:
    1, server.cpp:778:53: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/api/server.cpp:804:66: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    {"simplified", coherence_score < config_.response_modulation.coherence_threshold}
                                                                 ^
  Report hash: 540e3e8a871eca2a472d3d3394c2a008
  Steps:
    1, server.cpp:804:66: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in server.cpp

[MEDIUM] /sep/src/api/server.h:230:38: private field 'cycles_renderer_' is not used [clang-diagnostic-unused-private-field]
  sep::blender::ccl::CyclesRenderer* cycles_renderer_;
                                     ^
  Report hash: 3a19fccf9675b2d499ff0269f54d5087
  Steps:
    1, server.h:230:38: private field 'cycles_renderer_' is not used

Found 1 defect(s) in server.h

Found no defects in flocking_demo.cpp
[LOW] /sep/src/workbench/demos/genesis_pattern.cpp:109:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (key) {
    ^
  Report hash: b7476eb6b59fa98c05113c44f2363ec8
  Steps:
    1, genesis_pattern.cpp:109:5: switching on non-enum value without default case may not cover all cases

Found 1 defect(s) in genesis_pattern.cpp

Found no defects in sep_engine.cpp
Found no defects in neuro_sim.cpp
Found no defects in neural_demo.cpp
[LOW] /sep/src/workbench/demos/memory_garden.cpp:111:5: switching on non-enum value without default case may not cover all cases [bugprone-switch-missing-default-case]
    switch (key) {
    ^
  Report hash: dd35e159c90e9e1cd5eb82d4b5050f2e
  Steps:
    1, memory_garden.cpp:111:5: switching on non-enum value without default case may not cover all cases

Found 1 defect(s) in memory_garden.cpp

[CRITICAL] /sep/src/workbench/demos/physics_explorer_simple.cpp:34:37: non-const lvalue reference to type 'Pattern' cannot bind to a value of unrelated type 'sep::pattern::PatternData' [clang-diagnostic-error]
            evolution::applyGravity(p, center, 0.02f);
                                    ^
  Report hash: 4d9a93012d70ed8decc9d76cce07180a
  Steps:
    1, evolution.h:68:32: passing argument to parameter 'pattern' here
    2, physics_explorer_simple.cpp:34:37: non-const lvalue reference to type 'Pattern' cannot bind to a value of unrelated type 'sep::pattern::PatternData'

[CRITICAL] /sep/src/workbench/demos/physics_explorer_simple.cpp:35:43: non-const lvalue reference to type 'Pattern' cannot bind to a value of unrelated type 'sep::pattern::PatternData' [clang-diagnostic-error]
            evolution::randomPerturbation(p, 0.01f);
                                          ^
  Report hash: 5f64a09138465017a4e635e5f6932d44
  Steps:
    1, evolution.h:69:38: passing argument to parameter 'pattern' here
    2, physics_explorer_simple.cpp:35:43: non-const lvalue reference to type 'Pattern' cannot bind to a value of unrelated type 'sep::pattern::PatternData'

Found 2 defect(s) in physics_explorer_simple.cpp

Found no defects in bridge_c.cpp
[MEDIUM] /sep/src/crow/socket_adaptors.h:58:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.close(asio_ec);
            ^
  Report hash: 1629e317d5b05cb928c747742385246e
  Steps:
    1, socket_adaptors.h:58:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:58:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.close(asio_ec);
            ^
  Report hash: 091fa2ff02de86b4be59dbc550785154
  Steps:
    1, socket_adaptors.h:58:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:67:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.shutdown(tcp::socket::shutdown_both, asio_ec);
            ^
  Report hash: 69612480ceca2420530f07aad8de41dd
  Steps:
    1, socket_adaptors.h:67:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:67:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.shutdown(tcp::socket::shutdown_both, asio_ec);
            ^
  Report hash: b5222b50184cfb8b08d37b2b99b70afd
  Steps:
    1, socket_adaptors.h:67:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:76:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.shutdown(tcp::socket::shutdown_send, asio_ec);
            ^
  Report hash: 71fa8b3825baf69b7365148888335ea7
  Steps:
    1, socket_adaptors.h:76:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:76:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.shutdown(tcp::socket::shutdown_send, asio_ec);
            ^
  Report hash: 0d7b46c030b7a4b4ead511839acc1607
  Steps:
    1, socket_adaptors.h:76:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:85:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
            socket_.shutdown(tcp::socket::shutdown_receive, asio_ec);
            ^
  Report hash: 6bb1e574668db4158f049115d4b4b701
  Steps:
    1, socket_adaptors.h:85:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/src/crow/socket_adaptors.h:85:13: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
            socket_.shutdown(tcp::socket::shutdown_receive, asio_ec);
            ^
  Report hash: d4d0033e652761b687708956539b1022
  Steps:
    1, socket_adaptors.h:85:13: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 8 defect(s) in socket_adaptors.h

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

[LOW] /sep/src/audio/pipewire_capture.cpp:320:13: Call to blocking function 'fgets' inside of critical section [unix.BlockInCriticalSection]
        if (fgets(service_status, sizeof(service_status), fp) != nullptr) {
            ^
  Report hash: 616432282e756e15fcd5f97426498279
  Macro expansions:
    1, gthr-default.h:791:1: Macro '__GTHREAD_INLINE' expanded to 'inline __attribute__ ((__always_inline__ ))'
    2, gthr-default.h:795:12: Macro '__gthrw_(pthread_mutex_lock)' expanded to 'pthread_mutex_lock '
  Steps:
     1, pipewire_capture.cpp:208:33: Calling constructor for 'lock_guard<std::mutex>'
     2, std_mutex.h:251:7: Entered call from 'PipeWireCapture::init'
     3, std_mutex.h:252:9: Calling 'mutex::lock'
     4, std_mutex.h:112:5: Entered call from constructor for 'lock_guard<std::mutex>'
     5, std_mutex.h:115:17: Calling '__gthread_mutex_lock'
     6, gthr-default.h:791:1: Entered call from 'mutex::lock'
     7, gthr-default.h:795:12: Entering critical section here
     8, std_mutex.h:115:17: Returning from '__gthread_mutex_lock'
     9, std_mutex.h:118:11: Assuming '__e' is 0
    10, std_mutex.h:252:9: Returning from 'mutex::lock'
    11, std_mutex.h:252:9: Entering critical section here
    12, pipewire_capture.cpp:208:33: Returning from constructor for 'lock_guard<std::mutex>'
    13, pipewire_capture.cpp:208:33: Entering critical section here
    14, pipewire_capture.cpp:211:9: Assuming field 'ok' is true
    15, pipewire_capture.cpp:237:9: Assuming field 'loop_' is non-null
    16, pipewire_capture.cpp:246:9: Assuming 'start_err' is >= 0
    17, pipewire_capture.cpp:269:9: Assuming field 'context_' is non-null
    18, pipewire_capture.cpp:287:9: Assuming the condition is false
    19, pipewire_capture.cpp:301:30: Entering loop body
    20, pipewire_capture.cpp:304:13: Assuming the condition is false
    21, pipewire_capture.cpp:320:13: Call to blocking function 'fgets' inside of critical section

[LOW] /sep/src/audio/pipewire_capture.cpp:338:28: Call to blocking function 'fgets' inside of critical section [unix.BlockInCriticalSection]
                    while (fgets(status_buf, sizeof(status_buf), status_fp)) {
                           ^
  Report hash: 0a4eea9e7ffe17ffd7a730f13b29114e
  Macro expansions:
    1, gthr-default.h:791:1: Macro '__GTHREAD_INLINE' expanded to 'inline __attribute__ ((__always_inline__ ))'
    2, gthr-default.h:795:12: Macro '__gthrw_(pthread_mutex_lock)' expanded to 'pthread_mutex_lock '
  Steps:
     1, pipewire_capture.cpp:208:33: Calling constructor for 'lock_guard<std::mutex>'
     2, std_mutex.h:251:7: Entered call from 'PipeWireCapture::init'
     3, std_mutex.h:252:9: Calling 'mutex::lock'
     4, std_mutex.h:112:5: Entered call from constructor for 'lock_guard<std::mutex>'
     5, std_mutex.h:115:17: Calling '__gthread_mutex_lock'
     6, gthr-default.h:791:1: Entered call from 'mutex::lock'
     7, gthr-default.h:795:12: Entering critical section here
     8, std_mutex.h:115:17: Returning from '__gthread_mutex_lock'
     9, std_mutex.h:118:11: Assuming '__e' is 0
    10, std_mutex.h:252:9: Returning from 'mutex::lock'
    11, std_mutex.h:252:9: Entering critical section here
    12, pipewire_capture.cpp:208:33: Returning from constructor for 'lock_guard<std::mutex>'
    13, pipewire_capture.cpp:208:33: Entering critical section here
    14, pipewire_capture.cpp:211:9: Assuming field 'ok' is true
    15, pipewire_capture.cpp:237:9: Assuming field 'loop_' is non-null
    16, pipewire_capture.cpp:246:9: Assuming 'start_err' is >= 0
    17, pipewire_capture.cpp:269:9: Assuming field 'context_' is non-null
    18, pipewire_capture.cpp:287:9: Assuming the condition is false
    19, pipewire_capture.cpp:301:30: Entering loop body
    20, pipewire_capture.cpp:304:13: Assuming the condition is false
    21, pipewire_capture.cpp:331:17: Assuming 'start_result' is not equal to 0
    22, pipewire_capture.cpp:338:28: Call to blocking function 'fgets' inside of critical section

[LOW] /sep/src/audio/pipewire_capture.cpp:357:25: Call to blocking function 'fgets' inside of critical section [unix.BlockInCriticalSection]
                    if (fgets(service_status, sizeof(service_status), check_fp) != nullptr) {
                        ^
  Report hash: 2598cca41cc564f89858bd7d96772cd7
  Macro expansions:
    1, gthr-default.h:791:1: Macro '__GTHREAD_INLINE' expanded to 'inline __attribute__ ((__always_inline__ ))'
    2, gthr-default.h:795:12: Macro '__gthrw_(pthread_mutex_lock)' expanded to 'pthread_mutex_lock '
  Steps:
     1, pipewire_capture.cpp:208:33: Calling constructor for 'lock_guard<std::mutex>'
     2, std_mutex.h:251:7: Entered call from 'PipeWireCapture::init'
     3, std_mutex.h:252:9: Calling 'mutex::lock'
     4, std_mutex.h:112:5: Entered call from constructor for 'lock_guard<std::mutex>'
     5, std_mutex.h:115:17: Calling '__gthread_mutex_lock'
     6, gthr-default.h:791:1: Entered call from 'mutex::lock'
     7, gthr-default.h:795:12: Entering critical section here
     8, std_mutex.h:115:17: Returning from '__gthread_mutex_lock'
     9, std_mutex.h:118:11: Assuming '__e' is 0
    10, std_mutex.h:252:9: Returning from 'mutex::lock'
    11, std_mutex.h:252:9: Entering critical section here
    12, pipewire_capture.cpp:208:33: Returning from constructor for 'lock_guard<std::mutex>'
    13, pipewire_capture.cpp:208:33: Entering critical section here
    14, pipewire_capture.cpp:211:9: Assuming field 'ok' is true
    15, pipewire_capture.cpp:237:9: Assuming field 'loop_' is non-null
    16, pipewire_capture.cpp:246:9: Assuming 'start_err' is >= 0
    17, pipewire_capture.cpp:269:9: Assuming field 'context_' is non-null
    18, pipewire_capture.cpp:287:9: Assuming the condition is false
    19, pipewire_capture.cpp:301:30: Entering loop body
    20, pipewire_capture.cpp:304:13: Assuming the condition is false
    21, pipewire_capture.cpp:331:17: Assuming 'start_result' is equal to 0
    22, pipewire_capture.cpp:352:20: Entering loop body
    23, pipewire_capture.cpp:357:25: Call to blocking function 'fgets' inside of critical section

Found 3 defect(s) in pipewire_capture.cpp

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


----==== Severity Statistics ====----
----------------------------
Severity | Number of reports
----------------------------
CRITICAL |                28
MEDIUM   |                96
LOW      |                36
HIGH     |                 5
----------------------------
----=================----

----==== Checker Statistics ====----
-------------------------------------------------------------------------
Checker name                               | Severity | Number of reports
-------------------------------------------------------------------------
clang-diagnostic-error                     | CRITICAL |                28
clang-diagnostic-mismatched-tags           | MEDIUM   |                 2
clang-diagnostic-double-promotion          | MEDIUM   |                14
bugprone-forward-declaration-namespace     | LOW      |                 1
performance-move-const-arg                 | MEDIUM   |                 1
clang-diagnostic-unused-parameter          | MEDIUM   |                 3
cert-err33-c                               | MEDIUM   |                13
misc-header-include-cycle                  | LOW      |                18
clang-diagnostic-reserved-identifier       | MEDIUM   |                42
bugprone-switch-missing-default-case       | LOW      |                 8
bugprone-sizeof-expression                 | HIGH     |                 2
clang-diagnostic-reserved-macro-identifier | MEDIUM   |                 4
clang-diagnostic-unused-private-field      | MEDIUM   |                 5
readability-suspicious-call-argument       | LOW      |                 1
bugprone-integer-division                  | MEDIUM   |                 8
bugprone-unused-return-value               | MEDIUM   |                 4
deadcode.DeadStores                        | LOW      |                 3
core.CallAndMessage                        | HIGH     |                 2
core.NullDereference                       | HIGH     |                 1
unix.BlockInCriticalSection                | LOW      |                 5
-------------------------------------------------------------------------
----=================----

----==== File Statistics ====----
-----------------------------------------------
File name                   | Number of reports
-----------------------------------------------
workbench_main.cpp          |                 1
config_manager_stub.cpp     |                 1
memory_tier_manager.cpp     |                 2
evolution.cpp               |                 3
types.h                     |                 1
memory_tier.cpp             |                 1
memory.h                    |                 2
engine.cpp                  |                 1
control.h                   |                 1
core.h                      |                 3
impl-client.h               |                 2
impl-core.h                 |                 1
impl-device.h               |                 1
impl-factory.h              |                 1
impl-link.h                 |                 1
impl-metadata.h             |                 1
impl-node.h                 |                 1
impl-port.h                 |                 1
impl.h                      |                 1
properties.h                |                 2
utils.h                     |                 2
buffer.h                    |                 1
meta.h                      |                 1
format-utils.h              |                 2
buffers.h                   |                 1
port-config.h               |                 1
profile.h                   |                 1
route.h                     |                 1
builder.h                   |                 3
iter.h                      |                 2
parser.h                    |                 2
cleanup.h                   |                 7
json-core.h                 |                30
string.h                    |                 3
type.h                      |                 5
annealing_demo.cpp          |                 1
audio_visualizer_simple.cpp |                19
common.h                    |                 1
http_response.h             |                 1
annealing_sim.cpp           |                 1
config.cpp                  |                 4
audio_visualizer.cpp        |                 6
pipewire_capture.cpp        |                 4
pipewire_capture.h          |                 2
component_wise.hpp          |                 1
quaternion.hpp              |                 1
norm.hpp                    |                 1
renderer.cpp                |                 9
renderer.h                  |                 2
server.cpp                  |                 2
server.h                    |                 1
genesis_pattern.cpp         |                 1
memory_garden.cpp           |                 1
physics_explorer_simple.cpp |                 2
socket_adaptors.h           |                 8
bitfield.inl                |                 3
cetintrin.h                 |                 2
signal_set_service.ipp      |                 1
signal_set_service.ipp      |                 1
-----------------------------------------------
----=================----

----======== Summary ========----
-----------------------------------------------
Number of processed analyzer result files | 158
Number of analyzer reports                | 165
-----------------------------------------------
----=================----
