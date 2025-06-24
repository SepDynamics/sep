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
void process_pattern_kernel(
     ^
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

Found 14 defect(s) in crow_isolation.h

Found no defects in lock_free_rate_limiter.cpp
[CRITICAL] /sep/include/api/crow_request.h:19:35: no member named 'headers' in 'crow::request' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: 4fa7c76ecb6f6b653d92875740d23fd9
  Steps:
    1, crow_request.h:19:35: no member named 'headers' in 'crow::request'

[MEDIUM] /sep/include/api/crow_request.h:37:16: returning reference to local temporary object [clang-diagnostic-return-stack-address]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: bb0dbad5b1e8fee3c55448e19305e182
  Steps:
    1, crow_request.h:37:16: returning reference to local temporary object

[CRITICAL] /sep/include/api/crow_request.h:53:21: no member named 'remote_ip_address' in 'crow::request' [clang-diagnostic-error]
WARNING: source file content is changed or missing. Please re-analyze your project to update the reports.
  Report hash: db823d6212ece81b3b9cf26e1ccb96e8
  Steps:
    1, crow_request.h:53:21: no member named 'remote_ip_address' in 'crow::request'

Found 3 defect(s) in crow_request.h

[CRITICAL] /sep/src/api/server.cpp:80:9: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
    app_->port(config_.port)
        ^
  Report hash: 70ba00602e3872b04b8a9a460852d643
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:80:9: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:89:13: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
        app_->run();
            ^
  Report hash: 4164af67c9013bc3638fd353e6950996
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:89:13: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:124:9: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
    app_->stop();
        ^
  Report hash: d304de7a87fd3324a7179e9ab53982d6
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:124:9: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:310:11: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
      app_->get_middleware<RateLimitMiddleware>();
          ^
  Report hash: 6e4c37452577dc1a6c493da9f6089ce0
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:310:11: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:313:23: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
  auto& auth_mw = app_->get_middleware<AuthMiddleware>();
                      ^
  Report hash: c17f8c11414165e666f2c32e4b3767f1
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:313:23: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:331:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
  app_->route_dynamic("/api/v1/health").methods(::crow::HTTPMethod::GET)([this, &engine](const ::crow::request& req) {
      ^
  Report hash: b25fc95ee2c6188fa838c64199186e55
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:331:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:360:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
  app_->route_dynamic("/api/v1/pattern/evolve")
      ^
  Report hash: fcddd8093aff412eb0db4884ebfdb99d
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:360:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:361:22: no matching literal operator for call to 'operator""_method' with arguments of types 'const char *' and 'unsigned long', and no matching literal operator template [clang-diagnostic-error]
      .methods("POST"_method)([this, &engine](const ::crow::request& req) {
                     ^
  Report hash: 5f509c466655a247ab9a77d667cec67a
  Steps:
    1, server.cpp:361:22: no matching literal operator for call to 'operator""_method' with arguments of types 'const char *' and 'unsigned long', and no matching literal operator template

[CRITICAL] /sep/src/api/server.cpp:406:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
  app_->route_dynamic("/api/v1/memory/query")
      ^
  Report hash: 10da9b3e4225e4943fe073e0fc35290d
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:406:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:407:22: no matching literal operator for call to 'operator""_method' with arguments of types 'const char *' and 'unsigned long', and no matching literal operator template [clang-diagnostic-error]
      .methods("POST"_method)([this, &engine](const ::crow::request& req) {
                     ^
  Report hash: 5f509c466655a247ab9a77d667cec67a
  Steps:
    1, server.cpp:407:22: no matching literal operator for call to 'operator""_method' with arguments of types 'const char *' and 'unsigned long', and no matching literal operator template

[CRITICAL] /sep/src/api/server.cpp:449:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
  app_->route_dynamic("/api/v1/patterns/history")
      ^
  Report hash: 0ab37fad0cbde974978fdf9a3a39ab5f
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:449:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:450:22: no matching literal operator for call to 'operator""_method' with arguments of types 'const char *' and 'unsigned long', and no matching literal operator template [clang-diagnostic-error]
      .methods("POST"_method)([this, &engine](const ::crow::request& req) {
                     ^
  Report hash: 5f509c466655a247ab9a77d667cec67a
  Steps:
    1, server.cpp:450:22: no matching literal operator for call to 'operator""_method' with arguments of types 'const char *' and 'unsigned long', and no matching literal operator template

[CRITICAL] /sep/src/api/server.cpp:492:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
  app_->route_dynamic("/api/v1/context/process")
      ^
  Report hash: f2728f617ea274b22bba522c938383a4
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:492:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:493:22: no matching literal operator for call to 'operator""_method' with arguments of types 'const char *' and 'unsigned long', and no matching literal operator template [clang-diagnostic-error]
      .methods("POST"_method)([this, &engine](const ::crow::request& req) {
                     ^
  Report hash: 5f509c466655a247ab9a77d667cec67a
  Steps:
    1, server.cpp:493:22: no matching literal operator for call to 'operator""_method' with arguments of types 'const char *' and 'unsigned long', and no matching literal operator template

[CRITICAL] /sep/src/api/server.cpp:535:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
  app_->route_dynamic("/api/v1/embeddings/extract")
      ^
  Report hash: b4fcbcba1133122d62efff3c6a856916
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:535:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

[CRITICAL] /sep/src/api/server.cpp:536:22: no matching literal operator for call to 'operator""_method' with arguments of types 'const char *' and 'unsigned long', and no matching literal operator template [clang-diagnostic-error]
      .methods("POST"_method)([this, &engine](const ::crow::request& req) {
                     ^
  Report hash: 5f509c466655a247ab9a77d667cec67a
  Steps:
    1, server.cpp:536:22: no matching literal operator for call to 'operator""_method' with arguments of types 'const char *' and 'unsigned long', and no matching literal operator template

[CRITICAL] /sep/src/api/server.cpp:578:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>' [clang-diagnostic-error]
  app_->route_dynamic("/api/v1/pattern/analyze")
      ^
  Report hash: be41304fec12b7e4ffa53956c6a07407
  Steps:
    1, server.h:26:7: template is declared here
    2, server.cpp:578:7: implicit instantiation of undefined template 'crow::Crow<sep::api::RateLimitMiddleware, sep::api::AuthMiddleware>'

Found 17 defect(s) in server.cpp

[MEDIUM] /sep/include/api/crow_adapter.h:18:5: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
    struct request;
    ^
  Report hash: 053be98dcc8baba104f0b7f85e9d5ed7
  Steps:
    1, crow_isolation.h:47:11: previous use is here
    2, crow_adapter.h:18:5: did you mean class here?
    3, crow_adapter.h:18:5: struct 'request' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/crow_adapter.h:19:5: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
    struct response;
    ^
  Report hash: 318b9054c5805d5e8d4d7ff3823b790c
  Steps:
    1, crow_isolation.h:62:11: previous use is here
    2, crow_adapter.h:19:5: did you mean class here?
    3, crow_adapter.h:19:5: struct 'response' was previously declared as a class; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in crow_adapter.h

[MEDIUM] /sep/include/api/server.h:23:1: struct 'request' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
struct request;
^
  Report hash: bd9ac56903b13fea884a3e13c46efb96
  Steps:
    1, crow_isolation.h:47:11: previous use is here
    2, server.h:23:1: did you mean class here?
    3, server.h:23:1: struct 'request' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/server.h:24:1: struct 'response' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
struct response;
^
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

[MEDIUM] /sep/include/api/crow_adapter.h:18:5: struct 'request' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
    struct request;
    ^
  Report hash: 527c52fa142dd5f3d72df6fbc7c48661
  Steps:
    1, crow_isolation.h:47:11: previous use is here
    2, crow_adapter.h:18:5: did you mean class here?
    3, crow_adapter.h:18:5: struct 'request' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

[MEDIUM] /sep/include/api/crow_adapter.h:19:5: struct 'response' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI [clang-diagnostic-mismatched-tags]
    struct response;
    ^
  Report hash: aee0965a27b7af880c6c82b24148538f
  Steps:
    1, crow_isolation.h:62:11: previous use is here
    2, crow_adapter.h:19:5: did you mean class here?
    3, crow_adapter.h:19:5: struct 'response' was previously declared as a struct; this is valid, but may result in linker errors under the Microsoft C++ ABI

Found 2 defect(s) in crow_adapter.h

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


----==== Severity Statistics ====----
----------------------------
Severity | Number of reports
----------------------------
CRITICAL |               121
MEDIUM   |                49
LOW      |                35
HIGH     |                 2
----------------------------
----=================----

----==== Checker Statistics ====----
---------------------------------------------------------------------
Checker name                           | Severity | Number of reports
---------------------------------------------------------------------
clang-diagnostic-error                 | CRITICAL |               121
clang-diagnostic-unused-parameter      | MEDIUM   |                28
bugprone-forward-declaration-namespace | LOW      |                33
clang-diagnostic-mismatched-tags       | MEDIUM   |                10
clang-diagnostic-reorder-ctor          | MEDIUM   |                 2
clang-diagnostic-double-promotion      | MEDIUM   |                 3
unix.BlockInCriticalSection            | LOW      |                 1
clang-diagnostic-unused-function       | MEDIUM   |                 1
clang-diagnostic-return-stack-address  | MEDIUM   |                 4
clang-diagnostic-infinite-recursion    | MEDIUM   |                 1
bugprone-forwarding-reference-overload | LOW      |                 1
core.CallAndMessage                    | HIGH     |                 2
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
kernels.cuh                       |                 6
memory_tier_manager.cpp           |                 8
cuda_impl.h                       |                 5
cuda_runtime.h                    |                12
engine.h                          |                 2
redis_manager.cpp                 |                19
signal_set_service.ipp            |                 1
bridge.hpp                        |                 4
engine.cpp                        |                 1
crow_isolation.h                  |                14
crow_request.h                    |                 3
server.cpp                        |                17
crow_adapter.h                    |                 4
asio_isolation.h                  |                 6
crow_adapter.cpp                  |                 3
json.hpp                          |                 1
cetintrin.h                       |                 2
-----------------------------------------------------
----=================----

----======== Summary ========----
-----------------------------------------------
Number of processed analyzer result files | 273
Number of analyzer reports                | 207
-----------------------------------------------
----=================----
[WARNING 2025-06-24 06:51] - The following source file contents changed or missing since the latest analysis:
 - /sep/include/compat/cuda.h
 - /sep/third_party/crow/http_parser_merged.h
 - /sep/include/api/crow_request.h
 - /sep/include/compat/cuda_runtime.h
 - /sep/include/api/bridge_internal.hpp
 - /sep/src/api/bridge_c.cpp
 - /sep/src/api/bridge.cpp
 - /sep/third_party/crow/asio_isolation.h
 - /sep/include/core/engine.h
 - /sep/src/memory/memory_tier_manager.cpp
 - /sep/src/api/sep_engine.cpp
 - /sep/include/api/bridge.hpp
 - /sep/src/api/crow_adapter.cpp
 - /sep/include/compat/cuda_impl.h
 - /sep/include/crow/crow_isolation.h
 - /sep/src/blender/blender_integration.cpp
 - /sep/include/api/rate_limit_middleware.h
 - /sep/src/context/processor.cpp
 - /sep/include/api/server.h
 - /sep/src/memory/redis_manager.cpp
 - /sep/third_party/crow/crow_isolation.h
 - /sep/include/quantum/processor.h
 - /sep/src/main.cpp
 - /sep/third_party/crow/common.h
Please re-analyze your project to update the reports!
