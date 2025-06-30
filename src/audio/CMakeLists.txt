add_library(sep_audio
  STATIC
    config.cpp
    pipeline.cpp
    pipewire_capture.cpp
)

# Find PipeWire using both pkg-config and direct library find for robustness
find_package(PkgConfig QUIET)
set(PIPEWIRE_FOUND FALSE)

# Try using pkg-config first
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PIPEWIRE_PKG libpipewire-0.3 QUIET)
  if(PIPEWIRE_PKG_FOUND)
    if(NOT PIPEWIRE_PKG_LIBRARY_DIRS)
      message(WARNING "PipeWire pkg-config returned empty library path; disabling audio module")
      set(PIPEWIRE_FOUND FALSE)
    else()
      message(STATUS "PipeWire found via pkg-config: pipewire-0.3")
      find_library(PIPEWIRE_ACTUAL_LIB
        NAMES pipewire-0.3 libpipewire-0.3
        PATHS ${PIPEWIRE_PKG_LIBRARY_DIRS} /usr/lib /usr/lib64 /usr/local/lib /usr/local/lib64
        NO_DEFAULT_PATH
      )
      if(PIPEWIRE_ACTUAL_LIB)
        message(STATUS "Verified PipeWire library: ${PIPEWIRE_ACTUAL_LIB}")
        set(PIPEWIRE_FOUND TRUE)
        set(PIPEWIRE_LIBRARIES ${PIPEWIRE_ACTUAL_LIB})
        set(PIPEWIRE_INCLUDE_DIRS ${PIPEWIRE_PKG_INCLUDE_DIRS})
      else()
        message(WARNING "PipeWire libraries found by pkg-config but could not verify their existence")
        set(PIPEWIRE_FOUND FALSE)
      endif()
    endif()
  endif()
endif()

# Fall back to direct library find if pkg-config failed
if(NOT PIPEWIRE_FOUND)
  # Try to find the library directly in standard system locations
  find_library(PIPEWIRE_LIB
    NAMES pipewire-0.3 libpipewire-0.3
    PATHS /usr/lib /usr/lib64 /usr/local/lib /usr/local/lib64
    DOC "PipeWire library"
  )
  find_path(PIPEWIRE_INCLUDE_DIR
    NAMES pipewire/pipewire.h
    PATHS /usr/include /usr/local/include
    DOC "PipeWire include directory"
  )
  
  if(PIPEWIRE_LIB AND PIPEWIRE_INCLUDE_DIR)
    message(STATUS "PipeWire found via direct library search: ${PIPEWIRE_LIB}")
    set(PIPEWIRE_FOUND TRUE)
    set(PIPEWIRE_LIBRARIES ${PIPEWIRE_LIB})
    set(PIPEWIRE_INCLUDE_DIRS ${PIPEWIRE_INCLUDE_DIR})
  endif()
endif()

# Setup based on whether PipeWire was found
if(PIPEWIRE_FOUND)
  message(STATUS "Using PipeWire at: ${PIPEWIRE_LIBRARIES}")
  target_include_directories(sep_audio PRIVATE ${PIPEWIRE_INCLUDE_DIRS})
  target_link_libraries(sep_audio PRIVATE ${PIPEWIRE_LIBRARIES})
  # Propagate to parent scope for main executable linking
  set(PIPEWIRE_LIBRARIES ${PIPEWIRE_LIBRARIES} PARENT_SCOPE)
  set(PIPEWIRE_FOUND ${PIPEWIRE_FOUND} PARENT_SCOPE)
endif()

# FFTW (for CPU FFT implementation)
pkg_check_modules(FFTW QUIET fftw3f)
if(FFTW_FOUND)
  message(STATUS "Found FFTW: ${FFTW_LIBRARIES}")
  target_include_directories(sep_audio PRIVATE ${FFTW_INCLUDE_DIRS})
  target_link_libraries(sep_audio PRIVATE ${FFTW_LIBRARIES})
endif()

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
