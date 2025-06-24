Found no defects in pipewire_stubs.cpp
Found no defects in crow_error.cpp
Found no defects in pipeline.cpp
Found no defects in allocation_metrics.cpp
Found no defects in compression_utils.cpp
Found no defects in gpu_context.cpp
Found no defects in pattern_visualization_pipeline.cpp
Found no defects in dag_graph.cpp
Found no defects in js_integration.cpp
Found no defects in prometheus_exporter.cpp
Found no defects in tracing.cpp
Found no defects in curl_http_client.cpp
Found no defects in api.cpp
Found no defects in qbsa.cpp
Found no defects in raii.cpp
Found no defects in client.cpp
Found no defects in metrics_collector.cpp
Found no defects in compression.cpp
Found no defects in stream.cpp
Found no defects in qbsa_qfh.cpp
Found no defects in qfh.cpp
Found no defects in mesh_handler.cpp
Found no defects in ollama_client.cpp
[CRITICAL] /sep/src/main.cpp:1:10: 'config/manager.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bd758a45f2ff1b7826d8c6500f11dc6e
  Steps:
    1, main.cpp:1:10: 'config/manager.h' file not found

Found 1 defect(s) in main.cpp

Found no defects in error_handler.cpp
Found no defects in auth_middleware.cpp
[CRITICAL] /sep/include/api/bridge_internal.hpp:32:38: no member named 'Processor' in namespace 'sep::context' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99d49a670b75b96ba447c7d31b70daab
  Steps:
    1, bridge_internal.hpp:32:38: no member named 'Processor' in namespace 'sep::context'

Found 1 defect(s) in bridge_internal.hpp

[CRITICAL] /sep/src/api/bridge_c.cpp:5:10: 'config/manager.h' file not found [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e16cd8c93ba4b4dec2303c4b34db825b
  Steps:
    1, bridge_c.cpp:5:10: 'config/manager.h' file not found

Found 1 defect(s) in bridge_c.cpp

[CRITICAL] /sep/src/api/bridge.cpp:38:31: no member named 'Processor' in namespace 'sep::context' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 981cf1d1d4da21f186a7f74bd32043f6
  Steps:
    1, bridge.cpp:38:31: no member named 'Processor' in namespace 'sep::context'

Found 1 defect(s) in bridge.cpp

Found no defects in quantum_processor_qfh.cpp
Found no defects in types_serialization.cpp
Found no defects in quantum_processor.cpp
Found no defects in quantum_processor_qfh_common.cpp
Found no defects in manager.cpp
Found no defects in pattern_processor.cpp
[MEDIUM] /sep/src/api/client.cpp:142:47: unused parameter 'request' [clang-diagnostic-unused-parameter]
void Client::updateMetrics( const APIRequest &request,
                                              ^
  Report hash: ba54c1309e522460b22886b6d9677f58
  Steps:
    1, client.cpp:142:47: unused parameter 'request'

Found 1 defect(s) in client.cpp

Found no defects in processor.cpp
Found no defects in evolution.cpp
[CRITICAL] /sep/include/api/rate_limit_middleware.h:24:32: no member named 'config' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 02e9196a4653a2db44a5931da0474bbe
  Steps:
    1, rate_limit_middleware.h:24:32: no member named 'config' in namespace 'sep'

[CRITICAL] /sep/include/api/rate_limit_middleware.h:31:10: no member named 'config' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7ed29b9071b99d1066a607b1e8a9f8d9
  Steps:
    1, rate_limit_middleware.h:31:10: no member named 'config' in namespace 'sep'

Found 2 defect(s) in rate_limit_middleware.h

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

Found 1 defect(s) in query_static_constexpr_member.hpp

[LOW] /sep/include/api/sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
class Processor;
      ^
  Report hash: 7ddbaf54ea1b7346b97eb4b637f989f0
  Steps:
    1, processor.h:79:7: a definition of 'Processor' is found here
    2, sep_engine.h:23:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in sep_engine.h

[MEDIUM] /sep/src/api/sep_engine.cpp:323:67: unused parameter 'request_data' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d06191f01aa657f541e0410cc16c881a
  Steps:
    1, sep_engine.cpp:323:67: unused parameter 'request_data'

Found 1 defect(s) in sep_engine.cpp

[CRITICAL] /usr/include/c++/15/bits/unique_ptr.h:1085:34: call to deleted constructor of 'sep::quantum::QuantumProcessor' [clang-diagnostic-error]
    { return unique_ptr<_Tp>(new _Tp(std::forward<_Args>(__args)...)); }
                                 ^
  Report hash: 24034653917ba4d7bcdffd431ab0e991
  Steps:
    1, sep_engine.cpp:42:34: in instantiation of function template specialization 'std::make_unique<sep::quantum::QuantumProcessor>' requested here
    2, quantum_processor.h:36:5: 'QuantumProcessor' has been explicitly marked deleted here
    3, unique_ptr.h:1085:34: call to deleted constructor of 'sep::quantum::QuantumProcessor'

Found 1 defect(s) in unique_ptr.h

[MEDIUM] /sep/include/api/server.h:23:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7efebc8bbaef775aeab2e28a25dc4a5c
  Steps:
    1, crow_isolation.h:159:7: previous use is here
    2, server.h:23:1: did you mean class here?
    3, server.h:23:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/server.h:24:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 59bfeb4242018ecdefde0c5a8f41634e
  Steps:
    1, crow_isolation.h:174:7: previous use is here
    2, server.h:24:1: did you mean class here?
    3, server.h:24:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in server.h

[MEDIUM] /sep/third_party/crow/crow_isolation.h:290:27: unused parameter 'io_service' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d32bed3bb60378824ed7efc8ec7e3fd4
  Steps:
    1, crow_isolation.h:290:27: unused parameter 'io_service'

Found 1 defect(s) in crow_isolation.h

[LOW] /sep/third_party/crow/http_parser_merged.h:54:16: no definition found for 'http_parser', but a definition with the same name 'http_parser' found in another namespace 'crow::http_parser' [bugprone-forward-declaration-namespace]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
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

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: 8f88009dc93c254b41015b2c51e8f082
  Steps:
    1, http_request.h:34:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' is found here
    2, strand_executor_service.hpp:70:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:70:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow'

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

[LOW] /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
class strand_executor_service::invoker<Executor,
                               ^
  Report hash: fde1c26530434e84a4462d10ecaa85bf
  Steps:
    1, http_request.h:34:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' is found here
    2, strand_executor_service.hpp:138:32: no definition found for '(unnamed class at /usr/include/asio/detail/impl/strand_executor_service.hpp:138:32)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow'

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

[LOW] /usr/include/asio/prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: aa8a682738cc1f1effd1095fe4a24e8b
  Steps:
    1, http_request.h:34:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' is found here
    2, prefer.hpp:325:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:325:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow'

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

[LOW] /usr/include/asio/prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 5ab12f918fc3b22f2fc689d92be7eb3c
  Steps:
    1, http_request.h:34:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' is found here
    2, prefer.hpp:361:48: no definition found for '(unnamed struct at /usr/include/asio/prefer.hpp:361:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow'

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

[LOW] /usr/include/asio/require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: f470fdc9143a0fa6a6317973bc68d5ac
  Steps:
    1, http_request.h:34:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' is found here
    2, require.hpp:216:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:216:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow'

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

[LOW] /usr/include/asio/require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow' [bugprone-forward-declaration-namespace]
      typename call_traits<Impl, T, void(P0)>::result_type,
                                               ^
  Report hash: 8984bc5b5403f439c30a7a72a3b2bb6e
  Steps:
    1, http_request.h:34:8: a definition of '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' is found here
    2, require.hpp:252:48: no definition found for '(unnamed struct at /usr/include/asio/require.hpp:252:48)', but a definition with the same name '(unnamed struct at /sep/third_party/crow/http_request.h:34:8)' found in another namespace 'crow'

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

Found 2 defect(s) in query_static_constexpr_member.hpp

Found no defects in config.cpp
Found no defects in pipewire_capture.cpp
[MEDIUM] /sep/src/blender/blender_integration.cpp:29:7: field 'm_processing_thread_active' will be initialized after field 'm_gpu_context' [clang-diagnostic-reorder-ctor]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 75f3b2edb4dd16cf5fe801d520f3978c
  Notes:
    1, blender_integration.cpp:29:7: m_gpu_context(nullptr) (fixit)
    2, blender_integration.cpp:29:5: , m_gpu_context(nullptr) (fixit)
    3, blender_integration.cpp:29:7: ~~~~~~~~~~~~~~~~~~~~~~ (fixit)
    4, blender_integration.cpp:29:7: m_processing_thread_active(false) (fixit)
  Steps:
    1, blender_integration.cpp:29:7: field 'm_processing_thread_active' will be initialized after field 'm_gpu_context'

[CRITICAL] /sep/src/blender/blender_integration.cpp:83:32: no member named 'NOT_INITIALIZED' in 'sep::SEPResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 48a9fc9d7fdc7c488b517ed2bef906ea
  Steps:
    1, blender_integration.cpp:83:32: no member named 'NOT_INITIALIZED' in 'sep::SEPResult'

[CRITICAL] /sep/src/blender/blender_integration.cpp:92:24: no member named 'isValidConfig' in namespace 'sep::pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b22fa560763467810905804dd9f88d60
  Steps:
    1, blender_integration.cpp:92:24: no member named 'isValidConfig' in namespace 'sep::pattern'

[CRITICAL] /sep/src/blender/blender_integration.cpp:182:27: cannot initialize a parameter of type 'SEPResult' with an lvalue of type 'sep::SEPResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d2368d158505313713170dad765b9ebb
  Steps:
    1, pattern_observer.h:20:36: passing argument to parameter 'error' here
    2, blender_integration.cpp:182:27: cannot initialize a parameter of type 'SEPResult' with an lvalue of type 'sep::SEPResult'

[CRITICAL] /sep/src/blender/blender_integration.cpp:301:32: no member named 'NOT_INITIALIZED' in 'sep::SEPResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 48a9fc9d7fdc7c488b517ed2bef906ea
  Steps:
    1, blender_integration.cpp:301:32: no member named 'NOT_INITIALIZED' in 'sep::SEPResult'

[CRITICAL] /sep/src/blender/blender_integration.cpp:314:32: no member named 'NOT_INITIALIZED' in 'sep::SEPResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 48a9fc9d7fdc7c488b517ed2bef906ea
  Steps:
    1, blender_integration.cpp:314:32: no member named 'NOT_INITIALIZED' in 'sep::SEPResult'

[CRITICAL] /sep/src/blender/blender_integration.cpp:337:32: no member named 'NOT_INITIALIZED' in 'sep::SEPResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 48a9fc9d7fdc7c488b517ed2bef906ea
  Steps:
    1, blender_integration.cpp:337:32: no member named 'NOT_INITIALIZED' in 'sep::SEPResult'

[CRITICAL] /sep/src/blender/blender_integration.cpp:361:32: no member named 'NOT_INITIALIZED' in 'sep::SEPResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 48a9fc9d7fdc7c488b517ed2bef906ea
  Steps:
    1, blender_integration.cpp:361:32: no member named 'NOT_INITIALIZED' in 'sep::SEPResult'

[CRITICAL] /sep/src/blender/blender_integration.cpp:432:31: no member named 'mutations' in 'sep::pattern::PatternData' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a7284b5e0bf2b46147e507b528a1d3d8
  Steps:
    1, blender_integration.cpp:432:31: no member named 'mutations' in 'sep::pattern::PatternData'

Found 9 defect(s) in blender_integration.cpp

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

[CRITICAL] /sep/src/context/processor.cpp:75:12: no member named 'success' in 'sep::ollama::EmbeddingRequest' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e337d349aff4d67ae8b25c81da171cd0
  Steps:
    1, processor.cpp:75:12: no member named 'success' in 'sep::ollama::EmbeddingRequest'

[CRITICAL] /sep/src/context/processor.cpp:76:12: no member named 'error' in 'sep::ollama::EmbeddingRequest' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a10bc584450d14312d0d3bfecad12073
  Steps:
    1, processor.cpp:76:12: no member named 'error' in 'sep::ollama::EmbeddingRequest'

[CRITICAL] /sep/src/context/processor.cpp:80:1: unknown type name 'SimilarityResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f08209d74d7d38ec9ba61bd7401ac1d6
  Steps:
    1, processor.cpp:80:1: unknown type name 'SimilarityResult'

[CRITICAL] /sep/src/context/processor.cpp:82:5: unknown type name 'SimilarityResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ac3b86f7ebd1e6cea614ddaee88a2bd1
  Steps:
    1, processor.cpp:82:5: unknown type name 'SimilarityResult'

[CRITICAL] /sep/src/context/processor.cpp:88:1: unknown type name 'BlendResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d84e0890d31a322d4e63ca6c560f18ad
  Steps:
    1, processor.cpp:88:1: unknown type name 'BlendResult'

[CRITICAL] /sep/src/context/processor.cpp:90:5: unknown type name 'BlendResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 34452909a7ad4281dcfcc81fefece865
  Steps:
    1, processor.cpp:90:5: unknown type name 'BlendResult'

[CRITICAL] /sep/src/context/processor.cpp:177:34: unknown type name 'ProcessOptions' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c2c21d6ec628aede79c0b930b6172431
  Steps:
    1, processor.cpp:177:34: unknown type name 'ProcessOptions'

[CRITICAL] /sep/src/context/processor.cpp:178:11: member initializer 'options_' does not name a non-static data member or base class [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1af144d9becab7a2432ae32d4eca0a4c
  Steps:
    1, processor.cpp:178:11: member initializer 'options_' does not name a non-static data member or base class

[CRITICAL] /sep/src/context/processor.cpp:184:11: member initializer 'pattern_processor_' does not name a non-static data member or base class [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3dde507898db2b31ea1eb260cdce593c
  Steps:
    1, processor.cpp:184:11: member initializer 'pattern_processor_' does not name a non-static data member or base class

[CRITICAL] /sep/src/context/processor.cpp:222:13: unknown type name 'ValidationResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0949dbdfb21da609b3c72183c08d51fb
  Steps:
    1, processor.cpp:222:13: unknown type name 'ValidationResult'

[CRITICAL] /sep/src/context/processor.cpp:225:51: no member named 'fail' in 'sep::quantum::ProcessingResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3fdbc32f4c44d2c88ed37887649a81e2
  Steps:
    1, processor.cpp:225:51: no member named 'fail' in 'sep::quantum::ProcessingResult'

[CRITICAL] /sep/src/context/processor.cpp:229:29: no member named 'success' in 'sep::ollama::EmbeddingRequest' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cdd4205dcd88fc1ffae7d0db2a6b581e
  Steps:
    1, processor.cpp:229:29: no member named 'success' in 'sep::ollama::EmbeddingRequest'

[CRITICAL] /sep/src/context/processor.cpp:231:51: no member named 'fail' in 'sep::quantum::ProcessingResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 07699e3f198ff683ab5a0215a86efe0b
  Steps:
    1, processor.cpp:231:51: no member named 'fail' in 'sep::quantum::ProcessingResult'

[CRITICAL] /sep/src/context/processor.cpp:231:67: no member named 'error' in 'sep::ollama::EmbeddingRequest' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 89a1eff40ad747e1cbba5c47db4fea93
  Steps:
    1, processor.cpp:231:67: no member named 'error' in 'sep::ollama::EmbeddingRequest'

[CRITICAL] /sep/src/context/processor.cpp:234:62: no member named 'value' in 'sep::ollama::EmbeddingRequest' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: be2f5e647804d577de6e4795b3490f30
  Steps:
    1, processor.cpp:234:62: no member named 'value' in 'sep::ollama::EmbeddingRequest'

[CRITICAL] /sep/src/context/processor.cpp:238:77: no member named 'value' in 'sep::ollama::EmbeddingRequest' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bbe3ab38e28841788797b7b3d586dd5f
  Steps:
    1, processor.cpp:238:77: no member named 'value' in 'sep::ollama::EmbeddingRequest'

[CRITICAL] /sep/src/context/processor.cpp:243:21: use of undeclared identifier 'metrics_' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fbe7bdcf30e2652cf73246e0b72419e7
  Steps:
    1, processor.cpp:243:21: use of undeclared identifier 'metrics_'

[CRITICAL] /sep/src/context/processor.cpp:299:64: only virtual member functions can be marked 'override' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a4b922da4ebac7e4e8f3a14d2d502c44
  Steps:
    1, processor.cpp:299:64: only virtual member functions can be marked 'override'

[CRITICAL] /sep/src/context/processor.cpp:341:5: unknown type name 'ValidationResult' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 062cf6ad4a8157d8f2ac4751ac5a6aba
  Steps:
    1, processor.cpp:341:5: unknown type name 'ValidationResult'

Found 19 defect(s) in processor.cpp

[MEDIUM] /sep/include/compat/kernels.cuh:47:27: unused parameter 'pattern' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 31d5f2cb4a0073e44a643481c4cc0d9c
  Steps:
    1, kernels.cuh:47:27: unused parameter 'pattern'

[MEDIUM] /sep/include/compat/kernels.cuh:48:27: unused parameter 'result' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5e3e639c90699be9f1e68511d19a7617
  Steps:
    1, kernels.cuh:48:27: unused parameter 'result'

[MEDIUM] /sep/include/compat/kernels.cuh:49:35: unused parameter 'config' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eb015f5d301c785fe9ee54e041abb80c
  Steps:
    1, kernels.cuh:49:35: unused parameter 'config'

[MEDIUM] /sep/include/compat/kernels.cuh:50:12: unused parameter 'pattern_count' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bbdf69baf0632c6003f677ee2c7c0b3f
  Steps:
    1, kernels.cuh:50:12: unused parameter 'pattern_count'

[MEDIUM] /sep/include/compat/kernels.cuh:51:33: unused parameter 'previous_patterns' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e90d8ad10723fe736ad64b04db58160a
  Steps:
    1, kernels.cuh:51:33: unused parameter 'previous_patterns'

Found 5 defect(s) in kernels.cuh

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:178:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:178:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:208:38: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 474cc343d2d8bde6f322968e111612fc
  Steps:
    1, memory_tier_manager.cpp:208:38: implicit conversion increases floating-point precision: 'float' to 'double'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:228:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 22578aad3d8b17e3124a9ac3f3993c05
  Steps:
    1, memory_tier_manager.cpp:228:14: no member named 'metadata' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:229:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 38f223a02190f76379588c8f82e127aa
  Steps:
    1, memory_tier_manager.cpp:229:14: no member named 'metadata' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:230:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0a4b256439c8305744ebf9d9fb6229cb
  Steps:
    1, memory_tier_manager.cpp:230:14: no member named 'metadata' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:245:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 22578aad3d8b17e3124a9ac3f3993c05
  Steps:
    1, memory_tier_manager.cpp:245:14: no member named 'metadata' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:246:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 38f223a02190f76379588c8f82e127aa
  Steps:
    1, memory_tier_manager.cpp:246:14: no member named 'metadata' in 'sep::quantum::Pattern'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:247:14: no member named 'metadata' in 'sep::quantum::Pattern' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 0a4b256439c8305744ebf9d9fb6229cb
  Steps:
    1, memory_tier_manager.cpp:247:14: no member named 'metadata' in 'sep::quantum::Pattern'

Found 8 defect(s) in memory_tier_manager.cpp

[CRITICAL] /sep/include/compat/cuda_impl.h:48:5: redefinition of enumerator 'cudaMemcpyHostToHost' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9df86d3761701d1ed990d51164918230
  Steps:
    1, cuda_runtime.h:28:5: previous definition is here
    2, cuda_impl.h:48:5: redefinition of enumerator 'cudaMemcpyHostToHost'

[CRITICAL] /sep/include/compat/cuda_impl.h:49:5: redefinition of enumerator 'cudaMemcpyHostToDevice' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 68eb29c4520a2f0a38457ea58f75da6a
  Steps:
    1, cuda_runtime.h:29:5: previous definition is here
    2, cuda_impl.h:49:5: redefinition of enumerator 'cudaMemcpyHostToDevice'

[CRITICAL] /sep/include/compat/cuda_impl.h:50:5: redefinition of enumerator 'cudaMemcpyDeviceToHost' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 572503be17b3e2cfc81c67a439667646
  Steps:
    1, cuda_runtime.h:30:5: previous definition is here
    2, cuda_impl.h:50:5: redefinition of enumerator 'cudaMemcpyDeviceToHost'

[CRITICAL] /sep/include/compat/cuda_impl.h:51:5: redefinition of enumerator 'cudaMemcpyDeviceToDevice' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 28c6a3fa4e5f4eea314455b53d0c3019
  Steps:
    1, cuda_runtime.h:31:5: previous definition is here
    2, cuda_impl.h:51:5: redefinition of enumerator 'cudaMemcpyDeviceToDevice'

[CRITICAL] /sep/include/compat/cuda_impl.h:52:5: redefinition of enumerator 'cudaMemcpyDefault' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ce22c11374037f863aee098e9de03711
  Steps:
    1, cuda_runtime.h:32:5: previous definition is here
    2, cuda_impl.h:52:5: redefinition of enumerator 'cudaMemcpyDefault'

Found 5 defect(s) in cuda_impl.h

[CRITICAL] /sep/include/compat/cuda_runtime.h:59:5: redefinition of 'cudaSuccess' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 3fb6bc3f3f4172b08cbedb8641e049a7
  Steps:
    1, cuda.h:69:26: previous definition is here
    2, cuda_runtime.h:59:5: redefinition of 'cudaSuccess'

[CRITICAL] /sep/include/compat/cuda_runtime.h:60:5: redefinition of 'cudaErrorMemoryAllocation' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 69e6fe456e0eb77ed988e9efc6389f90
  Steps:
    1, cuda.h:70:26: previous definition is here
    2, cuda_runtime.h:60:5: redefinition of 'cudaErrorMemoryAllocation'

[CRITICAL] /sep/include/compat/cuda_runtime.h:61:5: redefinition of 'cudaErrorInitializationError' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 19ccb9fceeb6a90284020b18537ebd3c
  Steps:
    1, cuda.h:71:26: previous definition is here
    2, cuda_runtime.h:61:5: redefinition of 'cudaErrorInitializationError'

[CRITICAL] /sep/include/compat/cuda_runtime.h:62:5: redefinition of 'cudaErrorInvalidDevice' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: fa133e9808e523a7b9cb11be05869cb1
  Steps:
    1, cuda.h:72:26: previous definition is here
    2, cuda_runtime.h:62:5: redefinition of 'cudaErrorInvalidDevice'

[CRITICAL] /sep/include/compat/cuda_runtime.h:63:5: redefinition of 'cudaErrorDeviceUninitialized' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5aecdf8f7f6216889c22f87ddcfc627b
  Steps:
    1, cuda.h:73:26: previous definition is here
    2, cuda_runtime.h:63:5: redefinition of 'cudaErrorDeviceUninitialized'

[CRITICAL] /sep/include/compat/cuda_runtime.h:64:5: redefinition of 'cudaErrorInvalidValue' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 968f97a08375034aa52297d53de8ceb1
  Steps:
    1, cuda.h:74:26: previous definition is here
    2, cuda_runtime.h:64:5: redefinition of 'cudaErrorInvalidValue'

[CRITICAL] /sep/include/compat/cuda_runtime.h:65:5: redefinition of 'cudaErrorNotReady' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 35389934a7ff57fedb51567bbd26f761
  Steps:
    1, cuda.h:75:26: previous definition is here
    2, cuda_runtime.h:65:5: redefinition of 'cudaErrorNotReady'

[CRITICAL] /sep/include/compat/cuda_runtime.h:66:5: redefinition of 'cudaErrorSetOnActiveProcess' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 27eeb742d70f1bfe1af350d5993aa75f
  Steps:
    1, cuda.h:76:26: previous definition is here
    2, cuda_runtime.h:66:5: redefinition of 'cudaErrorSetOnActiveProcess'

[CRITICAL] /sep/include/compat/cuda_runtime.h:67:5: redefinition of 'cudaErrorStreamCaptureUnsupported' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 27cabff6f2e7a08effb78ddab57c6e1e
  Steps:
    1, cuda.h:77:26: previous definition is here
    2, cuda_runtime.h:67:5: redefinition of 'cudaErrorStreamCaptureUnsupported'

[CRITICAL] /sep/include/compat/cuda_runtime.h:68:5: redefinition of 'cudaErrorInvalidMemcpyDirection' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9e94535f869ad409b0a8f4801e825ffe
  Steps:
    1, cuda.h:78:26: previous definition is here
    2, cuda_runtime.h:68:5: redefinition of 'cudaErrorInvalidMemcpyDirection'

[CRITICAL] /sep/include/compat/cuda_runtime.h:69:5: redefinition of 'cudaErrorInvalidResourceHandle' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 57ea7be1ca19d2b4b246d91d3ab842ac
  Steps:
    1, cuda.h:79:26: previous definition is here
    2, cuda_runtime.h:69:5: redefinition of 'cudaErrorInvalidResourceHandle'

[CRITICAL] /sep/include/compat/cuda_runtime.h:70:3: typedef redefinition with different types ('enum cudaError_t' vs 'int') [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b5ab6717120159b0a29097ba5893581d
  Steps:
    1, cuda.h:19:13: previous definition is here
    2, cuda_runtime.h:70:3: typedef redefinition with different types ('enum cudaError_t' vs 'int')

Found 12 defect(s) in cuda_runtime.h

[CRITICAL] /sep/include/core/engine.h:52:57: no member named 'PinState' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: ab4970105732d2235d07299ab21e37f7
  Steps:
    1, engine.h:52:57: no member named 'PinState' in namespace 'sep'

[CRITICAL] /sep/include/core/engine.h:57:55: no member named 'PinState' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9f2c428153c2eabe4ffd98fb9c66429d
  Steps:
    1, engine.h:57:55: no member named 'PinState' in namespace 'sep'

Found 2 defect(s) in engine.h

[CRITICAL] /sep/src/memory/redis_manager.cpp:83:35: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: b62b8dc6257464bd7ba2a671f0dbfcf6
  Steps:
    1, redis_manager.cpp:83:35: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/memory/redis_manager.cpp:165:24: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6700d96ef57453f55a2a74b6909ae872
  Steps:
    1, redis_manager.cpp:165:24: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/memory/redis_manager.cpp:170:20: no viable conversion from returned value of type 'const nullopt_t' to function return type 'int' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4653b8086c92ae27f5088cd85c985af7
  Steps:
    1, redis_manager.cpp:170:20: no viable conversion from returned value of type 'const nullopt_t' to function return type 'int'

[CRITICAL] /sep/src/memory/redis_manager.cpp:184:20: no viable conversion from returned value of type 'const nullopt_t' to function return type 'int' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4653b8086c92ae27f5088cd85c985af7
  Steps:
    1, redis_manager.cpp:184:20: no viable conversion from returned value of type 'const nullopt_t' to function return type 'int'

[CRITICAL] /sep/src/memory/redis_manager.cpp:188:14: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: d7ce7b5e106eaa9ffeadec5222fc1b48
  Steps:
    1, redis_manager.cpp:188:14: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/memory/redis_manager.cpp:195:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 80e4a3f803d1a5b1f7c942613f8bbd16
  Steps:
    1, range_access.h:324:5: possible target for call
    2, range_access.h:335:5: possible target for call
    3, range_access.h:346:5: possible target for call
    4, range_access.h:356:5: possible target for call
    5, redis_manager.cpp:195:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:196:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 283285ef7d8c2b02a18982d7a82bcfca
  Steps:
    1, range_access.h:324:5: possible target for call
    2, range_access.h:335:5: possible target for call
    3, range_access.h:346:5: possible target for call
    4, range_access.h:356:5: possible target for call
    5, redis_manager.cpp:196:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:197:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 6a4c4f58e94e30d6c28f5c823dab79a0
  Steps:
    1, range_access.h:324:5: possible target for call
    2, range_access.h:335:5: possible target for call
    3, range_access.h:346:5: possible target for call
    4, range_access.h:356:5: possible target for call
    5, redis_manager.cpp:197:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:209:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf5dcc9631936578f943773a6b675930
  Steps:
    1, range_access.h:324:5: possible target for call
    2, range_access.h:335:5: possible target for call
    3, range_access.h:346:5: possible target for call
    4, range_access.h:356:5: possible target for call
    5, redis_manager.cpp:209:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:210:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c934e09eb97ac0353962eb8c25454e33
  Steps:
    1, range_access.h:324:5: possible target for call
    2, range_access.h:335:5: possible target for call
    3, range_access.h:346:5: possible target for call
    4, range_access.h:356:5: possible target for call
    5, redis_manager.cpp:210:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:211:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a45f55664e4f9cc776311963106aa07b
  Steps:
    1, range_access.h:324:5: possible target for call
    2, range_access.h:335:5: possible target for call
    3, range_access.h:346:5: possible target for call
    4, range_access.h:356:5: possible target for call
    5, redis_manager.cpp:211:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:214:13: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf5dcc9631936578f943773a6b675930
  Steps:
    1, range_access.h:324:5: possible target for call
    2, range_access.h:335:5: possible target for call
    3, range_access.h:346:5: possible target for call
    4, range_access.h:356:5: possible target for call
    5, redis_manager.cpp:214:13: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:243:22: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 05908cd9fa0be50cfbe28798fda4fa3a
  Steps:
    1, redis_manager.cpp:243:22: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/memory/redis_manager.cpp:260:17: reference to overloaded function could not be resolved; did you mean to call it? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 482315f63322f6db87bdc2ea024bf4bd
  Steps:
    1, range_access.h:324:5: possible target for call
    2, redis_manager.cpp:260:17: reference to overloaded function could not be resolved; did you mean to call it?

[CRITICAL] /sep/src/memory/redis_manager.cpp:260:17: use of undeclared identifier 'data'; did you mean 'std::data'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e453de42a79754ea267e5801e52c4bec
  Notes:
    1, redis_manager.cpp:260:17: std::data (fixit)
  Steps:
    1, range_access.h:324:5: 'std::data' declared here
    2, redis_manager.cpp:260:17: use of undeclared identifier 'data'; did you mean 'std::data'?

[CRITICAL] /sep/src/memory/redis_manager.cpp:269:83: use of undeclared identifier 'data' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 499ebe7d4556ac9feed530391345e963
  Steps:
    1, redis_manager.cpp:269:83: use of undeclared identifier 'data'

[CRITICAL] /sep/src/memory/redis_manager.cpp:272:16: use of undeclared identifier 'data'; did you mean 'atan'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5e2d26a72af0064272931a4d5c52dade
  Notes:
    1, redis_manager.cpp:272:16: atan (fixit)
  Steps:
    1, math.h:41:12: 'atan' declared here
    2, redis_manager.cpp:272:16: use of undeclared identifier 'data'; did you mean 'atan'?

[CRITICAL] /sep/src/memory/redis_manager.cpp:347:66: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: de39261a3b001e8fdcdc9247fb825f2a
  Steps:
    1, redis_manager.cpp:347:66: no member named 'pattern' in namespace 'sep'

[CRITICAL] /sep/src/memory/redis_manager.cpp:355:22: no member named 'pattern' in namespace 'sep' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f749ab34e6106feb93a6ec3598c50873
  Steps:
    1, redis_manager.cpp:355:22: no member named 'pattern' in namespace 'sep'

Found 19 defect(s) in redis_manager.cpp

Found no defects in memory_tier.cpp
Found no defects in manager.cpp
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

[CRITICAL] /sep/include/api/bridge.hpp:16:52: no type named 'quantum' in namespace 'sep::quantum' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2730f6f6a79670e237c7412901d4c2a1
  Steps:
    1, bridge.hpp:16:52: no type named 'quantum' in namespace 'sep::quantum'

[CRITICAL] /sep/include/api/bridge.hpp:17:1: no type named 'Context' in namespace 'sep::quantum'; did you mean 'context::Context'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 358a14f3123a2a7e6d2461719920c043
  Notes:
    1, bridge.hpp:17:1: context::Context (fixit)
  Steps:
    1, resource_predictor.h:12:8: 'context::Context' declared here
    2, bridge.hpp:17:1: no type named 'Context' in namespace 'sep::quantum'; did you mean 'context::Context'?

[CRITICAL] /sep/include/api/bridge.hpp:18:35: no type named 'CheckResult' in namespace 'sep::quantum'; did you mean 'context::CheckResult'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: afe2a7a47178d1f19cbc9506291dbc81
  Notes:
    1, bridge.hpp:18:35: context::CheckResult (fixit)
  Steps:
    1, resource_predictor.h:21:8: 'context::CheckResult' declared here
    2, bridge.hpp:18:35: no type named 'CheckResult' in namespace 'sep::quantum'; did you mean 'context::CheckResult'?

[CRITICAL] /sep/include/api/bridge.hpp:19:1: no type named 'CheckResult' in namespace 'sep::quantum'; did you mean 'context::CheckResult'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 5599c0cacc788f1d1c67d3db9eaf8a78
  Notes:
    1, bridge.hpp:19:1: context::CheckResult (fixit)
  Steps:
    1, resource_predictor.h:21:8: 'context::CheckResult' declared here
    2, bridge.hpp:19:1: no type named 'CheckResult' in namespace 'sep::quantum'; did you mean 'context::CheckResult'?

Found 4 defect(s) in bridge.hpp

[LOW] /sep/include/api/bridge_internal.hpp:10:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum' [bugprone-forward-declaration-namespace]
class Processor;
      ^
  Report hash: 5ca7fa69e4196bf69f99f847456b1c42
  Steps:
    1, processor.h:79:7: a definition of 'Processor' is found here
    2, bridge_internal.hpp:10:7: no definition found for 'Processor', but a definition with the same name 'Processor' found in another namespace 'sep::quantum'

Found 1 defect(s) in bridge_internal.hpp

[MEDIUM] /sep/src/blender/blender_integration.cpp:28:7: field 'm_processing_thread_active' will be initialized after field 'm_gpu_context' [clang-diagnostic-reorder-ctor]
    : m_processing_thread_active(false)
      ^
  Report hash: 75f3b2edb4dd16cf5fe801d520f3978c
  Notes:
    1, blender_integration.cpp:28:7: m_gpu_context(nullptr) (fixit)
    2, blender_integration.cpp:28:5: , m_gpu_context(nullptr) (fixit)
    3, blender_integration.cpp:28:7: ~~~~~~~~~~~~~~~~~~~~~~ (fixit)
    4, blender_integration.cpp:28:7: m_processing_thread_active(false) (fixit)
  Steps:
    1, blender_integration.cpp:28:7: field 'm_processing_thread_active' will be initialized after field 'm_gpu_context'

Found 1 defect(s) in blender_integration.cpp

[MEDIUM] /sep/include/compat/kernels.cuh:46:6: unused function 'process_pattern_kernel' [clang-diagnostic-unused-function]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 55269f362da979aebf488ceb4a65c5d9
  Steps:
    1, kernels.cuh:46:6: unused function 'process_pattern_kernel'

Found 1 defect(s) in kernels.cuh

[MEDIUM] /sep/src/core/engine.cpp:141:94: unused parameter 'tick' [clang-diagnostic-unused-parameter]
                             ::sep::shim::vector<std::uint32_t>& expectations, std::uint64_t tick ) {
                                                                                             ^
  Report hash: b101bc606cec5d9ec82a9c68115450f0
  Steps:
    1, engine.cpp:141:94: unused parameter 'tick'

Found 1 defect(s) in engine.cpp

Found no defects in lock_free_rate_limiter.cpp
Found no defects in rate_limit_middleware.cpp
Found no defects in pattern_processor_interface.cpp
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

[MEDIUM] /sep/include/api/types.h:48:54: unused parameter 'name' [clang-diagnostic-unused-parameter]
    virtual std::string getHeader(const std::string& name) const {
                                                     ^
  Report hash: 4d36d5ee5324c8a8ec1018571fda19eb
  Steps:
    1, types.h:48:54: unused parameter 'name'

Found 1 defect(s) in types.h

[MEDIUM] /sep/include/api/server.h:23:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
struct request;
^
  Report hash: 7efebc8bbaef775aeab2e28a25dc4a5c
  Steps:
    1, crow_isolation.h:47:11: previous use is here
    2, server.h:23:1: did you mean class here?
    3, server.h:23:1: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/server.h:24:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
struct response;
^
  Report hash: 59bfeb4242018ecdefde0c5a8f41634e
  Steps:
    1, crow_isolation.h:62:11: previous use is here
    2, server.h:24:1: did you mean class here?
    3, server.h:24:1: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in server.h

[LOW] /sep/include/compat/cuda_runtime.h:17:10: circular header file dependency detected while including 'cuda_runtime.h', please check the include path [misc-header-include-cycle]
#include <cuda_runtime.h>
         ^
  Report hash: 7770aa0834f5001780a1dba944dcc3db
  Steps:
    1, cuda_runtime.h:1:10: 'cuda_runtime.h' included from here
    2, cuda.h:8:10: 'cuda_runtime.h' included from here
    3, cuda_runtime.h:17:10: circular header file dependency detected while including 'cuda_runtime.h', please check the include path

Found 1 defect(s) in cuda_runtime.h

[LOW] /sep/include/cuda_runtime.h:1:10: circular header file dependency detected while including 'cuda_runtime.h', please check the include path [misc-header-include-cycle]
#include "compat/cuda_runtime.h"
         ^
  Report hash: 3d301c864812cdf7a962d3ef9218156a
  Steps:
    1, cuda_runtime.h:17:10: 'cuda_runtime.h' included from here
    2, cuda_runtime.h:1:10: 'cuda_runtime.h' included from here
    3, cuda_runtime.h:1:10: circular header file dependency detected while including 'cuda_runtime.h', please check the include path

Found 1 defect(s) in cuda_runtime.h

[MEDIUM] /sep/include/memory/memory_tier.hpp:61:41: field 'tier' will be initialized after field 'original_size' [clang-diagnostic-reorder-ctor]
        : ptr(p), size(s), offset(off), tier(t), original_size(s) {}
                                        ^
  Report hash: 5c71e33c2548a8c4133274e264137cc8
  Notes:
    1, memory_tier.hpp:61:41: original_size(s) tier(t) (fixit)
  Steps:
    1, memory_tier.hpp:61:41: field 'tier' will be initialized after field 'original_size'

Found 1 defect(s) in memory_tier.hpp

Found no defects in crow_adapter.cpp
[CRITICAL] /sep/include/compat/cuda_common.h:16:42: unknown type name 'cudaError_t' [clang-diagnostic-error]
void logCudaError(const char* operation, cudaError_t error);
                                         ^
  Report hash: 7d607958e3c3c1cea6a7c722226e6772
  Steps:
    1, cuda_common.h:16:42: unknown type name 'cudaError_t'

Found 1 defect(s) in cuda_common.h

[CRITICAL] /sep/include/compat/cuda_helpers.h:19:58: unknown type name 'cudaError_t' [clang-diagnostic-error]
SEP_HOST inline void logCudaError(const char* operation, cudaError_t error) {
                                                         ^
  Report hash: aeb98d23802a61556ef835f88e95b00d
  Steps:
    1, cuda_helpers.h:19:58: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/cuda_helpers.h:20:18: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
    if (error != cudaSuccess) {
                 ^
  Report hash: ccee2e2d33b422eb752b7570f0eceba5
  Steps:
    1, cuda_helpers.h:20:18: use of undeclared identifier 'cudaSuccess'

Found 2 defect(s) in cuda_helpers.h

[CRITICAL] /sep/include/compat/stream_impl.h:32:9: unknown type name 'cudaError_t' [clang-diagnostic-error]
        cudaError_t err = cudaStreamDestroy(stream_);
        ^
  Report hash: a5d7a58018108e34ea1cbc138451b901
  Steps:
    1, stream_impl.h:32:9: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/stream_impl.h:33:20: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
        if (err != cudaSuccess) {
                   ^
  Report hash: cfc6da8190c8939c04541069ec93d313
  Steps:
    1, stream_impl.h:33:20: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/stream_impl.h:45:7: unknown type name 'cudaError_t' [clang-diagnostic-error]
      cudaError_t err = cudaStreamDestroy(stream_);
      ^
  Report hash: 8e71a32e820008be5be981f0e4515842
  Steps:
    1, stream_impl.h:45:7: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/stream_impl.h:46:18: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
      if (err != cudaSuccess) {
                 ^
  Report hash: 24b44ad010a12880dbf185129646ba04
  Steps:
    1, stream_impl.h:46:18: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/stream_impl.h:55:7: unknown type name 'cudaError_t' [clang-diagnostic-error]
      cudaError_t err = cudaStreamSynchronize(stream_);
      ^
  Report hash: 08f3306cd6078a6da9018ca26432b498
  Steps:
    1, stream_impl.h:55:7: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/stream_impl.h:56:18: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
      if (err != cudaSuccess) {
                 ^
  Report hash: 24b44ad010a12880dbf185129646ba04
  Steps:
    1, stream_impl.h:56:18: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/stream_impl.h:66:11: unknown type name 'cudaError_t' [clang-diagnostic-error]
          cudaError_t err = cudaSuccess;
          ^
  Report hash: fa98e2def6ec5034f768f29eb40d7a20
  Steps:
    1, stream_impl.h:66:11: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/stream_impl.h:66:29: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
          cudaError_t err = cudaSuccess;
                            ^
  Report hash: 9238516b0c497ab838795f5496aceae9
  Steps:
    1, stream_impl.h:66:29: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/stream_impl.h:69:22: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
          if (err != cudaSuccess) {
                     ^
  Report hash: 861bb1c72cdb0c903ab376c44ed7db3b
  Steps:
    1, stream_impl.h:69:22: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/stream_impl.h:79:11: unknown type name 'cudaError_t' [clang-diagnostic-error]
          cudaError_t err = cudaSuccess;
          ^
  Report hash: fa98e2def6ec5034f768f29eb40d7a20
  Steps:
    1, stream_impl.h:79:11: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/stream_impl.h:79:29: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
          cudaError_t err = cudaSuccess;
                            ^
  Report hash: 9238516b0c497ab838795f5496aceae9
  Steps:
    1, stream_impl.h:79:29: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/stream_impl.h:82:22: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
          if (err != cudaSuccess) {
                     ^
  Report hash: 861bb1c72cdb0c903ab376c44ed7db3b
  Steps:
    1, stream_impl.h:82:22: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/include/compat/stream_impl.h:90:18: unknown type name 'cudaStream_t' [clang-diagnostic-error]
  void setHandle(cudaStream_t stream) { stream_ = stream; }
                 ^
  Report hash: c9edbb5cf38cca088f5b96053ad2c570
  Steps:
    1, stream_impl.h:90:18: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/stream_impl.h:93:3: unknown type name 'cudaStream_t' [clang-diagnostic-error]
  cudaStream_t stream_;
  ^
  Report hash: ab2929dc947bd71779f3d714ff34fcf2
  Steps:
    1, stream_impl.h:93:3: unknown type name 'cudaStream_t'

Found 14 defect(s) in stream_impl.h

[CRITICAL] /sep/src/compat/core/stream.cpp:21:30: cannot initialize object parameter of type 'sep::cuda::impl::StreamImpl' with an expression of type 'sep::cuda::Stream::Impl' [clang-diagnostic-error]
void Stream::synchronize() { impl_->synchronize(); }
                             ^
  Report hash: 74a1bcef00f489c35d29258a27a84781
  Steps:
    1, stream.cpp:21:30: cannot initialize object parameter of type 'sep::cuda::impl::StreamImpl' with an expression of type 'sep::cuda::Stream::Impl'

[CRITICAL] /sep/src/compat/core/stream.cpp:23:34: cannot initialize object parameter of type 'sep::cuda::impl::StreamImpl' with an expression of type 'sep::cuda::Stream::Impl' [clang-diagnostic-error]
void Stream::wait(void* event) { impl_->wait(event); }
                                 ^
  Report hash: 3cb86424a231a685e1f6b80b534307ca
  Steps:
    1, stream.cpp:23:34: cannot initialize object parameter of type 'sep::cuda::impl::StreamImpl' with an expression of type 'sep::cuda::Stream::Impl'

Found 2 defect(s) in stream.cpp

[MEDIUM] /sep/src/api/server.cpp:157:84: unused parameter 'body' [clang-diagnostic-unused-parameter]
void SEPApiServer::logRequest(const HttpRequest& req, int code, const std::string& body,
                                                                                   ^
  Report hash: 99c1cd2d504e0473209900460e079018
  Steps:
    1, server.cpp:157:84: unused parameter 'body'

[MEDIUM] /sep/src/api/server.cpp:217:50: unused parameter 'response_body' [clang-diagnostic-unused-parameter]
                              const std::string& response_body, int64_t duration_ms) {
                                                 ^
  Report hash: 9a6489114b93b9375b7c0c11dd032dae
  Steps:
    1, server.cpp:217:50: unused parameter 'response_body'

[MEDIUM] /sep/src/api/server.cpp:281:10: unused variable 'rate_limit_mw' [clang-diagnostic-unused-variable]
   auto& rate_limit_mw =
         ^
  Report hash: e3e4ad56877412f3b8b4bd5437469c50
  Steps:
    1, server.cpp:281:10: unused variable 'rate_limit_mw'

Found 3 defect(s) in server.cpp

[LOW] /sep/include/compat/cuda_runtime.h:17:10: circular header file dependency detected while including 'cuda_runtime.h', please check the include path [misc-header-include-cycle]
#include <cuda_runtime.h>
         ^
  Report hash: 7770aa0834f5001780a1dba944dcc3db
  Steps:
    1, cuda_runtime.h:1:10: 'cuda_runtime.h' included from here
    2, cuda_common.h:7:10: 'cuda_runtime.h' included from here
    3, cuda_runtime.h:17:10: circular header file dependency detected while including 'cuda_runtime.h', please check the include path

Found 1 defect(s) in cuda_runtime.h

[CRITICAL] /sep/src/core/metrics_collector.cpp:32:5: unknown type name 'cudaError_t' [clang-diagnostic-error]
    CUDA_CHECK(cudaEventCreate(&start_event_));
    ^
  Report hash: c053cd011464e3d00b9bbe2cd930ddb6
  Steps:
    1, cuda_helpers.h:29:9: expanded from macro 'CUDA_CHECK'
    2, metrics_collector.cpp:32:5: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/core/metrics_collector.cpp:32:5: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
    CUDA_CHECK(cudaEventCreate(&start_event_));
    ^
  Report hash: 10855b0a86d025d33b6d877d5dd3a881
  Steps:
    1, cuda_helpers.h:30:22: expanded from macro 'CUDA_CHECK'
    2, unistd.h:292:12: 'euidaccess' declared here
    3, metrics_collector.cpp:32:5: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:33:5: unknown type name 'cudaError_t' [clang-diagnostic-error]
    CUDA_CHECK(cudaEventCreate(&stop_event_));
    ^
  Report hash: f0142186ecc9c9b32abde69ec16dae82
  Steps:
    1, cuda_helpers.h:29:9: expanded from macro 'CUDA_CHECK'
    2, metrics_collector.cpp:33:5: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/core/metrics_collector.cpp:33:5: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
    CUDA_CHECK(cudaEventCreate(&stop_event_));
    ^
  Report hash: 97c8cd12ce273b9d1597dfecd4b5b9ca
  Steps:
    1, cuda_helpers.h:30:22: expanded from macro 'CUDA_CHECK'
    2, unistd.h:292:12: 'euidaccess' declared here
    3, metrics_collector.cpp:33:5: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:44:7: unknown type name 'cudaError_t' [clang-diagnostic-error]
      CUDA_CHECK(cudaEventDestroy(start_event_));
      ^
  Report hash: 487f342b07a6c7d216e691e4b9a4fb22
  Steps:
    1, cuda_helpers.h:29:9: expanded from macro 'CUDA_CHECK'
    2, metrics_collector.cpp:44:7: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/core/metrics_collector.cpp:44:7: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
      CUDA_CHECK(cudaEventDestroy(start_event_));
      ^
  Report hash: 58366daef6bd45e87a520ba538f0c9b2
  Steps:
    1, cuda_helpers.h:30:22: expanded from macro 'CUDA_CHECK'
    2, unistd.h:292:12: 'euidaccess' declared here
    3, metrics_collector.cpp:44:7: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:49:7: unknown type name 'cudaError_t' [clang-diagnostic-error]
      CUDA_CHECK(cudaEventDestroy(stop_event_));
      ^
  Report hash: 2027923da5709df3c426122992002a19
  Steps:
    1, cuda_helpers.h:29:9: expanded from macro 'CUDA_CHECK'
    2, metrics_collector.cpp:49:7: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/core/metrics_collector.cpp:49:7: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
      CUDA_CHECK(cudaEventDestroy(stop_event_));
      ^
  Report hash: df652a8cf576e55ab9f4c965219ca2f4
  Steps:
    1, cuda_helpers.h:30:22: expanded from macro 'CUDA_CHECK'
    2, unistd.h:292:12: 'euidaccess' declared here
    3, metrics_collector.cpp:49:7: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:91:7: unknown type name 'cudaError_t' [clang-diagnostic-error]
      CUDA_CHECK(cudaEventRecord(start_event_, nullptr));
      ^
  Report hash: 3491c1341fcfa57d0b22482da5d2a40f
  Steps:
    1, cuda_helpers.h:29:9: expanded from macro 'CUDA_CHECK'
    2, metrics_collector.cpp:91:7: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/core/metrics_collector.cpp:91:7: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
      CUDA_CHECK(cudaEventRecord(start_event_, nullptr));
      ^
  Report hash: a2d41cc89a96c074be0d36c0f2c6e538
  Steps:
    1, cuda_helpers.h:30:22: expanded from macro 'CUDA_CHECK'
    2, unistd.h:292:12: 'euidaccess' declared here
    3, metrics_collector.cpp:91:7: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:99:7: unknown type name 'cudaError_t' [clang-diagnostic-error]
      CUDA_CHECK(cudaEventRecord(stop_event_, nullptr));
      ^
  Report hash: b04d5a27b7c5d2a45d1de0a791d0cec6
  Steps:
    1, cuda_helpers.h:29:9: expanded from macro 'CUDA_CHECK'
    2, metrics_collector.cpp:99:7: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/core/metrics_collector.cpp:99:7: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
      CUDA_CHECK(cudaEventRecord(stop_event_, nullptr));
      ^
  Report hash: 01ec3e32cc423d342002a58894076d2c
  Steps:
    1, cuda_helpers.h:30:22: expanded from macro 'CUDA_CHECK'
    2, unistd.h:292:12: 'euidaccess' declared here
    3, metrics_collector.cpp:99:7: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:100:7: unknown type name 'cudaError_t' [clang-diagnostic-error]
      CUDA_CHECK(cudaEventSynchronize(stop_event_));
      ^
  Report hash: 3dd1d10e6fdeeb7b70641170de0a420d
  Steps:
    1, cuda_helpers.h:29:9: expanded from macro 'CUDA_CHECK'
    2, metrics_collector.cpp:100:7: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/core/metrics_collector.cpp:100:7: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
      CUDA_CHECK(cudaEventSynchronize(stop_event_));
      ^
  Report hash: c94594f9ed9e3dbe48ba26289153556d
  Steps:
    1, cuda_helpers.h:30:22: expanded from macro 'CUDA_CHECK'
    2, unistd.h:292:12: 'euidaccess' declared here
    3, metrics_collector.cpp:100:7: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/core/metrics_collector.cpp:223:3: unknown type name 'cudaEvent_t' [clang-diagnostic-error]
  cudaEvent_t start_event_{nullptr};
  ^
  Report hash: ff09312e89057d67720c9afcfe9ebf80
  Steps:
    1, metrics_collector.cpp:223:3: unknown type name 'cudaEvent_t'

[CRITICAL] /sep/src/core/metrics_collector.cpp:224:3: unknown type name 'cudaEvent_t' [clang-diagnostic-error]
  cudaEvent_t stop_event_{nullptr};
  ^
  Report hash: 6694da91cc605beeaa57784bf68f2f6a
  Steps:
    1, metrics_collector.cpp:224:3: unknown type name 'cudaEvent_t'

Found 16 defect(s) in metrics_collector.cpp

[LOW] /sep/include/compat/cuda_runtime.h:17:10: circular header file dependency detected while including 'cuda_runtime.h', please check the include path [misc-header-include-cycle]
#include <cuda_runtime.h>
         ^
  Report hash: 7770aa0834f5001780a1dba944dcc3db
  Steps:
    1, cuda_runtime.h:1:10: 'cuda_runtime.h' included from here
    2, cuda_defs.h:10:10: 'cuda_runtime.h' included from here
    3, cuda_runtime.h:17:10: circular header file dependency detected while including 'cuda_runtime.h', please check the include path

Found 1 defect(s) in cuda_runtime.h

[CRITICAL] /sep/include/compat/raii.h:26:3: unknown type name 'cudaStream_t' [clang-diagnostic-error]
  cudaStream_t get() const { return stream_; }
  ^
  Report hash: fdc710988b3c1dbee6980c05820b779b
  Steps:
    1, raii.h:26:3: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/raii.h:31:3: unknown type name 'cudaStream_t' [clang-diagnostic-error]
  cudaStream_t stream_{nullptr};
  ^
  Report hash: a7678a52d823191cb3c93c83d73733ae
  Steps:
    1, raii.h:31:3: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/raii.h:45:3: unknown type name 'cudaEvent_t' [clang-diagnostic-error]
  cudaEvent_t get() const { return event_; }
  ^
  Report hash: 6b9584b8b14fe69259fdd2f9bd80ddf3
  Steps:
    1, raii.h:45:3: unknown type name 'cudaEvent_t'

[CRITICAL] /sep/include/compat/raii.h:50:3: unknown type name 'cudaEvent_t' [clang-diagnostic-error]
  cudaEvent_t event_{nullptr};
  ^
  Report hash: 41801143c398346ed19618bb00b19e2d
  Steps:
    1, raii.h:50:3: unknown type name 'cudaEvent_t'

[CRITICAL] /sep/include/compat/raii.h:80:47: unknown type name 'cudaStream_t' [clang-diagnostic-error]
void* allocateUnifiedMemory(std::size_t size, cudaStream_t stream = nullptr);
                                              ^
  Report hash: 5cc0e97c327cb8ddeefae2035200c42a
  Steps:
    1, raii.h:80:47: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/raii.h:80:60: cannot initialize a parameter of type 'int' with an rvalue of type 'std::nullptr_t' [clang-diagnostic-error]
void* allocateUnifiedMemory(std::size_t size, cudaStream_t stream = nullptr);
                                                           ^
  Report hash: 2d91ae5fb70f13fd2da3892460dae32e
  Steps:
    1, raii.h:80:60: passing argument to parameter 'stream' here
    2, raii.h:80:60: cannot initialize a parameter of type 'int' with an rvalue of type 'std::nullptr_t'

Found 6 defect(s) in raii.h

[CRITICAL] /sep/src/compat/raii.cpp:86:74: use of undeclared identifier 'cudaStreamNonBlocking' [clang-diagnostic-error]
    unsigned int cuda_flags = (flags == sep::StreamFlags::NonBlocking) ? cudaStreamNonBlocking : cudaStreamDefault;
                                                                         ^
  Report hash: 503b2b903a5bf28997ffe43d7d79ca4d
  Steps:
    1, raii.cpp:86:74: use of undeclared identifier 'cudaStreamNonBlocking'

[CRITICAL] /sep/src/compat/raii.cpp:86:98: use of undeclared identifier 'cudaStreamDefault' [clang-diagnostic-error]
    unsigned int cuda_flags = (flags == sep::StreamFlags::NonBlocking) ? cudaStreamNonBlocking : cudaStreamDefault;
                                                                                                 ^
  Report hash: 261aae3bdd46e39af7c4297a45c3ca2c
  Steps:
    1, raii.cpp:86:98: use of undeclared identifier 'cudaStreamDefault'

[CRITICAL] /sep/src/compat/raii.cpp:87:5: unknown type name 'cudaError_t' [clang-diagnostic-error]
    cudaError_t err = cudaStreamCreateWithFlags(&stream_, cuda_flags);
    ^
  Report hash: cefbea40cee6719ffcb20d38dd92cb79
  Steps:
    1, raii.cpp:87:5: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/compat/raii.cpp:88:16: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
    if (err != cudaSuccess) {
               ^
  Report hash: b7cb460a47e0a5605e64dae50263d867
  Steps:
    1, raii.cpp:88:16: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/src/compat/raii.cpp:103:9: unknown type name 'cudaError_t' [clang-diagnostic-error]
        CUDA_CHECK(cudaStreamDestroy(stream_));
        ^
  Report hash: 479609d19033088084c8b1adf563e175
  Steps:
    1, cuda_helpers.h:29:9: expanded from macro 'CUDA_CHECK'
    2, raii.cpp:103:9: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/compat/raii.cpp:103:9: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
        CUDA_CHECK(cudaStreamDestroy(stream_));
        ^
  Report hash: e71d66244452f8ba24e8c0ca24524e8d
  Steps:
    1, cuda_helpers.h:30:22: expanded from macro 'CUDA_CHECK'
    2, raii.cpp:103:9: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/src/compat/raii.cpp:113:13: unknown type name 'cudaError_t' [clang-diagnostic-error]
            CUDA_CHECK(cudaStreamDestroy(stream_));
            ^
  Report hash: 6f2bf771e43d6b14c4285dcfbe1051d2
  Steps:
    1, cuda_helpers.h:29:9: expanded from macro 'CUDA_CHECK'
    2, raii.cpp:113:13: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/compat/raii.cpp:113:13: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
            CUDA_CHECK(cudaStreamDestroy(stream_));
            ^
  Report hash: 2055d168995bb0c576dc4e528dc5748c
  Steps:
    1, cuda_helpers.h:30:22: expanded from macro 'CUDA_CHECK'
    2, raii.cpp:113:13: use of undeclared identifier 'cudaSuccess'

[CRITICAL] /sep/src/compat/raii.cpp:122:9: unknown type name 'cudaError_t' [clang-diagnostic-error]
        cudaError_t err = cudaStreamSynchronize(stream_);
        ^
  Report hash: 156e30d30bd31e9b25198bdd654d0a64
  Steps:
    1, raii.cpp:122:9: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/compat/raii.cpp:123:20: use of undeclared identifier 'cudaSuccess' [clang-diagnostic-error]
        if (err != cudaSuccess) {
                   ^
  Report hash: 124659e63cabac83bfad13cbc6a8d469
  Steps:
    1, raii.cpp:123:20: use of undeclared identifier 'cudaSuccess'

Found 10 defect(s) in raii.cpp

[CRITICAL] /sep/include/compat/core.h:39:29: unknown type name 'cudaDeviceProp' [clang-diagnostic-error]
  Error getDeviceProperties(cudaDeviceProp& props, int device) const;
                            ^
  Report hash: 1b2d036837310e65a7c9e75b6ee0057a
  Steps:
    1, core.h:39:29: unknown type name 'cudaDeviceProp'

[CRITICAL] /sep/include/compat/core.h:43:23: unknown type name 'cudaStream_t' [clang-diagnostic-error]
  Error destroyStream(cudaStream_t stream);
                      ^
  Report hash: df749670bccbc19febfc012b5e5fa4c8
  Steps:
    1, core.h:43:23: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/core.h:44:27: unknown type name 'cudaStream_t' [clang-diagnostic-error]
  Error synchronizeStream(cudaStream_t stream);
                          ^
  Report hash: 7b008e791a196adf0c40a19554fef0e4
  Steps:
    1, core.h:44:27: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/core.h:54:30: unknown type name 'cudaError_t' [clang-diagnostic-error]
  std::string getErrorString(cudaError_t error) const;
                             ^
  Report hash: e98ce53d93ab0f9d1eaefc27808ebae8
  Steps:
    1, core.h:54:30: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/core.h:87:15: use of undeclared identifier 'cudaDeviceProp' [clang-diagnostic-error]
  std::vector<cudaDeviceProp> device_properties_;
              ^
  Report hash: 37967107a2eedc49e4952591692f441d
  Steps:
    1, core.h:87:15: use of undeclared identifier 'cudaDeviceProp'

Found 5 defect(s) in core.h

[CRITICAL] /sep/include/compat/memory.h:105:42: unknown type name 'cudaStream_t' [clang-diagnostic-error]
void* allocateUnifiedMemory(size_t size, cudaStream_t stream = nullptr);
                                         ^
  Report hash: 778eb68ec72c065fb8e6c3069eff4bed
  Steps:
    1, memory.h:105:42: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/memory.h:105:55: cannot initialize a parameter of type 'int' with an rvalue of type 'std::nullptr_t' [clang-diagnostic-error]
void* allocateUnifiedMemory(size_t size, cudaStream_t stream = nullptr);
                                                      ^
  Report hash: 5581149b8a466a5a3e5f2f3b3cb4ad78
  Steps:
    1, memory.h:105:55: passing argument to parameter 'stream' here
    2, memory.h:105:55: cannot initialize a parameter of type 'int' with an rvalue of type 'std::nullptr_t'

[MEDIUM] /sep/include/compat/memory.h:111:63: unused parameter 'stream' [clang-diagnostic-unused-parameter]
bool copyToDevice(T* dst, const T* src, size_t count, Stream* stream  = nullptr) {
                                                              ^
  Report hash: 03fe062d6c7e2fcf37a2b293d4c6d7a1
  Steps:
    1, memory.h:111:63: unused parameter 'stream'

[CRITICAL] /sep/include/compat/memory.h:113:5: unknown type name 'cudaError_t' [clang-diagnostic-error]
    cudaError_t error = cudaMemcpyAsync(dst, src, count * sizeof(T), cudaMemcpyHostToDevice, nullptr);
    ^
  Report hash: 28712aa078ffb9b3e9295e385680b4f2
  Steps:
    1, memory.h:113:5: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/memory.h:113:70: use of undeclared identifier 'cudaMemcpyHostToDevice' [clang-diagnostic-error]
    cudaError_t error = cudaMemcpyAsync(dst, src, count * sizeof(T), cudaMemcpyHostToDevice, nullptr);
                                                                     ^
  Report hash: 1022a35be4c91648bd2f03e049c53716
  Steps:
    1, memory.h:113:70: use of undeclared identifier 'cudaMemcpyHostToDevice'

[CRITICAL] /sep/include/compat/memory.h:114:21: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
    return error == cudaSuccess;
                    ^
  Report hash: 795b5d03ffee1826d9d21aa6fc3aeae4
  Notes:
    1, memory.h:114:21: euidaccess (fixit)
  Steps:
    1, unistd.h:292:12: 'euidaccess' declared here
    2, memory.h:114:21: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[MEDIUM] /sep/include/compat/memory.h:123:61: unused parameter 'stream' [clang-diagnostic-unused-parameter]
bool copyToHost(T* dst, const T* src, size_t count, Stream* stream  = nullptr) {
                                                            ^
  Report hash: d7af2a21b214b2470caa64843624d8f9
  Steps:
    1, memory.h:123:61: unused parameter 'stream'

[CRITICAL] /sep/include/compat/memory.h:125:5: unknown type name 'cudaError_t' [clang-diagnostic-error]
    cudaError_t error = cudaMemcpyAsync(dst, src, count * sizeof(T), cudaMemcpyDeviceToHost, nullptr);
    ^
  Report hash: ad40ece99f47fe489af663f4c18617f0
  Steps:
    1, memory.h:125:5: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/memory.h:125:70: use of undeclared identifier 'cudaMemcpyDeviceToHost' [clang-diagnostic-error]
    cudaError_t error = cudaMemcpyAsync(dst, src, count * sizeof(T), cudaMemcpyDeviceToHost, nullptr);
                                                                     ^
  Report hash: 9e54709eadd261e8e8782d8f2bc93fd0
  Steps:
    1, memory.h:125:70: use of undeclared identifier 'cudaMemcpyDeviceToHost'

[CRITICAL] /sep/include/compat/memory.h:126:21: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
    return error == cudaSuccess;
                    ^
  Report hash: 795b5d03ffee1826d9d21aa6fc3aeae4
  Notes:
    1, memory.h:126:21: euidaccess (fixit)
  Steps:
    1, unistd.h:292:12: 'euidaccess' declared here
    2, memory.h:126:21: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/include/compat/memory.h:138:57: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
    return cudaMemset(ptr, value, count * sizeof(T)) == cudaSuccess;
                                                        ^
  Report hash: 1597c0bef1b9a7a0bb59d7761982010b
  Notes:
    1, memory.h:138:57: euidaccess (fixit)
  Steps:
    1, unistd.h:292:12: 'euidaccess' declared here
    2, memory.h:138:57: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

Found 11 defect(s) in memory.h

[CRITICAL] /sep/src/core/engine.cpp:126:57: unknown type name 'cudaStream_t' [clang-diagnostic-error]
                cuda_core.synchronizeStream(static_cast<cudaStream_t>(impl_->stream_->handle()));
                                                        ^
  Report hash: d4dfb848040dd1bdf5e820ef622dcc8a
  Steps:
    1, engine.cpp:126:57: unknown type name 'cudaStream_t'

[CRITICAL] /sep/src/core/engine.cpp:151:5: unknown type name 'cudaError_t' [clang-diagnostic-error]
    SEP_CUDA_CHECK(cudaMemcpyAsync(impl_->d_chunks_.get(), reinterpret_cast<const std::uint64_t*>(inputs.data()),
    ^
  Report hash: 1951a5a1f0fad80f93564181787c05eb
  Steps:
    1, macros.h:102:9: expanded from macro 'SEP_CUDA_CHECK'
    2, engine.cpp:151:5: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/core/engine.cpp:152:75: use of undeclared identifier 'cudaMemcpyHostToDevice' [clang-diagnostic-error]
                                   inputs.size() * sizeof(std::uint64_t), cudaMemcpyHostToDevice,
                                                                          ^
  Report hash: bc2c2e56b551fb18b7c5cabefb2f818a
  Steps:
    1, engine.cpp:152:75: use of undeclared identifier 'cudaMemcpyHostToDevice'

[CRITICAL] /sep/src/core/engine.cpp:153:53: unknown type name 'cudaStream_t' [clang-diagnostic-error]
                                   reinterpret_cast<cudaStream_t>(impl_->stream_->handle())));
                                                    ^
  Report hash: 22080b7cf6710b501b36bfa5e0e89ea7
  Steps:
    1, engine.cpp:153:53: unknown type name 'cudaStream_t'

Found 4 defect(s) in engine.cpp

[CRITICAL] /sep/include/compat/kernels.cuh:59:10: unknown type name 'cudaError_t' [clang-diagnostic-error]
SEP_HOST cudaError_t launch_pattern_processing(
         ^
  Report hash: 441707b99de5a9b3d99b8dfab6207561
  Steps:
    1, kernels.cuh:59:10: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/kernels.cuh:65:5: unknown type name 'cudaStream_t' [clang-diagnostic-error]
    cudaStream_t stream = nullptr
    ^
  Report hash: 92b44483243eebd34307855121721c3c
  Steps:
    1, kernels.cuh:65:5: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/kernels.cuh:65:18: cannot initialize a parameter of type 'int' with an rvalue of type 'std::nullptr_t' [clang-diagnostic-error]
    cudaStream_t stream = nullptr
                 ^
  Report hash: 0979f6ce0bdbd039051de88d596f87fb
  Steps:
    1, kernels.cuh:65:18: passing argument to parameter 'stream' here
    2, kernels.cuh:65:18: cannot initialize a parameter of type 'int' with an rvalue of type 'std::nullptr_t'

Found 3 defect(s) in kernels.cuh

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:202:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
MemoryTier* MemoryTierManager::determineTier(float coherence, float stability, int generation_count) {
                                                                    ^
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:202:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:232:38: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
            if (it->second < config_.demote_threshold)
                                     ^
  Report hash: 474cc343d2d8bde6f322968e111612fc
  Steps:
    1, memory_tier_manager.cpp:232:38: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in memory_tier_manager.cpp

[CRITICAL] /sep/src/memory/memory_tier.cpp:44:9: unknown type name 'cudaError_t' [clang-diagnostic-error]
        cudaError_t err = cudaMallocManaged(&memory_pool_, config.size);
        ^
  Report hash: 09b9b6e9c30e03425c4935f6f5f4c535
  Steps:
    1, memory_tier.cpp:44:9: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/memory/memory_tier.cpp:44:27: use of undeclared identifier 'cudaMallocManaged' [clang-diagnostic-error]
        cudaError_t err = cudaMallocManaged(&memory_pool_, config.size);
                          ^
  Report hash: 4f9d9af93fd6fada071f2f4744cfb79f
  Steps:
    1, memory_tier.cpp:44:27: use of undeclared identifier 'cudaMallocManaged'

[CRITICAL] /sep/src/memory/memory_tier.cpp:45:20: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
        if (err != cudaSuccess)
                   ^
  Report hash: 88d063cfb1427de494e97b417fe0ac4c
  Notes:
    1, memory_tier.cpp:45:20: euidaccess (fixit)
  Steps:
    1, unistd.h:292:12: 'euidaccess' declared here
    2, memory_tier.cpp:45:20: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/memory/memory_tier.cpp:85:13: use of undeclared identifier 'cudaFree' [clang-diagnostic-error]
            cudaFree(memory_pool_);
            ^
  Report hash: 61259c5734b4899944324b86ae34e999
  Steps:
    1, memory_tier.cpp:85:13: use of undeclared identifier 'cudaFree'

[CRITICAL] /sep/src/memory/memory_tier.cpp:165:17: unknown type name 'cudaError_t' [clang-diagnostic-error]
                cudaError_t err = cudaMemcpy(new_location, block.ptr, block.size, cudaMemcpyDefault);
                ^
  Report hash: c7067085559b6ddc99478754109c1910
  Steps:
    1, memory_tier.cpp:165:17: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/memory/memory_tier.cpp:165:83: use of undeclared identifier 'cudaMemcpyDefault' [clang-diagnostic-error]
                cudaError_t err = cudaMemcpy(new_location, block.ptr, block.size, cudaMemcpyDefault);
                                                                                  ^
  Report hash: f1a39ea092cc5e8dab118c9b9dd1af4a
  Steps:
    1, memory_tier.cpp:165:83: use of undeclared identifier 'cudaMemcpyDefault'

[CRITICAL] /sep/src/memory/memory_tier.cpp:166:28: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
                if (err != cudaSuccess) {
                           ^
  Report hash: 03e66c11c83a0406de889ef8fc704552
  Notes:
    1, memory_tier.cpp:166:28: euidaccess (fixit)
  Steps:
    1, unistd.h:292:12: 'euidaccess' declared here
    2, memory_tier.cpp:166:28: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/memory/memory_tier.cpp:320:9: unknown type name 'cudaError_t' [clang-diagnostic-error]
        cudaError_t err = cudaMallocManaged(&new_pool, new_size);
        ^
  Report hash: 89313d6ae15926f596ded097c05fca35
  Steps:
    1, memory_tier.cpp:320:9: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/memory/memory_tier.cpp:320:27: use of undeclared identifier 'cudaMallocManaged' [clang-diagnostic-error]
        cudaError_t err = cudaMallocManaged(&new_pool, new_size);
                          ^
  Report hash: 266f72f2f1e38f7e3f1bb07b42b5691e
  Steps:
    1, memory_tier.cpp:320:27: use of undeclared identifier 'cudaMallocManaged'

[CRITICAL] /sep/src/memory/memory_tier.cpp:321:20: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
        if (err != cudaSuccess)
                   ^
  Report hash: 88d063cfb1427de494e97b417fe0ac4c
  Notes:
    1, memory_tier.cpp:321:20: euidaccess (fixit)
  Steps:
    1, unistd.h:292:12: 'euidaccess' declared here
    2, memory_tier.cpp:321:20: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/memory/memory_tier.cpp:342:17: use of undeclared identifier 'cudaFree' [clang-diagnostic-error]
                cudaFree(new_pool);
                ^
  Report hash: c5a1578126888dfd2613dd92d03567ff
  Steps:
    1, memory_tier.cpp:342:17: use of undeclared identifier 'cudaFree'

[CRITICAL] /sep/src/memory/memory_tier.cpp:377:13: use of undeclared identifier 'cudaFree' [clang-diagnostic-error]
            cudaFree(memory_pool_);
            ^
  Report hash: 61259c5734b4899944324b86ae34e999
  Steps:
    1, memory_tier.cpp:377:13: use of undeclared identifier 'cudaFree'

Found 12 defect(s) in memory_tier.cpp

Found no defects in CMakeCXXCompilerId.cpp
[MEDIUM] /sep/include/api/crow_request.h:41:16: returning reference to local temporary object [clang-diagnostic-return-stack-address]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bb0dbad5b1e8fee3c55448e19305e182
  Steps:
    1, crow_request.h:41:16: returning reference to local temporary object

Found 1 defect(s) in crow_request.h

[MEDIUM] /sep/src/api/server.cpp:162:84: unused parameter 'body' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 99c1cd2d504e0473209900460e079018
  Steps:
    1, server.cpp:162:84: unused parameter 'body'

[MEDIUM] /sep/src/api/server.cpp:222:50: unused parameter 'response_body' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 9a6489114b93b9375b7c0c11dd032dae
  Steps:
    1, server.cpp:222:50: unused parameter 'response_body'

[MEDIUM] /sep/src/api/server.cpp:286:10: unused variable 'rate_limit_mw' [clang-diagnostic-unused-variable]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e3e4ad56877412f3b8b4bd5437469c50
  Steps:
    1, server.cpp:286:10: unused variable 'rate_limit_mw'

Found 3 defect(s) in server.cpp

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:185:69: unused parameter 'stability' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: cf61dae537bc9ff6905aab12dd292eb8
  Steps:
    1, memory_tier_manager.cpp:185:69: unused parameter 'stability'

[MEDIUM] /sep/src/memory/memory_tier_manager.cpp:215:38: implicit conversion increases floating-point precision: 'float' to 'double' [clang-diagnostic-double-promotion]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 474cc343d2d8bde6f322968e111612fc
  Steps:
    1, memory_tier_manager.cpp:215:38: implicit conversion increases floating-point precision: 'float' to 'double'

Found 2 defect(s) in memory_tier_manager.cpp

[MEDIUM] /sep/include/api/crow_adapter.h:18:5: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 053be98dcc8baba104f0b7f85e9d5ed7
  Steps:
    1, crow_isolation.h:47:11: previous use is here
    2, crow_adapter.h:18:5: did you mean class here?
    3, crow_adapter.h:18:5: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/crow_adapter.h:19:5: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 318b9054c5805d5e8d4d7ff3823b790c
  Steps:
    1, crow_isolation.h:62:11: previous use is here
    2, crow_adapter.h:19:5: did you mean class here?
    3, crow_adapter.h:19:5: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in crow_adapter.h

[MEDIUM] /sep/include/api/server.h:23:1: struct 'request' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bd9ac56903b13fea884a3e13c46efb96
  Steps:
    1, crow_isolation.h:47:11: previous use is here
    2, server.h:23:1: did you mean class here?
    3, server.h:23:1: struct 'request' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/server.h:24:1: struct 'response' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 27113b3d5c88d70a2d5562e5ebb9c69a
  Steps:
    1, crow_isolation.h:62:11: previous use is here
    2, server.h:24:1: did you mean class here?
    3, server.h:24:1: struct 'response' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in server.h

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

[MEDIUM] /sep/include/crow/crow_isolation.h:157:44: unused parameter 'req' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 331d75dec6c0f922510123a10b531126
  Steps:
    1, crow_isolation.h:157:44: unused parameter 'req'

[MEDIUM] /sep/include/crow/crow_isolation.h:158:39: unused parameter 'res' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c7fc29ac6703380472bc6adf9c1fed11
  Steps:
    1, crow_isolation.h:158:39: unused parameter 'res'

[MEDIUM] /sep/include/crow/crow_isolation.h:159:39: unused parameter 'adaptor' [clang-diagnostic-unused-parameter]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1190635c4178fb8d0509a679385d250d
  Steps:
    1, crow_isolation.h:159:39: unused parameter 'adaptor'

Found 3 defect(s) in crow_isolation.h

[MEDIUM] /sep/src/api/crow_adapter.cpp:40:61: returning reference to local temporary object [clang-diagnostic-return-stack-address]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eddbc29709fee01e675b6fa62a95b001
  Steps:
    1, crow_adapter.cpp:40:61: returning reference to local temporary object

[MEDIUM] /sep/src/api/crow_adapter.cpp:44:62: returning reference to local temporary object [clang-diagnostic-return-stack-address]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 105faa0211d4e83dcd13488df5e6ced7
  Steps:
    1, crow_adapter.cpp:44:62: returning reference to local temporary object

[MEDIUM] /sep/src/api/crow_adapter.cpp:56:66: returning reference to local temporary object [clang-diagnostic-return-stack-address]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: eafad159adc4bf27982bb65a25645d7d
  Steps:
    1, crow_adapter.cpp:56:66: returning reference to local temporary object

Found 3 defect(s) in crow_adapter.cpp

[CRITICAL] /sep/third_party/nlohmann/json.hpp:23327:16: no matching function for call to 'input_adapter' [clang-diagnostic-error]
        parser(detail::input_adapter(std::forward<InputType>(i)), cb, allow_exceptions, ignore_comments).parse(true, result);
               ^
  Report hash: 64df407741a25929019b84260d50d9ab
  Steps:
    1, crow_adapter.cpp:121:37: in instantiation of function template specialization 'nlohmann::basic_json<>::parse<const sep::shim::string &>' requested here
    2, json.hpp:6562:27: candidate function not viable: no known conversion from 'const sep::shim::string' to 'std::FILE *' (aka '_IO_FILE *') for 1st argument
    3, json.hpp:6567:29: candidate function not viable: no known conversion from 'const sep::shim::string' to 'std::istream &' (aka 'basic_istream<char> &') for 1st argument
    4, json.hpp:6572:29: candidate function not viable: no known conversion from 'const sep::shim::string' to 'std::istream' (aka 'basic_istream<char>') for 1st argument
    5, json.hpp:6555:109: candidate template ignored: substitution failure [with ContainerType = sep::shim::string]: no type named 'adapter_type' in 'nlohmann::detail::container_input_adapter_factory_impl::container_input_adapter_factory<sep::shim::string>'
    6, json.hpp:6588:32: candidate template ignored: requirement 'std::is_pointer<sep::shim::string>::value' was not satisfied [with CharT = sep::shim::string]
    7, json.hpp:6596:6: candidate template ignored: could not match 'T[N]' against 'const sep::shim::string'
    8, json.hpp:6521:69: candidate function template not viable: requires 2 arguments, but 1 was provided
    9, json.hpp:23327:16: no matching function for call to 'input_adapter'

Found 1 defect(s) in json.hpp

[HIGH] /sep/include/api/crow_request.h:41:9: Address of stack memory associated with temporary object of type 'std::string' returned to caller [core.StackAddressEscape]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 8a65afeefa38760c4da20e58af6ead41
  Steps:
    1, crow_request.h:41:9: Address of stack memory associated with temporary object of type 'std::string' returned to caller

Found 1 defect(s) in crow_request.h

[LOW] /sep/include/compat/cuda_runtime.h:17:10: direct self-inclusion of header file 'cuda_runtime.h' [misc-header-include-cycle]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: be1562db9a8de9210a2d5229179b5614
  Steps:
    1, cuda_runtime.h:17:10: direct self-inclusion of header file 'cuda_runtime.h'

Found 1 defect(s) in cuda_runtime.h

[CRITICAL] /sep/include/compat/core.h:42:29: unknown type name 'cudaDeviceProp' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 1b2d036837310e65a7c9e75b6ee0057a
  Steps:
    1, core.h:42:29: unknown type name 'cudaDeviceProp'

[CRITICAL] /sep/include/compat/core.h:46:23: unknown type name 'cudaStream_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: df749670bccbc19febfc012b5e5fa4c8
  Steps:
    1, core.h:46:23: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/core.h:47:27: unknown type name 'cudaStream_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 7b008e791a196adf0c40a19554fef0e4
  Steps:
    1, core.h:47:27: unknown type name 'cudaStream_t'

[CRITICAL] /sep/include/compat/core.h:57:30: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: e98ce53d93ab0f9d1eaefc27808ebae8
  Steps:
    1, core.h:57:30: unknown type name 'cudaError_t'

[CRITICAL] /sep/include/compat/core.h:90:15: use of undeclared identifier 'cudaDeviceProp' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 37967107a2eedc49e4952591692f441d
  Steps:
    1, core.h:90:15: use of undeclared identifier 'cudaDeviceProp'

Found 5 defect(s) in core.h

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:199:48: expected ':' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 2495f461ee440bf6b72743ce92739387
  Notes:
    1, memory_tier_manager.cpp:199:48: :  (fixit)
  Steps:
    1, memory_tier_manager.cpp:199:36: to match this '?'
    2, memory_tier_manager.cpp:199:48: expected ':'

[CRITICAL] /sep/src/memory/memory_tier_manager.cpp:199:48: expected expression [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 010a93e9d60005921417671b87fd8cc9
  Steps:
    1, memory_tier_manager.cpp:199:48: expected expression

Found 2 defect(s) in memory_tier_manager.cpp

[HIGH] /sep/src/quantum/pattern_processor.cpp:141:95: reference 'patterns_' is not yet bound to a value when used here [clang-diagnostic-uninitialized]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: a86466e38791583c24e3c1d57791bf80
  Steps:
    1, pattern_processor.cpp:141:95: reference 'patterns_' is not yet bound to a value when used here

Found 1 defect(s) in pattern_processor.cpp

[CRITICAL] /sep/src/memory/memory_tier.cpp:47:9: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 09b9b6e9c30e03425c4935f6f5f4c535
  Steps:
    1, memory_tier.cpp:47:9: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/memory/memory_tier.cpp:47:27: use of undeclared identifier 'cudaMallocManaged' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4f9d9af93fd6fada071f2f4744cfb79f
  Steps:
    1, memory_tier.cpp:47:27: use of undeclared identifier 'cudaMallocManaged'

[CRITICAL] /sep/src/memory/memory_tier.cpp:48:20: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 88d063cfb1427de494e97b417fe0ac4c
  Notes:
    1, memory_tier.cpp:48:20: euidaccess (fixit)
  Steps:
    1, unistd.h:292:12: 'euidaccess' declared here
    2, memory_tier.cpp:48:20: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/memory/memory_tier.cpp:88:13: use of undeclared identifier 'cudaFree' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 61259c5734b4899944324b86ae34e999
  Steps:
    1, memory_tier.cpp:88:13: use of undeclared identifier 'cudaFree'

[CRITICAL] /sep/src/memory/memory_tier.cpp:168:17: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c7067085559b6ddc99478754109c1910
  Steps:
    1, memory_tier.cpp:168:17: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/memory/memory_tier.cpp:168:83: use of undeclared identifier 'cudaMemcpyDefault' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: f1a39ea092cc5e8dab118c9b9dd1af4a
  Steps:
    1, memory_tier.cpp:168:83: use of undeclared identifier 'cudaMemcpyDefault'

[CRITICAL] /sep/src/memory/memory_tier.cpp:169:28: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 03e66c11c83a0406de889ef8fc704552
  Notes:
    1, memory_tier.cpp:169:28: euidaccess (fixit)
  Steps:
    1, unistd.h:292:12: 'euidaccess' declared here
    2, memory_tier.cpp:169:28: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/memory/memory_tier.cpp:323:9: unknown type name 'cudaError_t' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 89313d6ae15926f596ded097c05fca35
  Steps:
    1, memory_tier.cpp:323:9: unknown type name 'cudaError_t'

[CRITICAL] /sep/src/memory/memory_tier.cpp:323:27: use of undeclared identifier 'cudaMallocManaged' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 266f72f2f1e38f7e3f1bb07b42b5691e
  Steps:
    1, memory_tier.cpp:323:27: use of undeclared identifier 'cudaMallocManaged'

[CRITICAL] /sep/src/memory/memory_tier.cpp:324:20: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'? [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 88d063cfb1427de494e97b417fe0ac4c
  Notes:
    1, memory_tier.cpp:324:20: euidaccess (fixit)
  Steps:
    1, unistd.h:292:12: 'euidaccess' declared here
    2, memory_tier.cpp:324:20: use of undeclared identifier 'cudaSuccess'; did you mean 'euidaccess'?

[CRITICAL] /sep/src/memory/memory_tier.cpp:345:17: use of undeclared identifier 'cudaFree' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: c5a1578126888dfd2613dd92d03567ff
  Steps:
    1, memory_tier.cpp:345:17: use of undeclared identifier 'cudaFree'

[CRITICAL] /sep/src/memory/memory_tier.cpp:380:13: use of undeclared identifier 'cudaFree' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 61259c5734b4899944324b86ae34e999
  Steps:
    1, memory_tier.cpp:380:13: use of undeclared identifier 'cudaFree'

Found 12 defect(s) in memory_tier.cpp


----==== Severity Statistics ====----
----------------------------
Severity | Number of reports
----------------------------
CRITICAL |               205
MEDIUM   |                67
LOW      |                40
HIGH     |                 2
----------------------------
----=================----

----==== Checker Statistics ====----
---------------------------------------------------------------------
Checker name                           | Severity | Number of reports
---------------------------------------------------------------------
clang-diagnostic-error                 | CRITICAL |               205
clang-diagnostic-unused-parameter      | MEDIUM   |                43
bugprone-forward-declaration-namespace | LOW      |                33
clang-diagnostic-mismatched-tags       | MEDIUM   |                 8
clang-diagnostic-reorder-ctor          | MEDIUM   |                 3
clang-diagnostic-double-promotion      | MEDIUM   |                 5
unix.BlockInCriticalSection            | LOW      |                 1
clang-diagnostic-unused-function       | MEDIUM   |                 1
misc-header-include-cycle              | LOW      |                 5
clang-diagnostic-unused-variable       | MEDIUM   |                 2
clang-diagnostic-return-stack-address  | MEDIUM   |                 4
clang-diagnostic-infinite-recursion    | MEDIUM   |                 1
bugprone-forwarding-reference-overload | LOW      |                 1
core.StackAddressEscape                | HIGH     |                 1
clang-diagnostic-uninitialized         | HIGH     |                 1
---------------------------------------------------------------------
----=================----

----==== File Statistics ====----
-----------------------------------------------------
File name                         | Number of reports
-----------------------------------------------------
main.cpp                          |                 1
bridge_internal.hpp               |                 2
bridge_c.cpp                      |                 1
bridge.cpp                        |                 1
client.cpp                        |                 1
rate_limit_middleware.h           |                 2
type_traits.hpp                   |                 1
executor.hpp                      |                 6
invocable_archetype.hpp           |                 1
is_applicable_property.hpp        |                 4
equality_comparable.hpp           |                 2
execute_member.hpp                |                 2
query_static_constexpr_member.hpp |                 3
sep_engine.h                      |                 1
sep_engine.cpp                    |                 1
unique_ptr.h                      |                 1
server.h                          |                 6
crow_isolation.h                  |                 1
http_parser_merged.h              |                 1
strand_executor_service.hpp       |                10
prefer.hpp                        |                10
require.hpp                       |                10
blender_integration.cpp           |                10
mesh_handler.cpp                  |                 2
processor.cpp                     |                19
kernels.cuh                       |                 9
memory_tier_manager.cpp           |                14
cuda_impl.h                       |                 5
cuda_runtime.h                    |                16
engine.h                          |                 2
redis_manager.cpp                 |                19
signal_set_service.ipp            |                 1
bridge.hpp                        |                 4
engine.cpp                        |                 5
crow_isolation.h                  |                20
types.h                           |                 1
cuda_runtime.h                    |                 1
memory_tier.hpp                   |                 1
cuda_common.h                     |                 1
cuda_helpers.h                    |                 2
stream_impl.h                     |                14
stream.cpp                        |                 2
server.cpp                        |                 6
metrics_collector.cpp             |                16
raii.h                            |                 6
raii.cpp                          |                10
core.h                            |                10
memory.h                          |                11
memory_tier.cpp                   |                24
crow_request.h                    |                 2
crow_adapter.h                    |                 2
asio_isolation.h                  |                 6
crow_adapter.cpp                  |                 3
json.hpp                          |                 1
pattern_processor.cpp             |                 1
-----------------------------------------------------
----=================----

----======== Summary ========----
-----------------------------------------------
Number of processed analyzer result files | 755
Number of analyzer reports                | 314
-----------------------------------------------
----=================----
[WARNING 2025-06-24 12:46] - The following source file contents changed or missing since the latest analysis:
 - /sep/third_party/crow/asio_isolation.h
 - /sep/include/compat/cuda_runtime.h
 - /sep/src/api/server.cpp
 - /sep/src/blender/blender_integration.cpp
 - /sep/src/main.cpp
 - /sep/src/api/bridge_c.cpp
 - /sep/third_party/crow/crow_isolation.h
 - /sep/src/api/bridge.cpp
 - /sep/include/crow/crow_isolation.h
 - /sep/include/compat/core.h
 - /sep/src/api/crow_adapter.cpp
 - /sep/src/context/processor.cpp
 - /sep/include/api/bridge_internal.hpp
 - /sep/include/api/server.h
 - /sep/include/compat/cuda.h
 - /sep/src/quantum/pattern_processor.cpp
 - /sep/src/memory/memory_tier.cpp
 - /sep/include/core/engine.h
 - /sep/third_party/crow/http_parser_merged.h
 - /sep/src/memory/memory_tier_manager.cpp
 - /sep/src/memory/redis_manager.cpp
 - /sep/src/api/sep_engine.cpp
 - /sep/include/quantum/processor.h
 - /sep/third_party/crow/common.h
 - /sep/include/compat/kernels.cuh
 - /sep/include/compat/cuda_impl.h
 - /sep/include/api/bridge.hpp
 - /sep/include/api/crow_adapter.h
 - /sep/include/api/crow_request.h
 - /sep/include/api/rate_limit_middleware.h
Please re-analyze your project to update the reports!
