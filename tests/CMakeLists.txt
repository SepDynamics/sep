cmake_minimum_required(VERSION 3.15)
project(pattern_metrics_tests)

find_package(GTest REQUIRED)
find_package(Torch REQUIRED)
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
target_include_directories(quantum_signal_bridge_test PRIVATE ${TORCH_INCLUDE_DIRS})
target_link_libraries(quantum_signal_bridge_test
    GTest::gtest
    GTest::gtest_main
    ${TORCH_LIBRARIES}
    /usr/local/lib/python3.10/dist-packages/torch/lib/libtorch_cpu.so
    sep_quantum
    sep_trader_cuda
    sep_trader_logic
    TBB::tbb
    -Wl,--no-as-needed
)
# Include GoogleTest headers
target_include_directories(quantum_signal_bridge_test PRIVATE /sep/third_party)
set_target_properties(quantum_signal_bridge_test PROPERTIES 
    CXX_STANDARD 17
    BUILD_RPATH "/usr/local/lib/python3.10/dist-packages/torch/lib"
    INSTALL_RPATH "/usr/local/lib/python3.10/dist-packages/torch/lib"
)
target_compile_definitions(quantum_signal_bridge_test PRIVATE -D_GLIBCXX_USE_CXX11_ABI=0)
target_compile_features(quantum_signal_bridge_test PRIVATE cxx_std_17)

target_include_directories(trajectory_metrics_test PRIVATE ../src)
target_link_libraries(trajectory_metrics_test GTest::gtest GTest::gtest_main sep_quantum sep_quantum_bitspace sep_trader_cuda CUDA::cudart TBB::tbb)

enable_testing()
include(GoogleTest)
gtest_discover_tests(pattern_metrics_test)
gtest_discover_tests(quantum_signal_bridge_test)
gtest_discover_tests(trajectory_metrics_test)

add_executable(oanda_trader_test
    oanda_trader_test.cpp)
target_include_directories(oanda_trader_test PRIVATE ../src)
target_link_libraries(oanda_trader_test GTest::gtest GTest::gtest_main oanda_trader_app_lib sep_trader_logic sep_trader_cuda sep_engine sep_connectors sep_quantum sep_ui imgui glad OpenGL::GL glfw Threads::Threads TBB::tbb ${CMAKE_DL_LIBS})
gtest_discover_tests(oanda_trader_test)

add_executable(oanda_trader_e2e_test
    oanda_trader_e2e_test.cpp)
target_include_directories(oanda_trader_e2e_test PRIVATE ../src)
target_link_libraries(oanda_trader_e2e_test GTest::gtest GTest::gtest_main oanda_trader_app_lib sep_trader_logic sep_trader_cuda sep_engine sep_connectors sep_quantum sep_ui imgui glad OpenGL::GL glfw Threads::Threads TBB::tbb ${CMAKE_DL_LIBS})
gtest_discover_tests(oanda_trader_e2e_test)

add_executable(test_forward_window_metrics
    test_forward_window_metrics.cpp)
target_include_directories(test_forward_window_metrics PRIVATE ../src)
target_link_libraries(test_forward_window_metrics GTest::gtest GTest::gtest_main sep_trader_logic sep_trader_cuda CUDA::cudart TBB::tbb)
gtest_discover_tests(test_forward_window_metrics)
