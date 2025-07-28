add_executable(oanda_trader
    main.cpp
    oanda_trader_app.cpp
    quantum_signal_bridge.cpp
)

target_include_directories(oanda_trader PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/imgui
)

target_link_libraries(oanda_trader PUBLIC
    sep_engine
    sep_connectors
    sep_quantum
    sep_ui
    imgui
    glad
    OpenGL::GL
    glfw
    Threads::Threads
    ${CMAKE_DL_LIBS}
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
    quantum_signal_bridge.cpp
    data_cache_manager.cpp
    tick_data_manager.cpp
    rolling_window_chart.cpp
    tick_cuda_kernels.cu
)

target_include_directories(quantum_tracker PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_SOURCE_DIR}/src
    ${CMAKE_SOURCE_DIR}/third_party/imgui
    ${implot_SOURCE_DIR}
)

target_link_libraries(quantum_tracker PUBLIC
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
    ${CMAKE_DL_LIBS}
    $<$<BOOL:${SEP_USE_CUDA}>:CUDA::cudart>
    $<$<BOOL:${SEP_USE_CUDA}>:CUDA::cuda_driver>
)

set_target_properties(quantum_tracker PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
    CUDA_STANDARD 17
    CUDA_STANDARD_REQUIRED ON
)

# CUDA separable compilation for better linking
set_property(TARGET quantum_tracker PROPERTY CUDA_SEPARABLE_COMPILATION ON)

install(TARGETS oanda_trader quantum_tracker DESTINATION bin)