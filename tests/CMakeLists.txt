cmake_minimum_required(VERSION 3.15)
project(pattern_metrics_tests)

find_package(GTest REQUIRED)
add_executable(pattern_metrics_test
    pattern_metrics_test.cpp
    quantum_processor_test.cpp)
target_include_directories(pattern_metrics_test PRIVATE ../src)
target_link_libraries(pattern_metrics_test GTest::gtest GTest::gtest_main sep_quantum)

add_executable(quantum_signal_bridge_test
    quantum_signal_bridge_test.cpp)

add_executable(trajectory_metrics_test
    trajectory_metrics_test.cpp)
target_include_directories(quantum_signal_bridge_test PRIVATE ../src)
target_link_libraries(quantum_signal_bridge_test GTest::gtest GTest::gtest_main sep_quantum sep_trader_cuda sep_trader_logic)

target_include_directories(trajectory_metrics_test PRIVATE ../src)
target_link_libraries(trajectory_metrics_test GTest::gtest GTest::gtest_main sep_quantum sep_quantum_bitspace sep_trader_cuda CUDA::cudart)

enable_testing()
include(GoogleTest)
gtest_discover_tests(pattern_metrics_test)

add_executable(oanda_trader_test
    oanda_trader_test.cpp)
target_include_directories(oanda_trader_test PRIVATE ../src)
target_link_libraries(oanda_trader_test GTest::gtest GTest::gtest_main sep_trader_logic sep_trader_cuda sep_engine sep_connectors sep_quantum sep_ui imgui glad OpenGL::GL glfw Threads::Threads ${CMAKE_DL_LIBS})
gtest_discover_tests(oanda_trader_test)

add_executable(oanda_trader_e2e_test
    oanda_trader_e2e_test.cpp)
target_include_directories(oanda_trader_e2e_test PRIVATE ../src)
target_link_libraries(oanda_trader_e2e_test GTest::gtest GTest::gtest_main sep_trader_logic sep_trader_cuda sep_engine sep_connectors sep_quantum sep_ui imgui glad OpenGL::GL glfw Threads::Threads ${CMAKE_DL_LIBS})
gtest_discover_tests(oanda_trader_e2e_test)
