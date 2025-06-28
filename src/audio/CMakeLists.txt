add_library(sep_audio STATIC config.cpp pipeline.cpp pipewire_capture.cpp pipewire_stubs.cpp)

# Find PipeWire using both pkg-config and direct library find for robustness
find_package(PkgConfig QUIET)
set(PIPEWIRE_FOUND FALSE)

# Try using pkg-config first
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PIPEWIRE_PKG libpipewire-0.3 QUIET)
  if(PIPEWIRE_PKG_FOUND)
    if(PIPEWIRE_PKG_LIBRARY_DIRS STREQUAL "")
      message(WARNING "PipeWire pkg-config returned empty library path; disabling audio module")
      set(PIPEWIRE_FOUND FALSE)
    else()
      message(STATUS "PipeWire found via pkg-config: ${PIPEWIRE_PKG_LIBRARIES}")
      set(PIPEWIRE_FOUND TRUE)
      set(PIPEWIRE_LIBRARIES ${PIPEWIRE_PKG_LIBRARIES})
      set(PIPEWIRE_INCLUDE_DIRS ${PIPEWIRE_PKG_INCLUDE_DIRS})
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
  add_compile_definitions(SEP_HAS_PIPEWIRE=1)
  target_include_directories(sep_audio PRIVATE ${PIPEWIRE_INCLUDE_DIRS})
  target_link_libraries(sep_audio PRIVATE ${PIPEWIRE_LIBRARIES})
  # Propagate to parent scope for main executable linking
  set(PIPEWIRE_LIBRARIES ${PIPEWIRE_LIBRARIES} PARENT_SCOPE)
  set(PIPEWIRE_FOUND ${PIPEWIRE_FOUND} PARENT_SCOPE)
else()
  message(STATUS "PipeWire not found, using stub implementation")
  add_compile_definitions(SEP_HAS_PIPEWIRE=0)
endif()

target_include_directories(sep_audio
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
