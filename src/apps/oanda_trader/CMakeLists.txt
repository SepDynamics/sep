enable_language(CUDA)
find_package(CUDA REQUIRED)

add_library(sep_trader_cuda
    tick_cuda_kernels.cu
    forward_window_kernels.cu
)

target_include_directories(sep_trader_cuda PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)
set_target_properties(sep_trader_cuda PROPERTIES
    CUDA_STANDARD 17
    CUDA_STANDARD_REQUIRED ON
    CUDA_SEPARABLE_COMPILATION ON
    CUDA_RESOLVE_DEVICE_SYMBOLS ON
    CUDA_ARCHITECTURES "61;75;86;89"
)

target_link_libraries(sep_trader_cuda PUBLIC 
    CUDA::cudart
    sep_quantum
    sep_engine
)

add_library(sep_trader_logic
    quantum_signal_bridge.cpp
    forward_window_kernels.cpp
    realtime_aggregator.cpp
    market_model_cache.cpp
    enhanced_market_model_cache.cpp
    multi_asset_signal_fusion.cpp
    market_regime_adaptive.cpp
)

target_link_libraries(sep_trader_logic PUBLIC 
    sep_trader_cuda
    sep_quantum
)

target_include_directories(sep_trader_logic PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
)

add_library(oanda_trader_app_lib
    oanda_trader_app.cpp
)
target_include_directories(oanda_trader_app_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/imgui
)

target_link_libraries(oanda_trader_app_lib PUBLIC
    imgui
    sep_trader_cuda
)

add_executable(oanda_trader
    main.cpp
)

target_include_directories(oanda_trader PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/imgui
)

target_link_libraries(oanda_trader PUBLIC
    oanda_trader_app_lib
    sep_trader_logic
    sep_trader_cuda
    sep_engine
    sep_connectors
    sep_quantum
    sep_ui
    imgui
    glad
    OpenGL::GL
    glfw
    Threads::Threads
    TBB::tbb
    ${CMAKE_DL_LIBS}
    CUDA::cudart
)

set_target_properties(oanda_trader PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# Add the new quantum tracker app with CUDA support
add_executable(quantum_tracker
    quantum_tracker_main.cpp
    quantum_tracker_app.cpp
    quantum_tracker_window.cpp
    data_cache_manager.cpp
    tick_data_manager.cpp
    rolling_window_chart.cpp
    weekend_optimizer.cpp
    market_model_cache.cpp
)

target_include_directories(quantum_tracker PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/imgui
    ${implot_SOURCE_DIR}
)
target_link_libraries(quantum_tracker PUBLIC
    sep_trader_logic
    sep_trader_cuda
    sep_engine
    sep_connectors
    sep_quantum
    sep_ui
    imgui
    implot
    glad
    OpenGL::GL
    glfw
    Threads::Threads
    TBB::tbb
    ${CMAKE_DL_LIBS}
    CUDA::cudart
)

set_target_properties(quantum_tracker PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

install(TARGETS oanda_trader quantum_tracker DESTINATION bin)
