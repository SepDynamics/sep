Found no defects in crow_error.cpp
Found no defects in pipewire_stubs.cpp
Found no defects in pipeline.cpp
Found no defects in js_integration.cpp
[CRITICAL] /sep/include/api/bridge.hpp:15:43: no member named 'context' in namespace 'sep' [clang-diagnostic-error]
nlohmann::json contextToJson(const ::sep::context::Context &context);
                                          ^
  Report hash: aba04f83769830a320cfa9e44d979198
  Steps:
    1, bridge.hpp:15:43: no member named 'context' in namespace 'sep'

[CRITICAL] /sep/include/api/bridge.hpp:16:8: no member named 'context' in namespace 'sep' [clang-diagnostic-error]
::sep::context::Context jsonToContext(const nlohmann::json &json);
       ^
  Report hash: c0fe1e2164a52be428c4b3c89c6315dd
  Steps:
    1, bridge.hpp:16:8: no member named 'context' in namespace 'sep'

[CRITICAL] /sep/include/api/bridge.hpp:17:42: no member named 'context' in namespace 'sep' [clang-diagnostic-error]
nlohmann::json resultToJson(const ::sep::context::CheckResult &result);
                                         ^
  Report hash: 2034ea78cd05b646732d3e9823d9ef15
  Steps:
    1, bridge.hpp:17:42: no member named 'context' in namespace 'sep'

[CRITICAL] /sep/include/api/bridge.hpp:18:8: no member named 'context' in namespace 'sep' [clang-diagnostic-error]
::sep::context::CheckResult jsonToCheckResult(const nlohmann::json &json);
       ^
  Report hash: eb4c9c312ab31ffcf110c413b11dc862
  Steps:
    1, bridge.hpp:18:8: no member named 'context' in namespace 'sep'

[CRITICAL] /sep/include/api/bridge.hpp:26:15: no type named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
  ::sep::api::ErrorCode mapSepError(::sep::api::ErrorCode code);
              ^
  Report hash: 40379116e37d47b309ce0d72ec1d0547
  Steps:
    1, bridge.hpp:26:15: no type named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/include/api/bridge.hpp:26:49: no type named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
  ::sep::api::ErrorCode mapSepError(::sep::api::ErrorCode code);
                                                ^
  Report hash: ee98c5dde4bd701b100e01948815dd4e
  Steps:
    1, bridge.hpp:26:49: no type named 'ErrorCode' in namespace 'sep::api'

Found 6 defect(s) in bridge.hpp

[CRITICAL] /sep/src/api/bridge_c.cpp:4:10: 'bridge_internal.hpp' file not found [clang-diagnostic-error]
#include "bridge_internal.hpp"
         ^
  Report hash: 70878f698f8eb2f42b460e79cd1baf05
  Steps:
    1, bridge_c.cpp:4:10: 'bridge_internal.hpp' file not found

Found 1 defect(s) in bridge_c.cpp

[LOW] /sep/include/blender/compression.h:7:10: circular header file dependency detected while including 'base_types.h', please check the include path [misc-header-include-cycle]
#include "blender/base_types.h"
         ^
  Report hash: c6fef258f05a95d6ca20b17559e22e85
  Steps:
    1, memory_tier.hpp:10:10: 'compression.h' included from here
    2, base_types.h:9:10: 'memory_tier.hpp' included from here
    3, pattern_bridge.h:7:10: 'base_types.h' included from here
    4, compression.h:7:10: circular header file dependency detected while including 'base_types.h', please check the include path

Found 1 defect(s) in compression.h

[CRITICAL] /sep/include/blender/pattern_bridge.h:37:5: unknown type name 'PatternStateEnum'; did you mean 'PatternState'? [clang-diagnostic-error]
    PatternStateEnum state = PatternStateEnum::UNINITIALIZED;
    ^
  Report hash: 9e4bf713f4d172fe27aa9503ded55534
  Notes:
    1, pattern_bridge.h:37:5: PatternState (fixit)
  Steps:
    1, base_types.h:49:8: 'PatternState' declared here
    2, pattern_bridge.h:37:5: unknown type name 'PatternStateEnum'; did you mean 'PatternState'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:37:30: use of undeclared identifier 'PatternStateEnum'; did you mean 'PatternState'? [clang-diagnostic-error]
    PatternStateEnum state = PatternStateEnum::UNINITIALIZED;
                             ^
  Report hash: def64e849402ad6b654008ccacc282ec
  Notes:
    1, pattern_bridge.h:37:30: PatternState (fixit)
  Steps:
    1, base_types.h:49:8: 'PatternState' declared here
    2, pattern_bridge.h:37:30: use of undeclared identifier 'PatternStateEnum'; did you mean 'PatternState'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:37:48: no member named 'UNINITIALIZED' in 'sep::pattern::PatternState' [clang-diagnostic-error]
    PatternStateEnum state = PatternStateEnum::UNINITIALIZED;
                                               ^
  Report hash: 64f35930aeec0a4aaea7b503a178d58b
  Steps:
    1, pattern_bridge.h:37:48: no member named 'UNINITIALIZED' in 'sep::pattern::PatternState'

[CRITICAL] /sep/include/blender/pattern_bridge.h:66:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult init(sep::GPUContext* ctx);
  ^
  Report hash: f0561c7cba173ffc5090745d8a150160
  Notes:
    1, pattern_bridge.h:66:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:66:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:69:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult registerObject(Object* obj, const sep::pattern::PatternConfig& config,
  ^
  Report hash: 68f3417a5cc0e0e120fe10136b48a4dd
  Notes:
    1, pattern_bridge.h:69:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:69:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:71:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult updateObject(sep::pattern::ObjectHandle handle,
  ^
  Report hash: 72825a90f849b6934bc1589fa29c9cfa
  Notes:
    1, pattern_bridge.h:71:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:71:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:73:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult cleanupObject(sep::pattern::ObjectHandle handle);  // Added from bridge.cpp context
  ^
  Report hash: 87235467185e407a8557ae5dd2f22580
  Notes:
    1, pattern_bridge.h:73:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:73:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:75:3: unknown type name 'PatternStateEnum'; did you mean 'PatternState'? [clang-diagnostic-error]
  PatternStateEnum getObjectState(
  ^
  Report hash: c62c4aefb5b85863234b50d203ba92eb
  Notes:
    1, pattern_bridge.h:75:3: PatternState (fixit)
  Steps:
    1, base_types.h:49:8: 'PatternState' declared here
    2, pattern_bridge.h:75:3: unknown type name 'PatternStateEnum'; did you mean 'PatternState'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:82:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult processPatterns();
  ^
  Report hash: fa8394f2f81ed8b960b509e36f61c19c
  Notes:
    1, pattern_bridge.h:82:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:82:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:83:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult startProcessingThread();
  ^
  Report hash: 0fa233f7b6e6128c728bc57f8d0b2715
  Notes:
    1, pattern_bridge.h:83:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:83:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:84:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult stopProcessingThread();
  ^
  Report hash: 6676da4c6b245020debd3138f72a74a7
  Notes:
    1, pattern_bridge.h:84:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:84:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:87:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult syncMemory(MemoryTierEnum tier, bool force);
  ^
  Report hash: d03ebf1fc313e2bfca359e30feaa9429
  Notes:
    1, pattern_bridge.h:87:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:87:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:88:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult promotePatterns(sep::pattern::ObjectHandle handle, MemoryTierEnum target_tier);
  ^
  Report hash: ee238de644642a12bf5626dc054fff36
  Notes:
    1, pattern_bridge.h:88:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:88:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:89:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult syncPatternData(sep::pattern::ObjectHandle handle, bool force);
  ^
  Report hash: 41f55f4fec92491c41e6d8baf528cac2
  Notes:
    1, pattern_bridge.h:89:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:89:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:98:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult processObjectPatterns(sep::pattern::ObjectHandle handle, ObjectState& state);
  ^
  Report hash: 272badd8508afcb400cd76642ef5bcac
  Notes:
    1, pattern_bridge.h:98:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:98:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:99:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult updatePatternMetrics(ObjectState& state);
  ^
  Report hash: d40a08d98603b1bf8d17560271799f86
  Notes:
    1, pattern_bridge.h:99:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:99:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

[CRITICAL] /sep/include/blender/pattern_bridge.h:100:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'? [clang-diagnostic-error]
  ::SEPResult validatePatternCoherence(const ObjectState& state);
  ^
  Report hash: 0b8f4a5e4030dc7b1b2a150f5da170e0
  Notes:
    1, pattern_bridge.h:100:3: SEPResult (fixit)
  Steps:
    1, data.hpp:33:12: 'SEPResult' declared here
    2, pattern_bridge.h:100:3: no type named 'SEPResult' in the global namespace; did you mean simply 'SEPResult'?

Found 17 defect(s) in pattern_bridge.h

[CRITICAL] /sep/include/blender/pattern_observer.h:17:53: unknown type name 'PatternStateEnum'; did you mean 'PatternState'? [clang-diagnostic-error]
    virtual void onStateChange(ObjectHandle handle, PatternStateEnum old_state, PatternStateEnum new_state) = 0;
                                                    ^
  Report hash: eb71671d6a003376739797c10628c55b
  Notes:
    1, pattern_observer.h:17:53: PatternState (fixit)
  Steps:
    1, base_types.h:49:8: 'PatternState' declared here
    2, pattern_observer.h:17:53: unknown type name 'PatternStateEnum'; did you mean 'PatternState'?

[CRITICAL] /sep/include/blender/pattern_observer.h:17:81: unknown type name 'PatternStateEnum'; did you mean 'PatternState'? [clang-diagnostic-error]
    virtual void onStateChange(ObjectHandle handle, PatternStateEnum old_state, PatternStateEnum new_state) = 0;
                                                                                ^
  Report hash: c0f10e8a5c298338d6b8f5f6276a02f7
  Notes:
    1, pattern_observer.h:17:81: PatternState (fixit)
  Steps:
    1, base_types.h:49:8: 'PatternState' declared here
    2, pattern_observer.h:17:81: unknown type name 'PatternStateEnum'; did you mean 'PatternState'?

Found 2 defect(s) in pattern_observer.h

[CRITICAL] /sep/include/blender/bridge.h:24:14: target of using declaration conflicts with declaration already in scope [clang-diagnostic-error]
using ::sep::SEPResult;
             ^
  Report hash: 308ab9a2077ed990307f54e39b1f1c66
  Steps:
    1, common.h:9:12: target of using declaration
    2, data.hpp:33:12: conflicting declaration
    3, bridge.h:24:14: target of using declaration conflicts with declaration already in scope

[CRITICAL] /sep/include/blender/bridge.h:67:5: unknown type name 'PatternStateEnum'; did you mean 'PatternState'? [clang-diagnostic-error]
    PatternStateEnum state;
    ^
  Report hash: ced06f5dbeb06e4cd750937e03c682be
  Notes:
    1, bridge.h:67:5: PatternState (fixit)
  Steps:
    1, base_types.h:49:8: 'PatternState' declared here
    2, bridge.h:67:5: unknown type name 'PatternStateEnum'; did you mean 'PatternState'?

[CRITICAL] /sep/include/blender/bridge.h:94:26: unknown type name 'PatternStateEnum'; did you mean 'PatternState'? [clang-diagnostic-error]
                         PatternStateEnum old_state,
                         ^
  Report hash: 7f7ec7f9af3487344040a70aada3c337
  Notes:
    1, bridge.h:94:26: PatternState (fixit)
  Steps:
    1, base_types.h:49:8: 'PatternState' declared here
    2, bridge.h:94:26: unknown type name 'PatternStateEnum'; did you mean 'PatternState'?

[CRITICAL] /sep/include/blender/bridge.h:95:26: unknown type name 'PatternStateEnum'; did you mean 'PatternState'? [clang-diagnostic-error]
                         PatternStateEnum new_state);
                         ^
  Report hash: ffdca8361da6cc6d137c374b13bec885
  Notes:
    1, bridge.h:95:26: PatternState (fixit)
  Steps:
    1, base_types.h:49:8: 'PatternState' declared here
    2, bridge.h:95:26: unknown type name 'PatternStateEnum'; did you mean 'PatternState'?

[CRITICAL] /sep/include/blender/bridge.h:122:33: no member named 'PatternProcessor' in namespace 'sep::pattern' [clang-diagnostic-error]
  std::unique_ptr<sep::pattern::PatternProcessor> pattern_processor_;
                                ^
  Report hash: e9d5d0d8582a2757e9b4e1532b4849fd
  Steps:
    1, bridge.h:122:33: no member named 'PatternProcessor' in namespace 'sep::pattern'

Found 5 defect(s) in bridge.h

[LOW] /sep/include/blender/compression.h:7:10: circular header file dependency detected while including 'base_types.h', please check the include path [misc-header-include-cycle]
#include "blender/base_types.h"
         ^
  Report hash: c6fef258f05a95d6ca20b17559e22e85
  Steps:
    1, memory_tier.hpp:10:10: 'compression.h' included from here
    2, base_types.h:9:10: 'memory_tier.hpp' included from here
    3, bridge.h:11:10: 'base_types.h' included from here
    4, compression.h:7:10: circular header file dependency detected while including 'base_types.h', please check the include path

Found 1 defect(s) in compression.h

[CRITICAL] /sep/src/blender/api.cpp:12:5: unknown type name 'SEPAudioMetrics' [clang-diagnostic-error]
    SEPAudioMetrics                              audio_metrics{};    // last computed audio metrics
    ^
  Report hash: ae4a297b2fe18bddebb423be007cd8ba
  Steps:
    1, api.cpp:12:5: unknown type name 'SEPAudioMetrics'

[CRITICAL] /sep/src/blender/api.cpp:13:5: unknown type name 'SEPPatternMetrics'; did you mean 'sep::pattern::PatternMetrics'? [clang-diagnostic-error]
    SEPPatternMetrics                            pattern_metrics{};  // last collected pattern metrics
    ^
  Report hash: 29ba1b1e3e5ef95ae8ebcda40a303845
  Notes:
    1, api.cpp:13:5: sep::pattern::PatternMetrics (fixit)
  Steps:
    1, base_types.h:58:8: 'sep::pattern::PatternMetrics' declared here
    2, api.cpp:13:5: unknown type name 'SEPPatternMetrics'; did you mean 'sep::pattern::PatternMetrics'?

[CRITICAL] /sep/src/blender/api.cpp:34:46: use of undeclared identifier 'SEPConfig' [clang-diagnostic-error]
    if (static_cast<size_t>(mesh->totvert) > SEPConfig::getDefault().pattern.max_vertices)
                                             ^
  Report hash: d93a706306990b30f7dda64efd22eb91
  Steps:
    1, api.cpp:34:46: use of undeclared identifier 'SEPConfig'

[CRITICAL] /sep/src/blender/api.cpp:40:12: unknown type name 'SEPResult'; did you mean 'sep::SEPResult'? [clang-diagnostic-error]
extern "C" SEPResult sep_blender_init(GPUContext* gpu_ctx, const SEPConfig* config, SEPBlenderBridge** bridge_out)
           ^
  Report hash: 7379eb8bbaa1f4f26626c0c851677c82
  Notes:
    1, api.cpp:40:12: sep::SEPResult (fixit)
  Steps:
    1, common.h:9:12: 'sep::SEPResult' declared here
    2, api.cpp:40:12: unknown type name 'SEPResult'; did you mean 'sep::SEPResult'?

[CRITICAL] /sep/src/blender/api.cpp:40:39: unknown type name 'GPUContext' [clang-diagnostic-error]
extern "C" SEPResult sep_blender_init(GPUContext* gpu_ctx, const SEPConfig* config, SEPBlenderBridge** bridge_out)
                                      ^
  Report hash: ce1115628829c62ea3ebf6379d03b56c
  Steps:
    1, api.cpp:40:39: unknown type name 'GPUContext'

[CRITICAL] /sep/src/blender/api.cpp:40:66: unknown type name 'SEPConfig' [clang-diagnostic-error]
extern "C" SEPResult sep_blender_init(GPUContext* gpu_ctx, const SEPConfig* config, SEPBlenderBridge** bridge_out)
                                                                 ^
  Report hash: e6f85f27a83a7632c059a7daf0aef9ad
  Steps:
    1, api.cpp:40:66: unknown type name 'SEPConfig'

[CRITICAL] /sep/src/blender/api.cpp:44:16: use of undeclared identifier 'SEPResult'; did you mean 'sep::SEPResult'? [clang-diagnostic-error]
        return SEPResult::INIT_FAILED;
               ^
  Report hash: 952fe47d5873bea87eb17e18e369e4f9
  Notes:
    1, api.cpp:44:16: sep::SEPResult (fixit)
  Steps:
    1, common.h:9:12: 'sep::SEPResult' declared here
    2, api.cpp:44:16: use of undeclared identifier 'SEPResult'; did you mean 'sep::SEPResult'?

[CRITICAL] /sep/src/blender/api.cpp:44:27: no member named 'INIT_FAILED' in 'sep::SEPResult' [clang-diagnostic-error]
        return SEPResult::INIT_FAILED;
                          ^
  Report hash: bebde489b47bd070460fe2adecbabb96
  Steps:
    1, api.cpp:44:27: no member named 'INIT_FAILED' in 'sep::SEPResult'

[CRITICAL] /sep/src/blender/api.cpp:53:16: use of undeclared identifier 'SEPResult'; did you mean 'sep::SEPResult'? [clang-diagnostic-error]
        return SEPResult::ALLOCATION_FAILED;
               ^
  Report hash: 239d2a8fb3ee3105c922bdd29a406307
  Notes:
    1, api.cpp:53:16: sep::SEPResult (fixit)
  Steps:
    1, common.h:9:12: 'sep::SEPResult' declared here
    2, api.cpp:53:16: use of undeclared identifier 'SEPResult'; did you mean 'sep::SEPResult'?

[CRITICAL] /sep/src/blender/api.cpp:56:35: use of undeclared identifier 'SEPAudioMetrics' [clang-diagnostic-error]
    bridge_ptr->audio_metrics   = SEPAudioMetrics{};
                                  ^
  Report hash: 5a45e06e2fe907ea209e5c65414258c4
  Steps:
    1, api.cpp:56:35: use of undeclared identifier 'SEPAudioMetrics'

[CRITICAL] /sep/src/blender/api.cpp:56:50: expected ';' after expression [clang-diagnostic-error]
    bridge_ptr->audio_metrics   = SEPAudioMetrics{};
                                                 ^
  Report hash: 78eddf6c71e204beda94d41503bcc7eb
  Notes:
    1, api.cpp:56:50: ; (fixit)
  Steps:
    1, api.cpp:56:50: expected ';' after expression

[CRITICAL] /sep/src/blender/api.cpp:57:35: use of undeclared identifier 'SEPPatternMetrics' [clang-diagnostic-error]
    bridge_ptr->pattern_metrics = SEPPatternMetrics{};
                                  ^
  Report hash: 4d8580ff7beb25f85be455345647a498
  Steps:
    1, api.cpp:57:35: use of undeclared identifier 'SEPPatternMetrics'

[CRITICAL] /sep/src/blender/api.cpp:57:52: expected ';' after expression [clang-diagnostic-error]
    bridge_ptr->pattern_metrics = SEPPatternMetrics{};
                                                   ^
  Report hash: 22b2339193fa76476d5d7b707c44dc65
  Notes:
    1, api.cpp:57:52: ; (fixit)
  Steps:
    1, api.cpp:57:52: expected ';' after expression

[CRITICAL] /sep/src/blender/api.cpp:59:5: unknown type name 'SEPResult'; did you mean 'sep::SEPResult'? [clang-diagnostic-error]
    SEPResult result = bridge_ptr->impl->init(reinterpret_cast<sep::GPUContext*>(gpu_ctx));
    ^
  Report hash: db3b00e840ee7555de66a05b4c767281
  Notes:
    1, api.cpp:59:5: sep::SEPResult (fixit)
  Steps:
    1, common.h:9:12: 'sep::SEPResult' declared here
    2, api.cpp:59:5: unknown type name 'SEPResult'; did you mean 'sep::SEPResult'?

Found 14 defect(s) in api.cpp

[CRITICAL] /sep/src/api/bridge.cpp:37:22: no member named 'context' in namespace 'sep' [clang-diagnostic-error]
std::unique_ptr<sep::context::Processor> g_context_processor_bridge;
                     ^
  Report hash: ec6fca3afca248d3668e54e56bfb6020
  Steps:
    1, bridge.cpp:37:22: no member named 'context' in namespace 'sep'

[CRITICAL] /sep/src/api/bridge.cpp:56:11: no type named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
sep::api::ErrorCode mapSepError(sep::api::ErrorCode code) {
          ^
  Report hash: a967de5a0a4186d92f4dbcdacb15e3b7
  Steps:
    1, bridge.cpp:56:11: no type named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:56:43: no type named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
sep::api::ErrorCode mapSepError(sep::api::ErrorCode code) {
                                          ^
  Report hash: ef3d72eb8a880ea5ddde6cadb9be11b1
  Steps:
    1, bridge.cpp:56:43: no type named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:58:20: no member named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
    case sep::api::ErrorCode::InvalidArgument:
                   ^
  Report hash: 0bd4fab6d51d514a87bc12a9a27c6fee
  Steps:
    1, bridge.cpp:58:20: no member named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:59:24: no member named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
      return sep::api::ErrorCode::InvalidParameter;
                       ^
  Report hash: 004d89f5bb537fbeaaa2ff20f6ac355f
  Steps:
    1, bridge.cpp:59:24: no member named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:60:20: no member named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
    case sep::api::ErrorCode::CudaError:
                   ^
  Report hash: 29c0e9e2de59c2d4160026fceb61f027
  Steps:
    1, bridge.cpp:60:20: no member named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:61:20: no member named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
    case sep::api::ErrorCode::ApiError:
                   ^
  Report hash: 2f2f294af0b06ad7a7bee303b13ddc6b
  Steps:
    1, bridge.cpp:61:20: no member named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:62:20: no member named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
    case sep::api::ErrorCode::InvalidOperation:
                   ^
  Report hash: c1eb935e1476a41aad6a080be6ee6c8f
  Steps:
    1, bridge.cpp:62:20: no member named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:63:20: no member named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
    case sep::api::ErrorCode::ResourceNotFound:
                   ^
  Report hash: 5ac652f6e9e8770adfc064675d5346c8
  Steps:
    1, bridge.cpp:63:20: no member named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:64:20: no member named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
    case sep::api::ErrorCode::OutOfMemory:
                   ^
  Report hash: 6f062a4021778679091771f891183005
  Steps:
    1, bridge.cpp:64:20: no member named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:65:20: no member named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
    case sep::api::ErrorCode::InvalidState:
                   ^
  Report hash: e2e1e4550891b02a730bb9ba4892c906
  Steps:
    1, bridge.cpp:65:20: no member named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:66:20: no member named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
    case sep::api::ErrorCode::SystemError:
                   ^
  Report hash: d9830e6a081f524d5188e7ec6d1201a5
  Steps:
    1, bridge.cpp:66:20: no member named 'ErrorCode' in namespace 'sep::api'

[CRITICAL] /sep/src/api/bridge.cpp:67:20: no member named 'ErrorCode' in namespace 'sep::api' [clang-diagnostic-error]
    case sep::api::ErrorCode::Unknown:
                   ^
  Report hash: a7fc2d4a0898a7817319cdf123664eaf
  Steps:
    1, bridge.cpp:67:20: no member named 'ErrorCode' in namespace 'sep::api'

Found 13 defect(s) in bridge.cpp

Found no defects in ollama_client.cpp
[LOW] /sep/include/api/sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
class Processor;
      ^
  Report hash: 7ddbaf54ea1b7346b97eb4b637f989f0
  Steps:
    1, processor.h:15:7: a definition of 'Processor' is found here
    2, sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in sep_engine.h

[LOW] /sep/include/blender/base_types.h:9:10: circular header file dependency detected while including 'memory_tier.hpp', please check the include path [misc-header-include-cycle]
#include "memory/memory_tier.hpp"
         ^
  Report hash: d2fc400173d22b75362b7481cf70e61f
  Steps:
    1, compression.h:7:10: 'base_types.h' included from here
    2, memory_tier.hpp:10:10: 'compression.h' included from here
    3, memory_tier_manager.hpp:13:10: 'memory_tier.hpp' included from here
    4, base_types.h:9:10: circular header file dependency detected while including 'memory_tier.hpp', please check the include path

Found 1 defect(s) in base_types.h

[CRITICAL] /sep/src/api/sep_engine.cpp:308:56: member access into incomplete type 'sep::pattern::PatternProcessor' [clang-diagnostic-error]
        const auto& patterns = impl_->pattern_processor->getPatterns();
                                                       ^
  Report hash: fb3ec886ae5509c0dfc05670f580a44e
  Steps:
    1, sep_engine.h:27:7: forward declaration of 'sep::pattern::PatternProcessor'
    2, sep_engine.cpp:308:56: member access into incomplete type 'sep::pattern::PatternProcessor'

[MEDIUM] /sep/src/api/sep_engine.cpp:323:67: unused parameter 'request_data' [clang-diagnostic-unused-parameter]
nlohmann::json SepEngine::extractEmbeddings(const nlohmann::json& request_data)
                                                                  ^
  Report hash: d06191f01aa657f541e0410cc16c881a
  Steps:
    1, sep_engine.cpp:323:67: unused parameter 'request_data'

Found 2 defect(s) in sep_engine.cpp

[CRITICAL] /usr/include/c++/15/bits/unique_ptr.h:91:16: invalid application of 'sizeof' to an incomplete type 'sep::pattern::PatternProcessor' [clang-diagnostic-error]
  static_assert(sizeof(_Tp)>0,
                ^
  Report hash: 275649a1731b0dc3c756dbdb02c012dd
  Steps:
    1, unique_ptr.h:399:4: in instantiation of member function 'std::default_delete<sep::pattern::PatternProcessor>::operator()' requested here
    2, sep_engine.cpp:44:29: in instantiation of member function 'std::unique_ptr<sep::pattern::PatternProcessor>::~unique_ptr' requested here
    3, sep_engine.h:27:7: forward declaration of 'sep::pattern::PatternProcessor'
    4, unique_ptr.h:91:16: invalid application of 'sizeof' to an incomplete type 'sep::pattern::PatternProcessor'

[CRITICAL] /usr/include/c++/15/bits/unique_ptr.h:1085:34: allocation of incomplete type 'sep::pattern::PatternProcessor' [clang-diagnostic-error]
    { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }
                                 ^
  Report hash: 1cb9eccadb0327b83d3d05b8b861e31f
  Steps:
    1, sep_engine.cpp:44:34: in instantiation of function template specialization 'std::make_unique<sep::pattern::PatternProcessor>' requested here
    2, sep_engine.h:27:7: forward declaration of 'sep::pattern::PatternProcessor'
    3, unique_ptr.h:1085:34: allocation of incomplete type 'sep::pattern::PatternProcessor'

[CRITICAL] /usr/include/c++/15/bits/unique_ptr.h:1085:34: call to implicitly-deleted default constructor of 'sep::quantum::QuantumProcessor' [clang-diagnostic-error]
    { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }
                                 ^
  Report hash: ef45dca4ce1fd2963df9cfada8936462
  Steps:
    1, sep_engine.cpp:42:34: in instantiation of function template specialization 'std::make_unique<sep::quantum::QuantumProcessor>' requested here
    2, quantum_processor.h:36:5: explicitly defaulted function was implicitly deleted here
    3, quantum_processor.h:21:26: default constructor of 'QuantumProcessor' is implicitly deleted because base class 'Processor' has no default constructor
    4, unique_ptr.h:1085:34: call to implicitly-deleted default constructor of 'sep::quantum::QuantumProcessor'

Found 3 defect(s) in unique_ptr.h

[MEDIUM] /sep/include/api/server.h:23:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
struct request;
^
  Report hash: 7efebc8bbaef775aeab2e28a25dc4a5c
  Steps:
    1, crow_isolation.h:159:7: previous use is here
    2, server.h:23:1: did you mean class here?
    3, server.h:23:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/server.h:24:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
struct response;
^
  Report hash: 59bfeb4242018ecdefde0c5a8f41634e
  Steps:
    1, crow_isolation.h:174:7: previous use is here
    2, server.h:24:1: did you mean class here?
    3, server.h:24:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in server.h

[MEDIUM] /sep/third_party/crow/crow_isolation.h:290:27: unused parameter 'io_service' [clang-diagnostic-unused-parameter]
    tcp(asio::io_context& io_service) {}
                          ^
  Report hash: d32bed3bb60378824ed7efc8ec7e3fd4
  Steps:
    1, crow_isolation.h:290:27: unused parameter 'io_service'

Found 1 defect(s) in crow_isolation.h

[LOW] /sep/third_party/crow/http_parser_merged.h:54:16: no definition found for 'http_parser', but a definition with the same name 'http_parser' found in another namespace 'crow::http_parser' [bugprone-forward-declaration-namespace]
typedef struct http_parser http_parser;
               ^
  Report hash: 4913a4fabdcbe721f3f7b89c678e682b
  Steps:
    1, crow_isolation.h:410:8: a definition of 'http_parser' is found here
    2, http_parser_merged.h:54:16: no definition found for 'http_parser', but a definition with the same name 'http_parser' found in another namespace 'crow::http_parser'

Found 1 defect(s) in http_parser_merged.h

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: 4e3b68afd5a0357226b4fd76d062c89d
  Steps:
    1, common.h:239:8: a definition of '(unnamed struct at /sep/third_party/crow/common.h:239:8)' is found here
    2, strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: 44842604bf14f5e58070227e84913e02
  Steps:
    1, http_parser_merged.h:147:12: a definition of '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' is found here
    2, strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow'

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: 5cd28890e247d60a4a99616ab99775c6
  Steps:
    1, http_request.h:33:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' is found here
    2, strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: 5a120faffb18561a872c83b3fe8c6aff
  Steps:
    1, http_response.h:37:8: a definition of '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' is found here
    2, strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: 62f646d4fef1931eafa69a952fd7a70e
  Steps:
    1, websocket.h:61:8: a definition of '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' is found here
    2, strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket'

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: 635d262db454c98ab9db568f1ed9eb96
  Steps:
    1, common.h:239:8: a definition of '(unnamed struct at /sep/third_party/crow/common.h:239:8)' is found here
    2, strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: feef1efeb0b098f115a8f50e8bacc6f2
  Steps:
    1, http_parser_merged.h:147:12: a definition of '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' is found here
    2, strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow'

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: 234fe07c089925d37ee2347c808e3ab9
  Steps:
    1, http_request.h:33:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' is found here
    2, strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: f75c14113b1a1c5652f61192ed8d5930
  Steps:
    1, http_response.h:37:8: a definition of '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' is found here
    2, strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: 3222ced367491e33f23b55f939f19dc0
  Steps:
    1, websocket.h:61:8: a definition of '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' is found here
    2, strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket'

Found 10 defect(s) in strand_executor_service.hpp

[CRITICAL] /usr/include/asio/detail/type_traits.hpp:21:11: redefinition of 'asio' as different kind of symbol [clang-diagnostic-error]
namespace asio {
          ^
  Report hash: 8ff3f9713f4739e2d02af4b5c3864584
  Steps:
    1, asio_isolation.h:625:11: previous definition is here
    2, type_traits.hpp:21:11: redefinition of 'asio' as different kind of symbol

Found 1 defect(s) in type_traits.hpp

[CRITICAL] /usr/include/asio/execution/executor.hpp:32:11: redefinition of 'asio' as different kind of symbol [clang-diagnostic-error]
namespace asio {
          ^
  Report hash: 4387afef58682735e706e1a1f8bab118
  Steps:
    1, asio_isolation.h:625:11: previous definition is here
    2, executor.hpp:32:11: redefinition of 'asio' as different kind of symbol

[CRITICAL] /usr/include/asio/execution/executor.hpp:39:30: expected class name [clang-diagnostic-error]
struct is_executor_of_impl : false_type
                             ^
  Report hash: a059ab6a282ac2ab925e15dd02b847fe
  Steps:
    1, executor.hpp:39:30: expected class name

[CRITICAL] /usr/include/asio/execution/executor.hpp:46:5: use of undeclared identifier 'traits'; did you mean 'boost::asio::traits'? [clang-diagnostic-error]
    traits::execute_member<add_const_t<T>, F>::is_valid
    ^
  Report hash: f0d2578e2cd9ba37f26bed795acc6288
  Steps:
    1, use_future.hpp:950:11: 'boost::asio::traits' declared here
    2, executor.hpp:46:5: use of undeclared identifier 'traits'; did you mean 'boost::asio::traits'?

[CRITICAL] /usr/include/asio/execution/executor.hpp:46:28: use of undeclared identifier 'add_const_t' [clang-diagnostic-error]
    traits::execute_member<add_const_t<T>, F>::is_valid
                           ^
  Report hash: befaa1577ce99cf4c44a103620e1af6a
  Steps:
    1, executor.hpp:46:28: use of undeclared identifier 'add_const_t'

[CRITICAL] /usr/include/asio/execution/executor.hpp:49:29: expected unqualified-id [clang-diagnostic-error]
    result_of_t<decay_t<F>&()>
                            ^
  Report hash: f1dbfa7f6c3d2a64f25a8c0b7aae3b4c
  Steps:
    1, executor.hpp:49:29: expected unqualified-id

[CRITICAL] /usr/include/asio/execution/executor.hpp:86:34: unknown type name 'invocable_archetype'; did you mean '::asio::execution::invocable_archetype'? [clang-diagnostic-error]
  detail::is_executor_of_impl<T, invocable_archetype>
                                 ^
  Report hash: 107e19bb14586f0fb86a34072972747d
  Steps:
    1, invocable_archetype.hpp:28:8: '::asio::execution::invocable_archetype' declared here
    2, executor.hpp:86:34: unknown type name 'invocable_archetype'; did you mean '::asio::execution::invocable_archetype'?

Found 6 defect(s) in executor.hpp

[CRITICAL] /usr/include/asio/execution/invocable_archetype.hpp:23:11: redefinition of 'asio' as different kind of symbol [clang-diagnostic-error]
namespace asio {
          ^
  Report hash: d67a68ffbfe439eee16c1b057f9a72ab
  Steps:
    1, asio_isolation.h:625:11: previous definition is here
    2, invocable_archetype.hpp:23:11: redefinition of 'asio' as different kind of symbol

Found 1 defect(s) in invocable_archetype.hpp

[CRITICAL] /usr/include/asio/is_applicable_property.hpp:21:11: redefinition of 'asio' as different kind of symbol [clang-diagnostic-error]
namespace asio {
          ^
  Report hash: 9afb6378d72f38a08d0ae159de280c03
  Steps:
    1, asio_isolation.h:625:11: previous definition is here
    2, is_applicable_property.hpp:21:11: redefinition of 'asio' as different kind of symbol

[CRITICAL] /usr/include/asio/is_applicable_property.hpp:25:39: expected class name [clang-diagnostic-error]
struct is_applicable_property_trait : false_type
                                      ^
  Report hash: 8b2b93d4be2813ebd6351d1db0175e59
  Steps:
    1, is_applicable_property.hpp:25:39: expected class name

[CRITICAL] /usr/include/asio/is_applicable_property.hpp:34:5: use of undeclared identifier 'enable_if_t' [clang-diagnostic-error]
    enable_if_t<
    ^
  Report hash: b52c66c88062e806e990f50748c8a6dd
  Steps:
    1, is_applicable_property.hpp:34:5: use of undeclared identifier 'enable_if_t'

[CRITICAL] /usr/include/asio/is_applicable_property.hpp:37:8: expected class name [clang-diagnostic-error]
  >> : true_type
       ^
  Report hash: c39c45c9a70039ec26ced37259f30a5c
  Steps:
    1, is_applicable_property.hpp:37:8: expected class name

Found 4 defect(s) in is_applicable_property.hpp

[LOW] /usr/include/asio/prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 3bea2f3716ee511789f538e829f3d02f
  Steps:
    1, common.h:239:8: a definition of '(unnamed struct at /sep/third_party/crow/common.h:239:8)' is found here
    2, prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 42c643fc70a952d9dacc7538bff98116
  Steps:
    1, http_parser_merged.h:147:12: a definition of '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' is found here
    2, prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow'

[LOW] /usr/include/asio/prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: e7f0a1b7d288117c4552c7fe0ed1f197
  Steps:
    1, http_request.h:33:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' is found here
    2, prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 530df9cd11488dbbe6b07e0e5cd119e2
  Steps:
    1, http_response.h:37:8: a definition of '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' is found here
    2, prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: f13fbee1596ce3620581594b13fc471b
  Steps:
    1, websocket.h:61:8: a definition of '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' is found here
    2, prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket'

[LOW] /usr/include/asio/prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 1e6dcc6cb7aae6c1aa30000187ee9334
  Steps:
    1, common.h:239:8: a definition of '(unnamed struct at /sep/third_party/crow/common.h:239:8)' is found here
    2, prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 661b07bcc9342fb0c8e61aff26bc4611
  Steps:
    1, http_parser_merged.h:147:12: a definition of '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' is found here
    2, prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow'

[LOW] /usr/include/asio/prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 3ee2a047e98c28114578942df4cc709b
  Steps:
    1, http_request.h:33:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' is found here
    2, prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 5ea6b6e4f992820b3ab941faaa589e4e
  Steps:
    1, http_response.h:37:8: a definition of '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' is found here
    2, prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: a8550bff25261ff0c2cf680301650a8e
  Steps:
    1, websocket.h:61:8: a definition of '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' is found here
    2, prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket'

Found 10 defect(s) in prefer.hpp

[LOW] /usr/include/asio/require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 05fc89777efeb8bcd792b4f333663d7b
  Steps:
    1, common.h:239:8: a definition of '(unnamed struct at /sep/third_party/crow/common.h:239:8)' is found here
    2, require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 42c476dc3e93c9847aeaaf1863516909
  Steps:
    1, http_parser_merged.h:147:12: a definition of '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' is found here
    2, require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow'

[LOW] /usr/include/asio/require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 65810a51f164448190fa5b4dca2708bf
  Steps:
    1, http_request.h:33:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' is found here
    2, require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: d08aff15ba46a16e2850c0f98347a011
  Steps:
    1, http_response.h:37:8: a definition of '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' is found here
    2, require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 71e289bb2cb3df5053dcd297872bac57
  Steps:
    1, websocket.h:61:8: a definition of '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' is found here
    2, require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket'

[LOW] /usr/include/asio/require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 9c0ff92bf1110eed89178fac22300758
  Steps:
    1, common.h:239:8: a definition of '(unnamed struct at /sep/third_party/crow/common.h:239:8)' is found here
    2, require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/common.h:239:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 0ab17596ae30fa2e938df10bb0454b45
  Steps:
    1, http_parser_merged.h:147:12: a definition of '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' is found here
    2, require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_parser_merged.h:147:12)' found in another namespace 'crow'

[LOW] /usr/include/asio/require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 6274272e9fe2f4670c2bd98bf57e0c57
  Steps:
    1, http_request.h:33:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' is found here
    2, require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:33:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: af520b5fdcb2c4bc6a2f614bd6130fe8
  Steps:
    1, http_response.h:37:8: a definition of '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' is found here
    2, require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_response.h:37:8)' found in another namespace 'crow'

[LOW] /usr/include/asio/require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: dfc08e1adedfe26eda14f02b3a6d7737
  Steps:
    1, websocket.h:61:8: a definition of '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' is found here
    2, require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/websocket.h:61:8)' found in another namespace 'crow::websocket'

Found 10 defect(s) in require.hpp

[CRITICAL] /usr/include/asio/traits/equality_comparable.hpp:25:11: redefinition of 'asio' as different kind of symbol [clang-diagnostic-error]
namespace asio {
          ^
  Report hash: 7c9f2c3f643f86e184e9df85c5c81cd2
  Steps:
    1, asio_isolation.h:625:11: previous definition is here
    2, equality_comparable.hpp:25:11: redefinition of 'asio' as different kind of symbol

[CRITICAL] /usr/include/asio/traits/equality_comparable.hpp:52:3: use of undeclared identifier 'void_t' [clang-diagnostic-error]
  void_t<
  ^
  Report hash: 4904c17913f420958867b1e2eaee306a
  Steps:
    1, equality_comparable.hpp:52:3: use of undeclared identifier 'void_t'

Found 2 defect(s) in equality_comparable.hpp

[CRITICAL] /usr/include/asio/traits/execute_member.hpp:27:11: redefinition of 'asio' as different kind of symbol [clang-diagnostic-error]
namespace asio {
          ^
  Report hash: 21fd120799ff886ebdb171cf997258a5
  Steps:
    1, asio_isolation.h:625:11: previous definition is here
    2, execute_member.hpp:27:11: redefinition of 'asio' as different kind of symbol

[CRITICAL] /usr/include/asio/traits/execute_member.hpp:54:3: use of undeclared identifier 'void_t' [clang-diagnostic-error]
  void_t<
  ^
  Report hash: 367913ce68acf835af58c0c148f49913
  Steps:
    1, execute_member.hpp:54:3: use of undeclared identifier 'void_t'

Found 2 defect(s) in execute_member.hpp

[CRITICAL] /usr/include/asio/traits/query_static_constexpr_member.hpp:29:11: redefinition of 'asio' as different kind of symbol [clang-diagnostic-error]
namespace asio {
          ^
  Report hash: 19c4496b019394bebe28a8adf8697e50
  Steps:
    1, asio_isolation.h:625:11: previous definition is here
    2, query_static_constexpr_member.hpp:29:11: redefinition of 'asio' as different kind of symbol

[CRITICAL] /usr/include/asio/traits/query_static_constexpr_member.hpp:49:16: use of undeclared identifier 'decay_t' [clang-diagnostic-error]
    is_same<T, decay_t<T>>::value
               ^
  Report hash: f18d7148b5db89425475770b903c3611
  Steps:
    1, query_static_constexpr_member.hpp:49:16: use of undeclared identifier 'decay_t'

[CRITICAL] /usr/include/asio/traits/query_static_constexpr_member.hpp:50:28: use of undeclared identifier 'decay_t' [clang-diagnostic-error]
      && is_same<Property, decay_t<Property>>::value,
                           ^
  Report hash: 37b509b1965962eb2a108e0632d88364
  Steps:
    1, query_static_constexpr_member.hpp:50:28: use of undeclared identifier 'decay_t'

Found 3 defect(s) in query_static_constexpr_member.hpp

[CRITICAL] /sep/src/blender/gpu_context.cpp:6:24: redefinition of 'operator()' [clang-diagnostic-error]
void GPUBufferDeleter::operator()(GPUBuffer* buffer) const noexcept {
                       ^
  Report hash: e6ece2881fb284cea6a1a2e9218cec45
  Steps:
    1, gpu_context.h:84:10: previous definition is here
    2, gpu_context.cpp:6:24: redefinition of 'operator()'

Found 1 defect(s) in gpu_context.cpp

[CRITICAL] /sep/include/core/error_handler.h:16:33: no type named 'Error' in namespace 'sep' [clang-diagnostic-error]
  void reportError(const ::sep::Error &error, std::function<bool()> retry = {});
                                ^
  Report hash: a58eabc0cc9b926aa2d68091f0f16d9c
  Steps:
    1, error_handler.h:16:33: no type named 'Error' in namespace 'sep'

[CRITICAL] /sep/include/core/error_handler.h:18:30: no member named 'Error' in namespace 'sep' [clang-diagnostic-error]
  ::sep::shim::vector<::sep::Error> getErrors() const;
                             ^
  Report hash: 0e6c0a1772a78cab88673118a1c0ffee
  Steps:
    1, error_handler.h:18:30: no member named 'Error' in namespace 'sep'

[CRITICAL] /sep/include/core/error_handler.h:26:12: no type named 'Error' in namespace 'sep' [clang-diagnostic-error]
    ::sep::Error error;
           ^
  Report hash: 4a759be76231f29039bb386baa4e21be
  Steps:
    1, error_handler.h:26:12: no type named 'Error' in namespace 'sep'

Found 3 defect(s) in error_handler.h

[MEDIUM] /sep/src/api/client.cpp:142:47: unused parameter 'request' [clang-diagnostic-unused-parameter]
void Client::updateMetrics( const APIRequest &request,
                                              ^
  Report hash: ba54c1309e522460b22886b6d9677f58
  Steps:
    1, client.cpp:142:47: unused parameter 'request'

Found 1 defect(s) in client.cpp

[LOW] /sep/include/memory/memory_tier.hpp:10:10: circular header file dependency detected while including 'compression.h', please check the include path [misc-header-include-cycle]
#include "blender/compression.h"
         ^
  Report hash: 0c1bd5c4e7eddc4f7662c84a8d7b6ab3
  Steps:
    1, base_types.h:9:10: 'memory_tier.hpp' included from here
    2, compression.h:7:10: 'base_types.h' included from here
    3, compression.cpp:1:10: 'compression.h' included from here
    4, memory_tier.hpp:10:10: circular header file dependency detected while including 'compression.h', please check the include path

[CRITICAL] /sep/include/memory/memory_tier.hpp:48:5: use of undeclared identifier 'blender' [clang-diagnostic-error]
    blender::CompressionMethod compression{blender::CompressionMethod::None};
    ^
  Report hash: ed89e080a32687a7bc77d06d8387e6eb
  Steps:
    1, memory_tier.hpp:48:5: use of undeclared identifier 'blender'

Found 2 defect(s) in memory_tier.hpp

[CRITICAL] /sep/src/main.cpp:1:10: 'config/manager.h' file not found [clang-diagnostic-error]
#include "config/manager.h"
         ^
  Report hash: bd758a45f2ff1b7826d8c6500f11dc6e
  Steps:
    1, main.cpp:1:10: 'config/manager.h' file not found

Found 1 defect(s) in main.cpp

[CRITICAL] /sep/include/compat/cuda_impl.h:240:16: use of undeclared identifier 'cudaErrorInvalidValue' [clang-diagnostic-error]
        return cudaErrorInvalidValue;
               ^
  Report hash: cd08ba146bd420d9d7dcc48ba1f08ee2
  Steps:
    1, cuda_impl.h:240:16: use of undeclared identifier 'cudaErrorInvalidValue'

[CRITICAL] /sep/include/compat/cuda_impl.h:261:16: use of undeclared identifier 'cudaErrorInvalidValue' [clang-diagnostic-error]
        return cudaErrorInvalidValue;
               ^
  Report hash: cd08ba146bd420d9d7dcc48ba1f08ee2
  Steps:
    1, cuda_impl.h:261:16: use of undeclared identifier 'cudaErrorInvalidValue'

[CRITICAL] /sep/include/compat/cuda_impl.h:264:35: use of undeclared identifier 'cudaErrorMemoryAllocation' [clang-diagnostic-error]
    return (*ptr) ? cudaSuccess : cudaErrorMemoryAllocation;
                                  ^
  Report hash: 6c74e8dc2917e746ed7cbfa2edc328ab
  Steps:
    1, cuda_impl.h:264:35: use of undeclared identifier 'cudaErrorMemoryAllocation'

[CRITICAL] /sep/include/compat/cuda_impl.h:275:16: use of undeclared identifier 'cudaErrorInvalidValue' [clang-diagnostic-error]
        return cudaErrorInvalidValue;
               ^
  Report hash: cd08ba146bd420d9d7dcc48ba1f08ee2
  Steps:
    1, cuda_impl.h:275:16: use of undeclared identifier 'cudaErrorInvalidValue'

[CRITICAL] /sep/include/compat/cuda_impl.h:279:16: use of undeclared identifier 'cudaErrorInvalidDevice' [clang-diagnostic-error]
        return cudaErrorInvalidDevice;
               ^
  Report hash: e0ad84f0da4c20247d51bf3fb090d6c3
  Steps:
    1, cuda_impl.h:279:16: use of undeclared identifier 'cudaErrorInvalidDevice'

[CRITICAL] /sep/include/compat/cuda_impl.h:282:22: no member named 'name' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        strcpy(prop->name, "Stub GPU Device");
                     ^
  Report hash: 9c3fced8815ba57207d155d313b68f77
  Steps:
    1, cuda_impl.h:282:22: no member named 'name' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:283:15: no member named 'totalGlobalMem' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->totalGlobalMem = 1024 * 1024 * 1024;  // 1GB
              ^
  Report hash: 645db400b2c39e95687a4ba0517115c7
  Steps:
    1, cuda_impl.h:283:15: no member named 'totalGlobalMem' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:286:15: no member named 'multiProcessorCount' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->multiProcessorCount = 8;
              ^
  Report hash: d71f38e5eaca4a561dd0226081ab32c4
  Steps:
    1, cuda_impl.h:286:15: no member named 'multiProcessorCount' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:287:15: no member named 'maxThreadsPerBlock' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->maxThreadsPerBlock = 1024;
              ^
  Report hash: a9875c22f9420d8c1cd070f46b5ad5c6
  Steps:
    1, cuda_impl.h:287:15: no member named 'maxThreadsPerBlock' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:288:15: no member named 'warpSize' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->warpSize = 32;
              ^
  Report hash: 56aee228839a23e8451307f02229c1a7
  Steps:
    1, cuda_impl.h:288:15: no member named 'warpSize' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:290:15: no member named 'sharedMemPerBlock' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->sharedMemPerBlock = 49152;
              ^
  Report hash: d98044173ed4e15347bf693ed2729702
  Steps:
    1, cuda_impl.h:290:15: no member named 'sharedMemPerBlock' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:291:15: no member named 'regsPerBlock' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->regsPerBlock = 65536;
              ^
  Report hash: 364368a875bdb4eb32abe2a187e1639d
  Steps:
    1, cuda_impl.h:291:15: no member named 'regsPerBlock' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:292:15: no member named 'memPitch' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->memPitch = 2147483647;
              ^
  Report hash: 4694af44a65b48f2144ae216011bb299
  Steps:
    1, cuda_impl.h:292:15: no member named 'memPitch' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:293:15: no member named 'maxThreadsDim' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->maxThreadsDim[0] = 1024;
              ^
  Report hash: be08a12e48338b876c9489497b779c9b
  Steps:
    1, cuda_impl.h:293:15: no member named 'maxThreadsDim' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:294:15: no member named 'maxThreadsDim' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->maxThreadsDim[1] = 1024;
              ^
  Report hash: ba350955d063a2cf06a865b497eacdac
  Steps:
    1, cuda_impl.h:294:15: no member named 'maxThreadsDim' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:295:15: no member named 'maxThreadsDim' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->maxThreadsDim[2] = 64;
              ^
  Report hash: db258745346fab56bc0c3b49013f7182
  Steps:
    1, cuda_impl.h:295:15: no member named 'maxThreadsDim' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:296:15: no member named 'maxGridSize' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->maxGridSize[0] = 65535;
              ^
  Report hash: 261e57aec9ac1c235ff056d34f6b2df8
  Steps:
    1, cuda_impl.h:296:15: no member named 'maxGridSize' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:297:15: no member named 'maxGridSize' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->maxGridSize[1] = 65535;
              ^
  Report hash: fd6f43cdc63888f671dcebe44099fc7f
  Steps:
    1, cuda_impl.h:297:15: no member named 'maxGridSize' in 'cuda_stub_constants::cudaDeviceProp'

[CRITICAL] /sep/include/compat/cuda_impl.h:298:15: no member named 'maxGridSize' in 'cuda_stub_constants::cudaDeviceProp' [clang-diagnostic-error]
        prop->maxGridSize[2] = 65535;
              ^
  Report hash: 3d3ea352c80433cc707a24b20f71d60f
  Steps:
    1, cuda_impl.h:298:15: no member named 'maxGridSize' in 'cuda_stub_constants::cudaDeviceProp'

Found 19 defect(s) in cuda_impl.h

[LOW] /sep/include/compat/stream.h:34:3: class 'Stream' can be made trivially destructible by defaulting the destructor on its first declaration [performance-trivially-destructible]
  ~Stream();
  ^
  Report hash: 31acd9122b83ee6d4d9ac3e51481d6f8
  Notes:
    1, stream.h:34:13: = default (fixit)
  Steps:
    1, stream.cpp:12:9: destructor definition is here
    2, stream.h:34:3: class 'Stream' can be made trivially destructible by defaulting the destructor on its first declaration

Found 1 defect(s) in stream.h

[CRITICAL] /sep/third_party/crow/http_request.h:25:15: no member named 'contains' in 'std::unordered_multimap<std::basic_string<char>, std::basic_string<char>, crow::ci_hash, crow::ci_key_eq>' [clang-diagnostic-error]
  if (headers.contains(key)) {
              ^
  Report hash: e35216a3c457ec75f05b98d2803e2800
  Steps:
    1, http_request.h:90:18: in instantiation of function template specialization 'crow::get_header_value<std::unordered_multimap<std::basic_string<char>, std::basic_string<char>, crow::ci_hash, crow::ci_key_eq>>' requested here
    2, http_request.h:25:15: no member named 'contains' in 'std::unordered_multimap<std::basic_string<char>, std::basic_string<char>, crow::ci_hash, crow::ci_key_eq>'

Found 1 defect(s) in http_request.h

Found no defects in pipewire_capture.cpp
Found no defects in allocation_metrics.cpp
[CRITICAL] /sep/src/blender/pattern_visualization_pipeline.cpp:35:29: no member named 'reloadComputeShaderIfNeeded' in 'sep::GPUContext' [clang-diagnostic-error]
  SEPResult res = gpu_ctx_->reloadComputeShaderIfNeeded();
                            ^
  Report hash: 15b9be366b5253a69e2eb69bbb4b276a
  Steps:
    1, pattern_visualization_pipeline.cpp:35:29: no member named 'reloadComputeShaderIfNeeded' in 'sep::GPUContext'

[CRITICAL] /sep/src/blender/pattern_visualization_pipeline.cpp:38:37: no member named 'getShaderRevision' in 'sep::GPUContext' [clang-diagnostic-error]
  last_shader_revision_ = gpu_ctx_->getShaderRevision();
                                    ^
  Report hash: 268f093118209464428216159f591f25
  Steps:
    1, pattern_visualization_pipeline.cpp:38:37: no member named 'getShaderRevision' in 'sep::GPUContext'

Found 2 defect(s) in pattern_visualization_pipeline.cpp

[LOW] /sep/include/memory/memory_tier.hpp:10:10: circular header file dependency detected while including 'compression.h', please check the include path [misc-header-include-cycle]
#include "blender/compression.h"
         ^
  Report hash: 0c1bd5c4e7eddc4f7662c84a8d7b6ab3
  Steps:
    1, base_types.h:9:10: 'memory_tier.hpp' included from here
    2, compression.h:7:10: 'base_types.h' included from here
    3, compression_utils.cpp:1:10: 'compression.h' included from here
    4, memory_tier.hpp:10:10: circular header file dependency detected while including 'compression.h', please check the include path

Found 1 defect(s) in memory_tier.hpp

Found no defects in config.cpp
Found no defects in prometheus_exporter.cpp
[LOW] /sep/include/compat/cuda_impl.h:27:16: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace '(global)' [bugprone-forward-declaration-namespace]
typedef struct CUstream_st* cudaStream_t;
               ^
  Report hash: e563db1f74d73b5ba7f84616a00506aa
  Steps:
    1, cuda_runtime.h:54:8: a declaration of 'CUstream_st' is found here
    2, cuda_impl.h:27:16: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace '(global)'

[LOW] /sep/include/compat/cuda_impl.h:28:16: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace '(global)' [bugprone-forward-declaration-namespace]
typedef struct CUevent_st* cudaEvent_t;
               ^
  Report hash: 69ce2cb14e5b8887a28860514a45aebe
  Steps:
    1, cuda_runtime.h:55:8: a declaration of 'CUevent_st' is found here
    2, cuda_impl.h:28:16: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace '(global)'

Found 2 defect(s) in cuda_impl.h

[LOW] /sep/include/compat/cuda_runtime.h:48:10: direct self-inclusion of header file 'cuda_runtime.h' [misc-header-include-cycle]
#include "compat/cuda_runtime.h"
         ^
  Report hash: e8be4dd80aece8f9538f66426447f4e7
  Steps:
    1, cuda_runtime.h:48:10: direct self-inclusion of header file 'cuda_runtime.h'

[LOW] /sep/include/compat/cuda_runtime.h:54:8: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
struct CUstream_st;
       ^
  Report hash: d55b7ae2c66d655a2cc9459b2f5ed583
  Steps:
    1, cuda_impl.h:27:16: a declaration of 'CUstream_st' is found here
    2, cuda_runtime.h:54:8: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants'

[LOW] /sep/include/compat/cuda_runtime.h:55:8: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
struct CUevent_st;
       ^
  Report hash: 1ae5ff5ec9e0ef1e7a11e59892cedcf4
  Steps:
    1, cuda_impl.h:28:16: a declaration of 'CUevent_st' is found here
    2, cuda_runtime.h:55:8: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in cuda_runtime.h

Found no defects in tracing.cpp
[CRITICAL] /sep/src/core/dag_graph.cpp:73:19: no member named 'contains' in 'std::unordered_map<unsigned long, sep::dag::DagNode>' [clang-diagnostic-error]
    return nodes_.contains(id);
                  ^
  Report hash: 1a2cbc147b2119557fc18ce0932c89b8
  Steps:
    1, dag_graph.cpp:73:19: no member named 'contains' in 'std::unordered_map<unsigned long, sep::dag::DagNode>'

Found 1 defect(s) in dag_graph.cpp

Found no defects in qbsa.cpp
[CRITICAL] /sep/include/api/rate_limit_middleware.h:24:32: no member named 'config' in namespace 'sep' [clang-diagnostic-error]
    void set_config(const sep::config::RateLimitConfig& config);
                               ^
  Report hash: 02e9196a4653a2db44a5931da0474bbe
  Steps:
    1, rate_limit_middleware.h:24:32: no member named 'config' in namespace 'sep'

[CRITICAL] /sep/include/api/rate_limit_middleware.h:31:10: no member named 'config' in namespace 'sep' [clang-diagnostic-error]
    sep::config::RateLimitConfig  config_;  // Store the config here
         ^
  Report hash: 7ed29b9071b99d1066a607b1e8a9f8d9
  Steps:
    1, rate_limit_middleware.h:31:10: no member named 'config' in namespace 'sep'

Found 2 defect(s) in rate_limit_middleware.h

Found no defects in manager.cpp
Found no defects in qbsa_qfh.cpp
Found no defects in qfh.cpp
[CRITICAL] /sep/include/quantum/evolution.h:53:10: no template named 'unique_ptr' in namespace 'std' [clang-diagnostic-error]
    std::unique_ptr<EvolutionEngineImpl> impl_;
         ^
  Report hash: c77f2c31b98a394b95b28e215f1bf35a
  Steps:
    1, evolution.h:53:10: no template named 'unique_ptr' in namespace 'std'

[MEDIUM] /sep/include/quantum/evolution.h:53:42: private field 'impl_' is not used [clang-diagnostic-unused-private-field]
    std::unique_ptr<EvolutionEngineImpl> impl_;
                                         ^
  Report hash: 44772baa8cd6ecf8449d40de4029a8eb
  Steps:
    1, evolution.h:53:42: private field 'impl_' is not used

Found 2 defect(s) in evolution.h

[CRITICAL] /sep/src/quantum/evolution.cpp:16:24: no member named 'invalid_argument' in namespace 'std' [clang-diagnostic-error]
            throw std::invalid_argument("Processor cannot be null");
                       ^
  Report hash: 0f62dec2da402c4ba600b4c21a39a157
  Steps:
    1, evolution.cpp:16:24: no member named 'invalid_argument' in namespace 'std'

[CRITICAL] /sep/src/quantum/evolution.cpp:264:38: implicit instantiation of undefined template 'std::atomic<unsigned long>' [clang-diagnostic-error]
        static std::atomic<uint64_t> counter{0};
                                     ^
  Report hash: 9d587aa4bebf183b7ca548741cc26132
  Steps:
    1, atomic_base.h:173:12: template is declared here
    2, evolution.cpp:264:38: implicit instantiation of undefined template 'std::atomic<unsigned long>'

[CRITICAL] /sep/src/quantum/evolution.cpp:269:25: no member named 'chrono' in namespace 'std' [clang-diagnostic-error]
        auto now = std::chrono::system_clock::now();
                        ^
  Report hash: abba39b4e33ba05dfbf636e00508c8c5
  Steps:
    1, evolution.cpp:269:25: no member named 'chrono' in namespace 'std'

[CRITICAL] /sep/src/quantum/evolution.cpp:270:21: no member named 'chrono' in namespace 'std' [clang-diagnostic-error]
        return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
                    ^
  Report hash: 948677b65810a2041699f4a4cdc66347
  Steps:
    1, evolution.cpp:270:21: no member named 'chrono' in namespace 'std'

[CRITICAL] /sep/src/quantum/evolution.cpp:270:48: no member named 'chrono' in namespace 'std' [clang-diagnostic-error]
        return std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
                                               ^
  Report hash: e3368f7c940d21d69824fc3d2df6257b
  Steps:
    1, evolution.cpp:270:48: no member named 'chrono' in namespace 'std'

[CRITICAL] /sep/src/quantum/evolution.cpp:283:18: definition of implicitly declared destructor [clang-diagnostic-error]
EvolutionEngine::~EvolutionEngine() = default;
                 ^
  Report hash: 084e60d03a42db6c723b39be360dd2b4
  Steps:
    1, evolution.cpp:283:18: definition of implicitly declared destructor

[CRITICAL] /sep/src/quantum/evolution.cpp:297:1: unknown type name 'EvolutionParams'; did you mean 'EvolutionEngine::EvolutionParams'? [clang-diagnostic-error]
EvolutionParams EvolutionEngine::getParams() const { return impl_->getParams(); }
^
  Report hash: 628b583ff2006f25837512d38bb49fd3
  Notes:
    1, evolution.cpp:297:1: EvolutionEngine::EvolutionParams (fixit)
  Steps:
    1, evolution.h:22:12: 'EvolutionEngine::EvolutionParams' declared here
    2, evolution.cpp:297:1: unknown type name 'EvolutionParams'; did you mean 'EvolutionEngine::EvolutionParams'?

Found 7 defect(s) in evolution.cpp

[MEDIUM] /sep/src/blender/mesh_handler.cpp:306:21: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
      area += 0.5 * std::sqrt(cx * cx + cy * cy + cz * cz);
                    ^
  Report hash: 96149c0ed1849daeb9b92afc0dcb3cc9
  Steps:
    1, mesh_handler.cpp:306:21: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/blender/mesh_handler.cpp:325:14: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    total += std::sqrt(dx * dx + dy * dy + dz * dz);
             ^
  Report hash: 31bfb7cbaa7554f38c92dbc1014c813e
  Steps:
    1, mesh_handler.cpp:325:14: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in mesh_handler.cpp

Found no defects in relationship.cpp
[CRITICAL] /sep/include/compat/shim.h:406:25: no viable overloaded '=' [clang-diagnostic-error]
            new_data[i] = data_[i];
                        ^
  Report hash: 8dc597525e78a0815593b2ce721148ac
  Steps:
    1, shim.h:392:9: in instantiation of member function 'sep::shim::vector<sep::core::ErrorHandler::Entry>::reserve' requested here
    2, error_handler.cpp:15:11: in instantiation of member function 'sep::shim::vector<sep::core::ErrorHandler::Entry>::push_back' requested here
    3, error_handler.h:25:10: candidate function (the implicit move assignment operator) not viable: expects an rvalue for 1st argument
    4, shim.h:406:25: no viable overloaded '='

[CRITICAL] /sep/include/compat/shim.h:464:20: no viable overloaded '=' [clang-diagnostic-error]
          data_[i] = data_[i + 1];
                   ^
  Report hash: 2513a60e1b17bba54ae38d53f4a6c9d1
  Steps:
    1, error_handler.cpp:45:22: in instantiation of member function 'sep::shim::vector<sep::core::ErrorHandler::Entry>::erase' requested here
    2, error_handler.h:25:10: candidate function (the implicit move assignment operator) not viable: expects an rvalue for 1st argument
    3, shim.h:464:20: no viable overloaded '='

Found 2 defect(s) in shim.h

[CRITICAL] /sep/src/core/error_handler.cpp:5:14: no member named 'Error' in namespace 'sep' [clang-diagnostic-error]
using ::sep::Error;
             ^
  Report hash: 2a1fb1e3782bf296dbbdcd8c35be62a2
  Steps:
    1, error_handler.cpp:5:14: no member named 'Error' in namespace 'sep'

[CRITICAL] /sep/src/core/error_handler.cpp:13:38: unknown type name 'Error' [clang-diagnostic-error]
void ErrorHandler::reportError(const Error &error, std::function<bool()> retry) {
                                     ^
  Report hash: 25c67e107ca76c8c73df94ea6ebde505
  Steps:
    1, error_handler.cpp:13:38: unknown type name 'Error'

[CRITICAL] /sep/src/core/error_handler.cpp:19:8: use of undeclared identifier 'Error' [clang-diagnostic-error]
vector<Error> ErrorHandler::getErrors() const {
       ^
  Report hash: a1dfbf31cb5218bf9188224982c2c648
  Steps:
    1, error_handler.cpp:19:8: use of undeclared identifier 'Error'

[CRITICAL] /sep/src/core/error_handler.cpp:21:10: use of undeclared identifier 'Error' [clang-diagnostic-error]
  vector<Error> result;
         ^
  Report hash: 65ed092d7245ce2fa57074e32db69ec2
  Steps:
    1, error_handler.cpp:21:10: use of undeclared identifier 'Error'

Found 4 defect(s) in error_handler.cpp

[LOW] /sep/include/blender/base_types.h:9:10: circular header file dependency detected while including 'memory_tier.hpp', please check the include path [misc-header-include-cycle]
#include "memory/memory_tier.hpp"
         ^
  Report hash: d2fc400173d22b75362b7481cf70e61f
  Steps:
    1, compression.h:7:10: 'base_types.h' included from here
    2, memory_tier.hpp:10:10: 'compression.h' included from here
    3, pattern_processor.h:6:10: 'memory_tier.hpp' included from here
    4, base_types.h:9:10: circular header file dependency detected while including 'memory_tier.hpp', please check the include path

Found 1 defect(s) in base_types.h

[CRITICAL] /sep/include/compat/cuda_impl.h:240:16: cannot initialize return object of type 'cudaError_t' with an lvalue of type 'const cudaError_t' (aka 'const int') [clang-diagnostic-error]
        return cudaErrorInvalidValue;
               ^
  Report hash: 121df4d528f97921944f185dc684974b
  Steps:
    1, cuda_impl.h:240:16: cannot initialize return object of type 'cudaError_t' with an lvalue of type 'const cudaError_t' (aka 'const int')

[CRITICAL] /sep/include/compat/cuda_impl.h:240:16: use of undeclared identifier 'cudaErrorInvalidValue'; did you mean 'sep::cuda::cudaErrorInvalidValue'? [clang-diagnostic-error]
        return cudaErrorInvalidValue;
               ^
  Report hash: b681d73c7396cd63f486ba7f97f7d72d
  Notes:
    1, cuda_impl.h:240:16: sep::cuda::cudaErrorInvalidValue (fixit)
  Steps:
    1, cuda.h:73:26: 'sep::cuda::cudaErrorInvalidValue' declared here
    2, cuda_impl.h:240:16: use of undeclared identifier 'cudaErrorInvalidValue'; did you mean 'sep::cuda::cudaErrorInvalidValue'?

[CRITICAL] /sep/include/compat/cuda_impl.h:261:16: cannot initialize return object of type 'cudaError_t' with an lvalue of type 'const cudaError_t' (aka 'const int') [clang-diagnostic-error]
        return cudaErrorInvalidValue;
               ^
  Report hash: 121df4d528f97921944f185dc684974b
  Steps:
    1, cuda_impl.h:261:16: cannot initialize return object of type 'cudaError_t' with an lvalue of type 'const cudaError_t' (aka 'const int')

[CRITICAL] /sep/include/compat/cuda_impl.h:261:16: use of undeclared identifier 'cudaErrorInvalidValue'; did you mean 'sep::cuda::cudaErrorInvalidValue'? [clang-diagnostic-error]
        return cudaErrorInvalidValue;
               ^
  Report hash: b681d73c7396cd63f486ba7f97f7d72d
  Notes:
    1, cuda_impl.h:261:16: sep::cuda::cudaErrorInvalidValue (fixit)
  Steps:
    1, cuda.h:73:26: 'sep::cuda::cudaErrorInvalidValue' declared here
    2, cuda_impl.h:261:16: use of undeclared identifier 'cudaErrorInvalidValue'; did you mean 'sep::cuda::cudaErrorInvalidValue'?

[CRITICAL] /sep/include/compat/cuda_impl.h:264:12: cannot initialize return object of type 'cudaError_t' with an rvalue of type 'int' [clang-diagnostic-error]
    return (*ptr) ? cudaSuccess : cudaErrorMemoryAllocation;
           ^
  Report hash: f5b990f29085c739ebb7a16b81d39be5
  Steps:
    1, cuda_impl.h:264:12: cannot initialize return object of type 'cudaError_t' with an rvalue of type 'int'

[CRITICAL] /sep/include/compat/cuda_impl.h:264:35: use of undeclared identifier 'cudaErrorMemoryAllocation'; did you mean 'sep::cuda::cudaErrorMemoryAllocation'? [clang-diagnostic-error]
    return (*ptr) ? cudaSuccess : cudaErrorMemoryAllocation;
                                  ^
  Report hash: 5b384f64bfc7bff7a934642a0928e7a1
  Notes:
    1, cuda_impl.h:264:35: sep::cuda::cudaErrorMemoryAllocation (fixit)
  Steps:
    1, cuda.h:69:26: 'sep::cuda::cudaErrorMemoryAllocation' declared here
    2, cuda_impl.h:264:35: use of undeclared identifier 'cudaErrorMemoryAllocation'; did you mean 'sep::cuda::cudaErrorMemoryAllocation'?

[CRITICAL] /sep/include/compat/cuda_impl.h:275:16: cannot initialize return object of type 'cudaError_t' with an lvalue of type 'const cudaError_t' (aka 'const int') [clang-diagnostic-error]
        return cudaErrorInvalidValue;
               ^
  Report hash: 121df4d528f97921944f185dc684974b
  Steps:
    1, cuda_impl.h:275:16: cannot initialize return object of type 'cudaError_t' with an lvalue of type 'const cudaError_t' (aka 'const int')

[CRITICAL] /sep/include/compat/cuda_impl.h:275:16: use of undeclared identifier 'cudaErrorInvalidValue'; did you mean 'sep::cuda::cudaErrorInvalidValue'? [clang-diagnostic-error]
        return cudaErrorInvalidValue;
               ^
  Report hash: b681d73c7396cd63f486ba7f97f7d72d
  Notes:
    1, cuda_impl.h:275:16: sep::cuda::cudaErrorInvalidValue (fixit)
  Steps:
    1, cuda.h:73:26: 'sep::cuda::cudaErrorInvalidValue' declared here
    2, cuda_impl.h:275:16: use of undeclared identifier 'cudaErrorInvalidValue'; did you mean 'sep::cuda::cudaErrorInvalidValue'?

[CRITICAL] /sep/include/compat/cuda_impl.h:279:16: cannot initialize return object of type 'cudaError_t' with an lvalue of type 'const cudaError_t' (aka 'const int') [clang-diagnostic-error]
        return cudaErrorInvalidDevice;
               ^
  Report hash: 97d4466a241740835640d26242bce954
  Steps:
    1, cuda_impl.h:279:16: cannot initialize return object of type 'cudaError_t' with an lvalue of type 'const cudaError_t' (aka 'const int')

[CRITICAL] /sep/include/compat/cuda_impl.h:279:16: use of undeclared identifier 'cudaErrorInvalidDevice'; did you mean 'sep::cuda::cudaErrorInvalidDevice'? [clang-diagnostic-error]
        return cudaErrorInvalidDevice;
               ^
  Report hash: aaefff12d8cd782c1a7b1f523ccc7bae
  Notes:
    1, cuda_impl.h:279:16: sep::cuda::cudaErrorInvalidDevice (fixit)
  Steps:
    1, cuda.h:71:26: 'sep::cuda::cudaErrorInvalidDevice' declared here
    2, cuda_impl.h:279:16: use of undeclared identifier 'cudaErrorInvalidDevice'; did you mean 'sep::cuda::cudaErrorInvalidDevice'?

Found 10 defect(s) in cuda_impl.h

[CRITICAL] /sep/include/compat/stream.h:47:48: no type named 'StreamFlags' in namespace 'sep' [clang-diagnostic-error]
  static std::shared_ptr<Stream> create(::sep::StreamFlags flags = ::sep::StreamFlags::Default);
                                               ^
  Report hash: d42cefdc304f03ce69f392323444b348
  Steps:
    1, stream.h:47:48: no type named 'StreamFlags' in namespace 'sep'

[CRITICAL] /sep/include/compat/stream.h:47:75: no member named 'StreamFlags' in namespace 'sep' [clang-diagnostic-error]
  static std::shared_ptr<Stream> create(::sep::StreamFlags flags = ::sep::StreamFlags::Default);
                                                                          ^
  Report hash: 43b8d31bf1d7e75871228c8f92735978
  Steps:
    1, stream.h:47:75: no member named 'StreamFlags' in namespace 'sep'

Found 2 defect(s) in stream.h

Found no defects in processor.cpp
[LOW] /usr/include/boost/asio/associated_allocator.hpp:91:54: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_allocator.hpp:91:54)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename associator<associated_allocator, T, A>::type
                                                     ^
  Report hash: 70fef44d943cb18b0c56016a1a481040
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, associated_allocator.hpp:91:54: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_allocator.hpp:91:54)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in associated_allocator.hpp

[LOW] /usr/include/boost/asio/associated_cancellation_slot.hpp:91:62: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_cancellation_slot.hpp:91:62)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename associator<associated_cancellation_slot, T, S>::type
                                                             ^
  Report hash: 6c84fd44b7ad7fbdeb4b0b2dd15b7040
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, associated_cancellation_slot.hpp:91:62: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_cancellation_slot.hpp:91:62)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in associated_cancellation_slot.hpp

[LOW] /usr/include/boost/asio/associated_executor.hpp:93:53: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_executor.hpp:93:53)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename associator<associated_executor, T, E>::type
                                                    ^
  Report hash: e2271b31569a015899763cb0ff54e3c8
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, associated_executor.hpp:93:53: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_executor.hpp:93:53)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in associated_executor.hpp

[LOW] /usr/include/boost/asio/associated_immediate_executor.hpp:67:21: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:67:21)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    is_executor<E>::value
                    ^
  Report hash: a1600ab13c4bb287d462f7e14cf59fb8
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, associated_immediate_executor.hpp:67:21: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:67:21)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/associated_immediate_executor.hpp:156:63: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:156:63)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename associator<associated_immediate_executor, T, E>::type
                                                              ^
  Report hash: 541887fb470a17ec884c11045594239d
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, associated_immediate_executor.hpp:156:63: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:156:63)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 2 defect(s) in associated_immediate_executor.hpp

[LOW] /usr/include/boost/asio/execution/bulk_execute.hpp:147:35: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:147:35)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    bulk_execute_member<S, F, N>::is_valid
                                  ^
  Report hash: 4ec7f6121677d56fe04e330c234a4de6
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, bulk_execute.hpp:147:35: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:147:35)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/bulk_execute.hpp:165:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:165:36)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !bulk_execute_member<S, F, N>::is_valid
                                   ^
  Report hash: f674ac7f591f34b2f0db0ea3b2aa39d0
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, bulk_execute.hpp:165:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:165:36)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/bulk_execute.hpp:186:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:186:36)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !bulk_execute_member<S, F, N>::is_valid
                                   ^
  Report hash: e96cf796de2e1b27844b5d69db4ee404
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, bulk_execute.hpp:186:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:186:36)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in bulk_execute.hpp

[LOW] /usr/include/boost/asio/execution/connect.hpp:180:55: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:180:55)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    is_operation_state<typename connect_member<S, R>::result_type>::value
                                                      ^
  Report hash: 423026ea700952e7cfa10b4dea0634fa
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, connect.hpp:180:55: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:180:55)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/connect.hpp:196:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:196:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    connect_free<S, R>::is_valid
                        ^
  Report hash: 006ec4a7c8956163cdb1f214545b78a4
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, connect.hpp:196:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:196:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/connect.hpp:215:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:215:26)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !connect_free<S, R>::is_valid
                         ^
  Report hash: 0d464df3f8e39dd5b041bcbe3d2eab9b
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, connect.hpp:215:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:215:26)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in connect.hpp

[LOW] /usr/include/boost/asio/execution/execute.hpp:145:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:145:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    execute_free<T, F>::is_valid
                        ^
  Report hash: 7d5c95c0f0d5390f77b4c128ab28774c
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, execute.hpp:145:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:145:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/execute.hpp:158:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:158:26)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !execute_free<T, F>::is_valid
                         ^
  Report hash: 56796fb8fa5ebe9f74739a3d97473d7d
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, execute.hpp:158:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:158:26)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 2 defect(s) in execute.hpp

[LOW] /usr/include/boost/asio/execution/executor.hpp:62:43: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/executor.hpp:62:43)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename result_of<typename decay<F>::type&()>::type
                                          ^
  Report hash: 2eeaab853776f0ba8cd75d1bdbc73e06
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, executor.hpp:62:43: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/executor.hpp:62:43)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in executor.hpp

[LOW] /usr/include/boost/asio/execution/schedule.hpp:118:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:118:23)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    schedule_free<S>::is_valid
                      ^
  Report hash: 5bf94ce1b1dc29d0ea3c612ac409ff6c
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, schedule.hpp:118:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:118:23)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/schedule.hpp:131:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:131:24)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !schedule_free<S>::is_valid
                       ^
  Report hash: 72f5e86dea02dd9b01030339e2ab99bd
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, schedule.hpp:131:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:131:24)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 2 defect(s) in schedule.hpp

[LOW] /usr/include/boost/asio/execution/sender.hpp:90:56: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/sender.hpp:90:56)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename has_error_types<S::template error_types>::type,
                                                       ^
  Report hash: a46bdbaeb43903042be2c804a9f59b41
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, sender.hpp:90:56: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/sender.hpp:90:56)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in sender.hpp

[LOW] /usr/include/boost/asio/execution/set_done.hpp:115:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_done.hpp:115:23)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    set_done_free<R>::is_valid
                      ^
  Report hash: 704a00346b3fabbce808c34a20667085
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, set_done.hpp:115:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_done.hpp:115:23)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in set_done.hpp

[LOW] /usr/include/boost/asio/execution/set_error.hpp:115:27: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_error.hpp:115:27)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    set_error_free<R, E>::is_valid
                          ^
  Report hash: 309b7da127fc659ab4fe6d41658e4aef
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, set_error.hpp:115:27: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_error.hpp:115:27)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in set_error.hpp

[LOW] /usr/include/boost/asio/execution/set_value.hpp:118:28: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_value.hpp:118:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    set_value_free<R, Vs>::is_valid
                           ^
  Report hash: 4acb15d71c11afc2847029c9059f47ec
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, set_value.hpp:118:28: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_value.hpp:118:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in set_value.hpp

[LOW] /usr/include/boost/asio/execution/start.hpp:112:20: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/start.hpp:112:20)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    start_free<R>::is_valid
                   ^
  Report hash: 90c32a5d05834d659f3190a942570700
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, start.hpp:112:20: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/start.hpp:112:20)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in start.hpp

[LOW] /usr/include/boost/asio/execution/submit.hpp:148:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:148:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    is_sender_to<S, R>::value
                        ^
  Report hash: 6a7174c92432db41adc0d9f36fc3d937
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, submit.hpp:148:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:148:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/submit.hpp:161:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:161:24)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    submit_free<S, R>::is_valid
                       ^
  Report hash: 1db8948ae610b0f9cc2b92ca65da9cd5
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, submit.hpp:161:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:161:24)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/submit.hpp:177:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:177:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !submit_free<S, R>::is_valid
                        ^
  Report hash: ce178ee755a8021505e73e93992ecb96
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, submit.hpp:177:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:177:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in submit.hpp

[LOW] /usr/include/boost/asio/executor_work_guard.hpp:162:41: no definition found for '(unnamed class at /usr/include/boost/asio/executor_work_guard.hpp:162:41)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
      execution::is_executor<Executor>::value
                                        ^
  Report hash: 07056f6befa6401089813143f2701c29
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, executor_work_guard.hpp:162:41: no definition found for '(unnamed class at /usr/include/boost/asio/executor_work_guard.hpp:162:41)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in executor_work_guard.hpp

[LOW] /usr/include/boost/asio/prefer.hpp:168:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:168:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 2238920b3c290a76c9b423a07a43a6d2
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:168:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:168:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:193:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:193:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 1e6e8afac2bac81734c1c33af9f24479
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:193:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:193:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:215:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:215:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 6e0bbb106931fccd949fbd81d6371ab6
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:215:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:215:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:240:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:240:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: cd5310347573725d62517f5cce2cc410
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:240:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:240:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:268:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:268:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 0c6a666b99a4650e7dbee6903c1d2be1
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:268:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:268:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:299:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:299:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: a4694409cea859df04375695635e3a06
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:299:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:299:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:335:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:335:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 09a1f432e82f49101d8b075d91e59386
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:335:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:335:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:371:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:371:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 9ebf31eabe5216c4d127ab0a3f6e2a53
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:371:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:371:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 8 defect(s) in prefer.hpp

[LOW] /usr/include/boost/asio/query.hpp:142:32: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:142:32)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    static_query<T, Property>::is_valid
                               ^
  Report hash: d366360eda1cfbbd9b488be45557927d
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, query.hpp:142:32: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:142:32)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/query.hpp:158:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:158:33)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !static_query<T, Property>::is_valid
                                ^
  Report hash: fcd5cefdcec8710474a93ec3a72a1f8a
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, query.hpp:158:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:158:33)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/query.hpp:177:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:177:33)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !static_query<T, Property>::is_valid
                                ^
  Report hash: 120f460ca0d0977150693fc7bb6f4609
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, query.hpp:177:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:177:33)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in query.hpp

[LOW] /usr/include/boost/asio/require.hpp:151:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:151:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable
                           ^
  Report hash: 85737c9140cdb8d6add1b9cfde149e21
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, require.hpp:151:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:151:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require.hpp:176:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:176:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable
                           ^
  Report hash: 0d4ffc2c4b1c37d53294ade401836d6a
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, require.hpp:176:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:176:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require.hpp:198:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:198:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable
                           ^
  Report hash: bc2fd662aec4f894c148763a5223b5ab
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, require.hpp:198:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:198:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require.hpp:222:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:222:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: de3a4d17267602ff9fbf9b865639fc8b
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, require.hpp:222:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:222:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require.hpp:258:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:258:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 2f2feda367d72eb391a92b4ec2d263bd
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, require.hpp:258:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:258:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 5 defect(s) in require.hpp

[LOW] /usr/include/boost/asio/require_concept.hpp:149:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:149:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable_concept
                           ^
  Report hash: f5bdaa01caae4fc36e58e46857b32ef7
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, require_concept.hpp:149:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:149:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require_concept.hpp:169:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:169:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable_concept
                           ^
  Report hash: ff4b3bfb307d099918c340b9d0227ad5
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, require_concept.hpp:169:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:169:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require_concept.hpp:197:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:197:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable_concept
                           ^
  Report hash: 8a1289ec2b687e0c263a335fe4cc73e9
  Steps:
    1, cuda_impl.h:30:9: a definition of 'cudaDeviceProp' is found here
    2, require_concept.hpp:197:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:197:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in require_concept.hpp

Found no defects in types_serialization.cpp
[CRITICAL] /sep/src/quantum/processor.cpp:285:61: no member named 'gaussRand' in namespace 'glm' [clang-diagnostic-error]
        state.coherence = glm::clamp(state.coherence + glm::gaussRand(0.0f, config_.mutation_rate), 0.0f, 1.0f);
                                                            ^
  Report hash: 5c62784a77a3fc19bb2cf05194470ff6
  Steps:
    1, processor.cpp:285:61: no member named 'gaussRand' in namespace 'glm'

[CRITICAL] /sep/src/quantum/processor.cpp:286:61: no member named 'gaussRand' in namespace 'glm' [clang-diagnostic-error]
        state.stability = glm::clamp(state.stability + glm::gaussRand(0.0f, config_.mutation_rate * 0.5f), 0.0f, 1.0f);
                                                            ^
  Report hash: 8c68d414f672bf0d65028a883359e2ff
  Steps:
    1, processor.cpp:286:61: no member named 'gaussRand' in namespace 'glm'

[CRITICAL] /sep/src/quantum/processor.cpp:287:57: no member named 'gaussRand' in namespace 'glm' [clang-diagnostic-error]
        state.entropy = glm::clamp(state.entropy + glm::gaussRand(0.0f, config_.mutation_rate * 2.0f), 0.0f, 1.0f);
                                                        ^
  Report hash: 48386e8365724af4920c056ac8771a76
  Steps:
    1, processor.cpp:287:57: no member named 'gaussRand' in namespace 'glm'

[CRITICAL] /sep/src/quantum/processor.cpp:288:45: no member named 'gaussRand' in namespace 'glm' [clang-diagnostic-error]
        state.mutation_rate *= (1.0f + glm::gaussRand(0.0f, 0.1f));
                                            ^
  Report hash: a4655dd4bab7263ab40f05acc663d4ed
  Steps:
    1, processor.cpp:288:45: no member named 'gaussRand' in namespace 'glm'

[CRITICAL] /sep/src/quantum/processor.cpp:322:38: implicit instantiation of undefined template 'std::atomic<unsigned long>' [clang-diagnostic-error]
        static std::atomic<uint64_t> counter{0};
                                     ^
  Report hash: 835eca2a8acfb4f8ceb84dafd76c70ba
  Steps:
    1, atomic_base.h:173:12: template is declared here
    2, processor.cpp:322:38: implicit instantiation of undefined template 'std::atomic<unsigned long>'

[CRITICAL] /sep/src/quantum/processor.cpp:345:22: return type of out-of-line definition of 'sep::quantum::Processor::init' differs from that in the declaration [clang-diagnostic-error]
SEPResult Processor::init(GPUContext* gpu_context) { return impl_->init(gpu_context); }
                     ^
  Report hash: 588b40c3d7103847b6ce764ecd9bf1fe
  Steps:
    1, processor.h:22:15: previous declaration is here
    2, processor.cpp:345:22: return type of out-of-line definition of 'sep::quantum::Processor::init' differs from that in the declaration

[CRITICAL] /sep/src/quantum/processor.cpp:347:22: return type of out-of-line definition of 'sep::quantum::Processor::addPattern' differs from that in the declaration [clang-diagnostic-error]
SEPResult Processor::addPattern(const Pattern& pattern) { return impl_->addPattern(pattern); }
                     ^
  Report hash: b9e36f2ffe3b61055c7620c3ed1cb45b
  Steps:
    1, processor.h:25:15: previous declaration is here
    2, processor.cpp:347:22: return type of out-of-line definition of 'sep::quantum::Processor::addPattern' differs from that in the declaration

[CRITICAL] /sep/src/quantum/processor.cpp:348:22: return type of out-of-line definition of 'sep::quantum::Processor::removePattern' differs from that in the declaration [clang-diagnostic-error]
SEPResult Processor::removePattern(const std::string& pattern_id) { return impl_->removePattern(pattern_id); }
                     ^
  Report hash: f97b6e18809fcb23415fe89f6b79e1fc
  Steps:
    1, processor.h:26:15: previous declaration is here
    2, processor.cpp:348:22: return type of out-of-line definition of 'sep::quantum::Processor::removePattern' differs from that in the declaration

[CRITICAL] /sep/src/quantum/processor.cpp:349:22: return type of out-of-line definition of 'sep::quantum::Processor::updatePattern' differs from that in the declaration [clang-diagnostic-error]
SEPResult Processor::updatePattern(const std::string& pattern_id, const Pattern& pattern) { return impl_->updatePattern(pattern_id, pattern); }
                     ^
  Report hash: b06b88e6a5e79feac81f0b0bbe229191
  Steps:
    1, processor.h:27:15: previous declaration is here
    2, processor.cpp:349:22: return type of out-of-line definition of 'sep::quantum::Processor::updatePattern' differs from that in the declaration

[CRITICAL] /sep/src/quantum/processor.cpp:364:22: return type of out-of-line definition of 'sep::quantum::Processor::addRelationship' differs from that in the declaration [clang-diagnostic-error]
SEPResult Processor::addRelationship(const std::string& pattern_id1, const std::string& pattern_id2, float strength, RelationshipType type) {
                     ^
  Report hash: 0d1ccde5d06c8b64e2fe1cd8b50ae938
  Steps:
    1, processor.h:46:15: previous declaration is here
    2, processor.cpp:364:22: return type of out-of-line definition of 'sep::quantum::Processor::addRelationship' differs from that in the declaration

Found 10 defect(s) in processor.cpp

[LOW] /sep/include/blender/base_types.h:9:10: circular header file dependency detected while including 'memory_tier.hpp', please check the include path [misc-header-include-cycle]
#include "memory/memory_tier.hpp"
         ^
  Report hash: d2fc400173d22b75362b7481cf70e61f
  Steps:
    1, compression.h:7:10: 'base_types.h' included from here
    2, memory_tier.hpp:10:10: 'compression.h' included from here
    3, memory_tier_manager.cpp:6:10: 'memory_tier.hpp' included from here
    4, base_types.h:9:10: circular header file dependency detected while including 'memory_tier.hpp', please check the include path

Found 1 defect(s) in base_types.h

[LOW] /sep/include/memory/memory_tier_manager.hpp:70:5: class 'MemoryTierManager' can be made trivially destructible by defaulting the destructor on its first declaration [performance-trivially-destructible]
    ~MemoryTierManager();
    ^
  Report hash: e6aa05158f1a009ee1d4df23d48cf44e
  Notes:
    1, memory_tier_manager.hpp:70:26: = default (fixit)
  Steps:
    1, memory_tier_manager.cpp:28:20: destructor definition is here
    2, memory_tier_manager.hpp:70:5: class 'MemoryTierManager' can be made trivially destructible by defaulting the destructor on its first declaration

Found 1 defect(s) in memory_tier_manager.hpp

[MEDIUM] /sep/include/compat/cuda_runtime.h:12:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define _GLIBCXX_MATH_H 1
        ^
  Report hash: 4ccf1f1bb2cc6373d7985afcca245c69
  Steps:
    1, cuda_runtime.h:12:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_runtime.h:15:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDACC_RTC__ 1
        ^
  Report hash: 30860bb5b6e354c70e7c2552c64c9a40
  Steps:
    1, cuda_runtime.h:15:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_runtime.h:16:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __MATH_FUNCTIONS_DECL__ inline
        ^
  Report hash: f623cde0597b0ac7a640444958aeee8c
  Steps:
    1, cuda_runtime.h:16:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_runtime.h:17:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __MATH_FUNCTIONS_DEVICE_DECL__ inline
        ^
  Report hash: 153b6bc728348ab92d95e8ea6d09c375
  Steps:
    1, cuda_runtime.h:17:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_runtime.h:21:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDABE__
        ^
  Report hash: 2d57f8d3dc0408e51cf4f95a21c0cd7b
  Steps:
    1, cuda_runtime.h:21:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_runtime.h:26:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDA_ARCH__ 520  // Minimum supported architecture
        ^
  Report hash: 4a2b995644cf657e5ed2763652350c95
  Steps:
    1, cuda_runtime.h:26:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_runtime.h:31:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDA_ARCH_PTX__ 520
        ^
  Report hash: 69a0e458124254e1b6451c31e5197e01
  Steps:
    1, cuda_runtime.h:31:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_runtime.h:36:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDACC_VER_MAJOR__ 12
        ^
  Report hash: 056bd5f2b1a9d597c6cc7282445f396e
  Steps:
    1, cuda_runtime.h:36:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_runtime.h:40:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDACC_VER_MINOR__ 9
        ^
  Report hash: 4372726d2af15c49ffd1f8ea842fc4b4
  Steps:
    1, cuda_runtime.h:40:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_runtime.h:44:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDACC_VER_BUILD__ 0
        ^
  Report hash: d3ff101d30a4df98a06a4d07a1836e14
  Steps:
    1, cuda_runtime.h:44:9: macro name is a reserved identifier

[CRITICAL] /sep/include/compat/cuda_runtime.h:78:13: conflicting types for 'cudaStreamDestroy' [clang-diagnostic-error]
cudaError_t cudaStreamDestroy(cudaStream_t stream);
            ^
  Report hash: ab7a3dac97fdc3e107b6b2097674b6a1
  Steps:
    1, cuda.h:55:13: previous declaration is here
    2, cuda_runtime.h:78:13: conflicting types for 'cudaStreamDestroy'

[CRITICAL] /sep/include/compat/cuda_runtime.h:88:13: conflicting types for 'cudaFree' [clang-diagnostic-error]
cudaError_t cudaFree(void* ptr);
            ^
  Report hash: d041c5fa40445d94a4ec91111b68b131
  Steps:
    1, cuda.h:58:13: previous declaration is here
    2, cuda_runtime.h:88:13: conflicting types for 'cudaFree'

[CRITICAL] /sep/include/compat/cuda_runtime.h:89:13: conflicting types for 'cudaMemGetInfo' [clang-diagnostic-error]
cudaError_t cudaMemGetInfo(size_t* free, size_t* total);
            ^
  Report hash: cfd70cf40d1e19271bf1d131a7ed8c8f
  Steps:
    1, cuda.h:63:13: previous declaration is here
    2, cuda_runtime.h:89:13: conflicting types for 'cudaMemGetInfo'

[CRITICAL] /sep/include/compat/cuda_runtime.h:90:13: conflicting types for 'cudaMemcpyAsync' [clang-diagnostic-error]
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream);
            ^
  Report hash: b6073229254a78923c86a3a12ab7476e
  Steps:
    1, cuda.h:62:13: previous declaration is here
    2, cuda_runtime.h:90:13: conflicting types for 'cudaMemcpyAsync'

[CRITICAL] /sep/include/compat/cuda_runtime.h:94:13: conflicting types for 'cudaSetDevice' [clang-diagnostic-error]
cudaError_t cudaSetDevice(int device);
            ^
  Report hash: 531f094dc23fb59caa5fa85d9a237da6
  Steps:
    1, cuda.h:49:13: previous declaration is here
    2, cuda_runtime.h:94:13: conflicting types for 'cudaSetDevice'

[CRITICAL] /sep/include/compat/cuda_runtime.h:95:13: conflicting types for 'cudaGetDeviceCount' [clang-diagnostic-error]
cudaError_t cudaGetDeviceCount(int* count);
            ^
  Report hash: 705d208caa73f3fda93d18ac64093203
  Steps:
    1, cuda.h:50:13: previous declaration is here
    2, cuda_runtime.h:95:13: conflicting types for 'cudaGetDeviceCount'

[CRITICAL] /sep/include/compat/cuda_runtime.h:96:13: conflicting types for 'cudaGetDeviceProperties' [clang-diagnostic-error]
cudaError_t cudaGetDeviceProperties(struct cudaDeviceProp* prop, int device);
            ^
  Report hash: c570657abe6acafe512070a38e2cc61f
  Steps:
    1, cuda.h:51:13: previous declaration is here
    2, cuda_runtime.h:96:13: conflicting types for 'cudaGetDeviceProperties'

[CRITICAL] /sep/include/compat/cuda_runtime.h:97:13: conflicting types for 'cudaGetLastError' [clang-diagnostic-error]
cudaError_t cudaGetLastError(void);
            ^
  Report hash: 382ba18bf22b7198c1ccf99852730763
  Steps:
    1, cuda.h:52:13: previous declaration is here
    2, cuda_runtime.h:97:13: conflicting types for 'cudaGetLastError'

[CRITICAL] /sep/include/compat/cuda_runtime.h:98:13: conflicting types for 'cudaGetErrorString' [clang-diagnostic-error]
const char* cudaGetErrorString(cudaError_t error);
            ^
  Report hash: f9723040318b8841136d12020dad28ab
  Steps:
    1, cuda.h:53:13: previous declaration is here
    2, cuda_runtime.h:98:13: conflicting types for 'cudaGetErrorString'

Found 19 defect(s) in cuda_runtime.h

[CRITICAL] /sep/include/core/engine.h:52:57: no member named 'PinState' in namespace 'sep' [clang-diagnostic-error]
  void generate_probes(const ::sep::shim::vector<::sep::PinState> &inputs,
                                                        ^
  Report hash: ab4970105732d2235d07299ab21e37f7
  Steps:
    1, engine.h:52:57: no member named 'PinState' in namespace 'sep'

[CRITICAL] /sep/include/core/engine.h:57:55: no member named 'PinState' in namespace 'sep' [clang-diagnostic-error]
  void process_batch(const ::sep::shim::vector<::sep::PinState> &inputs,
                                                      ^
  Report hash: 9f2c428153c2eabe4ffd98fb9c66429d
  Steps:
    1, engine.h:57:55: no member named 'PinState' in namespace 'sep'

Found 2 defect(s) in engine.h

Found no defects in quantum_processor.cpp
[LOW] /sep/include/blender/base_types.h:9:10: circular header file dependency detected while including 'memory_tier.hpp', please check the include path [misc-header-include-cycle]
#include "memory/memory_tier.hpp"
         ^
  Report hash: d2fc400173d22b75362b7481cf70e61f
  Steps:
    1, compression.h:7:10: 'base_types.h' included from here
    2, memory_tier.hpp:10:10: 'compression.h' included from here
    3, memory_tier.cpp:11:10: 'memory_tier.hpp' included from here
    4, base_types.h:9:10: circular header file dependency detected while including 'memory_tier.hpp', please check the include path

Found 1 defect(s) in base_types.h

[CRITICAL] /sep/src/quantum/pattern_processor.cpp:24:29: no member named 'evolveState' in 'sep::quantum::QuantumProcessor' [clang-diagnostic-error]
        quantum_processor_->evolveState(result.state, pattern_id);
                            ^
  Report hash: dff875a30b3fb09de985acd8d80d69bf
  Steps:
    1, pattern_processor.cpp:24:29: no member named 'evolveState' in 'sep::quantum::QuantumProcessor'

[CRITICAL] /sep/src/quantum/pattern_processor.cpp:30:39: no member named 'STABILITY_THRESHOLD' in namespace 'sep::quantum::constants'; did you mean 'pattern::STABILITY_THRESHOLD'? [clang-diagnostic-error]
            result.stability_score >= constants::STABILITY_THRESHOLD) {
                                      ^
  Report hash: 961446c62167d7b6469edcbd93831af2
  Notes:
    1, pattern_processor.cpp:30:39: pattern::STABILITY_THRESHOLD (fixit)
  Steps:
    1, math_common.h:71:24: 'pattern::STABILITY_THRESHOLD' declared here
    2, pattern_processor.cpp:30:39: no member named 'STABILITY_THRESHOLD' in namespace 'sep::quantum::constants'; did you mean 'pattern::STABILITY_THRESHOLD'?

[CRITICAL] /sep/src/quantum/pattern_processor.cpp:55:63: no member named 'amplitudes' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
        return quantum_processor_->calculateCoherence(state_a.amplitudes, state_b.amplitudes);
                                                              ^
  Report hash: 86ec646f14b92dc419842ca41b7dd719
  Steps:
    1, pattern_processor.cpp:55:63: no member named 'amplitudes' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_processor.cpp:55:83: no member named 'amplitudes' in 'sep::quantum::QuantumState' [clang-diagnostic-error]
        return quantum_processor_->calculateCoherence(state_a.amplitudes, state_b.amplitudes);
                                                                                  ^
  Report hash: c9b5206413db62d62b0fd6092846b6a1
  Steps:
    1, pattern_processor.cpp:55:83: no member named 'amplitudes' in 'sep::quantum::QuantumState'

[CRITICAL] /sep/src/quantum/pattern_processor.cpp:59:35: no member named 'STABILITY_THRESHOLD' in namespace 'sep::quantum::constants'; did you mean 'pattern::STABILITY_THRESHOLD'? [clang-diagnostic-error]
        return state.stability >= constants::STABILITY_THRESHOLD;
                                  ^
  Report hash: e917a3094b62646c7cd10cdc8a2ce103
  Notes:
    1, pattern_processor.cpp:59:35: pattern::STABILITY_THRESHOLD (fixit)
  Steps:
    1, math_common.h:71:24: 'pattern::STABILITY_THRESHOLD' declared here
    2, pattern_processor.cpp:59:35: no member named 'STABILITY_THRESHOLD' in namespace 'sep::quantum::constants'; did you mean 'pattern::STABILITY_THRESHOLD'?

[CRITICAL] /sep/src/quantum/pattern_processor.cpp:63:34: no member named 'MIN_COHERENCE' in namespace 'sep::quantum::constants'; did you mean 'pattern::MIN_COHERENCE'? [clang-diagnostic-error]
        return state.coherence < constants::MIN_COHERENCE;
                                 ^
  Report hash: e413badcd3e9757394870b5062d0db40
  Notes:
    1, pattern_processor.cpp:63:34: pattern::MIN_COHERENCE (fixit)
  Steps:
    1, math_common.h:67:24: 'pattern::MIN_COHERENCE' declared here
    2, pattern_processor.cpp:63:34: no member named 'MIN_COHERENCE' in namespace 'sep::quantum::constants'; did you mean 'pattern::MIN_COHERENCE'?

Found 6 defect(s) in pattern_processor.cpp

Found no defects in quantum_processor_qfh.cpp
[LOW] /sep/include/blender/base_types.h:9:10: circular header file dependency detected while including 'memory_tier.hpp', please check the include path [misc-header-include-cycle]
#include "memory/memory_tier.hpp"
         ^
  Report hash: d2fc400173d22b75362b7481cf70e61f
  Steps:
    1, compression.h:7:10: 'base_types.h' included from here
    2, memory_tier.hpp:10:10: 'compression.h' included from here
    3, quantum_pattern_processor.h:5:10: 'memory_tier.hpp' included from here
    4, base_types.h:9:10: circular header file dependency detected while including 'memory_tier.hpp', please check the include path

Found 1 defect(s) in base_types.h

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:24:40: expected class name [clang-diagnostic-error]
class QuantumPatternProcessor : public CPUPatternProcessor {
                                       ^
  Report hash: 2db556a474874496f5e96430f185cc18
  Steps:
    1, quantum_pattern_processor.h:24:40: expected class name

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:27:32: only virtual member functions can be marked 'override' [clang-diagnostic-error]
    ~QuantumPatternProcessor() override = default;
                               ^
  Report hash: 5943ea5267d1933f1c9ea45ce4947319
  Steps:
    1, quantum_pattern_processor.h:27:32: only virtual member functions can be marked 'override'

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:29:20: unknown type name 'GPUContext'; did you mean 'quantum::GPUContext'? [clang-diagnostic-error]
    SEPResult init(GPUContext* ctx) override;
                   ^
  Report hash: fd53f5e817897774a7f77e360052eee7
  Notes:
    1, quantum_pattern_processor.h:29:20: quantum::GPUContext (fixit)
  Steps:
    1, gpu_context.h:5:8: 'quantum::GPUContext' declared here
    2, quantum_pattern_processor.h:29:20: unknown type name 'GPUContext'; did you mean 'quantum::GPUContext'?

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:29:37: only virtual member functions can be marked 'override' [clang-diagnostic-error]
    SEPResult init(GPUContext* ctx) override;
                                    ^
  Report hash: ada0891b369e27287e991107c0156e95
  Steps:
    1, quantum_pattern_processor.h:29:37: only virtual member functions can be marked 'override'

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:30:27: only virtual member functions can be marked 'override' [clang-diagnostic-error]
    void evolvePatterns() override;
                          ^
  Report hash: f608e0651983a1e55917d8990660b672
  Steps:
    1, quantum_pattern_processor.h:30:27: only virtual member functions can be marked 'override'

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:31:58: only virtual member functions can be marked 'override' [clang-diagnostic-error]
    PatternData mutatePattern(const PatternData& parent) override;
                                                         ^
  Report hash: 062372d52c7810e201606878c057a54f
  Steps:
    1, quantum_pattern_processor.h:31:58: only virtual member functions can be marked 'override'

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:32:17: use of undeclared identifier 'PatternProcessResult' [clang-diagnostic-error]
    std::vector<PatternProcessResult> processBatch(const std::vector<QuantumState>& states,
                ^
  Report hash: f99f767b58d4af2e7ac4fc0389489c9c
  Steps:
    1, quantum_pattern_processor.h:32:17: use of undeclared identifier 'PatternProcessResult'

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:32:70: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'? [clang-diagnostic-error]
    std::vector<PatternProcessResult> processBatch(const std::vector<QuantumState>& states,
                                                                     ^
  Report hash: bf6c22bd2fd21bdfb3f021bf125fb14d
  Notes:
    1, quantum_pattern_processor.h:32:70: quantum::QuantumState (fixit)
  Steps:
    1, types.h:26:8: 'quantum::QuantumState' declared here
    2, quantum_pattern_processor.h:32:70: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'?

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:38:16: use of undeclared identifier 'patterns_' [clang-diagnostic-error]
        return patterns_;
               ^
  Report hash: 644060e45b3fc9f319411130658694ce
  Steps:
    1, quantum_pattern_processor.h:38:16: use of undeclared identifier 'patterns_'

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:42:5: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'? [clang-diagnostic-error]
    QuantumState patternToQuantumState(const PatternData& pattern) const;
    ^
  Report hash: 6164be6387f24443e9082294436a342b
  Notes:
    1, quantum_pattern_processor.h:42:5: quantum::QuantumState (fixit)
  Steps:
    1, types.h:26:8: 'quantum::QuantumState' declared here
    2, quantum_pattern_processor.h:42:5: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'?

[CRITICAL] /sep/include/quantum/quantum_pattern_processor.h:43:68: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'? [clang-diagnostic-error]
    void updatePatternFromQuantumState(PatternData& pattern, const QuantumState& state);
                                                                   ^
  Report hash: c17a9ee7160d5c526f619a56953987a1
  Notes:
    1, quantum_pattern_processor.h:43:68: quantum::QuantumState (fixit)
  Steps:
    1, types.h:26:8: 'quantum::QuantumState' declared here
    2, quantum_pattern_processor.h:43:68: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'?

Found 11 defect(s) in quantum_pattern_processor.h

[CRITICAL] /sep/src/quantum/quantum_pattern_processor.cpp:10:41: unknown type name 'GPUContext' [clang-diagnostic-error]
SEPResult QuantumPatternProcessor::init(GPUContext* ctx) {
                                        ^
  Report hash: 6096390d8eb09a5c6dbe9c731833364f
  Steps:
    1, quantum_pattern_processor.cpp:10:41: unknown type name 'GPUContext'

[CRITICAL] /sep/src/quantum/quantum_pattern_processor.cpp:11:12: use of undeclared identifier 'CPUPatternProcessor' [clang-diagnostic-error]
    return CPUPatternProcessor::init(ctx);
           ^
  Report hash: b2a57f554c064dd13b14b975d60ca1a8
  Steps:
    1, quantum_pattern_processor.cpp:11:12: use of undeclared identifier 'CPUPatternProcessor'

[CRITICAL] /sep/src/quantum/quantum_pattern_processor.cpp:14:1: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'? [clang-diagnostic-error]
QuantumState QuantumPatternProcessor::patternToQuantumState(const PatternData& pattern) const {
^
  Report hash: 28843abc6ffa4bddf1dfa4e9bd30501e
  Notes:
    1, quantum_pattern_processor.cpp:14:1: quantum::QuantumState (fixit)
  Steps:
    1, types.h:26:8: 'quantum::QuantumState' declared here
    2, quantum_pattern_processor.cpp:14:1: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'?

[CRITICAL] /sep/src/quantum/quantum_pattern_processor.cpp:15:5: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'? [clang-diagnostic-error]
    QuantumState state{};
    ^
  Report hash: 93e5855ff86e363a3d7ac4a9781a6dba
  Notes:
    1, quantum_pattern_processor.cpp:15:5: quantum::QuantumState (fixit)
  Steps:
    1, types.h:26:8: 'quantum::QuantumState' declared here
    2, quantum_pattern_processor.cpp:15:5: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'?

[CRITICAL] /sep/src/quantum/quantum_pattern_processor.cpp:20:57: no member named 'mutations' in 'sep::pattern::PatternData' [clang-diagnostic-error]
    state.access_frequency = static_cast<float>(pattern.mutations) /
                                                        ^
  Report hash: a36b9e4644ffef78ffda844d0236f97d
  Steps:
    1, quantum_pattern_processor.cpp:20:57: no member named 'mutations' in 'sep::pattern::PatternData'

[CRITICAL] /sep/src/quantum/quantum_pattern_processor.cpp:26:66: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'? [clang-diagnostic-error]
                                                           const QuantumState& state) {
                                                                 ^
  Report hash: 58568653c6df6bf24527f4612add5a7f
  Notes:
    1, quantum_pattern_processor.cpp:26:66: quantum::QuantumState (fixit)
  Steps:
    1, types.h:26:8: 'quantum::QuantumState' declared here
    2, quantum_pattern_processor.cpp:26:66: unknown type name 'QuantumState'; did you mean 'quantum::QuantumState'?

[CRITICAL] /sep/src/quantum/quantum_pattern_processor.cpp:31:13: no member named 'mutations' in 'sep::pattern::PatternData' [clang-diagnostic-error]
    pattern.mutations = static_cast<uint32_t>(state.access_frequency * state.generation);
            ^
  Report hash: 9a0157b0437e544a005976f4e5e5248a
  Steps:
    1, quantum_pattern_processor.cpp:31:13: no member named 'mutations' in 'sep::pattern::PatternData'

[CRITICAL] /sep/src/quantum/quantum_pattern_processor.cpp:34:13: use of undeclared identifier 'PatternProcessResult' [clang-diagnostic-error]
std::vector<PatternProcessResult> QuantumPatternProcessor::processBatch(const std::vector<QuantumState>& states,
            ^
  Report hash: a3bd1b8e311b448bab4fa31263fb9cd9
  Steps:
    1, quantum_pattern_processor.cpp:34:13: use of undeclared identifier 'PatternProcessResult'

Found 8 defect(s) in quantum_pattern_processor.cpp

Found no defects in quantum_processor_qfh_common.cpp
[CRITICAL] /sep/src/memory/redis_manager.cpp:83:35: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
                       const sep::pattern::PatternData& data,
                                  ^
  Report hash: b62b8dc6257464bd7ba2a671f0dbfcf6
  Steps:
    1, redis_manager.cpp:83:35: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/memory/redis_manager.cpp:165:24: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
    std::optional<sep::pattern::PatternData> loadPattern( std::size_t id,
                       ^
  Report hash: 6700d96ef57453f55a2a74b6909ae872
  Steps:
    1, redis_manager.cpp:165:24: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/memory/redis_manager.cpp:170:20: no viable conversion from returned value of type 'const nullopt_t' to function return type 'int' [clang-diagnostic-error]
            return std::nullopt;
                   ^
  Report hash: 4653b8086c92ae27f5088cd85c985af7
  Steps:
    1, redis_manager.cpp:170:20: no viable conversion from returned value of type 'const nullopt_t' to function return type 'int'

[CRITICAL] /sep/src/memory/redis_manager.cpp:184:20: no viable conversion from returned value of type 'const nullopt_t' to function return type 'int' [clang-diagnostic-error]
            return std::nullopt;
                   ^
  Report hash: 4653b8086c92ae27f5088cd85c985af7
  Steps:
    1, redis_manager.cpp:184:20: no viable conversion from returned value of type 'const nullopt_t' to function return type 'int'

[CRITICAL] /sep/src/memory/redis_manager.cpp:188:14: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
        sep::pattern::PatternData data;
             ^
  Report hash: 0afdf9fc7ecf13d230d41877ab62e46e
  Steps:
    1, redis_manager.cpp:188:14: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/memory/redis_manager.cpp:188:35: use of undeclared identifier 'data'; did you mean 'std::data'? [clang-diagnostic-error]
        sep::pattern::PatternData data;
                                  ^
  Report hash: 2e1bfb6702d95d8015ae589fd49229bb
  Notes:
    1, redis_manager.cpp:188:35: std::data (fixit)
  Steps:
    1, range_access.h:324:5: 'std::data' declared here
    2, redis_manager.cpp:188:35: use of undeclared identifier 'data'; did you mean 'std::data'?

[CRITICAL] /sep/src/memory/redis_manager.cpp:195:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
            data.position.x = reply->element[0]->str ? std::stof(reply->element[0]->str) : 0.0f;
            ^
  Report hash: 022769b0eac9ff75a452d37359f30a4e
  Steps:
    1, range_access.h:324:5: possible target for call
    2, redis_manager.cpp:195:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:195:13: use of undeclared identifier 'data'; did you mean 'std::data'? [clang-diagnostic-error]
            data.position.x = reply->element[0]->str ? std::stof(reply->element[0]->str) : 0.0f;
            ^
  Report hash: 97d400a667a21fa31925f0beb64701bb
  Notes:
    1, redis_manager.cpp:195:13: std::data (fixit)
  Steps:
    1, range_access.h:324:5: 'std::data' declared here
    2, redis_manager.cpp:195:13: use of undeclared identifier 'data'; did you mean 'std::data'?

[CRITICAL] /sep/src/memory/redis_manager.cpp:196:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
            data.position.y = reply->element[1]->str ? std::stof(reply->element[1]->str) : 0.0f;
            ^
  Report hash: b74da82791cd90ec67e63a0832ff0d8f
  Steps:
    1, range_access.h:324:5: possible target for call
    2, redis_manager.cpp:196:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:196:13: use of undeclared identifier 'data'; did you mean 'std::data'? [clang-diagnostic-error]
            data.position.y = reply->element[1]->str ? std::stof(reply->element[1]->str) : 0.0f;
            ^
  Report hash: 0d02201f33c1d61f3d0478222ac89fa4
  Notes:
    1, redis_manager.cpp:196:13: std::data (fixit)
  Steps:
    1, range_access.h:324:5: 'std::data' declared here
    2, redis_manager.cpp:196:13: use of undeclared identifier 'data'; did you mean 'std::data'?

[CRITICAL] /sep/src/memory/redis_manager.cpp:197:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
            data.position.z = reply->element[2]->str ? std::stof(reply->element[2]->str) : 0.0f;
            ^
  Report hash: 64b2bfef7684951c2af18c437317ff75
  Steps:
    1, range_access.h:324:5: possible target for call
    2, redis_manager.cpp:197:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:197:13: use of undeclared identifier 'data'; did you mean 'std::data'? [clang-diagnostic-error]
            data.position.z = reply->element[2]->str ? std::stof(reply->element[2]->str) : 0.0f;
            ^
  Report hash: e4450270b3c83957647e44a0985f2e83
  Notes:
    1, redis_manager.cpp:197:13: std::data (fixit)
  Steps:
    1, range_access.h:324:5: 'std::data' declared here
    2, redis_manager.cpp:197:13: use of undeclared identifier 'data'; did you mean 'std::data'?

[CRITICAL] /sep/src/memory/redis_manager.cpp:209:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
            data.coherence = reply->element[0]->str ? std::stof(reply->element[0]->str) : 0.0f;
            ^
  Report hash: bd33815efe3438d32aca57accd266d4a
  Steps:
    1, range_access.h:324:5: possible target for call
    2, redis_manager.cpp:209:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:209:13: use of undeclared identifier 'data'; did you mean 'std::data'? [clang-diagnostic-error]
            data.coherence = reply->element[0]->str ? std::stof(reply->element[0]->str) : 0.0f;
            ^
  Report hash: 5a6bf5f00b97bc1e0e3f188b06ce9558
  Notes:
    1, redis_manager.cpp:209:13: std::data (fixit)
  Steps:
    1, range_access.h:324:5: 'std::data' declared here
    2, redis_manager.cpp:209:13: use of undeclared identifier 'data'; did you mean 'std::data'?

[CRITICAL] /sep/src/memory/redis_manager.cpp:210:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
            data.stability = reply->element[1]->str ? std::stof(reply->element[1]->str) : 0.0f;
            ^
  Report hash: 51d52ab70b91e941a042c06574074730
  Steps:
    1, range_access.h:324:5: possible target for call
    2, redis_manager.cpp:210:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:210:13: use of undeclared identifier 'data'; did you mean 'std::data'? [clang-diagnostic-error]
            data.stability = reply->element[1]->str ? std::stof(reply->element[1]->str) : 0.0f;
            ^
  Report hash: ca673dc65730c91b0d2ccde25da84ce5
  Notes:
    1, redis_manager.cpp:210:13: std::data (fixit)
  Steps:
    1, range_access.h:324:5: 'std::data' declared here
    2, redis_manager.cpp:210:13: use of undeclared identifier 'data'; did you mean 'std::data'?

[CRITICAL] /sep/src/memory/redis_manager.cpp:211:13: use of undeclared identifier 'data'; did you mean 'std::data'? [clang-diagnostic-error]
            data.generation = reply->element[2]->str ? std::stoi(reply->element[2]->str) : 0;
            ^
  Report hash: ca21d02c22efa6aa4a3526d97721ceb4
  Notes:
    1, redis_manager.cpp:211:13: std::data (fixit)
  Steps:
    1, range_access.h:324:5: 'std::data' declared here
    2, redis_manager.cpp:211:13: use of undeclared identifier 'data'; did you mean 'std::data'?

[CRITICAL] /sep/src/memory/redis_manager.cpp:347:66: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
    void bulkStore(const std::vector<std::pair<std::size_t, sep::pattern::PatternData>>& patterns, const std::string& tier)
                                                                 ^
  Report hash: de39261a3b001e8fdcdc9247fb825f2a
  Steps:
    1, redis_manager.cpp:347:66: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/memory/redis_manager.cpp:355:22: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
    std::vector<sep::pattern::PatternData> bulkLoad(const std::vector<std::size_t>& ids, const std::string& tier)
                     ^
  Report hash: f749ab34e6106feb93a6ec3598c50873
  Steps:
    1, redis_manager.cpp:355:22: no member named 'pattern' in namespace 'sep'

Found 19 defect(s) in redis_manager.cpp

[CRITICAL] /sep/src/api/crow_error.cpp:1:29: expected '>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 80b0e04fd5a65ded71cab4f84bfd342c
  Steps:
    1, crow_error.cpp:1:10: to match this '<'
    2, crow_error.cpp:1:29: expected '>'

[CRITICAL] /sep/src/api/crow_error.cpp:2:33: expected '>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 304bb6966702b29d763135f6ce68d6e6
  Steps:
    1, crow_error.cpp:2:10: to match this '<'
    2, crow_error.cpp:2:33: expected '>'

[CRITICAL] /sep/src/api/crow_error.cpp:5:6: use of undeclared identifier 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 90406ea894e72476ed92257ba82cfe7e
  Steps:
    1, crow_error.cpp:5:6: use of undeclared identifier 'sep'

[CRITICAL] /sep/src/api/crow_error.cpp:5:28: unknown type name 'Code' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 058d58c6a411d3c1ca9f917e6fea2925
  Steps:
    1, crow_error.cpp:5:28: unknown type name 'Code'

[CRITICAL] /sep/src/api/crow_error.cpp:5:45: use of undeclared identifier 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 75a52db196b1eea0f8c6b7e32e2dee98
  Steps:
    1, crow_error.cpp:5:45: use of undeclared identifier 'sep'

Found 5 defect(s) in crow_error.cpp

[CRITICAL] /sep/include/blender/base_types.h:8:10: 'cuda/math_common.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 14d63576805f2ec2cc5fd227e28d309f
  Steps:
    1, base_types.h:8:10: 'cuda/math_common.h' file not found

Found 1 defect(s) in base_types.h

[CRITICAL] /sep/include/api/sep_engine.h:12:10: 'config/types.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e4c22c4f4fff746652e4212b842beaa8
  Steps:
    1, sep_engine.h:12:10: 'config/types.h' file not found

[LOW] /sep/include/api/sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7ddbaf54ea1b7346b97eb4b637f989f0
  Steps:
    1, processor.h:16:7: a definition of 'Processor' is found here
    2, sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

[LOW] /sep/include/api/sep_engine.h:151:5: class 'SepEngine' can be made trivially destructible by defaulting the destructor on its first declaration [performance-trivially-destructible]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0d0fcdf5b43e14ba786abcd240cfd8d7
  Notes:
    1, sep_engine.h:151:18: = default (fixit)
  Steps:
    1, sep_engine.cpp:77:12: destructor definition is here
    2, sep_engine.h:151:5: class 'SepEngine' can be made trivially destructible by defaulting the destructor on its first declaration

Found 3 defect(s) in sep_engine.h

[CRITICAL] /sep/src/api/crow_adapter.cpp:13:33: expected '>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: aa27658135a4c68f52676201eba5d797
  Steps:
    1, crow_adapter.cpp:13:10: to match this '<'
    2, crow_adapter.cpp:13:33: expected '>'

[CRITICAL] /sep/src/api/crow_adapter.cpp:14:33: expected '>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b24552a64edeb9cde23c3cc550a62b31
  Steps:
    1, crow_adapter.cpp:14:10: to match this '<'
    2, crow_adapter.cpp:14:33: expected '>'

Found 2 defect(s) in crow_adapter.cpp

[LOW] /sep/third_party/crow/asio_isolation.h:62:5: constructor accepting a forwarding reference can hide the copy and move constructors [bugprone-forwarding-reference-overload]
    any_io_executor(T&&)
    ^
  Report hash: b786196d73895c2b845e8d923b7f37a1
  Steps:
    1, asio_isolation.h:56:7: copy constructor declared here
    2, asio_isolation.h:56:7: move constructor declared here
    3, asio_isolation.h:62:5: constructor accepting a forwarding reference can hide the copy and move constructors

[LOW] /sep/third_party/crow/asio_isolation.h:319:5: constructor accepting a forwarding reference can hide the copy and move constructors [bugprone-forwarding-reference-overload]
    any_executor(T&&)
    ^
  Report hash: d89311ada9400764aafa17d3bb8b33b7
  Steps:
    1, asio_isolation.h:319:5: constructor accepting a forwarding reference can hide the copy and move constructors

Found 2 defect(s) in asio_isolation.h

[CRITICAL] /sep/src/api/auth_middleware.cpp:1:33: expected '>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b9ad40ae5c93465a47c3d7335576fdf3
  Steps:
    1, auth_middleware.cpp:1:10: to match this '<'
    2, auth_middleware.cpp:1:33: expected '>'

Found 1 defect(s) in auth_middleware.cpp

[CRITICAL] /sep/third_party/crow/socket_adaptors.h:43:20: no type named 'error_code' in namespace 'boost::asio'; did you mean 'std::error_code'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e4d064f72327acb6d4be4e358a3be9e5
  Notes:
    1, socket_adaptors.h:43:20: std::error_code (fixit)
  Steps:
    1, system_error:225:9: 'std::error_code' declared here
    2, socket_adaptors.h:43:20: no type named 'error_code' in namespace 'boost::asio'; did you mean 'std::error_code'?

[MEDIUM] /sep/third_party/crow/socket_adaptors.h:67:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [bugprone-unused-return-value]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 08f1d037db9333c5e76443b950332860
  Steps:
    1, socket_adaptors.h:67:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

[MEDIUM] /sep/third_party/crow/socket_adaptors.h:67:5: the value returned by this function should not be disregarded; neglecting it may lead to errors [cert-err33-c]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c66b42edf17b6c6ec7f17def8f3f7253
  Steps:
    1, socket_adaptors.h:67:5: the value returned by this function should not be disregarded; neglecting it may lead to errors

Found 3 defect(s) in socket_adaptors.h

[CRITICAL] /sep/include/blender/mesh_handler.h:32:31: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 80ac925ee5b9f214fc270019e429835a
  Steps:
    1, mesh_handler.h:32:31: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/blender/mesh_handler.h:72:42: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e0552d50f0a9ffeae3f98e2ad61a2647
  Steps:
    1, mesh_handler.h:72:42: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/blender/mesh_handler.h:83:39: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 52e11b9af41f3458d49a1b35d80fe6c7
  Steps:
    1, mesh_handler.h:83:39: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/blender/mesh_handler.h:84:41: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b39e94bf394073852cb36188d4e6cea3
  Steps:
    1, mesh_handler.h:84:41: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/blender/mesh_handler.h:89:35: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2c96c7982a6447e639f6be80b829cd86
  Steps:
    1, mesh_handler.h:89:35: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/blender/mesh_handler.h:155:45: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 07bfb513f68a73d8b907fba917bc72b2
  Steps:
    1, mesh_handler.h:155:45: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/blender/mesh_handler.h:157:41: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 265cd253fc47e0ff532912cc8bb64e64
  Steps:
    1, mesh_handler.h:157:41: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/blender/mesh_handler.h:159:60: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f65cb5110797acfe74dfdc552b0fd15e
  Steps:
    1, mesh_handler.h:159:60: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/blender/mesh_handler.h:160:61: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 28b518c75732d8fc349a6121f2a7c22f
  Steps:
    1, mesh_handler.h:160:61: no member named 'pattern' in namespace 'sep'

Found 9 defect(s) in mesh_handler.h

[CRITICAL] /sep/include/blender/pattern_visualization_pipeline.h:17:40: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a13afda1694daef83f102494af22a421
  Steps:
    1, pattern_visualization_pipeline.h:17:40: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/blender/pattern_visualization_pipeline.h:22:39: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c7e302595e909608867dfb7312a1b375
  Steps:
    1, pattern_visualization_pipeline.h:22:39: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/blender/pattern_visualization_pipeline.h:26:41: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6a854a7faae1441417962fadeab31a36
  Steps:
    1, pattern_visualization_pipeline.h:26:41: no member named 'pattern' in namespace 'sep'

Found 3 defect(s) in pattern_visualization_pipeline.h

[CRITICAL] /sep/src/blender/pattern_visualization_pipeline.cpp:43:16: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
    const sep::pattern::PatternData &pattern) {
               ^
  Report hash: 9301d8c8607e078c0a0c8ad994d9689c
  Steps:
    1, pattern_visualization_pipeline.cpp:43:16: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/pattern_visualization_pipeline.cpp:62:16: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
    const sep::pattern::PatternData &pattern, int dimensionality) {
               ^
  Report hash: b21ecabc028ecc4db01a5296442f5a68
  Steps:
    1, pattern_visualization_pipeline.cpp:62:16: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/pattern_visualization_pipeline.cpp:70:16: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
    const sep::pattern::PatternData &pattern, int dimensionality) {
               ^
  Report hash: b21ecabc028ecc4db01a5296442f5a68
  Steps:
    1, pattern_visualization_pipeline.cpp:70:16: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/pattern_visualization_pipeline.cpp:102:8: 'pattern' is not a class, namespace, or enumeration [clang-diagnostic-error]
  sep::pattern::PatternData projected_pattern = pattern;
       ^
  Report hash: cb12805c04c0968bbef86fdccb7ebc85
  Steps:
    1, pattern_visualization_pipeline.cpp:70:38: 'pattern' declared here
    2, pattern_visualization_pipeline.cpp:102:8: 'pattern' is not a class, namespace, or enumeration

Found 4 defect(s) in pattern_visualization_pipeline.cpp

[CRITICAL] /sep/src/api/lock_free_rate_limiter.cpp:3:33: expected '>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b2ebc392dafd21788d76a3b9e5aea3f5
  Steps:
    1, lock_free_rate_limiter.cpp:3:10: to match this '<'
    2, lock_free_rate_limiter.cpp:3:33: expected '>'

Found 1 defect(s) in lock_free_rate_limiter.cpp

[CRITICAL] /sep/include/compat/cuda_impl.h:90:39: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 101e6df1c004dc07acf020f894f4538d
  Notes:
    1, cuda_impl.h:90:39: cuda_stub_constants::cudaError_t (fixit)
  Steps:
    1, cuda_impl.h:31:34: 'cuda_stub_constants::cudaError_t' declared here
    2, cuda_impl.h:90:39: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:94:22: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0da1de487d671fd09be1907cfa03b298
  Notes:
    1, cuda_impl.h:94:22: cuda_stub_constants::cudaSuccess (fixit)
  Steps:
    1, cuda_impl.h:31:16: 'cuda_stub_constants::cudaSuccess' declared here
    2, cuda_impl.h:94:22: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'?

[CRITICAL] /sep/include/compat/cuda_impl.h:100:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a369a3dfb78f167e3e501d80b8607a83
  Notes:
    1, cuda_impl.h:100:8: cuda_stub_constants::cudaError_t (fixit)
  Steps:
    1, cuda_impl.h:31:34: 'cuda_stub_constants::cudaError_t' declared here
    2, cuda_impl.h:100:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:105:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c44881aaf9fc0b73ea302927ff1d456
  Notes:
    1, cuda_impl.h:105:12: cuda_stub_constants::cudaSuccess (fixit)
  Steps:
    1, cuda_impl.h:31:16: 'cuda_stub_constants::cudaSuccess' declared here
    2, cuda_impl.h:105:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'?

[CRITICAL] /sep/include/compat/cuda_impl.h:111:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 996524ba65441311a21b4f6e1651b56b
  Notes:
    1, cuda_impl.h:111:8: cuda_stub_constants::cudaError_t (fixit)
  Steps:
    1, cuda_impl.h:31:34: 'cuda_stub_constants::cudaError_t' declared here
    2, cuda_impl.h:111:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:118:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c44881aaf9fc0b73ea302927ff1d456
  Notes:
    1, cuda_impl.h:118:12: cuda_stub_constants::cudaSuccess (fixit)
  Steps:
    1, cuda_impl.h:31:16: 'cuda_stub_constants::cudaSuccess' declared here
    2, cuda_impl.h:118:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'?

[CRITICAL] /sep/include/compat/cuda_impl.h:123:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e325a1af3bb1be1c80a1bddae3b48082
  Notes:
    1, cuda_impl.h:123:8: cuda_stub_constants::cudaError_t (fixit)
  Steps:
    1, cuda_impl.h:31:34: 'cuda_stub_constants::cudaError_t' declared here
    2, cuda_impl.h:123:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:123:36: unknown type name 'cudaEvent_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a58272eb5fc9cbe53d735fbf296826b1
  Steps:
    1, cuda_impl.h:123:36: unknown type name 'cudaEvent_t'

[CRITICAL] /sep/include/compat/cuda_impl.h:130:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c44881aaf9fc0b73ea302927ff1d456
  Notes:
    1, cuda_impl.h:130:12: cuda_stub_constants::cudaSuccess (fixit)
  Steps:
    1, cuda_impl.h:31:16: 'cuda_stub_constants::cudaSuccess' declared here
    2, cuda_impl.h:130:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'?

[CRITICAL] /sep/include/compat/cuda_impl.h:134:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: dd810c5b6e74be9a5f99633d0135134e
  Notes:
    1, cuda_impl.h:134:8: cuda_stub_constants::cudaError_t (fixit)
  Steps:
    1, cuda_impl.h:31:34: 'cuda_stub_constants::cudaError_t' declared here
    2, cuda_impl.h:134:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:134:37: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5ad55e4aa46a0953c1817c966ac8b595
  Notes:
    1, cuda_impl.h:134:37: cuda_stub_constants::cudaEvent_t (fixit)
  Steps:
    1, cuda_impl.h:30:28: 'cuda_stub_constants::cudaEvent_t' declared here
    2, cuda_impl.h:134:37: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:139:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c44881aaf9fc0b73ea302927ff1d456
  Notes:
    1, cuda_impl.h:139:12: cuda_stub_constants::cudaSuccess (fixit)
  Steps:
    1, cuda_impl.h:31:16: 'cuda_stub_constants::cudaSuccess' declared here
    2, cuda_impl.h:139:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'?

[CRITICAL] /sep/include/compat/cuda_impl.h:143:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9cec4d308c4029f271d4c44d11002f78
  Notes:
    1, cuda_impl.h:143:8: cuda_stub_constants::cudaError_t (fixit)
  Steps:
    1, cuda_impl.h:31:34: 'cuda_stub_constants::cudaError_t' declared here
    2, cuda_impl.h:143:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:143:36: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6ed84311e300751cb2be13750920ba15
  Notes:
    1, cuda_impl.h:143:36: cuda_stub_constants::cudaEvent_t (fixit)
  Steps:
    1, cuda_impl.h:30:28: 'cuda_stub_constants::cudaEvent_t' declared here
    2, cuda_impl.h:143:36: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:143:55: unknown type name 'cudaStream_t'; did you mean 'cuda_stub_constants::cudaStream_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a2b998c0a1c7495102eda2c33b59a130
  Notes:
    1, cuda_impl.h:143:55: cuda_stub_constants::cudaStream_t (fixit)
  Steps:
    1, cuda_impl.h:29:29: 'cuda_stub_constants::cudaStream_t' declared here
    2, cuda_impl.h:143:55: unknown type name 'cudaStream_t'; did you mean 'cuda_stub_constants::cudaStream_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:149:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c44881aaf9fc0b73ea302927ff1d456
  Notes:
    1, cuda_impl.h:149:12: cuda_stub_constants::cudaSuccess (fixit)
  Steps:
    1, cuda_impl.h:31:16: 'cuda_stub_constants::cudaSuccess' declared here
    2, cuda_impl.h:149:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'?

[CRITICAL] /sep/include/compat/cuda_impl.h:153:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eb7ff6583bd0628e4c2ba2a108d3264b
  Notes:
    1, cuda_impl.h:153:8: cuda_stub_constants::cudaError_t (fixit)
  Steps:
    1, cuda_impl.h:31:34: 'cuda_stub_constants::cudaError_t' declared here
    2, cuda_impl.h:153:8: unknown type name 'cudaError_t'; did you mean 'cuda_stub_constants::cudaError_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:153:41: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 92b758fe165c1180a48af9702e3ce24a
  Notes:
    1, cuda_impl.h:153:41: cuda_stub_constants::cudaEvent_t (fixit)
  Steps:
    1, cuda_impl.h:30:28: 'cuda_stub_constants::cudaEvent_t' declared here
    2, cuda_impl.h:153:41: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'?

[CRITICAL] /sep/include/compat/cuda_impl.h:158:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6c44881aaf9fc0b73ea302927ff1d456
  Notes:
    1, cuda_impl.h:158:12: cuda_stub_constants::cudaSuccess (fixit)
  Steps:
    1, cuda_impl.h:31:16: 'cuda_stub_constants::cudaSuccess' declared here
    2, cuda_impl.h:158:12: use of undeclared identifier 'cudaSuccess'; did you mean 'cuda_stub_constants::cudaSuccess'?

Found 19 defect(s) in cuda_impl.h

[CRITICAL] /sep/include/api/rate_limit_middleware.h:11:10: 'config/types.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f56db8d7cf6023cb7486b61ef3e4a58f
  Steps:
    1, rate_limit_middleware.h:11:10: 'config/types.h' file not found

Found 1 defect(s) in rate_limit_middleware.h

[CRITICAL] /sep/src/core/manager.cpp:1:10: 'config/manager.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ad02d91f1be1b0c94eda1aef1eabdeed
  Steps:
    1, manager.cpp:1:10: 'config/manager.h' file not found

Found 1 defect(s) in manager.cpp

[CRITICAL] /sep/src/quantum/evolution.cpp:1:10: 'evolution.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 39011af82aea5e63ec440695f817e313
  Steps:
    1, evolution.cpp:1:10: 'evolution.h' file not found

Found 1 defect(s) in evolution.cpp

[CRITICAL] /sep/include/memory/types.h:28:50: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c14a0ec525f1a91bf3e022ca727e4b21
  Steps:
    1, types.h:28:50: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/memory/types.h:31:24: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 655400132cab640a16bbacf57cb54d25
  Steps:
    1, types.h:31:24: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/memory/types.h:40:66: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 487fc903cfde3f20544b4ac83c4eb4a0
  Steps:
    1, types.h:40:66: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/memory/types.h:41:22: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5c08d903f96aa86c7fc918bce326aef4
  Steps:
    1, types.h:41:22: no member named 'pattern' in namespace 'sep'

Found 4 defect(s) in types.h

[CRITICAL] /sep/src/api/server.cpp:2:33: expected '>' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b8f589ef768e016710bae9b6a42fc659
  Steps:
    1, server.cpp:2:10: to match this '<'
    2, server.cpp:2:33: expected '>'

Found 1 defect(s) in server.cpp

[CRITICAL] /sep/src/blender/mesh_handler.cpp:43:42: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
SEPResult MeshHandler::update(const sep::pattern::PatternData& pattern_data) {
                                         ^
  Report hash: fc64513e6bc90d1a7ec7ae0d137c4a62
  Steps:
    1, mesh_handler.cpp:43:42: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/mesh_handler.cpp:198:53: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
SEPResult MeshHandler::generateHyperMesh(const sep::pattern::PatternData& pattern,
                                                    ^
  Report hash: 0c6a3b3c210e49fbbfab44c856af4aa2
  Steps:
    1, mesh_handler.cpp:198:53: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/mesh_handler.cpp:223:50: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
SEPResult MeshHandler::updateVertices(const sep::pattern::PatternData& pattern_data) {
                                                 ^
  Report hash: e1d47e2f90c9d21fe0de6d9d6709e347
  Steps:
    1, mesh_handler.cpp:223:50: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/mesh_handler.cpp:239:52: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
SEPResult MeshHandler::updateCustomData(const sep::pattern::PatternData& pattern_data) {
                                                   ^
  Report hash: 528d7654d5909356973a9c672b9287ca
  Steps:
    1, mesh_handler.cpp:239:52: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/mesh_handler.cpp:267:46: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
bool MeshHandler::validatePattern(const sep::pattern::PatternData& pattern_data) const {
                                             ^
  Report hash: 58ae0c6a1347567ae742401cc9973ad6
  Steps:
    1, mesh_handler.cpp:267:46: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/mesh_handler.cpp:346:56: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
float MeshHandler::calculateVertexInfluence(const sep::pattern::PatternData& pattern,
                                                       ^
  Report hash: 6dc8533c6199572a15ca66ea407e1f54
  Steps:
    1, mesh_handler.cpp:346:56: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/mesh_handler.cpp:356:52: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
void MeshHandler::calculateDisplacement(const sep::pattern::PatternData& pattern,
                                                   ^
  Report hash: 34e7b789c93e1b192d4a2932bc3843c6
  Steps:
    1, mesh_handler.cpp:356:52: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/mesh_handler.cpp:366:46: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
                                  const sep::pattern::PatternData& pattern) {
                                             ^
  Report hash: 88d26d4ba815839255295aa308e58def
  Steps:
    1, mesh_handler.cpp:366:46: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/blender/mesh_handler.cpp:375:47: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
                                   const sep::pattern::PatternData& pattern) {
                                              ^
  Report hash: f642494085d7579417971f08c5bfa14a
  Steps:
    1, mesh_handler.cpp:375:47: no member named 'pattern' in namespace 'sep'

Found 9 defect(s) in mesh_handler.cpp

[CRITICAL] /sep/include/quantum/relationship.h:18:10: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6257da3cec214e98e39ec0c7d2c10a13
  Steps:
    1, relationship.h:18:10: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/include/quantum/relationship.h:38:31: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 806b7a6e8d29fc305aa84bafca28ed9b
  Steps:
    1, relationship.h:38:31: no member named 'pattern' in namespace 'sep'

Found 2 defect(s) in relationship.h

[LOW] /usr/include/boost/asio/associated_allocator.hpp:91:54: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_allocator.hpp:91:54)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename associator<associated_allocator, T, A>::type
                                                     ^
  Report hash: 70fef44d943cb18b0c56016a1a481040
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, associated_allocator.hpp:91:54: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_allocator.hpp:91:54)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in associated_allocator.hpp

[LOW] /usr/include/boost/asio/associated_cancellation_slot.hpp:91:62: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_cancellation_slot.hpp:91:62)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename associator<associated_cancellation_slot, T, S>::type
                                                             ^
  Report hash: 6c84fd44b7ad7fbdeb4b0b2dd15b7040
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, associated_cancellation_slot.hpp:91:62: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_cancellation_slot.hpp:91:62)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in associated_cancellation_slot.hpp

[LOW] /usr/include/boost/asio/associated_executor.hpp:93:53: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_executor.hpp:93:53)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename associator<associated_executor, T, E>::type
                                                    ^
  Report hash: e2271b31569a015899763cb0ff54e3c8
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, associated_executor.hpp:93:53: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_executor.hpp:93:53)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in associated_executor.hpp

[LOW] /usr/include/boost/asio/associated_immediate_executor.hpp:67:21: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:67:21)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    is_executor<E>::value
                    ^
  Report hash: a1600ab13c4bb287d462f7e14cf59fb8
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, associated_immediate_executor.hpp:67:21: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:67:21)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/associated_immediate_executor.hpp:156:63: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:156:63)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename associator<associated_immediate_executor, T, E>::type
                                                              ^
  Report hash: 541887fb470a17ec884c11045594239d
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, associated_immediate_executor.hpp:156:63: no definition found for '(unnamed struct at /usr/include/boost/asio/associated_immediate_executor.hpp:156:63)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 2 defect(s) in associated_immediate_executor.hpp

[LOW] /usr/include/boost/asio/execution/bulk_execute.hpp:147:35: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:147:35)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    bulk_execute_member<S, F, N>::is_valid
                                  ^
  Report hash: 4ec7f6121677d56fe04e330c234a4de6
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, bulk_execute.hpp:147:35: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:147:35)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/bulk_execute.hpp:165:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:165:36)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !bulk_execute_member<S, F, N>::is_valid
                                   ^
  Report hash: f674ac7f591f34b2f0db0ea3b2aa39d0
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, bulk_execute.hpp:165:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:165:36)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/bulk_execute.hpp:186:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:186:36)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !bulk_execute_member<S, F, N>::is_valid
                                   ^
  Report hash: e96cf796de2e1b27844b5d69db4ee404
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, bulk_execute.hpp:186:36: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/bulk_execute.hpp:186:36)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in bulk_execute.hpp

[LOW] /usr/include/boost/asio/execution/connect.hpp:180:55: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:180:55)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    is_operation_state<typename connect_member<S, R>::result_type>::value
                                                      ^
  Report hash: 423026ea700952e7cfa10b4dea0634fa
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, connect.hpp:180:55: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:180:55)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/connect.hpp:196:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:196:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    connect_free<S, R>::is_valid
                        ^
  Report hash: 006ec4a7c8956163cdb1f214545b78a4
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, connect.hpp:196:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:196:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/connect.hpp:215:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:215:26)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !connect_free<S, R>::is_valid
                         ^
  Report hash: 0d464df3f8e39dd5b041bcbe3d2eab9b
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, connect.hpp:215:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/connect.hpp:215:26)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in connect.hpp

[LOW] /usr/include/boost/asio/execution/execute.hpp:145:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:145:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    execute_free<T, F>::is_valid
                        ^
  Report hash: 7d5c95c0f0d5390f77b4c128ab28774c
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, execute.hpp:145:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:145:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/execute.hpp:158:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:158:26)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !execute_free<T, F>::is_valid
                         ^
  Report hash: 56796fb8fa5ebe9f74739a3d97473d7d
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, execute.hpp:158:26: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/execute.hpp:158:26)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 2 defect(s) in execute.hpp

[LOW] /usr/include/boost/asio/execution/executor.hpp:62:43: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/executor.hpp:62:43)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename result_of<typename decay<F>::type&()>::type
                                          ^
  Report hash: 2eeaab853776f0ba8cd75d1bdbc73e06
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, executor.hpp:62:43: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/executor.hpp:62:43)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in executor.hpp

[LOW] /usr/include/boost/asio/execution/schedule.hpp:118:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:118:23)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    schedule_free<S>::is_valid
                      ^
  Report hash: 5bf94ce1b1dc29d0ea3c612ac409ff6c
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, schedule.hpp:118:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:118:23)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/schedule.hpp:131:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:131:24)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !schedule_free<S>::is_valid
                       ^
  Report hash: 72f5e86dea02dd9b01030339e2ab99bd
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, schedule.hpp:131:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/schedule.hpp:131:24)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 2 defect(s) in schedule.hpp

[LOW] /usr/include/boost/asio/execution/sender.hpp:90:56: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/sender.hpp:90:56)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    typename has_error_types<S::template error_types>::type,
                                                       ^
  Report hash: a46bdbaeb43903042be2c804a9f59b41
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, sender.hpp:90:56: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/sender.hpp:90:56)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in sender.hpp

[LOW] /usr/include/boost/asio/execution/set_done.hpp:115:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_done.hpp:115:23)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    set_done_free<R>::is_valid
                      ^
  Report hash: 704a00346b3fabbce808c34a20667085
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, set_done.hpp:115:23: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_done.hpp:115:23)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in set_done.hpp

[LOW] /usr/include/boost/asio/execution/set_error.hpp:115:27: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_error.hpp:115:27)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    set_error_free<R, E>::is_valid
                          ^
  Report hash: 309b7da127fc659ab4fe6d41658e4aef
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, set_error.hpp:115:27: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_error.hpp:115:27)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in set_error.hpp

[LOW] /usr/include/boost/asio/execution/set_value.hpp:118:28: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_value.hpp:118:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    set_value_free<R, Vs>::is_valid
                           ^
  Report hash: 4acb15d71c11afc2847029c9059f47ec
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, set_value.hpp:118:28: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/set_value.hpp:118:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in set_value.hpp

[LOW] /usr/include/boost/asio/execution/start.hpp:112:20: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/start.hpp:112:20)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    start_free<R>::is_valid
                   ^
  Report hash: 90c32a5d05834d659f3190a942570700
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, start.hpp:112:20: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/start.hpp:112:20)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in start.hpp

[LOW] /usr/include/boost/asio/execution/submit.hpp:148:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:148:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    is_sender_to<S, R>::value
                        ^
  Report hash: 6a7174c92432db41adc0d9f36fc3d937
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, submit.hpp:148:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:148:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/submit.hpp:161:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:161:24)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    submit_free<S, R>::is_valid
                       ^
  Report hash: 1db8948ae610b0f9cc2b92ca65da9cd5
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, submit.hpp:161:24: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:161:24)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/execution/submit.hpp:177:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:177:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !submit_free<S, R>::is_valid
                        ^
  Report hash: ce178ee755a8021505e73e93992ecb96
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, submit.hpp:177:25: no definition found for '(unnamed struct at /usr/include/boost/asio/execution/submit.hpp:177:25)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in submit.hpp

[LOW] /usr/include/boost/asio/executor_work_guard.hpp:162:41: no definition found for '(unnamed class at /usr/include/boost/asio/executor_work_guard.hpp:162:41)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
      execution::is_executor<Executor>::value
                                        ^
  Report hash: 07056f6befa6401089813143f2701c29
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, executor_work_guard.hpp:162:41: no definition found for '(unnamed class at /usr/include/boost/asio/executor_work_guard.hpp:162:41)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 1 defect(s) in executor_work_guard.hpp

[LOW] /usr/include/boost/asio/prefer.hpp:168:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:168:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 2238920b3c290a76c9b423a07a43a6d2
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:168:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:168:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:193:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:193:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 1e6e8afac2bac81734c1c33af9f24479
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:193:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:193:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:215:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:215:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 6e0bbb106931fccd949fbd81d6371ab6
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:215:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:215:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:240:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:240:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: cd5310347573725d62517f5cce2cc410
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:240:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:240:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:268:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:268:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: 0c6a666b99a4650e7dbee6903c1d2be1
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:268:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:268:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:299:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:299:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_preferable
                           ^
  Report hash: a4694409cea859df04375695635e3a06
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:299:28: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:299:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:335:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:335:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 09a1f432e82f49101d8b075d91e59386
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:335:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:335:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/prefer.hpp:371:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:371:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 9ebf31eabe5216c4d127ab0a3f6e2a53
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, prefer.hpp:371:48: no definition found for '(unnamed struct at /usr/include/boost/asio/prefer.hpp:371:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 8 defect(s) in prefer.hpp

[LOW] /usr/include/boost/asio/query.hpp:142:32: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:142:32)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    static_query<T, Property>::is_valid
                               ^
  Report hash: d366360eda1cfbbd9b488be45557927d
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, query.hpp:142:32: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:142:32)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/query.hpp:158:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:158:33)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !static_query<T, Property>::is_valid
                                ^
  Report hash: fcd5cefdcec8710474a93ec3a72a1f8a
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, query.hpp:158:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:158:33)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/query.hpp:177:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:177:33)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    !static_query<T, Property>::is_valid
                                ^
  Report hash: 120f460ca0d0977150693fc7bb6f4609
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, query.hpp:177:33: no definition found for '(unnamed struct at /usr/include/boost/asio/query.hpp:177:33)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in query.hpp

[LOW] /usr/include/boost/asio/require.hpp:151:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:151:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable
                           ^
  Report hash: 85737c9140cdb8d6add1b9cfde149e21
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, require.hpp:151:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:151:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require.hpp:176:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:176:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable
                           ^
  Report hash: 0d4ffc2c4b1c37d53294ade401836d6a
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, require.hpp:176:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:176:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require.hpp:198:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:198:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable
                           ^
  Report hash: bc2fd662aec4f894c148763a5223b5ab
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, require.hpp:198:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:198:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require.hpp:222:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:222:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: de3a4d17267602ff9fbf9b865639fc8b
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, require.hpp:222:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:222:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require.hpp:258:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:258:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 2f2feda367d72eb391a92b4ec2d263bd
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, require.hpp:258:48: no definition found for '(unnamed struct at /usr/include/boost/asio/require.hpp:258:48)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 5 defect(s) in require.hpp

[LOW] /usr/include/boost/asio/require_concept.hpp:149:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:149:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable_concept
                           ^
  Report hash: f5bdaa01caae4fc36e58e46857b32ef7
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, require_concept.hpp:149:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:149:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require_concept.hpp:169:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:169:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable_concept
                           ^
  Report hash: ff4b3bfb307d099918c340b9d0227ad5
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, require_concept.hpp:169:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:169:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

[LOW] /usr/include/boost/asio/require_concept.hpp:197:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:197:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
    decay<Property>::type::is_requirable_concept
                           ^
  Report hash: 8a1289ec2b687e0c263a335fe4cc73e9
  Steps:
    1, cuda_impl.h:32:9: a definition of 'cudaDeviceProp' is found here
    2, require_concept.hpp:197:28: no definition found for '(unnamed struct at /usr/include/boost/asio/require_concept.hpp:197:28)', but a definition with the same name 'cudaDeviceProp' found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in require_concept.hpp

[CRITICAL] /sep/src/quantum/types_serialization.cpp:1:10: 'types.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 926ca1eb2edc0b4e3d62632d3c586e61
  Steps:
    1, types_serialization.cpp:1:10: 'types.h' file not found

Found 1 defect(s) in types_serialization.cpp

[CRITICAL] /sep/src/quantum/processor.cpp:345:22: return type of out-of-line definition of 'sep::quantum::Processor::init' differs from that in the declaration [clang-diagnostic-error]
SEPResult Processor::init(GPUContext* gpu_context) { return impl_->init(gpu_context); }
                     ^
  Report hash: 588b40c3d7103847b6ce764ecd9bf1fe
  Steps:
    1, processor.h:23:15: previous declaration is here
    2, processor.cpp:345:22: return type of out-of-line definition of 'sep::quantum::Processor::init' differs from that in the declaration

[CRITICAL] /sep/src/quantum/processor.cpp:347:22: return type of out-of-line definition of 'sep::quantum::Processor::addPattern' differs from that in the declaration [clang-diagnostic-error]
SEPResult Processor::addPattern(const Pattern& pattern) { return impl_->addPattern(pattern); }
                     ^
  Report hash: b9e36f2ffe3b61055c7620c3ed1cb45b
  Steps:
    1, processor.h:26:15: previous declaration is here
    2, processor.cpp:347:22: return type of out-of-line definition of 'sep::quantum::Processor::addPattern' differs from that in the declaration

[CRITICAL] /sep/src/quantum/processor.cpp:348:22: return type of out-of-line definition of 'sep::quantum::Processor::removePattern' differs from that in the declaration [clang-diagnostic-error]
SEPResult Processor::removePattern(const std::string& pattern_id) { return impl_->removePattern(pattern_id); }
                     ^
  Report hash: f97b6e18809fcb23415fe89f6b79e1fc
  Steps:
    1, processor.h:27:15: previous declaration is here
    2, processor.cpp:348:22: return type of out-of-line definition of 'sep::quantum::Processor::removePattern' differs from that in the declaration

[CRITICAL] /sep/src/quantum/processor.cpp:349:22: return type of out-of-line definition of 'sep::quantum::Processor::updatePattern' differs from that in the declaration [clang-diagnostic-error]
SEPResult Processor::updatePattern(const std::string& pattern_id, const Pattern& pattern) { return impl_->updatePattern(pattern_id, pattern); }
                     ^
  Report hash: b06b88e6a5e79feac81f0b0bbe229191
  Steps:
    1, processor.h:28:15: previous declaration is here
    2, processor.cpp:349:22: return type of out-of-line definition of 'sep::quantum::Processor::updatePattern' differs from that in the declaration

[CRITICAL] /sep/src/quantum/processor.cpp:364:22: return type of out-of-line definition of 'sep::quantum::Processor::addRelationship' differs from that in the declaration [clang-diagnostic-error]
SEPResult Processor::addRelationship(const std::string& pattern_id1, const std::string& pattern_id2, float strength, RelationshipType type) {
                     ^
  Report hash: 0d1ccde5d06c8b64e2fe1cd8b50ae938
  Steps:
    1, processor.h:47:15: previous declaration is here
    2, processor.cpp:364:22: return type of out-of-line definition of 'sep::quantum::Processor::addRelationship' differs from that in the declaration

Found 5 defect(s) in processor.cpp

[LOW] /sep/include/memory/memory_tier_manager.hpp:63:5: class 'MemoryTierManager' can be made trivially destructible by defaulting the destructor on its first declaration [performance-trivially-destructible]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e6aa05158f1a009ee1d4df23d48cf44e
  Notes:
    1, memory_tier_manager.hpp:63:26: = default (fixit)
  Steps:
    1, memory_tier_manager.cpp:28:20: destructor definition is here
    2, memory_tier_manager.hpp:63:5: class 'MemoryTierManager' can be made trivially destructible by defaulting the destructor on its first declaration

Found 1 defect(s) in memory_tier_manager.hpp

[CRITICAL] /sep/include/memory/types.h:28:45: no member named 'pattern' in namespace 'sep'; did you mean 'quantum::Pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8890ba6b5ab040f72412686ca493f308
  Notes:
    1, types.h:28:45: quantum::Pattern (fixit)
  Steps:
    1, types.h:39:8: 'quantum::Pattern' declared here
    2, types.h:28:45: no member named 'pattern' in namespace 'sep'; did you mean 'quantum::Pattern'?

[CRITICAL] /sep/include/memory/types.h:28:59: no type named 'PatternData' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e6daee70e6f2c78ce1d25065b5bb9c27
  Steps:
    1, types.h:28:59: no type named 'PatternData' in 'sep::quantum::Pattern'

[CRITICAL] /sep/include/memory/types.h:31:19: no member named 'pattern' in namespace 'sep'; did you mean 'quantum::Pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 52a231bcc5159a607fe352b91c64c592
  Notes:
    1, types.h:31:19: quantum::Pattern (fixit)
  Steps:
    1, types.h:39:8: 'quantum::Pattern' declared here
    2, types.h:31:19: no member named 'pattern' in namespace 'sep'; did you mean 'quantum::Pattern'?

[CRITICAL] /sep/include/memory/types.h:31:33: no member named 'PatternData' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 78b2ab41c208871703c057c4c180457b
  Steps:
    1, types.h:31:33: no member named 'PatternData' in 'sep::quantum::Pattern'

[CRITICAL] /sep/include/memory/types.h:40:61: no member named 'pattern' in namespace 'sep'; did you mean 'quantum::Pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 613675ba07d1b67c752ddffd6255fb6d
  Notes:
    1, types.h:40:61: quantum::Pattern (fixit)
  Steps:
    1, types.h:39:8: 'quantum::Pattern' declared here
    2, types.h:40:61: no member named 'pattern' in namespace 'sep'; did you mean 'quantum::Pattern'?

[CRITICAL] /sep/include/memory/types.h:40:75: no member named 'PatternData' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 363c282ef8826b199474d3fc82eab43e
  Steps:
    1, types.h:40:75: no member named 'PatternData' in 'sep::quantum::Pattern'

[CRITICAL] /sep/include/memory/types.h:41:17: no member named 'pattern' in namespace 'sep'; did you mean 'quantum::Pattern'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bdf636d5c20063ba90345bf8bed36ac7
  Notes:
    1, types.h:41:17: quantum::Pattern (fixit)
  Steps:
    1, types.h:39:8: 'quantum::Pattern' declared here
    2, types.h:41:17: no member named 'pattern' in namespace 'sep'; did you mean 'quantum::Pattern'?

[CRITICAL] /sep/include/memory/types.h:41:31: no member named 'PatternData' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ea37892549b279acb2256dfbc860375e
  Steps:
    1, types.h:41:31: no member named 'PatternData' in 'sep::quantum::Pattern'

Found 8 defect(s) in types.h

[CRITICAL] /sep/include/quantum/quantum_processor_qfh.h:64:5: no type named 'MemoryTierEnum' in namespace 'sep'; did you mean simply 'MemoryTierEnum'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bdccf9fcff73a4ae0cad60daa4331c8e
  Notes:
    1, quantum_processor_qfh.h:64:5: MemoryTierEnum (fixit)
  Steps:
    1, types.h:10:12: 'MemoryTierEnum' declared here
    2, quantum_processor_qfh.h:64:5: no type named 'MemoryTierEnum' in namespace 'sep'; did you mean simply 'MemoryTierEnum'?

Found 1 defect(s) in quantum_processor_qfh.h

[CRITICAL] /sep/src/quantum/quantum_processor.cpp:19:1: use of undeclared identifier 'QuantumProcessorError' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 95ce5ff7af2ec35799b018d1cd709c19
  Steps:
    1, quantum_processor.cpp:19:1: use of undeclared identifier 'QuantumProcessorError'

[CRITICAL] /sep/src/quantum/quantum_processor.cpp:57:19: definition of implicitly declared destructor [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 81078ae9f276b7578616a4b50135c7de
  Steps:
    1, quantum_processor.cpp:57:19: definition of implicitly declared destructor

[CRITICAL] /sep/src/quantum/quantum_processor.cpp:61:19: out-of-line definition of 'QuantumProcessor' does not match any declaration in 'sep::quantum::QuantumProcessor' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b48e930ae5323bba6181eb1d990d4c56
  Steps:
    1, quantum_processor.h:10:7: QuantumProcessor defined here
    2, quantum_processor.cpp:61:19: out-of-line definition of 'QuantumProcessor' does not match any declaration in 'sep::quantum::QuantumProcessor'

[CRITICAL] /sep/src/quantum/quantum_processor.cpp:61:60: member initializer 'config_' does not name a non-static data member or base class [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d1bfdc9377ae8a9247ff4e1040c1d574
  Steps:
    1, quantum_processor.cpp:61:60: member initializer 'config_' does not name a non-static data member or base class

[CRITICAL] /sep/src/quantum/quantum_processor.cpp:62:5: use of undeclared identifier 'validateConfig'; did you mean 'updateConfig'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 197f2151d2b4da41c04326309ffc9158
  Notes:
    1, quantum_processor.cpp:62:5: updateConfig (fixit)
  Steps:
    1, processor.h:53:10: 'updateConfig' declared here
    2, quantum_processor.cpp:62:5: use of undeclared identifier 'validateConfig'; did you mean 'updateConfig'?

[CRITICAL] /sep/src/quantum/quantum_processor.cpp:63:5: use of undeclared identifier 'initializeProcessor' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c630aadd5f8dde62472e0ed695fc85c9
  Steps:
    1, quantum_processor.cpp:63:5: use of undeclared identifier 'initializeProcessor'

[CRITICAL] /sep/src/quantum/quantum_processor.cpp:64:5: use of undeclared identifier 'qbsa_processor_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 570c2488745842f902c34897c6763e58
  Steps:
    1, quantum_processor.cpp:64:5: use of undeclared identifier 'qbsa_processor_'

[CRITICAL] /sep/src/quantum/quantum_processor.cpp:64:23: no matching function for call to 'createQFHBasedQBSAProcessor' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 60e967f1cd3fd3d344028be100608678
  Steps:
    1, qbsa_qfh.h:10:32: candidate function not viable: requires single argument 'options', but no arguments were provided
    2, quantum_processor.cpp:64:23: no matching function for call to 'createQFHBasedQBSAProcessor'

[CRITICAL] /sep/src/quantum/quantum_processor.cpp:67:24: out-of-line definition of 'validateConfig' does not match any declaration in 'sep::quantum::QuantumProcessor' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f6e0b58dfb3a1e23ff180876e6d7a393
  Steps:
    1, quantum_processor.h:10:7: QuantumProcessor defined here
    2, quantum_processor.cpp:67:24: out-of-line definition of 'validateConfig' does not match any declaration in 'sep::quantum::QuantumProcessor'

[CRITICAL] /sep/src/quantum/quantum_processor.cpp:68:16: no member named 'coherence_threshold' in 'sep::quantum::QuantumProcessor::Config' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b85a0bdc5d5f1d99b7c7f8ea048c57dd
  Steps:
    1, quantum_processor.cpp:68:16: no member named 'coherence_threshold' in 'sep::quantum::QuantumProcessor::Config'

Found 10 defect(s) in quantum_processor.cpp

[LOW] /sep/include/compat/cuda_impl.h:27:8: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace '(global)' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f0e98b9148519d8a62db36d7394a7e88
  Steps:
    1, cuda_runtime.h:54:8: a declaration of 'CUstream_st' is found here
    2, cuda_impl.h:27:8: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace '(global)'

[LOW] /sep/include/compat/cuda_impl.h:28:8: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace '(global)' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 83141a6b2f893b75a4fa7be82c25d01e
  Steps:
    1, cuda_runtime.h:55:8: a declaration of 'CUevent_st' is found here
    2, cuda_impl.h:28:8: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace '(global)'

Found 2 defect(s) in cuda_impl.h

[LOW] /sep/include/compat/cuda_runtime.h:48:10: direct self-inclusion of header file 'cuda_runtime.h' [misc-header-include-cycle]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1d5a1eb47c7579928a382cabaa121702
  Steps:
    1, cuda_runtime.h:48:10: direct self-inclusion of header file 'cuda_runtime.h'

[LOW] /sep/include/compat/cuda_runtime.h:54:8: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d55b7ae2c66d655a2cc9459b2f5ed583
  Steps:
    1, cuda_impl.h:27:8: a declaration of 'CUstream_st' is found here
    2, cuda_runtime.h:54:8: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants'

[LOW] /sep/include/compat/cuda_runtime.h:55:8: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1ae5ff5ec9e0ef1e7a11e59892cedcf4
  Steps:
    1, cuda_impl.h:28:8: a declaration of 'CUevent_st' is found here
    2, cuda_runtime.h:55:8: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants'

Found 3 defect(s) in cuda_runtime.h

[CRITICAL] /sep/include/core/engine.h:12:10: 'config/types.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9a5b90eb104043827fd5e19ed3ddae8a
  Steps:
    1, engine.h:12:10: 'config/types.h' file not found

Found 1 defect(s) in engine.h

[CRITICAL] /sep/include/quantum/quantum_processor_qfh.h:64:12: no type named 'MemoryTierEnum' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 395b83f6449577d1b9e538c10504a979
  Steps:
    1, quantum_processor_qfh.h:64:12: no type named 'MemoryTierEnum' in namespace 'sep'

Found 1 defect(s) in quantum_processor_qfh.h

[CRITICAL] /sep/src/quantum/quantum_processor_qfh.cpp:11:1: unknown type name 'MemoryTierEnum' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 983cc7c485b0f1280128130a0a4a284e
  Steps:
    1, quantum_processor_qfh.cpp:11:1: unknown type name 'MemoryTierEnum'

[CRITICAL] /sep/src/quantum/quantum_processor_qfh.cpp:14:16: use of undeclared identifier 'MemoryTierEnum' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 287e07fbe97ba9e44cc6e37baa37c4f6
  Steps:
    1, quantum_processor_qfh.cpp:14:16: use of undeclared identifier 'MemoryTierEnum'

[CRITICAL] /sep/src/quantum/quantum_processor_qfh.cpp:16:16: use of undeclared identifier 'MemoryTierEnum' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: be4ac9a5dd7db041975daa4d8a80b40e
  Steps:
    1, quantum_processor_qfh.cpp:16:16: use of undeclared identifier 'MemoryTierEnum'

[CRITICAL] /sep/src/quantum/quantum_processor_qfh.cpp:17:12: use of undeclared identifier 'MemoryTierEnum' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8fb1655c7550df702475ad68c09bbcc7
  Steps:
    1, quantum_processor_qfh.cpp:17:12: use of undeclared identifier 'MemoryTierEnum'

Found 4 defect(s) in quantum_processor_qfh.cpp

[CRITICAL] /sep/include/memory/types.h:28:59: no type named 'PatternData' in namespace 'sep::pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3091a32a5b484d21153edce82dbd6943
  Steps:
    1, types.h:28:59: no type named 'PatternData' in namespace 'sep::pattern'

[CRITICAL] /sep/include/memory/types.h:31:33: no member named 'PatternData' in namespace 'sep::pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: de15ea8c42b2818c8c02ad9fd6b81f65
  Steps:
    1, types.h:31:33: no member named 'PatternData' in namespace 'sep::pattern'

[CRITICAL] /sep/include/memory/types.h:40:75: no member named 'PatternData' in namespace 'sep::pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 896d1d77920a7b77028edfb0e8814bd4
  Steps:
    1, types.h:40:75: no member named 'PatternData' in namespace 'sep::pattern'

[CRITICAL] /sep/include/memory/types.h:41:31: no member named 'PatternData' in namespace 'sep::pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 47713e170a3c3ec57a260b94edd8695a
  Steps:
    1, types.h:41:31: no member named 'PatternData' in namespace 'sep::pattern'

Found 4 defect(s) in types.h


----==== Severity Statistics ====----
----------------------------
Severity | Number of reports
----------------------------
CRITICAL |               304
LOW      |               146
MEDIUM   |                20
----------------------------
----=================----

----==== Checker Statistics ====----
-------------------------------------------------------------------------
Checker name                               | Severity | Number of reports
-------------------------------------------------------------------------
clang-diagnostic-error                     | CRITICAL |               304
misc-header-include-cycle                  | LOW      |                11
bugprone-forward-declaration-namespace     | LOW      |               129
clang-diagnostic-unused-parameter          | MEDIUM   |                 3
clang-diagnostic-mismatched-tags           | MEDIUM   |                 2
performance-trivially-destructible         | LOW      |                 4
clang-diagnostic-unused-private-field      | MEDIUM   |                 1
clang-diagnostic-double-promotion          | MEDIUM   |                 2
clang-diagnostic-reserved-macro-identifier | MEDIUM   |                10
bugprone-forwarding-reference-overload     | LOW      |                 2
bugprone-unused-return-value               | MEDIUM   |                 1
cert-err33-c                               | MEDIUM   |                 1
-------------------------------------------------------------------------
----=================----

----==== File Statistics ====----
------------------------------------------------------
File name                          | Number of reports
------------------------------------------------------
bridge.hpp                         |                 6
bridge_c.cpp                       |                 1
compression.h                      |                 2
pattern_bridge.h                   |                17
pattern_observer.h                 |                 2
bridge.h                           |                 5
api.cpp                            |                14
bridge.cpp                         |                13
sep_engine.h                       |                 4
base_types.h                       |                 6
sep_engine.cpp                     |                 2
unique_ptr.h                       |                 3
server.h                           |                 2
crow_isolation.h                   |                 1
http_parser_merged.h               |                 1
strand_executor_service.hpp        |                10
type_traits.hpp                    |                 1
executor.hpp                       |                 6
invocable_archetype.hpp            |                 1
is_applicable_property.hpp         |                 4
prefer.hpp                         |                10
require.hpp                        |                10
equality_comparable.hpp            |                 2
execute_member.hpp                 |                 2
query_static_constexpr_member.hpp  |                 3
gpu_context.cpp                    |                 1
error_handler.h                    |                 3
client.cpp                         |                 1
memory_tier.hpp                    |                 3
main.cpp                           |                 1
cuda_impl.h                        |                52
stream.h                           |                 3
http_request.h                     |                 1
pattern_visualization_pipeline.cpp |                 6
cuda_runtime.h                     |                25
dag_graph.cpp                      |                 1
rate_limit_middleware.h            |                 3
evolution.h                        |                 2
evolution.cpp                      |                 8
mesh_handler.cpp                   |                11
shim.h                             |                 2
error_handler.cpp                  |                 4
associated_allocator.hpp           |                 2
associated_cancellation_slot.hpp   |                 2
associated_executor.hpp            |                 2
associated_immediate_executor.hpp  |                 4
bulk_execute.hpp                   |                 6
connect.hpp                        |                 6
execute.hpp                        |                 4
executor.hpp                       |                 2
schedule.hpp                       |                 4
sender.hpp                         |                 2
set_done.hpp                       |                 2
set_error.hpp                      |                 2
set_value.hpp                      |                 2
start.hpp                          |                 2
submit.hpp                         |                 6
executor_work_guard.hpp            |                 2
prefer.hpp                         |                16
query.hpp                          |                 6
require.hpp                        |                10
require_concept.hpp                |                 6
processor.cpp                      |                15
memory_tier_manager.hpp            |                 2
engine.h                           |                 3
pattern_processor.cpp              |                 6
quantum_pattern_processor.h        |                11
quantum_pattern_processor.cpp      |                 8
redis_manager.cpp                  |                19
crow_error.cpp                     |                 5
crow_adapter.cpp                   |                 2
asio_isolation.h                   |                 2
auth_middleware.cpp                |                 1
socket_adaptors.h                  |                 3
mesh_handler.h                     |                 9
pattern_visualization_pipeline.h   |                 3
lock_free_rate_limiter.cpp         |                 1
manager.cpp                        |                 1
types.h                            |                16
server.cpp                         |                 1
relationship.h                     |                 2
types_serialization.cpp            |                 1
quantum_processor_qfh.h            |                 2
quantum_processor.cpp              |                10
quantum_processor_qfh.cpp          |                 4
------------------------------------------------------
----=================----

----======== Summary ========----
-----------------------------------------------
Number of processed analyzer result files | 163
Number of analyzer reports                | 470
-----------------------------------------------
----=================----
[WARNING 2025-06-23 07:40] - The following source file contents changed or missing since the latest analysis:
 - /sep/third_party/crow/socket_adaptors.h
 - /sep/include/memory/types.h
 - /sep/include/api/rate_limit_middleware.h
 - /sep/include/quantum/processor.h
 - /sep/include/blender/mesh_handler.h
 - /sep/src/api/crow_error.cpp
 - /sep/src/quantum/quantum_processor.cpp
 - /sep/include/quantum/types.h
 - /sep/include/memory/memory_tier_manager.hpp
 - /sep/include/quantum/quantum_processor_qfh.h
 - /sep/src/api/auth_middleware.cpp
 - /sep/src/memory/memory_tier_manager.cpp
 - /sep/include/core/engine.h
 - /sep/src/quantum/types_serialization.cpp
 - /sep/src/core/manager.cpp
 - /sep/include/compat/cuda_impl.h
 - /sep/include/compat/cuda_runtime.h
 - /sep/include/quantum/relationship.h
 - /sep/include/blender/pattern_visualization_pipeline.h
 - /sep/include/quantum/quantum_processor.h
 - /sep/include/api/sep_engine.h
 - /sep/src/api/lock_free_rate_limiter.cpp
 - /sep/include/blender/base_types.h
 - /sep/src/api/crow_adapter.cpp
 - /sep/src/quantum/evolution.cpp
 - /sep/src/api/server.cpp
 - /sep/src/api/sep_engine.cpp
 - /sep/src/quantum/quantum_processor_qfh.cpp
Please re-analyze your project to update the reports!
