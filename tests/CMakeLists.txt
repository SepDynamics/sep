find_package(GTest REQUIRED)

add_executable(blender_api_test blender/api_test.cpp)
target_link_libraries(blender_api_test sep_blender sep_core GTest::GTest GTest::Main)

add_executable(quantum_manifold_optimizer_test quantum/quantum_manifold_optimizer_test.cpp)
target_link_libraries(quantum_manifold_optimizer_test sep_quantum GTest::GTest GTest::Main)

add_test(NAME quantum_manifold_optimizer_test COMMAND quantum_manifold_optimizer_test)
