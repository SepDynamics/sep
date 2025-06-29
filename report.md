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

[MEDIUM] /sep/include/api/types.h:50:54: unused parameter 'name' [clang-diagnostic-unused-parameter]
    virtual std::string getHeader(const std::string& name) const {
                                                     ^
  Report hash: 4d36d5ee5324c8a8ec1018571fda19eb
  Steps:
    1, types.h:50:54: unused parameter 'name'

Found 1 defect(s) in types.h

[MEDIUM] /sep/include/memory/memory_tier.hpp:61:41: field 'tier' will be initialized after field 'original_size' [clang-diagnostic-reorder-ctor]
        : ptr(p), size(s), offset(off), tier(t), original_size(s) {}
                                        ^
  Report hash: 5c71e33c2548a8c4133274e264137cc8
  Notes:
    1, memory_tier.hpp:61:41: original_size(s) tier(t) (fixit)
  Steps:
    1, memory_tier.hpp:61:41: field 'tier' will be initialized after field 'original_size'

Found 1 defect(s) in memory_tier.hpp

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

[MEDIUM] /sep/include/api/server.h:25:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
struct request;
^
  Report hash: 7efebc8bbaef775aeab2e28a25dc4a5c
  Steps:
    1, crow_isolation.h:47:11: previous use is here
    2, server.h:25:1: did you mean class here?
    3, server.h:25:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/server.h:26:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
struct response;
^
  Report hash: 59bfeb4242018ecdefde0c5a8f41634e
  Steps:
    1, crow_isolation.h:62:11: previous use is here
    2, server.h:26:1: did you mean class here?
    3, server.h:26:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in server.h

[MEDIUM] /sep/include/crow/crow_isolation.h:53:63: unused parameter 'key' [clang-diagnostic-unused-parameter]
        const char* get_header_value(const sep::shim::string& key) const {
                                                              ^
  Report hash: 62f61d32c9a1eb93849c6508c799e1f7
  Steps:
    1, crow_isolation.h:53:63: unused parameter 'key'

[MEDIUM] /sep/include/crow/crow_isolation.h:71:50: unused parameter 'key' [clang-diagnostic-unused-parameter]
        void set_header(const sep::shim::string& key, const sep::shim::string& value) {}
                                                 ^
  Report hash: 2dcbcb3089937a19ab4b9ac68a1a1ed0
  Steps:
    1, crow_isolation.h:71:50: unused parameter 'key'

[MEDIUM] /sep/include/crow/crow_isolation.h:71:80: unused parameter 'value' [clang-diagnostic-unused-parameter]
        void set_header(const sep::shim::string& key, const sep::shim::string& value) {}
                                                                               ^
  Report hash: c932c497b7c88c3fc859ac59506ed06f
  Steps:
    1, crow_isolation.h:71:80: unused parameter 'value'

[MEDIUM] /sep/include/crow/crow_isolation.h:72:50: unused parameter 'key' [clang-diagnostic-unused-parameter]
        void add_header(const sep::shim::string& key, const sep::shim::string& value) {}
                                                 ^
  Report hash: 44d5d1ebd334029d89397f3b39f13a59
  Steps:
    1, crow_isolation.h:72:50: unused parameter 'key'

[MEDIUM] /sep/include/crow/crow_isolation.h:72:80: unused parameter 'value' [clang-diagnostic-unused-parameter]
        void add_header(const sep::shim::string& key, const sep::shim::string& value) {}
                                                                               ^
  Report hash: 55b258b10389c0d1aefa22553669b74f
  Steps:
    1, crow_isolation.h:72:80: unused parameter 'value'

[MEDIUM] /sep/include/crow/crow_isolation.h:89:53: unused parameter 'text' [clang-diagnostic-unused-parameter]
            void send_text(const sep::shim::string& text) {}
                                                    ^
  Report hash: 5bea7f740f0657009ee1f1afd08ac61e
  Steps:
    1, crow_isolation.h:89:53: unused parameter 'text'

[MEDIUM] /sep/include/crow/crow_isolation.h:90:55: unused parameter 'data' [clang-diagnostic-unused-parameter]
            void send_binary(const sep::shim::string& data) {}
                                                      ^
  Report hash: 58e0946831766745a3fc9d4cd5ebcb3c
  Steps:
    1, crow_isolation.h:90:55: unused parameter 'data'

[MEDIUM] /sep/include/crow/crow_isolation.h:91:49: unused parameter 'msg' [clang-diagnostic-unused-parameter]
            void close(const sep::shim::string& msg = "") {}
                                                ^
  Report hash: 106bc8660edac42a0fc1f430fe62e2f3
  Steps:
    1, crow_isolation.h:91:49: unused parameter 'msg'

[MEDIUM] /sep/include/crow/crow_isolation.h:116:36: unused parameter 'f' [clang-diagnostic-unused-parameter]
        DummyRoute& operator()(F&& f) {
                                   ^
  Report hash: 1e21f1c075e5210a149252ece37c5f4f
  Steps:
    1, crow_isolation.h:116:36: unused parameter 'f'

[MEDIUM] /sep/include/crow/crow_isolation.h:131:28: unused parameter 'p' [clang-diagnostic-unused-parameter]
        void port(uint16_t p) {}
                           ^
  Report hash: b934b84fca2436fdef70c3516da41cb7
  Steps:
    1, crow_isolation.h:131:28: unused parameter 'p'

[MEDIUM] /sep/include/crow/crow_isolation.h:132:48: unused parameter 'addr' [clang-diagnostic-unused-parameter]
        void bindaddr(const sep::shim::string& addr) {}
                                               ^
  Report hash: 250fcf67dede6ba4be840f4641fa6e3e
  Steps:
    1, crow_isolation.h:132:48: unused parameter 'addr'

[MEDIUM] /sep/include/crow/crow_isolation.h:172:44: unused parameter 'req' [clang-diagnostic-unused-parameter]
        void handle_upgrade(const request& req,
                                           ^
  Report hash: 331d75dec6c0f922510123a10b531126
  Steps:
    1, crow_isolation.h:172:44: unused parameter 'req'

[MEDIUM] /sep/include/crow/crow_isolation.h:173:39: unused parameter 'res' [clang-diagnostic-unused-parameter]
                            response& res,
                                      ^
  Report hash: c7fc29ac6703380472bc6adf9c1fed11
  Steps:
    1, crow_isolation.h:173:39: unused parameter 'res'

[MEDIUM] /sep/include/crow/crow_isolation.h:174:39: unused parameter 'adaptor' [clang-diagnostic-unused-parameter]
                            Adaptor&& adaptor) {}
                                      ^
  Report hash: 1190635c4178fb8d0509a679385d250d
  Steps:
    1, crow_isolation.h:174:39: unused parameter 'adaptor'

[MEDIUM] /sep/include/crow/crow_isolation.h:191:44: unused parameter 'req' [clang-diagnostic-unused-parameter]
        void handle_upgrade(const request& req,
                                           ^
  Report hash: 331d75dec6c0f922510123a10b531126
  Steps:
    1, crow_isolation.h:191:44: unused parameter 'req'

[MEDIUM] /sep/include/crow/crow_isolation.h:192:39: unused parameter 'res' [clang-diagnostic-unused-parameter]
                            response& res,
                                      ^
  Report hash: c7fc29ac6703380472bc6adf9c1fed11
  Steps:
    1, crow_isolation.h:192:39: unused parameter 'res'

[MEDIUM] /sep/include/crow/crow_isolation.h:193:39: unused parameter 'adaptor' [clang-diagnostic-unused-parameter]
                            Adaptor&& adaptor) {}
                                      ^
  Report hash: 1190635c4178fb8d0509a679385d250d
  Steps:
    1, crow_isolation.h:193:39: unused parameter 'adaptor'

Found 17 defect(s) in crow_isolation.h

Found no defects in redis_manager.cpp
[MEDIUM] /sep/include/compat/cufft.h:62:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan1d(cufftHandle* plan, int nx, int type, int batch) {
                                            ^
  Report hash: 778d90af82563e80323fc4c239514480
  Steps:
    1, cufft.h:62:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:62:55: unused parameter 'nx' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan1d(cufftHandle* plan, int nx, int type, int batch) {
                                                      ^
  Report hash: b476163126b044d8ba41d4a962581631
  Steps:
    1, cufft.h:62:55: unused parameter 'nx'

[MEDIUM] /sep/include/compat/cufft.h:62:63: unused parameter 'type' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan1d(cufftHandle* plan, int nx, int type, int batch) {
                                                              ^
  Report hash: 5e3348ee34f25afe8fae762c3b7452a5
  Steps:
    1, cufft.h:62:63: unused parameter 'type'

[MEDIUM] /sep/include/compat/cufft.h:62:73: unused parameter 'batch' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan1d(cufftHandle* plan, int nx, int type, int batch) {
                                                                        ^
  Report hash: c5274b8d065a571b140d8322f057d2e8
  Steps:
    1, cufft.h:62:73: unused parameter 'batch'

[MEDIUM] /sep/include/compat/cufft.h:66:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan2d(cufftHandle* plan, int nx, int ny, int type) {
                                            ^
  Report hash: 17108eb882f5bb6a8b2b9db88957828a
  Steps:
    1, cufft.h:66:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:66:55: unused parameter 'nx' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan2d(cufftHandle* plan, int nx, int ny, int type) {
                                                      ^
  Report hash: fde9f899b55f47cddd45027c5389e57e
  Steps:
    1, cufft.h:66:55: unused parameter 'nx'

[MEDIUM] /sep/include/compat/cufft.h:66:63: unused parameter 'ny' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan2d(cufftHandle* plan, int nx, int ny, int type) {
                                                              ^
  Report hash: 718e0340681176b0c8200b76210f28bf
  Steps:
    1, cufft.h:66:63: unused parameter 'ny'

[MEDIUM] /sep/include/compat/cufft.h:66:71: unused parameter 'type' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan2d(cufftHandle* plan, int nx, int ny, int type) {
                                                                      ^
  Report hash: f7a711971179e7029e13c64a411637a3
  Steps:
    1, cufft.h:66:71: unused parameter 'type'

[MEDIUM] /sep/include/compat/cufft.h:70:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan3d(cufftHandle* plan, int nx, int ny, int nz, int type) {
                                            ^
  Report hash: 9d6175258154b19c50fb5ac3d7e0dec6
  Steps:
    1, cufft.h:70:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:70:55: unused parameter 'nx' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan3d(cufftHandle* plan, int nx, int ny, int nz, int type) {
                                                      ^
  Report hash: c2f07ccbdc23d01f675aaaa2d3ed138b
  Steps:
    1, cufft.h:70:55: unused parameter 'nx'

[MEDIUM] /sep/include/compat/cufft.h:70:63: unused parameter 'ny' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan3d(cufftHandle* plan, int nx, int ny, int nz, int type) {
                                                              ^
  Report hash: aee850b7a6a03122e896b2e1b3b6e78d
  Steps:
    1, cufft.h:70:63: unused parameter 'ny'

[MEDIUM] /sep/include/compat/cufft.h:70:71: unused parameter 'nz' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan3d(cufftHandle* plan, int nx, int ny, int nz, int type) {
                                                                      ^
  Report hash: 829e464e06cec51c751f22f6dee266d6
  Steps:
    1, cufft.h:70:71: unused parameter 'nz'

[MEDIUM] /sep/include/compat/cufft.h:70:79: unused parameter 'type' [clang-diagnostic-unused-parameter]
inline cufftResult cufftPlan3d(cufftHandle* plan, int nx, int ny, int nz, int type) {
                                                                              ^
  Report hash: b4cff53f1288526fa7e464e4a64ab0ac
  Steps:
    1, cufft.h:70:79: unused parameter 'type'

[MEDIUM] /sep/include/compat/cufft.h:74:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
inline cufftResult cufftDestroy(cufftHandle plan) {
                                            ^
  Report hash: c1463fb0af25faaa7cd8f8892bfc86c7
  Steps:
    1, cufft.h:74:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:78:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
inline cufftResult cufftExecC2C(cufftHandle plan, void* idata, void* odata, int direction) {
                                            ^
  Report hash: 75220db65e344c8c27a290a16ce2ecbb
  Steps:
    1, cufft.h:78:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:78:57: unused parameter 'idata' [clang-diagnostic-unused-parameter]
inline cufftResult cufftExecC2C(cufftHandle plan, void* idata, void* odata, int direction) {
                                                        ^
  Report hash: 9f8d731b236cc34712d826a684e30ca0
  Steps:
    1, cufft.h:78:57: unused parameter 'idata'

[MEDIUM] /sep/include/compat/cufft.h:78:70: unused parameter 'odata' [clang-diagnostic-unused-parameter]
inline cufftResult cufftExecC2C(cufftHandle plan, void* idata, void* odata, int direction) {
                                                                     ^
  Report hash: a454f14b2fea1b6842416ebad447307e
  Steps:
    1, cufft.h:78:70: unused parameter 'odata'

[MEDIUM] /sep/include/compat/cufft.h:78:81: unused parameter 'direction' [clang-diagnostic-unused-parameter]
inline cufftResult cufftExecC2C(cufftHandle plan, void* idata, void* odata, int direction) {
                                                                                ^
  Report hash: 693efb129496780ca772c4b327572393
  Steps:
    1, cufft.h:78:81: unused parameter 'direction'

[MEDIUM] /sep/include/compat/cufft.h:82:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
inline cufftResult cufftExecR2C(cufftHandle plan, void* idata, void* odata) {
                                            ^
  Report hash: c140b8fed08fab4f88d281deff697e49
  Steps:
    1, cufft.h:82:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:82:57: unused parameter 'idata' [clang-diagnostic-unused-parameter]
inline cufftResult cufftExecR2C(cufftHandle plan, void* idata, void* odata) {
                                                        ^
  Report hash: 11e0980bfec3bd8ce185a7592f34f82e
  Steps:
    1, cufft.h:82:57: unused parameter 'idata'

[MEDIUM] /sep/include/compat/cufft.h:82:70: unused parameter 'odata' [clang-diagnostic-unused-parameter]
inline cufftResult cufftExecR2C(cufftHandle plan, void* idata, void* odata) {
                                                                     ^
  Report hash: 1f1c0d2a7218750708f044376383a42c
  Steps:
    1, cufft.h:82:70: unused parameter 'odata'

[MEDIUM] /sep/include/compat/cufft.h:86:45: unused parameter 'plan' [clang-diagnostic-unused-parameter]
inline cufftResult cufftExecC2R(cufftHandle plan, void* idata, void* odata) {
                                            ^
  Report hash: ff4e39ce2b67e163875ca1153065cfe5
  Steps:
    1, cufft.h:86:45: unused parameter 'plan'

[MEDIUM] /sep/include/compat/cufft.h:86:57: unused parameter 'idata' [clang-diagnostic-unused-parameter]
inline cufftResult cufftExecC2R(cufftHandle plan, void* idata, void* odata) {
                                                        ^
  Report hash: c2765670cf6718cb3a4bad62e43a6ee0
  Steps:
    1, cufft.h:86:57: unused parameter 'idata'

[MEDIUM] /sep/include/compat/cufft.h:86:70: unused parameter 'odata' [clang-diagnostic-unused-parameter]
inline cufftResult cufftExecC2R(cufftHandle plan, void* idata, void* odata) {
                                                                     ^
  Report hash: 8baaf8257d40f376236212d6b9b0b96a
  Steps:
    1, cufft.h:86:70: unused parameter 'odata'

Found 24 defect(s) in cufft.h

[LOW] /sep/include/quantum/pattern_evolution_bridge.h:16:7: no definition found for 'QuantumManifoldOptimizer', but a definition with the same name 'QuantumManifoldOptimizer' found in another namespace 'sep::quantum::manifold' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c441508debea10c14cf2d6db211eb7f0
  Steps:
    1, quantum_manifold_optimizer.h:57:7: a definition of 'QuantumManifoldOptimizer' is found here
    2, pattern_evolution_bridge.h:16:7: no definition found for 'QuantumManifoldOptimizer', but a definition with the same name 'QuantumManifoldOptimizer' found in another namespace 'sep::quantum::manifold'

Found 1 defect(s) in pattern_evolution_bridge.h

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

Found 8 defect(s) in quantum_manifold_optimizer.h

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

Found no defects in bridge.cpp
[LOW] /sep/include/api/bridge_internal.hpp:12:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
class Processor;
      ^
  Report hash: 5ca7fa69e4196bf69f99f847456b1c42
  Steps:
    1, processor.h:80:7: a definition of 'Processor' is found here
    2, bridge_internal.hpp:12:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in bridge_internal.hpp

[MEDIUM] /sep/include/crow/asio_isolation.h:52:31: all paths through this function will call itself [clang-diagnostic-infinite-recursion]
            context context() { return context(); }
                              ^
  Report hash: 3224361f449a83099b40b66d08419d91
  Steps:
    1, asio_isolation.h:52:31: all paths through this function will call itself

[LOW] /sep/include/crow/asio_isolation.h:64:9: constructor accepting a forwarding reference can hide the copy and move constructors [bugprone-forwarding-reference-overload]
        any_io_executor(T&&) {}
        ^
  Report hash: cbbc29282533261ab411323c6ecc789c
  Steps:
    1, asio_isolation.h:64:9: constructor accepting a forwarding reference can hide the copy and move constructors

[MEDIUM] /sep/include/crow/asio_isolation.h:89:59: unused parameter 'ctx' [clang-diagnostic-unused-parameter]
        execution_context_service_base(execution_context& ctx) {}
                                                          ^
  Report hash: f3c5052afd97cf4aa573322cb2c80d1a
  Steps:
    1, asio_isolation.h:89:59: unused parameter 'ctx'

[MEDIUM] /sep/include/crow/asio_isolation.h:128:36: unused parameter 'io' [clang-diagnostic-unused-parameter]
                socket(io_context& io) {}
                                   ^
  Report hash: e7dd341eb5bc0746a04737188f427783
  Steps:
    1, asio_isolation.h:128:36: unused parameter 'io'

[MEDIUM] /sep/include/crow/asio_isolation.h:131:57: unused parameter 'ex' [clang-diagnostic-unused-parameter]
                socket(const io_context::executor_type& ex) {}
                                                        ^
  Report hash: 56b0e986888e6dd3e14c781644929cd4
  Steps:
    1, asio_isolation.h:131:57: unused parameter 'ex'

[MEDIUM] /sep/include/crow/asio_isolation.h:139:58: unused parameter 'type' [clang-diagnostic-unused-parameter]
                void shutdown(socket_base::shutdown_type type, error_code& ec) { 
                                                         ^
  Report hash: 62a23a5fbb5f9103df06be212fb003cb
  Steps:
    1, asio_isolation.h:139:58: unused parameter 'type'

Found 6 defect(s) in asio_isolation.h

[MEDIUM] /sep/include/crow/logging.h:21:50: unused parameter 'message' [clang-diagnostic-unused-parameter]
        void operator()(const sep::shim::string& message) {
                                                 ^
  Report hash: d9252f04e6e54412898c044a50e0d541
  Steps:
    1, logging.h:21:50: unused parameter 'message'

[MEDIUM] /sep/include/crow/logging.h:32:37: unused parameter 'value' [clang-diagnostic-unused-parameter]
        Logger& operator<<(T const& value) {
                                    ^
  Report hash: 4dc92635fe2d82eaccd2033278129f52
  Steps:
    1, logging.h:32:37: unused parameter 'value'

[MEDIUM] /sep/include/crow/logging.h:44:18: private field 'level_' is not used [clang-diagnostic-unused-private-field]
        LogLevel level_;
                 ^
  Report hash: 94dcbf70094f439983d3e049a3f4f5d8
  Steps:
    1, logging.h:44:18: private field 'level_' is not used

[MEDIUM] /sep/include/crow/logging.h:47:20: private field 'handler_' is not used [clang-diagnostic-unused-private-field]
        LogHandler handler_;
                   ^
  Report hash: c22310aeee52e4c4a190a94cfd9dbcda
  Steps:
    1, logging.h:47:20: private field 'handler_' is not used

Found 4 defect(s) in logging.h

[MEDIUM] /sep/include/crow/socket_adaptors.h:29:46: unused parameter 'io_context' [clang-diagnostic-unused-parameter]
        SocketAdaptor(asio_stub::io_context& io_context, context*) : socket_() {}
                                             ^
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

Found no defects in crow_adapter.cpp
[LOW] /sep/include/api/sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
class Processor;
      ^
  Report hash: 7ddbaf54ea1b7346b97eb4b637f989f0
  Steps:
    1, processor.h:80:7: a definition of 'Processor' is found here
    2, sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in sep_engine.h

[MEDIUM] /sep/src/api/sep_engine.cpp:304:67: unused parameter 'request_data' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d06191f01aa657f541e0410cc16c881a
  Steps:
    1, sep_engine.cpp:304:67: unused parameter 'request_data'

Found 1 defect(s) in sep_engine.cpp

Found no defects in api.cpp
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

Found no defects in pattern_visualization_pipeline.cpp
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

Found no defects in raii.cpp
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

Found no defects in component_bridge.cpp
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

Found no defects in memory_tier.cpp
Found no defects in manager.cpp
Found no defects in quantum_processor.cpp
[MEDIUM] /sep/src/quantum/evolution.cpp:127:52: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        state.phase += (nextFloat() * 2.0f - 1.0f) * sigma * M_PI; // Add phase mutation
                                                   ^
  Report hash: c5677e825a782969782849e7a3b87e33
  Steps:
    1, evolution.cpp:127:52: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/quantum/evolution.cpp:325:42: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    state.phase += (rnd() * 2.0f - 1.0f) * sigma * M_PI; // Add phase mutation
                                         ^
  Report hash: cad84b4fd50cd0e5a22044d2f4918545
  Steps:
    1, evolution.cpp:325:42: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/quantum/evolution.cpp:377:45: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        pattern.quantum_state.phase = rnd() * 2.0f * M_PI; // Initialize phase
                                            ^
  Report hash: 686c468a0c599aae810685813b0a5284
  Steps:
    1, evolution.cpp:377:45: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in evolution.cpp

Found no defects in quantum_processor_qfh.cpp
Found no defects in quantum_processor_qfh_common.cpp
Found no defects in processor.cpp
Found no defects in types_serialization.cpp
Found no defects in pattern_processor.cpp
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

Found no defects in stream.cpp
[CRITICAL] /sep/include/compat/cuda_impl.h:22:12: no member named 'cudaMemcpyAsync' in the global namespace; did you mean 'sep::cuda::cudaMemcpyAsync'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 090fe72da399bf3f9ed46d260fddcf67
  Notes:
    1, cuda_impl.h:22:12: sep::cuda::cudaMemcpyAsync (fixit)
  Steps:
    1, cuda_runtime.h:96:13: 'sep::cuda::cudaMemcpyAsync' declared here
    2, cuda_impl.h:22:12: no member named 'cudaMemcpyAsync' in the global namespace; did you mean 'sep::cuda::cudaMemcpyAsync'?

Found 1 defect(s) in cuda_impl.h

Found no defects in metrics_collector.cpp
Found no defects in pipewire_capture.cpp
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

Found no defects in auth_middleware.cpp
[MEDIUM] /sep/src/api/client.cpp:141:46: unused parameter 'request' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 91d07ab03d93db50c8196e95accb3896
  Steps:
    1, client.cpp:141:46: unused parameter 'request'

Found 1 defect(s) in client.cpp

Found no defects in crow_error.cpp
Found no defects in curl_http_client.cpp
Found no defects in js_integration.cpp
[CRITICAL] /sep/src/api/lock_free_rate_limiter.cpp:20:9: use of undeclared identifier 'metrics_mutex_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: caaa666c662948bd157ab08052140f51
  Steps:
    1, lock_free_rate_limiter.cpp:20:9: use of undeclared identifier 'metrics_mutex_'

Found 1 defect(s) in lock_free_rate_limiter.cpp

Found no defects in ollama_client.cpp
Found no defects in rate_limit_middleware.cpp
Found no defects in pipeline.cpp
Found no defects in config.cpp
Found no defects in gpu_context.cpp
Found no defects in compression_utils.cpp
Found no defects in compression.cpp
Found no defects in dag_graph.cpp
Found no defects in allocation_metrics.cpp
Found no defects in error_handler.cpp
Found no defects in prometheus_exporter.cpp
Found no defects in tracing.cpp
Found no defects in manager.cpp
Found no defects in qbsa.cpp
Found no defects in qbsa_qfh.cpp
Found no defects in qfh.cpp
[LOW] /sep/src/api/bridge_c.cpp:65:7: Value stored to 'processor' is never read [deadcode.DeadStores]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d25b69bf56c879240a29cdbed26a6467
  Steps:
    1, bridge_c.cpp:65:7: Value stored to 'processor' is never read

Found 1 defect(s) in bridge_c.cpp

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

Found no defects in pattern_processor_interface.cpp
[MEDIUM] /sep/src/api/client.cpp:143:46: unused parameter 'request' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 91d07ab03d93db50c8196e95accb3896
  Steps:
    1, client.cpp:143:46: unused parameter 'request'

Found 1 defect(s) in client.cpp

[MEDIUM] /sep/src/api/sep_engine.cpp:309:67: unused parameter 'request_data' [clang-diagnostic-unused-parameter]
nlohmann::json SepEngine::extractEmbeddings(const nlohmann::json& request_data)
                                                                  ^
  Report hash: d06191f01aa657f541e0410cc16c881a
  Steps:
    1, sep_engine.cpp:309:67: unused parameter 'request_data'

Found 1 defect(s) in sep_engine.cpp

[MEDIUM] /sep/src/blender/blender_integration.cpp:32:7: field 'thread_running_' will be initialized after field 'gpu_context_' [clang-diagnostic-reorder-ctor]
    : thread_running_(false),
      ^
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
[MEDIUM] /sep/include/blender/gpu_context.h:23:32: unused parameter 'device_index' [clang-diagnostic-unused-parameter]
    virtual SEPResult init(int device_index  = -1) { return SEPResult::SUCCESS; }
                               ^
  Report hash: 23c51f2f8d9ec7df2beafa42afa62014
  Steps:
    1, gpu_context.h:23:32: unused parameter 'device_index'

[MEDIUM] /sep/include/blender/gpu_context.h:26:60: unused parameter 'path' [clang-diagnostic-unused-parameter]
    SEPResult loadComputeShader(const ::sep::shim::string& path ) {
                                                           ^
  Report hash: 68077b0bb23b247f4345203fbdaffb8d
  Steps:
    1, gpu_context.h:26:60: unused parameter 'path'

[MEDIUM] /sep/include/blender/gpu_context.h:35:42: unused parameter 'buffer' [clang-diagnostic-unused-parameter]
    virtual void deleteBuffer(GPUBuffer* buffer ) {}
                                         ^
  Report hash: 966ce6630cd2b7c68963f475b1b4bcf0
  Steps:
    1, gpu_context.h:35:42: unused parameter 'buffer'

[MEDIUM] /sep/include/blender/gpu_context.h:36:40: unused parameter 'buffer' [clang-diagnostic-unused-parameter]
    virtual void* mapBuffer(GPUBuffer* buffer ) { return nullptr; }
                                       ^
  Report hash: b9dbd47ac557d5a1b758c3f054a1b8f4
  Steps:
    1, gpu_context.h:36:40: unused parameter 'buffer'

[MEDIUM] /sep/include/blender/gpu_context.h:37:41: unused parameter 'buffer' [clang-diagnostic-unused-parameter]
    virtual void unmapBuffer(GPUBuffer* buffer ) {}
                                        ^
  Report hash: 9fbb8fa205d3a852dec7cb5e37429a91
  Steps:
    1, gpu_context.h:37:41: unused parameter 'buffer'

Found 5 defect(s) in gpu_context.h

[MEDIUM] /sep/src/core/engine.cpp:237:96: unused parameter 'tick' [clang-diagnostic-unused-parameter]
                              ::sep::shim::vector<std::uint32_t>& expectations, std::uint64_t  tick) {
                                                                                               ^
  Report hash: 0c9da63e951de4ba7edf8df7fb562619
  Steps:
    1, engine.cpp:237:96: unused parameter 'tick'

Found 1 defect(s) in engine.cpp

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:120:14: unused variable 'tier_analysis' [clang-diagnostic-unused-variable]
        auto tier_analysis = analyzeTierCoherence();
             ^
  Report hash: 01708387c81904505d93db53cf5517df
  Steps:
    1, quantum_coherence_manager.cpp:120:14: unused variable 'tier_analysis'

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:548:73: unused parameter 'target_tier' [clang-diagnostic-unused-parameter]
                                            sep::memory::MemoryTierEnum target_tier) const {
                                                                        ^
  Report hash: a560e4c242292f1e5fe22ef0b6668c7b
  Steps:
    1, quantum_coherence_manager.cpp:548:73: unused parameter 'target_tier'

[MEDIUM] /sep/src/memory/quantum_coherence_manager.cpp:573:58: implicit conversion turns floating-point number into integer: 'float' to 'size_t' (aka 'unsigned long') [clang-diagnostic-float-conversion]
        size_t demote_count = demotion_candidates.size() * 0.2f;  // Demote 20%
                                                         ^
  Report hash: 29ca0ceabea30d8423bb3e1ab80a80a5
  Steps:
    1, quantum_coherence_manager.cpp:573:58: implicit conversion turns floating-point number into integer: 'float' to 'size_t' (aka 'unsigned long')

Found 3 defect(s) in quantum_coherence_manager.cpp

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:261:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
MemoryTier* MemoryTierManager::determineTier(float coherence, float stability, int generation_count) {
                                                                    ^
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:261:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:305:26: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
                sum += r.second;  // r.second is already double
                         ^
  Report hash: 005a58abc587efa85ab38a672b887462
  Steps:
    1, memory_tier_manager.cpp:305:26: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:388:23: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
        if (p.second->coherence < static_cast<double>(config_.demote_threshold))
                      ^
  Report hash: 0c34aea053751feecbd4d9b55c48af7c
  Steps:
    1, memory_tier_manager.cpp:388:23: implicit conversion increases floating-point precision: 'float' to 'double'

Found 3 defect(s) in memory_tier_manager.cpp

[MEDIUM] /sep/src/api/server.cpp:160:84: unused parameter 'body' [clang-diagnostic-unused-parameter]
void SEPApiServer::logRequest(const HttpRequest& req, int code, const std::string& body,
                                                                                   ^
  Report hash: 99c1cd2d504e0473209900460e079018
  Steps:
    1, server.cpp:160:84: unused parameter 'body'

[MEDIUM] /sep/src/api/server.cpp:220:50: unused parameter 'response_body' [clang-diagnostic-unused-parameter]
                              const std::string& response_body, int64_t duration_ms) {
                                                 ^
  Report hash: 9a6489114b93b9375b7c0c11dd032dae
  Steps:
    1, server.cpp:220:50: unused parameter 'response_body'

Found 2 defect(s) in server.cpp

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:16:36: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
    mc.convergence_threshold = cfg.convergence_threshold;
                                   ^
  Report hash: a4a36d003e7c59c4fd7d6b1c343daf57
  Steps:
    1, quantum_manifold_optimizer.cpp:16:36: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:17:24: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
    mc.step_size = cfg.evolution_step_size;
                       ^
  Report hash: 641c514a85911525bc59527032c66824
  Steps:
    1, quantum_manifold_optimizer.cpp:17:24: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:18:34: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
    mc.neighborhood_radius = cfg.interaction_radius;
                                 ^
  Report hash: 6c0d904c2c9b962d28959a1f52e0a24c
  Steps:
    1, quantum_manifold_optimizer.cpp:18:34: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:19:31: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
    mc.target_coherence = cfg.target_coherence;
                              ^
  Report hash: 31dc183ddee5964860acdcaaece450bd
  Steps:
    1, quantum_manifold_optimizer.cpp:19:31: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:20:31: implicit conversion increases floating-point precision: 'const float' to 'double' [clang-diagnostic-double-promotion]
    mc.target_stability = cfg.target_stability;
                              ^
  Report hash: 16d39340a5c9b05f0487c0526cdeb125
  Steps:
    1, quantum_manifold_optimizer.cpp:20:31: implicit conversion increases floating-point precision: 'const float' to 'double'

[MEDIUM] /sep/src/quantum/quantum_manifold_optimizer.cpp:67:86: unused parameter 'position' [clang-diagnostic-unused-parameter]
std::vector<glm::vec3> QuantumManifoldOptimizer::sampleTangentSpace(const glm::vec3& position,
                                                                                     ^
  Report hash: b944f2e95ab8742ef5da653a467252fb
  Steps:
    1, quantum_manifold_optimizer.cpp:67:86: unused parameter 'position'

Found 6 defect(s) in quantum_manifold_optimizer.cpp

[LOW] /sep/src/memory/quantum_coherence_manager.cpp:120:14: Value stored to 'tier_analysis' during its initialization is never read [deadcode.DeadStores]
        auto tier_analysis = analyzeTierCoherence();
             ^
  Report hash: bd401bfb4e259755fb945e81b4f274cc
  Steps:
    1, quantum_coherence_manager.cpp:120:14: Value stored to 'tier_analysis' during its initialization is never read

Found 1 defect(s) in quantum_coherence_manager.cpp

[MEDIUM] /sep/src/audio/config.cpp:50:31: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                              ^
  Report hash: 4e63877f53b67c4fbc4445ca607d7ea4
  Steps:
    1, config.cpp:50:31: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:50:39: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                                      ^
  Report hash: 6512e0d023076ff627be170438437f62
  Steps:
    1, config.cpp:50:39: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:50:66: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                                                                 ^
  Report hash: c6d2f61264b32d1de6f0cf1588bf3524
  Steps:
    1, config.cpp:50:66: implicit conversion increases floating-point precision: 'float' to 'double'

[MEDIUM] /sep/src/audio/config.cpp:50:113: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
    float quantum_coherence = 1.0f / (1.0f + sep::math::exp_safe(-5.0f * (static_cast<double>(base_coherence) - 0.5f)));
                                                                                                                ^
  Report hash: e4cd4b34889d40705121d381bb2d262d
  Steps:
    1, config.cpp:50:113: implicit conversion increases floating-point precision: 'float' to 'double'

Found 4 defect(s) in config.cpp


----==== Severity Statistics ====----
----------------------------
Severity | Number of reports
----------------------------
MEDIUM   |               202
CRITICAL |                71
LOW      |                28
HIGH     |                 5
----------------------------
----=================----

----==== Checker Statistics ====----
-------------------------------------------------------------------------
Checker name                               | Severity | Number of reports
-------------------------------------------------------------------------
clang-diagnostic-unused-parameter          | MEDIUM   |                71
clang-diagnostic-error                     | CRITICAL |                71
clang-diagnostic-reorder-ctor              | MEDIUM   |                 3
clang-diagnostic-double-promotion          | MEDIUM   |                29
clang-diagnostic-mismatched-tags           | MEDIUM   |                 6
bugprone-forward-declaration-namespace     | LOW      |                 3
clang-diagnostic-unused-variable           | MEDIUM   |                 2
clang-diagnostic-float-conversion          | MEDIUM   |                 2
clang-diagnostic-infinite-recursion        | MEDIUM   |                 1
bugprone-forwarding-reference-overload     | LOW      |                 1
clang-diagnostic-unused-private-field      | MEDIUM   |                 2
clang-diagnostic-unused-but-set-variable   | MEDIUM   |                 1
cert-err33-c                               | MEDIUM   |                35
clang-diagnostic-reserved-identifier       | MEDIUM   |                42
bugprone-switch-missing-default-case       | LOW      |                 4
misc-header-include-cycle                  | LOW      |                17
bugprone-sizeof-expression                 | HIGH     |                 2
clang-diagnostic-reserved-macro-identifier | MEDIUM   |                 4
core.NullDereference                       | HIGH     |                 1
deadcode.DeadStores                        | LOW      |                 3
misc-definitions-in-headers                | MEDIUM   |                 4
core.CallAndMessage                        | HIGH     |                 2
-------------------------------------------------------------------------
----=================----

----==== File Statistics ====----
--------------------------------------------------
File name                      | Number of reports
--------------------------------------------------
types.h                        |                 2
data.hpp                       |                 1
processor.h                    |                 1
quantum_processor_qfh.h        |                 1
types.h                        |                 4
memory_tier.hpp                |                 1
memory_tier_manager.cpp        |                 9
server.h                       |                 2
crow_isolation.h               |                17
cufft.h                        |                24
pattern_evolution_bridge.h     |                 1
quantum_manifold_optimizer.h   |                12
quantum_coherence_manager.cpp  |                 8
pattern_evolution_bridge.cpp   |                16
bridge_internal.hpp            |                 1
asio_isolation.h               |                 6
logging.h                      |                 4
socket_adaptors.h              |                 1
bridge_c.cpp                   |                 2
main.cpp                       |                21
sep_engine.h                   |                 1
sep_engine.cpp                 |                 2
blender_integration.cpp        |                 2
server.cpp                     |                 3
mesh_handler.cpp               |                11
cycles_renderer.cpp            |                12
buffer.h                       |                 1
meta.h                         |                 1
format-utils.h                 |                 2
buffers.h                      |                 1
port-config.h                  |                 1
profile.h                      |                 1
route.h                        |                 1
builder.h                      |                 3
iter.h                         |                 2
parser.h                       |                 2
cleanup.h                      |                 7
json-core.h                    |                30
string.h                       |                 3
type.h                         |                 5
control.h                      |                 1
core.h                         |                 2
impl-client.h                  |                 2
impl-core.h                    |                 1
impl-device.h                  |                 1
impl-factory.h                 |                 1
impl-link.h                    |                 1
impl-metadata.h                |                 1
impl-node.h                    |                 1
impl-port.h                    |                 1
impl.h                         |                 1
properties.h                   |                 2
protocol.h                     |                 1
utils.h                        |                 2
engine.cpp                     |                28
evolution.cpp                  |                 3
quantum_manifold_optimizer.cpp |                12
cycles_test.cpp                |                 5
cuda_impl.h                    |                 1
client.cpp                     |                 2
lock_free_rate_limiter.cpp     |                 1
cetintrin.h                    |                 2
gpu_context.h                  |                 5
config.cpp                     |                 4
--------------------------------------------------
----=================----

----======== Summary ========----
-----------------------------------------------
Number of processed analyzer result files | 520
Number of analyzer reports                | 306
-----------------------------------------------
----=================----
[WARNING 2025-06-29 15:07] - The following source file contents changed or missing since the latest analysis:
 - /sep/include/api/types.h
 - /sep/src/quantum/quantum_manifold_optimizer.cpp
 - /sep/include/quantum/types.h
 - /sep/src/blender/cycles_renderer.cpp
 - /sep/src/quantum/pattern_evolution_bridge.cpp
 - /sep/include/quantum/processor.h
 - /sep/include/quantum/pattern_evolution_bridge.h
 - /sep/src/memory/quantum_coherence_manager.cpp
 - /sep/include/quantum/data.hpp
 - /sep/src/tests/cycles_test.cpp
 - /sep/src/memory/memory_tier_manager.cpp
 - /sep/src/blender/mesh_handler.cpp
 - /sep/src/api/lock_free_rate_limiter.cpp
 - /sep/src/api/server.cpp
 - /sep/src/api/sep_engine.cpp
 - /sep/src/main.cpp
 - /sep/src/core/engine.cpp
 - /sep/include/compat/cuda_impl.h
 - /sep/include/quantum/quantum_manifold_optimizer.h
 - /sep/src/api/client.cpp
 - /sep/include/quantum/quantum_processor_qfh.h
 - /sep/src/blender/blender_integration.cpp
 - /sep/src/api/bridge_c.cpp
 - /sep/include/memory/types.h
Please re-analyze your project to update the reports!
