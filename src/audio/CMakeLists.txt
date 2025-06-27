add_library(sep_audio STATIC config.cpp pipeline.cpp pipewire_capture.cpp pipewire_stubs.cpp)

# Find PipeWire using pkg-config
find_package(PkgConfig QUIET)
if(PKG_CONFIG_FOUND)
  pkg_check_modules(PIPEWIRE libpipewire-0.3 QUIET)
  if(PIPEWIRE_FOUND)
    message(STATUS "Found PipeWire: ${PIPEWIRE_LIBRARIES}")
    add_compile_definitions(SEP_HAS_PIPEWIRE=1)
    target_include_directories(sep_audio PRIVATE ${PIPEWIRE_INCLUDE_DIRS})
    target_link_libraries(sep_audio PRIVATE ${PIPEWIRE_LIBRARIES})
  else()
    message(STATUS "PipeWire not found, using stub implementation")
    add_compile_definitions(SEP_HAS_PIPEWIRE=0)
  endif()
else()
  message(STATUS "pkg-config not found, cannot check for PipeWire")
  add_compile_definitions(SEP_HAS_PIPEWIRE=0)
endif()

target_include_directories(sep_audio
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)
