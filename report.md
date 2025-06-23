----==== Severity Statistics ====----
----------------------------
Severity | Number of reports
----------------------------
LOW      |                46
CRITICAL |               244
MEDIUM   |                81
----------------------------
----=================----

----==== Checker Statistics ====----
-------------------------------------------------------------------------
Checker name                               | Severity | Number of reports
-------------------------------------------------------------------------
misc-header-include-cycle                  | LOW      |                10
clang-diagnostic-error                     | CRITICAL |               244
bugprone-forward-declaration-namespace     | LOW      |                36
clang-diagnostic-reserved-macro-identifier | MEDIUM   |                14
clang-diagnostic-double-promotion          | MEDIUM   |                55
clang-diagnostic-unused-parameter          | MEDIUM   |                 9
clang-diagnostic-unused-private-field      | MEDIUM   |                 1
clang-diagnostic-mismatched-tags           | MEDIUM   |                 2
-------------------------------------------------------------------------
----=================----

----==== File Statistics ====----
------------------------------------------------------
File name                          | Number of reports
------------------------------------------------------
base_types.h                       |                 5
cuda_common.h                      |                 1
cuda_impl.h                        |                 2
cuda_runtime.h                     |                24
cuda_unified_fix.h                 |                56
raii.h                             |                 4
raii.cpp                           |                 9
compression.h                      |                 2
pattern_bridge.h                   |                17
pattern_observer.h                 |                 2
memory_tier_manager.cpp            |                 8
stream.h                           |                 2
resource_predictor.h               |                 5
processor.cpp                      |                12
memory_tier.hpp                    |                 3
memory_tier.cpp                    |                11
http_request.h                     |                 1
dag_graph.cpp                      |                 1
stream_impl.h                      |                 5
stream.cpp                         |                 5
metrics_collector.cpp              |                10
gpu_context.cpp                    |                 1
pattern_visualization_pipeline.cpp |                 2
evolution.h                        |                 2
evolution.cpp                      |                 7
bridge.h                           |                 5
api.cpp                            |                14
shim.h                             |                 2
error_handler.h                    |                 3
error_handler.cpp                  |                 4
processor.cpp                      |                10
pattern_processor.cpp              |                 6
bridge.hpp                         |                 6
bridge.cpp                         |                13
bridge_c.cpp                       |                 1
quantum_pattern_processor.h        |                11
quantum_pattern_processor.cpp      |                 8
client.cpp                         |                 1
engine.h                           |                 2
redis_manager.cpp                  |                19
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
main.cpp                           |                 1
sep_engine.h                       |                 1
sep_engine.cpp                     |                 2
unique_ptr.h                       |                 3
rate_limit_middleware.h            |                 2
mesh_handler.cpp                   |                 2
kernels.cuh                        |                 5
------------------------------------------------------
----=================----

----======== Summary ========----
-----------------------------------------------
Number of processed analyzer result files | 98 
Number of analyzer reports                | 371
-----------------------------------------------

----=================----

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

[CRITICAL] /sep/include/compat/cuda_common.h:18:42: no type named 'cudaError_t' in namespace 'cuda_stub_constants'; did you mean simply 'cudaError_t'? [clang-diagnostic-error]
void logCudaError(const char* operation, cuda_stub_constants::cudaError_t error);
                                         ^
  Report hash: 1048f77b38a196c61955645a1f2e3974
  Notes:
    1, cuda_common.h:18:42: cudaError_t (fixit)
  Steps:
    1, cuda.h:19:13: 'cudaError_t' declared here
    2, cuda_common.h:18:42: no type named 'cudaError_t' in namespace 'cuda_stub_constants'; did you mean simply 'cudaError_t'?

Found 1 defect(s) in cuda_common.h

[LOW] /sep/include/compat/cuda_impl.h:42:16: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace '(global)' [bugprone-forward-declaration-namespace]
typedef struct CUstream_st* cudaStream_t;
               ^
  Report hash: e563db1f74d73b5ba7f84616a00506aa
  Steps:
    1, cuda_runtime.h:54:8: a declaration of 'CUstream_st' is found here
    2, cuda_impl.h:42:16: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace '(global)'

[LOW] /sep/include/compat/cuda_impl.h:43:16: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace '(global)' [bugprone-forward-declaration-namespace]
typedef struct CUevent_st* cudaEvent_t;
               ^
  Report hash: 69ce2cb14e5b8887a28860514a45aebe
  Steps:
    1, cuda_runtime.h:55:8: a declaration of 'CUevent_st' is found here
    2, cuda_impl.h:43:16: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace '(global)'

Found 2 defect(s) in cuda_impl.h

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
    1, cuda_impl.h:42:16: a declaration of 'CUstream_st' is found here
    2, cuda_runtime.h:54:8: declaration 'CUstream_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants'

[LOW] /sep/include/compat/cuda_runtime.h:55:8: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants' [bugprone-forward-declaration-namespace]
struct CUevent_st;
       ^
  Report hash: 1ae5ff5ec9e0ef1e7a11e59892cedcf4
  Steps:
    1, cuda_impl.h:43:16: a declaration of 'CUevent_st' is found here
    2, cuda_runtime.h:55:8: declaration 'CUevent_st' is never referenced, but a declaration with the same name found in another namespace 'cuda_stub_constants'

[CRITICAL] /sep/include/compat/cuda_runtime.h:60:3: typedef redefinition with different types ('enum cudaError' vs 'int') [clang-diagnostic-error]
} cudaError_t;
  ^
  Report hash: af4f042f117ab57403c9723d5c7b62d2
  Steps:
    1, cuda.h:19:13: previous definition is here
    2, cuda_runtime.h:60:3: typedef redefinition with different types ('enum cudaError' vs 'int')

[CRITICAL] /sep/include/compat/cuda_runtime.h:78:13: conflicting types for 'cudaStreamDestroy' [clang-diagnostic-error]
cudaError_t cudaStreamDestroy(cudaStream_t stream);
            ^
  Report hash: ab7a3dac97fdc3e107b6b2097674b6a1
  Steps:
    1, cuda.h:55:13: previous declaration is here
    2, cuda_runtime.h:78:13: conflicting types for 'cudaStreamDestroy'

[CRITICAL] /sep/include/compat/cuda_runtime.h:90:13: conflicting types for 'cudaMemcpyAsync' [clang-diagnostic-error]
cudaError_t cudaMemcpyAsync(void* dst, const void* src, size_t count, enum cudaMemcpyKind kind, cudaStream_t stream);
            ^
  Report hash: b6073229254a78923c86a3a12ab7476e
  Steps:
    1, cuda.h:62:13: previous declaration is here
    2, cuda_runtime.h:90:13: conflicting types for 'cudaMemcpyAsync'

[CRITICAL] /sep/include/compat/cuda_runtime.h:96:13: conflicting types for 'cudaGetDeviceProperties' [clang-diagnostic-error]
cudaError_t cudaGetDeviceProperties(struct cudaDeviceProp* prop, int device);
            ^
  Report hash: c570657abe6acafe512070a38e2cc61f
  Steps:
    1, cuda.h:51:13: previous declaration is here
    2, cuda_runtime.h:96:13: conflicting types for 'cudaGetDeviceProperties'

[CRITICAL] /sep/include/compat/cuda_runtime.h:109:13: conflicting types for 'cudaMallocManaged' [clang-diagnostic-error]
cudaError_t cudaMallocManaged(void** ptr, size_t size, unsigned int flags);
            ^
  Report hash: 08dcf3315e018c902b6c04d23d6c3b8b
  Steps:
    1, cuda.h:60:13: previous declaration is here
    2, cuda_runtime.h:109:13: conflicting types for 'cudaMallocManaged'

Found 18 defect(s) in cuda_runtime.h

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:9:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDA_NO_FP8_CONVERSIONS__ 1
        ^
  Report hash: b47a20e2861af27d716799fa4ee0aa98
  Steps:
    1, cuda_unified_fix.h:9:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:13:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDA_NO_HALF_OPERATORS__ 1
        ^
  Report hash: b7964d24e2c29e671f700a3428169a46
  Steps:
    1, cuda_unified_fix.h:13:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:17:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDA_NO_HALF2_OPERATORS__ 1
        ^
  Report hash: 85a550749f0a0f6519fa61863d4a69a1
  Steps:
    1, cuda_unified_fix.h:17:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:21:9: macro name is a reserved identifier [clang-diagnostic-reserved-macro-identifier]
#define __CUDA_NO_BFLOAT16_CONVERSIONS__ 1 
        ^
  Report hash: e06de4dcdebe2b7fb009d22de2c5b116
  Steps:
    1, cuda_unified_fix.h:21:9: macro name is a reserved identifier

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:188:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return asin((double)x);
           ^
  Report hash: eefbc8b0681c7e90a94b15d9bc7326fa
  Steps:
    1, cuda_unified_fix.h:188:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:191:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return atan((double)x);
           ^
  Report hash: 2bce4ae48b292444e1581e0ea28e7874
  Steps:
    1, cuda_unified_fix.h:191:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:194:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return atan2((double)y, (double)x);
           ^
  Report hash: 2b48db1b95d334b3c98f75c362775b0d
  Steps:
    1, cuda_unified_fix.h:194:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:197:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return ceil((double)x);
           ^
  Report hash: c961f8a2aada070b7a41bbf0f6011f33
  Steps:
    1, cuda_unified_fix.h:197:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:200:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return cos((double)x);
           ^
  Report hash: 1f6a42f74fe0ff981b5a0bfdd1d63424
  Steps:
    1, cuda_unified_fix.h:200:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:203:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return cosh((double)x);
           ^
  Report hash: 70a58ddc3fbc85db9ce5b3027c5f0e24
  Steps:
    1, cuda_unified_fix.h:203:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:206:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return exp((double)x);
           ^
  Report hash: 8fa767bbc6cfa6482e9675694ec326bf
  Steps:
    1, cuda_unified_fix.h:206:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:209:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return fabs((double)x);
           ^
  Report hash: 357f6cd24e82bffd6a8940448c4bb57d
  Steps:
    1, cuda_unified_fix.h:209:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:212:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return floor((double)x);
           ^
  Report hash: 28a46c503cb225cf58116ef7775653f6
  Steps:
    1, cuda_unified_fix.h:212:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:215:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return fmod((double)x, (double)y);
           ^
  Report hash: cb4f992f0a444eb348f87b553fd9de2d
  Steps:
    1, cuda_unified_fix.h:215:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:218:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return frexp((double)x, exp);
           ^
  Report hash: ec0c137b321eceeb0d1e938df8f881ce
  Steps:
    1, cuda_unified_fix.h:218:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:221:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return ldexp((double)x, exp);
           ^
  Report hash: 5f7a1942fb1771eba3bc063f821fa752
  Steps:
    1, cuda_unified_fix.h:221:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:228:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return log((double)x);
           ^
  Report hash: 15aeacd06d3df228b7cb20928d4c740c
  Steps:
    1, cuda_unified_fix.h:228:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:231:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return log10((double)x);
           ^
  Report hash: 91a61fa3f104c4dd52447dbf73bb6d8a
  Steps:
    1, cuda_unified_fix.h:231:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:236:13: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    *iptr = int_part;
            ^
  Report hash: e8f415a9ecda8b51982f2f6b8c083079
  Steps:
    1, cuda_unified_fix.h:236:13: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:237:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return result;
           ^
  Report hash: 4097e62211fa869cbe091433f40f0ead
  Steps:
    1, cuda_unified_fix.h:237:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:240:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return pow((double)x, (double)y);
           ^
  Report hash: a9b8ced7129b7d2921537ddc617e4075
  Steps:
    1, cuda_unified_fix.h:240:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:243:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return sin((double)x);
           ^
  Report hash: b2748a533e464ce935bfa6db224fa73a
  Steps:
    1, cuda_unified_fix.h:243:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:246:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return sinh((double)x);
           ^
  Report hash: 1c78254eb1b1d1c4fb2c33306f8fda05
  Steps:
    1, cuda_unified_fix.h:246:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:249:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return sqrt((double)x);
           ^
  Report hash: 71eb7f609243ed93120fa72176b3e14c
  Steps:
    1, cuda_unified_fix.h:249:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:252:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return tan((double)x);
           ^
  Report hash: ad0a8d96494ca4d5972a2e82e1b3fb06
  Steps:
    1, cuda_unified_fix.h:252:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:255:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return tanh((double)x);
           ^
  Report hash: 7621d5dec9e640c0f63b27c1844d89d9
  Steps:
    1, cuda_unified_fix.h:255:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:260:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return acosh((double)x);
           ^
  Report hash: bb04fb713a01e3892468911b7bad2a2f
  Steps:
    1, cuda_unified_fix.h:260:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:263:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return asinh((double)x);
           ^
  Report hash: ad17b87232fd16208dba0446b2cbea5c
  Steps:
    1, cuda_unified_fix.h:263:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:266:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return atanh((double)x);
           ^
  Report hash: 5ab6705345292604001ae6681c3569d4
  Steps:
    1, cuda_unified_fix.h:266:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:269:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return cbrt((double)x);
           ^
  Report hash: c0cb217c511b2eb481b077a1550c0d2a
  Steps:
    1, cuda_unified_fix.h:269:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:272:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return copysign((double)x, (double)y);
           ^
  Report hash: f9683809cdbfd3b18f7b5ef370d59c32
  Steps:
    1, cuda_unified_fix.h:272:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:275:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return erf((double)x);
           ^
  Report hash: 597bbe9ebbf956430b4876e9657c61d4
  Steps:
    1, cuda_unified_fix.h:275:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:278:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return erfc((double)x);
           ^
  Report hash: bac3a37f1c8358ce3e976ba5c3135f08
  Steps:
    1, cuda_unified_fix.h:278:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:281:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return exp2((double)x);
           ^
  Report hash: e2316b7904f6c41b42d7e2bbe5eb9699
  Steps:
    1, cuda_unified_fix.h:281:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:284:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return expm1((double)x);
           ^
  Report hash: 7c59ba2da237407389066bbe5138ea20
  Steps:
    1, cuda_unified_fix.h:284:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:287:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return fdim((double)x, (double)y);
           ^
  Report hash: a5e69eca3be3bde6ad2aa1d353d804b1
  Steps:
    1, cuda_unified_fix.h:287:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:290:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return fma((double)x, (double)y, (double)z);
           ^
  Report hash: 49fde53fca68b1dbf21f1eb7da16f483
  Steps:
    1, cuda_unified_fix.h:290:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:293:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return fmax((double)x, (double)y);
           ^
  Report hash: 1715599e1bac1ecee844472db7691ab3
  Steps:
    1, cuda_unified_fix.h:293:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:296:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return fmin((double)x, (double)y);
           ^
  Report hash: ed6fc4bd672109dfc54e3a942330c262
  Steps:
    1, cuda_unified_fix.h:296:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:299:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return hypot((double)x, (double)y);
           ^
  Report hash: b99f0e0eb29e9ccc2dc39c6cf40cda0f
  Steps:
    1, cuda_unified_fix.h:299:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:305:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return lgamma((double)x);
           ^
  Report hash: 0b6a107223b62447d90c397c2005a21f
  Steps:
    1, cuda_unified_fix.h:305:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:314:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return log1p((double)x);
           ^
  Report hash: 102c3fe54fb9ba4d98ca3582bc90402f
  Steps:
    1, cuda_unified_fix.h:314:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:317:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return log2((double)x);
           ^
  Report hash: ee4977af931e4f1a1d578eeda8c02b60
  Steps:
    1, cuda_unified_fix.h:317:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:320:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return logb((double)x);
           ^
  Report hash: d0c044bb2ad434afdceabe6af3fe35d9
  Steps:
    1, cuda_unified_fix.h:320:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:329:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return nan(tagp);
           ^
  Report hash: fa5064685ec2cd71b2594ee4ab043c34
  Steps:
    1, cuda_unified_fix.h:329:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:332:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return nearbyint((double)x);
           ^
  Report hash: 964267d1ed3a8915ce12c75c8d2489ec
  Steps:
    1, cuda_unified_fix.h:332:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:335:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return nextafter((double)x, (double)y);
           ^
  Report hash: a8d66c6a0f82a0cf0546fffbc9a11c35
  Steps:
    1, cuda_unified_fix.h:335:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:344:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return nextafter((double)x, (double)y);
           ^
  Report hash: a8d66c6a0f82a0cf0546fffbc9a11c35
  Steps:
    1, cuda_unified_fix.h:344:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:347:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return remainder((double)x, (double)y);
           ^
  Report hash: 4724e77661a08231cc2a96fabfbbc1e6
  Steps:
    1, cuda_unified_fix.h:347:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:350:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return remquo((double)x, (double)y, quo);
           ^
  Report hash: 9376d0496bbd8b745a5af793b5e879a9
  Steps:
    1, cuda_unified_fix.h:350:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:359:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return nearbyint((double)x);
           ^
  Report hash: 964267d1ed3a8915ce12c75c8d2489ec
  Steps:
    1, cuda_unified_fix.h:359:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:362:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return round((double)x);
           ^
  Report hash: 800d94a104ff4f0b3a027c8b287a937f
  Steps:
    1, cuda_unified_fix.h:362:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:365:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return scalbln((double)x, n);
           ^
  Report hash: 1349c0a610bb360d8acebda58bcf52b7
  Steps:
    1, cuda_unified_fix.h:365:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:368:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return scalbn((double)x, n);
           ^
  Report hash: 1e90f39c653a2bea31f23a439218f168
  Steps:
    1, cuda_unified_fix.h:368:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:371:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return tgamma((double)x);
           ^
  Report hash: 866b1fe93df7712b96cc62b5875d3964
  Steps:
    1, cuda_unified_fix.h:371:12: implicit conversion increases floating-point precision: 'double' to 'long double'

[MEDIUM] /sep/include/compat/cuda_unified_fix.h:374:12: implicit conversion increases floating-point precision: 'double' to 'long double' [clang-diagnostic-double-promotion]
    return trunc((double)x);
           ^
  Report hash: cdd9f9dd1dd8d526a6bc9fe275af7b25
  Steps:
    1, cuda_unified_fix.h:374:12: implicit conversion increases floating-point precision: 'double' to 'long double'

Found 56 defect(s) in cuda_unified_fix.h

[CRITICAL] /sep/include/compat/raii.h:17:30: no type named 'StreamFlags' in namespace 'sep' [clang-diagnostic-error]
  explicit StreamRAII(::sep::StreamFlags flags = ::sep::StreamFlags::Default);
                             ^
  Report hash: 529fe1416d274a6b8d733f163153fc87
  Steps:
    1, raii.h:17:30: no type named 'StreamFlags' in namespace 'sep'

[CRITICAL] /sep/include/compat/raii.h:17:57: no member named 'StreamFlags' in namespace 'sep' [clang-diagnostic-error]
  explicit StreamRAII(::sep::StreamFlags flags = ::sep::StreamFlags::Default);
                                                        ^
  Report hash: 1674993516fb18ddd11cb3c8a3c839c4
  Steps:
    1, raii.h:17:57: no member named 'StreamFlags' in namespace 'sep'

[CRITICAL] /sep/include/compat/raii.h:45:3: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'? [clang-diagnostic-error]
  cudaEvent_t get() const { return event_; }
  ^
  Report hash: 7561bdf6b61e7ecafa4a3c9b9c486d09
  Notes:
    1, raii.h:45:3: cuda_stub_constants::cudaEvent_t (fixit)
  Steps:
    1, cuda_impl.h:43:28: 'cuda_stub_constants::cudaEvent_t' declared here
    2, raii.h:45:3: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'?

[CRITICAL] /sep/include/compat/raii.h:50:3: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'? [clang-diagnostic-error]
  cudaEvent_t event_{nullptr};
  ^
  Report hash: 904a281ab2e4a7986bbb52bb1e526b9e
  Notes:
    1, raii.h:50:3: cuda_stub_constants::cudaEvent_t (fixit)
  Steps:
    1, cuda_impl.h:43:28: 'cuda_stub_constants::cudaEvent_t' declared here
    2, raii.h:50:3: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'?

Found 4 defect(s) in raii.h

[CRITICAL] /sep/src/compat/raii.cpp:33:29: no type named 'StreamFlags' in namespace 'sep' [clang-diagnostic-error]
StreamRAII::StreamRAII(sep::StreamFlags flags) {
                            ^
  Report hash: 0c5a767822bc03e9a939ac88fd5828a1
  Steps:
    1, raii.cpp:33:29: no type named 'StreamFlags' in namespace 'sep'

[CRITICAL] /sep/src/compat/raii.cpp:34:46: no member named 'StreamFlags' in namespace 'sep' [clang-diagnostic-error]
    unsigned int cuda_flags = (flags == sep::StreamFlags::NonBlocking) ? cudaStreamNonBlocking : cudaStreamDefault;
                                             ^
  Report hash: 21380f1df7de8d941f5835e75637a60b
  Steps:
    1, raii.cpp:34:46: no member named 'StreamFlags' in namespace 'sep'

[CRITICAL] /sep/src/compat/raii.cpp:34:74: use of undeclared identifier 'cudaStreamNonBlocking' [clang-diagnostic-error]
    unsigned int cuda_flags = (flags == sep::StreamFlags::NonBlocking) ? cudaStreamNonBlocking : cudaStreamDefault;
                                                                         ^
  Report hash: 503b2b903a5bf28997ffe43d7d79ca4d
  Steps:
    1, raii.cpp:34:74: use of undeclared identifier 'cudaStreamNonBlocking'

[CRITICAL] /sep/src/compat/raii.cpp:34:98: use of undeclared identifier 'cudaStreamDefault'; did you mean 'cudaStreamDestroy'? [clang-diagnostic-error]
    unsigned int cuda_flags = (flags == sep::StreamFlags::NonBlocking) ? cudaStreamNonBlocking : cudaStreamDefault;
                                                                                                 ^
  Report hash: b9b50bfa10d158fa6af59585d16c7437
  Notes:
    1, raii.cpp:34:98: cudaStreamDestroy (fixit)
  Steps:
    1, cuda.h:55:13: 'cudaStreamDestroy' declared here
    2, raii.cpp:34:98: use of undeclared identifier 'cudaStreamDefault'; did you mean 'cudaStreamDestroy'?

[CRITICAL] /sep/src/compat/raii.cpp:35:23: no matching function for call to 'cudaStreamCreateWithFlags' [clang-diagnostic-error]
    cudaError_t err = cudaStreamCreateWithFlags(&stream_, cuda_flags);
                      ^
  Report hash: 05dd868e7fdba08246a20f014de2b345
  Steps:
    1, cuda_runtime.h:77:13: candidate function not viable: no known conversion from 'cudaStream_t *' (aka 'void **') to 'cudaStream_t *' (aka 'CUstream_st **') for 1st argument
    2, raii.cpp:35:23: no matching function for call to 'cudaStreamCreateWithFlags'

[CRITICAL] /sep/src/compat/raii.cpp:70:9: no matching function for call to 'cudaStreamSynchronize_ptsz' [clang-diagnostic-error]
        cudaStreamSynchronize(stream_);
        ^
  Report hash: f28b70bf51d54c7aee80561788608788
  Steps:
    1, cuda_runtime.h:117:31: expanded from macro 'cudaStreamSynchronize'
    2, cuda_runtime.h:75:13: candidate function not viable: cannot convert argument of incomplete type 'const cudaStream_t' (aka 'void *const') to 'cudaStream_t' (aka 'CUstream_st *') for 1st argument
    3, raii.cpp:70:9: no matching function for call to 'cudaStreamSynchronize_ptsz'

[CRITICAL] /sep/src/compat/raii.cpp:75:23: no matching function for call to 'cudaEventCreate' [clang-diagnostic-error]
    cudaError_t err = cudaEventCreate(&event_);
                      ^
  Report hash: 9df802cdb7face7bda124418ad1bd039
  Steps:
    1, cuda_runtime.h:81:13: candidate function not viable: no known conversion from 'cuda_stub_constants::cudaEvent_t *' (aka 'cuda_stub_constants::CUevent_st **') to 'cudaEvent_t *' (aka 'CUevent_st **') for 1st argument
    2, raii.cpp:75:23: no matching function for call to 'cudaEventCreate'

[CRITICAL] /sep/src/compat/raii.cpp:91:20: no matching function for call to 'cudaEventDestroy' [clang-diagnostic-error]
        CUDA_CHECK(cudaEventDestroy(event_));
                   ^
  Report hash: 9098cbd7b6992d372c5b1cdf7f450f99
  Steps:
    1, cuda_helpers.h:29:29: expanded from macro 'CUDA_CHECK'
    2, cuda_runtime.h:82:13: candidate function not viable: cannot convert argument of incomplete type 'cuda_stub_constants::cudaEvent_t' (aka 'cuda_stub_constants::CUevent_st *') to 'cudaEvent_t' (aka 'CUevent_st *') for 1st argument
    3, raii.cpp:91:20: no matching function for call to 'cudaEventDestroy'

[CRITICAL] /sep/src/compat/raii.cpp:101:24: no matching function for call to 'cudaEventDestroy' [clang-diagnostic-error]
            CUDA_CHECK(cudaEventDestroy(event_));
                       ^
  Report hash: a12e8e3fe08ef930a46b289c2d644a66
  Steps:
    1, cuda_helpers.h:29:29: expanded from macro 'CUDA_CHECK'
    2, cuda_runtime.h:82:13: candidate function not viable: cannot convert argument of incomplete type 'cuda_stub_constants::cudaEvent_t' (aka 'cuda_stub_constants::CUevent_st *') to 'cudaEvent_t' (aka 'CUevent_st *') for 1st argument
    3, raii.cpp:101:24: no matching function for call to 'cudaEventDestroy'

Found 9 defect(s) in raii.cpp

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

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:178:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
MemoryTier* MemoryTierManager::determineTier(float coherence, float stability, int generation_count) {
                                                                    ^
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:178:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:208:38: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
            if (it->second < config_.demote_threshold)
                                     ^
  Report hash: 474cc343d2d8bde6f322968e111612fc
  Steps:
    1, memory_tier_manager.cpp:208:38: implicit conversion increases floating-point precision: 'float' to 'double'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:228:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
    pattern->metadata.coherence_score = data->coherence;
             ^
  Report hash: 22578aad3d8b17e3124a9ac3f3993c05
  Steps:
    1, memory_tier_manager.cpp:228:14: no member named 'metadata' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:229:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
    pattern->metadata.stability_score = data->stability;
             ^
  Report hash: 38f223a02190f76379588c8f82e127aa
  Steps:
    1, memory_tier_manager.cpp:229:14: no member named 'metadata' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:230:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
    pattern->metadata.generation_count = data->generation;
             ^
  Report hash: 0a4b256439c8305744ebf9d9fb6229cb
  Steps:
    1, memory_tier_manager.cpp:230:14: no member named 'metadata' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:245:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
    pattern->metadata.coherence_score = data->coherence;
             ^
  Report hash: 22578aad3d8b17e3124a9ac3f3993c05
  Steps:
    1, memory_tier_manager.cpp:245:14: no member named 'metadata' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:246:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
    pattern->metadata.stability_score = data->stability;
             ^
  Report hash: 38f223a02190f76379588c8f82e127aa
  Steps:
    1, memory_tier_manager.cpp:246:14: no member named 'metadata' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:247:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
    pattern->metadata.generation_count = data->generation;
             ^
  Report hash: 0a4b256439c8305744ebf9d9fb6229cb
  Steps:
    1, memory_tier_manager.cpp:247:14: no member named 'metadata' in 'sep::quantum::Pattern'

Found 8 defect(s) in memory_tier_manager.cpp

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

[CRITICAL] /sep/include/quantum/resource_predictor.h:14:56: unknown type name 'Batch' [clang-diagnostic-error]
  virtual ResourcePredictor predictResourceNeeds(const Batch &batch) = 0;
                                                       ^
  Report hash: 1d721bae7af1896800aca397b5433653
  Steps:
    1, resource_predictor.h:14:56: unknown type name 'Batch'

[CRITICAL] /sep/include/quantum/resource_predictor.h:17:44: unknown type name 'Batch' [clang-diagnostic-error]
  virtual void recordBatchProcessing(const Batch &batch,
                                           ^
  Report hash: 172883d2fa3ee5da7c0401e71b130c69
  Steps:
    1, resource_predictor.h:17:44: unknown type name 'Batch'

[CRITICAL] /sep/include/quantum/resource_predictor.h:22:11: unknown type name 'ResourceState' [clang-diagnostic-error]
  virtual ResourceState getCurrentState() const = 0;
          ^
  Report hash: 4d1645ff7d866990c0b7b53c6a4422ca
  Steps:
    1, resource_predictor.h:22:11: unknown type name 'ResourceState'

[CRITICAL] /sep/include/quantum/resource_predictor.h:32:41: unknown type name 'UsagePattern' [clang-diagnostic-error]
  virtual void recordUsagePattern(const UsagePattern &pattern) = 0;
                                        ^
  Report hash: a8cc273f3a3e21305ab2e555073a1663
  Steps:
    1, resource_predictor.h:32:41: unknown type name 'UsagePattern'

[CRITICAL] /sep/include/quantum/resource_predictor.h:35:11: unknown type name 'ResourceMetrics' [clang-diagnostic-error]
  virtual ResourceMetrics getResourceMetrics() const = 0;
          ^
  Report hash: 83627b8b1e2f98484963edb93b19a86f
  Steps:
    1, resource_predictor.h:35:11: unknown type name 'ResourceMetrics'

Found 5 defect(s) in resource_predictor.h

[CRITICAL] /sep/src/context/processor.cpp:75:12: no member named 'success' in 'sep::ollama::EmbeddingRequest' [clang-diagnostic-error]
    result.success = false;
           ^
  Report hash: e337d349aff4d67ae8b25c81da171cd0
  Steps:
    1, processor.cpp:75:12: no member named 'success' in 'sep::ollama::EmbeddingRequest'

[CRITICAL] /sep/src/context/processor.cpp:76:12: no member named 'error' in 'sep::ollama::EmbeddingRequest' [clang-diagnostic-error]
    result.error = sep::shim::string(msg.c_str());
           ^
  Report hash: a10bc584450d14312d0d3bfecad12073
  Steps:
    1, processor.cpp:76:12: no member named 'error' in 'sep::ollama::EmbeddingRequest'

[CRITICAL] /sep/src/context/processor.cpp:80:1: unknown type name 'SimilarityResult' [clang-diagnostic-error]
SimilarityResult makeSimilarityError(const std::string& msg)
^
  Report hash: f08209d74d7d38ec9ba61bd7401ac1d6
  Steps:
    1, processor.cpp:80:1: unknown type name 'SimilarityResult'

[CRITICAL] /sep/src/context/processor.cpp:82:5: unknown type name 'SimilarityResult' [clang-diagnostic-error]
    SimilarityResult result;
    ^
  Report hash: ac3b86f7ebd1e6cea614ddaee88a2bd1
  Steps:
    1, processor.cpp:82:5: unknown type name 'SimilarityResult'

[CRITICAL] /sep/src/context/processor.cpp:88:1: unknown type name 'BlendResult' [clang-diagnostic-error]
BlendResult makeBlendError(const std::string& msg)
^
  Report hash: d84e0890d31a322d4e63ca6c560f18ad
  Steps:
    1, processor.cpp:88:1: unknown type name 'BlendResult'

[CRITICAL] /sep/src/context/processor.cpp:90:5: unknown type name 'BlendResult' [clang-diagnostic-error]
    BlendResult result;
    ^
  Report hash: 34452909a7ad4281dcfcc81fefece865
  Steps:
    1, processor.cpp:90:5: unknown type name 'BlendResult'

[CRITICAL] /sep/src/context/processor.cpp:177:34: unknown type name 'ProcessOptions' [clang-diagnostic-error]
    explicit ProcessorImpl(const ProcessOptions& options)
                                 ^
  Report hash: c2c21d6ec628aede79c0b930b6172431
  Steps:
    1, processor.cpp:177:34: unknown type name 'ProcessOptions'

[CRITICAL] /sep/src/context/processor.cpp:178:11: member initializer 'options_' does not name a non-static data member or base class [clang-diagnostic-error]
        : options_(options)
          ^
  Report hash: 1af144d9becab7a2432ae32d4eca0a4c
  Steps:
    1, processor.cpp:178:11: member initializer 'options_' does not name a non-static data member or base class

[CRITICAL] /sep/src/context/processor.cpp:184:56: no member named 'PatternProcessor' in namespace 'sep::pattern' [clang-diagnostic-error]
        , pattern_processor_(std::make_unique<pattern::PatternProcessor>())
                                                       ^
  Report hash: 350fef00dd5153e7a9553ce159731dad
  Steps:
    1, processor.cpp:184:56: no member named 'PatternProcessor' in namespace 'sep::pattern'

[CRITICAL] /sep/src/context/processor.cpp:222:13: unknown type name 'ValidationResult' [clang-diagnostic-error]
            ValidationResult validation = validateContext(context);
            ^
  Report hash: 0949dbdfb21da609b3c72183c08d51fb
  Steps:
    1, processor.cpp:222:13: unknown type name 'ValidationResult'

[CRITICAL] /sep/src/context/processor.cpp:299:50: unknown type name 'Batch' [clang-diagnostic-error]
    quantum::ProcessingResult processBatch(const Batch& batch) override
                                                 ^
  Report hash: 7edb8fa254c6448d1a18e1dd078a68c4
  Steps:
    1, processor.cpp:299:50: unknown type name 'Batch'

[CRITICAL] /sep/src/context/processor.cpp:341:5: unknown type name 'ValidationResult' [clang-diagnostic-error]
    ValidationResult validateContext(const fmt::context& context) override
    ^
  Report hash: 062cf6ad4a8157d8f2ac4751ac5a6aba
  Steps:
    1, processor.cpp:341:5: unknown type name 'ValidationResult'

Found 12 defect(s) in processor.cpp

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

[CRITICAL] /sep/src/memory/memory_tier.cpp:165:83: use of undeclared identifier 'cudaMemcpyDefault'; did you mean 'cuda_stub_constants::cudaMemcpyDefault'? [clang-diagnostic-error]
                cudaError_t err = cudaMemcpy(new_location, block.ptr, block.size, cudaMemcpyDefault);
                                                                                  ^
  Report hash: 9a6ee42988dc718411ce46c7c57d78ad
  Notes:
    1, memory_tier.cpp:165:83: cuda_stub_constants::cudaMemcpyDefault (fixit)
  Steps:
    1, cuda_impl.h:51:5: 'cuda_stub_constants::cudaMemcpyDefault' declared here
    2, memory_tier.cpp:165:83: use of undeclared identifier 'cudaMemcpyDefault'; did you mean 'cuda_stub_constants::cudaMemcpyDefault'?

[CRITICAL] /sep/src/memory/memory_tier.cpp:389:18: out-of-line definition of 'canAcceptPattern' does not match any declaration in 'sep::memory::MemoryTier' [clang-diagnostic-error]
bool MemoryTier::canAcceptPattern(const sep::pattern::PatternData& pattern) const {
                 ^
  Report hash: c9b839062d252af44648d316cf8e3b7e
  Steps:
    1, memory_tier.hpp:68:7: MemoryTier defined here
    2, memory_tier.hpp:109:33: type of 1st parameter of member declaration does not match definition ('const persistence::PatternData &' vs 'const sep::pattern::PatternData &')
    3, memory_tier.cpp:389:18: out-of-line definition of 'canAcceptPattern' does not match any declaration in 'sep::memory::MemoryTier'

[CRITICAL] /sep/src/memory/memory_tier.cpp:392:16: member access into incomplete type 'const sep::pattern::PatternData' [clang-diagnostic-error]
    if (pattern.coherence < m_coherence_threshold)
               ^
  Report hash: ed4ee32248cffe079f734fc46e49459a
  Steps:
    1, memory_tier_manager.hpp:33:8: forward declaration of 'sep::pattern::PatternData'
    2, memory_tier.cpp:392:16: member access into incomplete type 'const sep::pattern::PatternData'

[CRITICAL] /sep/src/memory/memory_tier.cpp:396:16: member access into incomplete type 'const sep::pattern::PatternData' [clang-diagnostic-error]
    if (pattern.memory_tier != MemoryTierEnum::STM && pattern.generation < m_min_generations)
               ^
  Report hash: a32ddc85e4af4692fdb00c56b3ce6b0b
  Steps:
    1, memory_tier_manager.hpp:33:8: forward declaration of 'sep::pattern::PatternData'
    2, memory_tier.cpp:396:16: member access into incomplete type 'const sep::pattern::PatternData'

[CRITICAL] /sep/src/memory/memory_tier.cpp:396:62: member access into incomplete type 'const sep::pattern::PatternData' [clang-diagnostic-error]
    if (pattern.memory_tier != MemoryTierEnum::STM && pattern.generation < m_min_generations)
                                                             ^
  Report hash: 91d1ebe43ddf06536c6648e8e3333f3c
  Steps:
    1, memory_tier_manager.hpp:33:8: forward declaration of 'sep::pattern::PatternData'
    2, memory_tier.cpp:396:62: member access into incomplete type 'const sep::pattern::PatternData'

[CRITICAL] /sep/src/memory/memory_tier.cpp:401:18: out-of-line definition of 'addPattern' does not match any declaration in 'sep::memory::MemoryTier' [clang-diagnostic-error]
void MemoryTier::addPattern(size_t id, sep::pattern::PatternData pattern) {
                 ^
  Report hash: ff0eb9c2d5c0f6b3bdee73021ead8b67
  Steps:
    1, memory_tier.hpp:68:7: MemoryTier defined here
    2, memory_tier.hpp:110:32: type of 2nd parameter of member declaration does not match definition ('persistence::PatternData' vs 'sep::pattern::PatternData')
    3, memory_tier.cpp:401:18: out-of-line definition of 'addPattern' does not match any declaration in 'sep::memory::MemoryTier'

[CRITICAL] /sep/src/memory/memory_tier.cpp:401:66: variable has incomplete type 'sep::pattern::PatternData' [clang-diagnostic-error]
void MemoryTier::addPattern(size_t id, sep::pattern::PatternData pattern) {
                                                                 ^
  Report hash: 5d44c9e036cedf27e3fe43d260981c32
  Steps:
    1, memory_tier_manager.hpp:33:8: forward declaration of 'sep::pattern::PatternData'
    2, memory_tier.cpp:401:66: variable has incomplete type 'sep::pattern::PatternData'

[CRITICAL] /sep/src/memory/memory_tier.cpp:413:46: return type of out-of-line definition of 'sep::memory::MemoryTier::getPattern' differs from that in the declaration [clang-diagnostic-error]
const sep::pattern::PatternData* MemoryTier::getPattern(size_t id) const {
                                             ^
  Report hash: cfcd965b41e08401e76e6277634b786b
  Steps:
    1, memory_tier.hpp:112:37: previous declaration is here
    2, memory_tier.cpp:413:46: return type of out-of-line definition of 'sep::memory::MemoryTier::getPattern' differs from that in the declaration

[CRITICAL] /sep/src/memory/memory_tier.cpp:415:12: cannot initialize return object of type 'const sep::pattern::PatternData *' with an rvalue of type 'const sep::persistence::PatternData *' [clang-diagnostic-error]
    return it == m_patterns.end() ? nullptr : &it->second;
           ^
  Report hash: 5b3c01df262b8aa2010ece26380cb883
  Steps:
    1, memory_tier.cpp:415:12: cannot initialize return object of type 'const sep::pattern::PatternData *' with an rvalue of type 'const sep::persistence::PatternData *'

[CRITICAL] /sep/src/memory/memory_tier.cpp:418:40: return type of out-of-line definition of 'sep::memory::MemoryTier::getPattern' differs from that in the declaration [clang-diagnostic-error]
sep::pattern::PatternData* MemoryTier::getPattern(size_t id) {
                                       ^
  Report hash: 483d43ee525c2987e1b52be30d69f091
  Steps:
    1, memory_tier.hpp:113:31: previous declaration is here
    2, memory_tier.cpp:418:40: return type of out-of-line definition of 'sep::memory::MemoryTier::getPattern' differs from that in the declaration

[CRITICAL] /sep/src/memory/memory_tier.cpp:420:12: cannot initialize return object of type 'sep::pattern::PatternData *' with an rvalue of type 'sep::persistence::PatternData *' [clang-diagnostic-error]
    return it == m_patterns.end() ? nullptr : &it->second;
           ^
  Report hash: 9880abb8533a4dd2947af474ef1f3a88
  Steps:
    1, memory_tier.cpp:420:12: cannot initialize return object of type 'sep::pattern::PatternData *' with an rvalue of type 'sep::persistence::PatternData *'

Found 11 defect(s) in memory_tier.cpp

[CRITICAL] /sep/third_party/crow/http_request.h:25:15: no member named 'contains' in 'std::unordered_multimap<std::basic_string<char>, std::basic_string<char>, crow::ci_hash, crow::ci_key_eq>' [clang-diagnostic-error]
  if (headers.contains(key)) {
              ^
  Report hash: e35216a3c457ec75f05b98d2803e2800
  Steps:
    1, http_request.h:90:18: in instantiation of function template specialization 'crow::get_header_value<std::unordered_multimap<std::basic_string<char>, std::basic_string<char>, crow::ci_hash, crow::ci_key_eq>>' requested here
    2, http_request.h:25:15: no member named 'contains' in 'std::unordered_multimap<std::basic_string<char>, std::basic_string<char>, crow::ci_hash, crow::ci_key_eq>'

Found 1 defect(s) in http_request.h

[CRITICAL] /sep/src/core/dag_graph.cpp:73:19: no member named 'contains' in 'std::unordered_map<unsigned long, sep::dag::DagNode>' [clang-diagnostic-error]
    return nodes_.contains(id);
                  ^
  Report hash: 1a2cbc147b2119557fc18ce0932c89b8
  Steps:
    1, dag_graph.cpp:73:19: no member named 'contains' in 'std::unordered_map<unsigned long, sep::dag::DagNode>'

Found 1 defect(s) in dag_graph.cpp

[CRITICAL] /sep/include/compat/stream_impl.h:65:11: no matching function for call to 'cudaStreamWaitEvent' [clang-diagnostic-error]
          cudaStreamWaitEvent(stream_, static_cast<cudaEvent_t>(event), 0);
          ^
  Report hash: fd3d2f21fa79507d14cb474688cc11fa
  Steps:
    1, cuda_impl.h:397:20: candidate function not viable: requires 2 arguments, but 3 were provided
    2, stream_impl.h:65:11: no matching function for call to 'cudaStreamWaitEvent'

[CRITICAL] /sep/include/compat/stream_impl.h:65:52: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'? [clang-diagnostic-error]
          cudaStreamWaitEvent(stream_, static_cast<cudaEvent_t>(event), 0);
                                                   ^
  Report hash: ed91806116175b4eb6b3531293a93bbb
  Notes:
    1, stream_impl.h:65:52: cuda_stub_constants::cudaEvent_t (fixit)
  Steps:
    1, cuda_impl.h:43:28: 'cuda_stub_constants::cudaEvent_t' declared here
    2, stream_impl.h:65:52: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'?

[CRITICAL] /sep/include/compat/stream_impl.h:75:11: use of undeclared identifier 'cudaEventRecord'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventRecord'? [clang-diagnostic-error]
          cudaEventRecord(static_cast<cudaEvent_t>(event), stream_);
          ^
  Report hash: eead8ce420214352e79a6c152d1d2b70
  Notes:
    1, stream_impl.h:75:11: cuda_stub_constants::cuda_stub_constants::cudaEventRecord (fixit)
  Steps:
    1, cuda_impl.h:199:20: 'cuda_stub_constants::cuda_stub_constants::cudaEventRecord' declared here
    2, stream_impl.h:75:11: use of undeclared identifier 'cudaEventRecord'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventRecord'?

[CRITICAL] /sep/include/compat/stream_impl.h:75:39: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'? [clang-diagnostic-error]
          cudaEventRecord(static_cast<cudaEvent_t>(event), stream_);
                                      ^
  Report hash: 617e8c5d81e5e7f07a40dd9174309770
  Notes:
    1, stream_impl.h:75:39: cuda_stub_constants::cudaEvent_t (fixit)
  Steps:
    1, cuda_impl.h:43:28: 'cuda_stub_constants::cudaEvent_t' declared here
    2, stream_impl.h:75:39: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'?

[CRITICAL] /sep/include/compat/stream_impl.h:75:60: cannot initialize a parameter of type 'cudaStream_t' (aka 'cuda_stub_constants::CUstream_st *') with an lvalue of type 'cudaStream_t' (aka 'void *') [clang-diagnostic-error]
          cudaEventRecord(static_cast<cudaEvent_t>(event), stream_);
                                                           ^
  Report hash: cd2a80de7e2b87bcedc9a019c5e4685d
  Steps:
    1, cuda_impl.h:199:68: passing argument to parameter 'stream' here
    2, stream_impl.h:75:60: cannot initialize a parameter of type 'cudaStream_t' (aka 'cuda_stub_constants::CUstream_st *') with an lvalue of type 'cudaStream_t' (aka 'void *')

Found 5 defect(s) in stream_impl.h

[CRITICAL] /sep/src/compat/core/stream.cpp:30:45: no type named 'StreamFlags' in namespace 'sep' [clang-diagnostic-error]
std::shared_ptr<Stream> Stream::create(sep::StreamFlags flags) {
                                            ^
  Report hash: 42d6ab88709d3956eafa28b2a5a689bc
  Steps:
    1, stream.cpp:30:45: no type named 'StreamFlags' in namespace 'sep'

[CRITICAL] /sep/src/compat/core/stream.cpp:35:22: no member named 'StreamFlags' in namespace 'sep' [clang-diagnostic-error]
      (flags == sep::StreamFlags::NonBlocking) ? cudaStreamNonBlocking : cudaStreamDefault;
                     ^
  Report hash: a31a0aa48da491c1b03941bff458c929
  Steps:
    1, stream.cpp:35:22: no member named 'StreamFlags' in namespace 'sep'

[CRITICAL] /sep/src/compat/core/stream.cpp:35:50: use of undeclared identifier 'cudaStreamNonBlocking' [clang-diagnostic-error]
      (flags == sep::StreamFlags::NonBlocking) ? cudaStreamNonBlocking : cudaStreamDefault;
                                                 ^
  Report hash: 1af351aa9260072c0c2bc8d5144315b4
  Steps:
    1, stream.cpp:35:50: use of undeclared identifier 'cudaStreamNonBlocking'

[CRITICAL] /sep/src/compat/core/stream.cpp:35:74: use of undeclared identifier 'cudaStreamDefault'; did you mean 'cudaStreamDestroy'? [clang-diagnostic-error]
      (flags == sep::StreamFlags::NonBlocking) ? cudaStreamNonBlocking : cudaStreamDefault;
                                                                         ^
  Report hash: 8d89a3d2c58a01653644a2ed200b5c2d
  Notes:
    1, stream.cpp:35:74: cudaStreamDestroy (fixit)
  Steps:
    1, cuda.h:55:13: 'cudaStreamDestroy' declared here
    2, stream.cpp:35:74: use of undeclared identifier 'cudaStreamDefault'; did you mean 'cudaStreamDestroy'?

[CRITICAL] /sep/src/compat/core/stream.cpp:37:21: use of undeclared identifier 'cudaStreamCreateWithFlags' [clang-diagnostic-error]
  cudaError_t err = cudaStreamCreateWithFlags(&cuda_stream, cuda_flags);
                    ^
  Report hash: 3500fc9fd89ec193928b99c381e2e95a
  Steps:
    1, stream.cpp:37:21: use of undeclared identifier 'cudaStreamCreateWithFlags'

Found 5 defect(s) in stream.cpp

[CRITICAL] /sep/src/core/metrics_collector.cpp:32:16: use of undeclared identifier 'cudaEventCreate'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventCreate'? [clang-diagnostic-error]
    CUDA_CHECK(cudaEventCreate(&start_event_));
               ^
  Report hash: b25508b5710f2f49244d0a33f85f65b6
  Notes:
    1, metrics_collector.cpp:32:16: cuda_stub_constants::cuda_stub_constants::cudaEventCreate (fixit)
  Steps:
    1, cuda_helpers.h:29:29: expanded from macro 'CUDA_CHECK'
    2, cuda_impl.h:179:20: 'cuda_stub_constants::cuda_stub_constants::cudaEventCreate' declared here
    3, metrics_collector.cpp:32:16: use of undeclared identifier 'cudaEventCreate'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventCreate'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:33:16: use of undeclared identifier 'cudaEventCreate'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventCreate'? [clang-diagnostic-error]
    CUDA_CHECK(cudaEventCreate(&stop_event_));
               ^
  Report hash: 91d29f50a1368ddcb1984747cb89f5d9
  Notes:
    1, metrics_collector.cpp:33:16: cuda_stub_constants::cuda_stub_constants::cudaEventCreate (fixit)
  Steps:
    1, cuda_helpers.h:29:29: expanded from macro 'CUDA_CHECK'
    2, cuda_impl.h:179:20: 'cuda_stub_constants::cuda_stub_constants::cudaEventCreate' declared here
    3, metrics_collector.cpp:33:16: use of undeclared identifier 'cudaEventCreate'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventCreate'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:39:18: use of undeclared identifier 'cudaEventDestroy'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventDestroy'? [clang-diagnostic-error]
      CUDA_CHECK(cudaEventDestroy(start_event_));
                 ^
  Report hash: c7291b4a8c9fcc75e53a9a149c286b7a
  Notes:
    1, metrics_collector.cpp:39:18: cuda_stub_constants::cuda_stub_constants::cudaEventDestroy (fixit)
  Steps:
    1, cuda_helpers.h:29:29: expanded from macro 'CUDA_CHECK'
    2, cuda_impl.h:190:20: 'cuda_stub_constants::cuda_stub_constants::cudaEventDestroy' declared here
    3, metrics_collector.cpp:39:18: use of undeclared identifier 'cudaEventDestroy'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventDestroy'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:42:18: use of undeclared identifier 'cudaEventDestroy'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventDestroy'? [clang-diagnostic-error]
      CUDA_CHECK(cudaEventDestroy(stop_event_));
                 ^
  Report hash: e5e7b804fe696dc785bde1e6e929c1d0
  Notes:
    1, metrics_collector.cpp:42:18: cuda_stub_constants::cuda_stub_constants::cudaEventDestroy (fixit)
  Steps:
    1, cuda_helpers.h:29:29: expanded from macro 'CUDA_CHECK'
    2, cuda_impl.h:190:20: 'cuda_stub_constants::cuda_stub_constants::cudaEventDestroy' declared here
    3, metrics_collector.cpp:42:18: use of undeclared identifier 'cudaEventDestroy'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventDestroy'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:82:18: use of undeclared identifier 'cudaEventRecord'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventRecord'? [clang-diagnostic-error]
      CUDA_CHECK(cudaEventRecord(start_event_, nullptr));
                 ^
  Report hash: 021a79bc61b84d2510d0d8bc741021dd
  Notes:
    1, metrics_collector.cpp:82:18: cuda_stub_constants::cuda_stub_constants::cudaEventRecord (fixit)
  Steps:
    1, cuda_helpers.h:29:29: expanded from macro 'CUDA_CHECK'
    2, cuda_impl.h:199:20: 'cuda_stub_constants::cuda_stub_constants::cudaEventRecord' declared here
    3, metrics_collector.cpp:82:18: use of undeclared identifier 'cudaEventRecord'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventRecord'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:88:18: use of undeclared identifier 'cudaEventRecord'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventRecord'? [clang-diagnostic-error]
      CUDA_CHECK(cudaEventRecord(stop_event_, nullptr));
                 ^
  Report hash: 7e7d7c45a172efcb54ccdac7e25f4f5d
  Notes:
    1, metrics_collector.cpp:88:18: cuda_stub_constants::cuda_stub_constants::cudaEventRecord (fixit)
  Steps:
    1, cuda_helpers.h:29:29: expanded from macro 'CUDA_CHECK'
    2, cuda_impl.h:199:20: 'cuda_stub_constants::cuda_stub_constants::cudaEventRecord' declared here
    3, metrics_collector.cpp:88:18: use of undeclared identifier 'cudaEventRecord'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventRecord'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:89:18: use of undeclared identifier 'cudaEventSynchronize'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventSynchronize'? [clang-diagnostic-error]
      CUDA_CHECK(cudaEventSynchronize(stop_event_));
                 ^
  Report hash: 0cb75e0551f2372d9e0f4580fb440065
  Notes:
    1, metrics_collector.cpp:89:18: cuda_stub_constants::cuda_stub_constants::cudaEventSynchronize (fixit)
  Steps:
    1, cuda_helpers.h:29:29: expanded from macro 'CUDA_CHECK'
    2, cuda_impl.h:209:20: 'cuda_stub_constants::cuda_stub_constants::cudaEventSynchronize' declared here
    3, metrics_collector.cpp:89:18: use of undeclared identifier 'cudaEventSynchronize'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventSynchronize'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:93:20: use of undeclared identifier 'cudaEventElapsedTime'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventElapsedTime'? [clang-diagnostic-error]
        CUDA_CHECK(cudaEventElapsedTime(&elapsed_time, start_event_, stop_event_));
                   ^
  Report hash: e914954a9d9a917a15150cbec0ec5d9d
  Notes:
    1, metrics_collector.cpp:93:20: cuda_stub_constants::cuda_stub_constants::cudaEventElapsedTime (fixit)
  Steps:
    1, cuda_helpers.h:29:29: expanded from macro 'CUDA_CHECK'
    2, cuda_impl.h:218:20: 'cuda_stub_constants::cuda_stub_constants::cudaEventElapsedTime' declared here
    3, metrics_collector.cpp:93:20: use of undeclared identifier 'cudaEventElapsedTime'; did you mean 'cuda_stub_constants::cuda_stub_constants::cudaEventElapsedTime'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:209:3: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'? [clang-diagnostic-error]
  cudaEvent_t start_event_{nullptr};
  ^
  Report hash: 7830a555ff4f4c841dd5c13fa13e66c2
  Notes:
    1, metrics_collector.cpp:209:3: cuda_stub_constants::cudaEvent_t (fixit)
  Steps:
    1, cuda_impl.h:43:28: 'cuda_stub_constants::cudaEvent_t' declared here
    2, metrics_collector.cpp:209:3: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:210:3: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'? [clang-diagnostic-error]
  cudaEvent_t stop_event_{nullptr};
  ^
  Report hash: 0263a68ec6b0c2230629583cc80349d3
  Notes:
    1, metrics_collector.cpp:210:3: cuda_stub_constants::cudaEvent_t (fixit)
  Steps:
    1, cuda_impl.h:43:28: 'cuda_stub_constants::cudaEvent_t' declared here
    2, metrics_collector.cpp:210:3: unknown type name 'cudaEvent_t'; did you mean 'cuda_stub_constants::cudaEvent_t'?

Found 10 defect(s) in metrics_collector.cpp

[CRITICAL] /sep/src/blender/gpu_context.cpp:6:24: redefinition of 'operator()' [clang-diagnostic-error]
void GPUBufferDeleter::operator()(GPUBuffer* buffer) const noexcept {
                       ^
  Report hash: e6ece2881fb284cea6a1a2e9218cec45
  Steps:
    1, gpu_context.h:84:10: previous definition is here
    2, gpu_context.cpp:6:24: redefinition of 'operator()'

Found 1 defect(s) in gpu_context.cpp

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

[CRITICAL] /sep/src/api/bridge_c.cpp:4:10: 'bridge_internal.hpp' file not found [clang-diagnostic-error]
#include "bridge_internal.hpp"
         ^
  Report hash: 70878f698f8eb2f42b460e79cd1baf05
  Steps:
    1, bridge_c.cpp:4:10: 'bridge_internal.hpp' file not found

Found 1 defect(s) in bridge_c.cpp

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

[MEDIUM] /sep/src/api/client.cpp:142:47: unused parameter 'request' [clang-diagnostic-unused-parameter]
void Client::updateMetrics( const APIRequest &request,
                                              ^
  Report hash: ba54c1309e522460b22886b6d9677f58
  Steps:
    1, client.cpp:142:47: unused parameter 'request'

Found 1 defect(s) in client.cpp

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

Found 6 defect(s) in cuda_runtime.h

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

Found no defects in pipewire_stubs.cpp
Found no defects in crow_error.cpp
Found no defects in allocation_metrics.cpp
Found no defects in pipeline.cpp
Found no defects in tracing.cpp
Found no defects in prometheus_exporter.cpp
Found no defects in qbsa.cpp
Found no defects in qfh.cpp
Found no defects in qbsa_qfh.cpp
Found no defects in js_integration.cpp
Found no defects in relationship.cpp
[CRITICAL] /sep/src/main.cpp:1:10: 'config/manager.h' file not found [clang-diagnostic-error]
#include "config/manager.h"
         ^
  Report hash: bd758a45f2ff1b7826d8c6500f11dc6e
  Steps:
    1, main.cpp:1:10: 'config/manager.h' file not found

Found 1 defect(s) in main.cpp

Found no defects in mesh_handler.cpp
Found no defects in quantum_processor_qfh.cpp
Found no defects in quantum_processor.cpp
Found no defects in types_serialization.cpp
Found no defects in quantum_processor_qfh_common.cpp
Found no defects in ollama_client.cpp
Found no defects in config.cpp
Found no defects in manager.cpp
[LOW] /sep/include/api/sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
class Processor;
      ^
  Report hash: 7ddbaf54ea1b7346b97eb4b637f989f0
  Steps:
    1, processor.h:15:7: a definition of 'Processor' is found here
    2, sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in sep_engine.h

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

Found no defects in pipewire_capture.cpp
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

[MEDIUM] /sep/include/compat/kernels.cuh:47:27: unused parameter 'pattern' [clang-diagnostic-unused-parameter]
    pattern::PatternData* pattern,
                          ^
  Report hash: 31d5f2cb4a0073e44a643481c4cc0d9c
  Steps:
    1, kernels.cuh:47:27: unused parameter 'pattern'

[MEDIUM] /sep/include/compat/kernels.cuh:48:27: unused parameter 'result' [clang-diagnostic-unused-parameter]
    pattern::PatternData* result,
                          ^
  Report hash: 5e3e639c90699be9f1e68511d19a7617
  Steps:
    1, kernels.cuh:48:27: unused parameter 'result'

[MEDIUM] /sep/include/compat/kernels.cuh:49:35: unused parameter 'config' [clang-diagnostic-unused-parameter]
    const pattern::PatternConfig* config,
                                  ^
  Report hash: eb015f5d301c785fe9ee54e041abb80c
  Steps:
    1, kernels.cuh:49:35: unused parameter 'config'

[MEDIUM] /sep/include/compat/kernels.cuh:50:12: unused parameter 'pattern_count' [clang-diagnostic-unused-parameter]
    size_t pattern_count,
           ^
  Report hash: bbdf69baf0632c6003f677ee2c7c0b3f
  Steps:
    1, kernels.cuh:50:12: unused parameter 'pattern_count'

[MEDIUM] /sep/include/compat/kernels.cuh:51:33: unused parameter 'previous_patterns' [clang-diagnostic-unused-parameter]
    const pattern::PatternData* previous_patterns
                                ^
  Report hash: e90d8ad10723fe736ad64b04db58160a
  Steps:
    1, kernels.cuh:51:33: unused parameter 'previous_patterns'

Found 5 defect(s) in kernels.cuh

