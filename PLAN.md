SEP Engine Prune and Restructure Tasklist
1. Directory Structure Cleanup

Remove Build Artifacts:
Delete all CMakeFiles directories: find /sep/src -name "CMakeFiles" -exec rm -r {} +
Remove cmake_install.cmake from /sep/src/quantum/: rm /sep/src/quantum/cmake_install.cmake


Consolidate Tests:
Move /sep/src/blender/tests/* to /sep/tests/blender/: mkdir -p /sep/tests/blender && mv /sep/src/blender/tests/* /sep/tests/blender/ && rm -r /sep/src/blender/tests
Verify no other modules have test subdirs (e.g., find /sep/src -name "tests"); move any found to /sep/tests/<module>/


Relocate Assets:
Create /sep/assets/shaders: mkdir -p /sep/assets/shaders
Move shaders (assumed in /sep/src/blender/shaders/): mv /sep/src/blender/shaders/* /sep/assets/shaders/
Move test scenes: mkdir -p /sep/assets/test_data && mv /sep/src/blender/tests/scenes/* /sep/assets/test_data/


Move default.json:
Relocate /sep/src/core/default.json to /sep/config/: mkdir -p /sep/config && mv /sep/src/core/default.json /sep/config/


Remove Empty Dirs:
Delete empty directories: find /sep/src -type d -empty -delete



2. Header File Reorganization

Ensure Namespaced Headers:
Verify headers are in /sep/include/sep/<module>/ (e.g., /sep/include/sep/api/connection_manager.h)
If not, move them: mkdir -p /sep/include/sep/{api,audio,blender,compat,context,core,memory,quantum} && mv /sep/include/api/* /sep/include/sep/api/ (repeat for each module)


Handle Consolidated Modules:
Move /sep/include/sep/config/* to /sep/include/sep/core/: mv /sep/include/sep/config/* /sep/include/sep/core/ && rm -r /sep/include/sep/config
Move /sep/include/sep/metrics/* to /sep/include/sep/core/: mv /sep/include/sep/metrics/* /sep/include/sep/core/ && rm -r /sep/include/sep/metrics
Move /sep/include/sep/quantum/dag_graph.h to /sep/include/sep/core/: mv /sep/include/sep/quantum/dag_graph.h /sep/include/sep/core/


Third-Party Headers:
Move /sep/include/crow/* to /sep/third_party/crow/: mkdir -p /sep/third_party/crow && mv /sep/include/crow/* /sep/third_party/crow/
Move /sep/include/nlohmann/* to /sep/third_party/nlohmann/: mkdir -p /sep/third_party/nlohmann && mv /sep/include/nlohmann/* /sep/third_party/nlohmann/



3. Code Updates

Update Include Paths:
Replace old includes with namespaced ones: find /sep/src -type f -exec sed -i 's|#include "\([a-z]\+\)/|#include <sep/\1/|g' {} \;
Update third-party includes: find /sep/src -type f -exec sed -i 's|#include "crow/|#include <crow/|g' {} \;
Update nlohmann includes: find /sep/src -type f -exec sed -i 's|#include "nlohmann/|#include <nlohmann/|g' {} \;


Fix Config Path:
Update references to default.json in /sep/src/core/manager.cpp to /sep/config/default.json


Asset Path Updates:
Search for shader paths (e.g., src/blender/shaders/) and replace with assets/shaders/: find /sep/src -type f -exec sed -i 's|src/blender/shaders/|assets/shaders/|g' {} \;
Update test data paths: find /sep/src -type f -exec sed -i 's|src/blender/tests/scenes/|assets/test_data/|g' {} \;


Namespace Consistency:
Verify sep::<module> usage in all files (e.g., sep::api in connection_manager.cpp); fix any inconsistencies with grep -r "namespace sep" /sep/src



4. Build System Updates

Root CMakeLists.txt:
Update include dirs: echo "include_directories(/sep/include /sep/third_party)" >> /sep/CMakeLists.txt
Add subdirs: echo "add_subdirectory(src/api)" >> /sep/CMakeLists.txt (repeat for audio, blender, compat, context, core, memory, quantum)
Add executable: echo "add_executable(sep_engine src/main.cpp)" >> /sep/CMakeLists.txt
Link libraries: echo "target_link_libraries(sep_engine sep_core sep_api sep_context sep_memory sep_quantum sep_audio sep_blender sep_compat)" >> /sep/CMakeLists.txt


Module CMakeLists.txt:
For /sep/src/api/CMakeLists.txt: echo "add_library(sep_api STATIC connection_manager.cpp auth_middleware.cpp bridge.cpp bridge_c.cpp client.cpp crow_adapter.cpp crow_error.cpp curl_http_client.cpp js_integration.cpp lock_free_rate_limiter.cpp ollama_client.cpp rate_limit_middleware.cpp sep_engine.cpp server.cpp)" > /sep/src/api/CMakeLists.txt
For /sep/src/core/CMakeLists.txt: echo "add_library(sep_core STATIC allocation_metrics.cpp dag_graph.cpp engine.cpp error_handler.cpp manager.cpp metrics_collector.cpp prometheus_exporter.cpp tracing.cpp)" > /sep/src/core/CMakeLists.txt
Similar for other modules (list their .cpp and .cu files)


Tests CMakeLists.txt:
Create /sep/tests/CMakeLists.txt: echo "add_executable(blender_api_test blender/api_test.cpp)" > /sep/tests/CMakeLists.txt (add for each test)
Link tests: echo "target_link_libraries(blender_api_test sep_blender sep_core)" >> /sep/tests/CMakeLists.txt (adjust for each test)


Third-Party CMake:
Add /sep/third_party/crow/CMakeLists.txt if vendored: echo "add_library(crow INTERFACE)" > /sep/third_party/crow/CMakeLists.txt
Include nlohmann: echo "include_directories(/sep/third_party/nlohmann)" >> /sep/CMakeLists.txt



5. Verification and Final Cleanup

Build and Test:
Run cd /sep && cmake . && make
Execute tests: cd /sep/tests/blender && ./run_all_tests.sh


Validate Functionality:
Test API server: /sep/bin/sep_engine --config /sep/config/default.json
Test Blender integration (if applicable): Run a sample Blender script
Test audio capture (if applicable): Run a sample audio pipeline


Remove Obsolete Files:
Delete old test scripts if redundant: rm /sep/tests/blender/run_tests.sh (if run_all_tests.sh covers it)
Remove empty dirs: find /sep -type d -empty -delete


Update Documentation:
Update /sep/src/<module>/README.md with new paths and module responsibilities
Create /sep/docs/ARCHITECTURE.md with updated dependency graph (e.g., core → quantum → memory → context → api)



Notes

Backup First: tar -czf /tmp/sep_backup.tar.gz /sep
Incremental Testing: Test after each section (e.g., build after CMake updates)
Future Move: To move to /sep at root, use sudo mv /sep / and sudo chown -R root:root /sep && sudo chmod -R 755 /sep when ready
