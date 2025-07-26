
# Install headers
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/audio/
        DESTINATION include/sep/audio
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")

install(DIRECTORY ${CMAKE_SOURCE_DIR}/src/audio/
        DESTINATION include/sep/audio/internal
        FILES_MATCHING
        PATTERN "*.h"
        PATTERN "*.hpp")


add_library(sep_audio
  STATIC
    config.cpp
    pipeline.cpp
    pipewire_capture.cpp
    factory.cpp
)

# Check for required dependencies - audio is now required
find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)
pkg_check_modules(FFTW REQUIRED fftw3f)

# No conditional check - these are now required dependencies

message(STATUS "Using PipeWire at: ${PIPEWIRE_LIBRARIES}")
message(STATUS "Using FFTW at: ${FFTW_LIBRARIES}")

target_include_directories(sep_audio
    PRIVATE
        ${PIPEWIRE_INCLUDE_DIRS}
        ${FFTW_INCLUDE_DIRS}
        /usr/include
        ${CMAKE_SOURCE_DIR}/src/engine
)

target_link_libraries(sep_audio
    PRIVATE
        ${PIPEWIRE_LIBRARIES}
        ${FFTW_LIBRARIES}
    PUBLIC
        sep_engine
        CUDA::cudart
        CUDA::cuda_driver
)

# Configure symbol visibility
set_target_properties(sep_audio PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VISIBILITY_INLINES_HIDDEN ON
    C_VISIBILITY_PRESET hidden
    CXX_VISIBILITY_PRESET hidden
)

# Define export macro for proper symbol visibility
target_compile_definitions(sep_audio
    PRIVATE
        SEP_AUDIO_EXPORTS
    PUBLIC
        SEP_HAS_AUDIO=1
)

# Propagate to parent scope for main executable linking
set(PIPEWIRE_LIBRARIES ${PIPEWIRE_LIBRARIES} PARENT_SCOPE)
set(FFTW_LIBRARIES ${FFTW_LIBRARIES} PARENT_SCOPE)

target_include_directories(sep_audio
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
    PRIVATE
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/cycles/src>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include>
        $<BUILD_INTERFACE:${CMAKE_SOURCE_DIR}/extern/eigen>
)
