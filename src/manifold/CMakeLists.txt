add_library(sep_manifold INTERFACE)


# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/manifold/
        DESTINATION include/sep/manifold
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/manifold/
        DESTINATION include/sep/manifold/internal
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

target_include_directories(sep_manifold INTERFACE
    $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
    $<INSTALL_INTERFACE:include>
)
