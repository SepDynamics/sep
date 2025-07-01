include(SetCXXStandard)

add_library(sep_audio
  STATIC
    config.cpp
    pipeline.cpp
    pipewire_capture.cpp
    factory.cpp
)

# Required dependencies
find_package(PkgConfig REQUIRED)
pkg_check_modules(PIPEWIRE REQUIRED libpipewire-0.3)
pkg_check_modules(FFTW REQUIRED fftw3f)

message(STATUS "Using PipeWire at: ${PIPEWIRE_LIBRARIES}")
message(STATUS "Using FFTW at: ${FFTW_LIBRARIES}")

target_include_directories(sep_audio
    PRIVATE
        ${PIPEWIRE_INCLUDE_DIRS}
        ${FFTW_INCLUDE_DIRS}
)

target_link_libraries(sep_audio
    PRIVATE
        ${PIPEWIRE_LIBRARIES}
        ${FFTW_LIBRARIES}
    PUBLIC
        sep_core
)

# Ensure symbols are exported
set_target_properties(sep_audio PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    VISIBILITY_INLINES_HIDDEN ON
    C_VISIBILITY_PRESET default
    CXX_VISIBILITY_PRESET default
)

# Propagate to parent scope for main executable linking
set(PIPEWIRE_LIBRARIES ${PIPEWIRE_LIBRARIES} PARENT_SCOPE)
set(FFTW_LIBRARIES ${FFTW_LIBRARIES} PARENT_SCOPE)

target_include_directories(sep_audio
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/include/api
        ${CMAKE_SOURCE_DIR}/include/audio
        ${CMAKE_SOURCE_DIR}/include/blender
        ${CMAKE_SOURCE_DIR}/include/compat
        ${CMAKE_SOURCE_DIR}/include/core
        ${CMAKE_SOURCE_DIR}/include/crow
        ${CMAKE_SOURCE_DIR}/include/memory
        ${CMAKE_SOURCE_DIR}/include/quantum
        ${CMAKE_SOURCE_DIR}/cycles/src
        ${CMAKE_SOURCE_DIR}/extern
        ${CMAKE_SOURCE_DIR}/extern/cycles/src
        ${CMAKE_SOURCE_DIR}/extern/blender/lib/linux_x64/osl/include
        ${CMAKE_SOURCE_DIR}/extern/eigen
)
