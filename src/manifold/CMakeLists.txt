add_library(sep_manifold INTERFACE)

target_include_directories(sep_manifold INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
