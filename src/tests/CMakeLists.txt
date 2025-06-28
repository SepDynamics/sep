# CMakeLists.txt for Cycles integration tests

add_executable(cycles_test cycles_test.cpp)
target_include_directories(cycles_test
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/cycles/src
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
        ${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include
        ${CMAKE_SOURCE_DIR}/extern/eigen
)

# Link against the same libraries as the main executable to ensure consistency
if(CMAKE_CXX_COMPILER_ID MATCHES "GNU|Clang")
    target_link_libraries(cycles_test
        PRIVATE
        # First link Cycles libraries with whole-archive to ensure all symbols are included
        -Wl,--whole-archive
        ${CYCLES_LINK_ORDER}
        -Wl,--no-whole-archive
        # OSL libraries
        ${OSL_COMP_LIBRARY}
        ${OSL_EXEC_LIBRARY}
        ${OSL_QUERY_LIBRARY}
        # Then link internal libraries that might depend on Cycles
        ${SEP_INTERNAL_LIBS}
        ${BLENDER_LINK_ORDER}
        ${EXTERNAL_LINK_ORDER}
        ${CURL_LIBRARIES}
        ${HIREDIS_LIBRARIES}
        ${CUDA_LIBRARIES}
        Threads::Threads
        fmt::fmt
        spdlog::spdlog
        dl
        m
    )
else()
    # Fallback for other compilers
    target_link_libraries(cycles_test
        PRIVATE
        ${CYCLES_LIBRARIES}
        # OSL libraries
        ${OSL_COMP_LIBRARY}
        ${OSL_EXEC_LIBRARY}
        ${OSL_QUERY_LIBRARY}
        ${SEP_INTERNAL_LIBS}
        ${BLENDER_LINK_ORDER}
        ${EXTERNAL_LINK_ORDER}
        ${CURL_LIBRARIES}
        ${HIREDIS_LIBRARIES}
        CUDAToolkit::cudart
        CUDAToolkit::cuda_driver
        Threads::Threads
        fmt::fmt
        spdlog::spdlog
        dl
        m
    )
endif()

add_custom_target(run_cycles_test
    COMMAND cycles_test ${CMAKE_BINARY_DIR}/cycles_render.ppm
    DEPENDS cycles_test
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
    COMMENT "Running Cycles renderer test"
)

